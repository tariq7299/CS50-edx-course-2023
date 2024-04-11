import os
import helpers
import re
from datetime import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for, jsonify
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Adding lookup() from py in jinja2 in order to be able to use it in the jinja templates
app.jinja_env.globals.update(lookup=lookup, usd=usd)


# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

def extract_current_cash_from_db(current_user_id):
    account_cash =  db.execute("SELECT cash FROM users WHERE id = ?", current_user_id)[0]['cash']
    return account_cash

def calculate_current_account_total_holdings_value(current_user_id):
    rows = db.execute("SELECT * FROM users_stocks WHERE user_id = ?", current_user_id)
    total_holdings_value = 0  # Initialize the variable
    for row in rows:
        holding_value = row['shares'] * lookup(row['stock_symbol'])["price"]
        total_holdings_value += holding_value
    return total_holdings_value

def calculate_current_account_grand_total(current_user_id):
    total_holdings_value = calculate_current_account_total_holdings_value(current_user_id)
    grand_total = total_holdings_value + extract_current_cash_from_db(current_user_id)
    return grand_total

def calculate_current_account_totals(current_user_id):
    account_cash = extract_current_cash_from_db(current_user_id)
    total_holdings_value = calculate_current_account_total_holdings_value(current_user_id)
    grand_total = calculate_current_account_grand_total(current_user_id)
    return account_cash, total_holdings_value, grand_total

def validate_password(password):
    import re # regular expression
    if len(password) < 8:
        return ("Password should be at least 8 characters or longer")
    elif not re.search("[0-9]", password):
        return ("Password must contain at least one digit")
    elif not re.search("[A-Z]", password):
        return ("Password must contain at least one uppercase letter")
    elif not re.search("[@_!#$%&^*()<>?~+-/\{}:]",password):
        return ("password must contain at least one special charaster")

@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():

    # this stores the current client id from the session into "current_user_id"
    current_user_id = session["user_id"]

    # grand total = total cash in account + total holding values (each tocks shares time price per share)
    account_cash, total_holdings_value, grand_total = calculate_current_account_totals(current_user_id)

    # This will be used in the shared html page across pages that would preview ther account cash, total holdings value and grand total
    username = db.execute("SELECT username FROM users WHERE id = ?", current_user_id)[0]['username']

    # Extracts all the user_stocks the user owns to be displayed in a table
    user_stocks = db.execute("SELECT ROW_NUMBER() OVER (ORDER BY shares DESC) AS RowNumber, stock_symbol, shares FROM users_stocks WHERE user_id = ?", current_user_id)

    return render_template("index.html", username=username, account_cash=account_cash, total_holdings_value=total_holdings_value, grand_total=grand_total, user_stocks=user_stocks)

@app.route('/profile', methods=['POST', 'GET'])
@login_required
def profile():

    current_user_id = session["user_id"]

    if request.method == "POST":

        """
        request.form is a dictionary-like object that holds the data submitted through an HTML form using the POST method. It allows you to access form data by treating the form fields as keys in the dictionary.

        When you access a specific field using request.form, you typically use square brackets and the field name to access its value. For example: request.form['fieldname']
        # Handle password change form submission
        password = request.form['password'].strip()
        password_confirm = request.form['confirmation'].strip()

        Notice that in user_profile.html, there are mutiple <form>s and even if you filled all input fields in the page and clicked on one of the two,  this very parent <form> of the submitted <button> will "POST" its data even if the other input fields contain data !!
        And that is becasue there is a <form> element encapsulates each button

        for example:
        - Try to print(request.form) or print(request.form.get), after pressing on one of the buttons and filling all input fields
        """
        # If "Change Password was submitted", only the input fields data in <form> of password will be submitted
        if 'password' in request.form:

            # Handle password change form submission
            password = request.form['password'].strip()
            password_confirm = request.form['confirmation'].strip()

             # If user left any of the fields empty
            if not password or not password_confirm:
                flash("Please fill in all fields!")
                return redirect(url_for('profile'))

            # IF two passwords fields don't match
            if password != password_confirm:
                flash("Passwords don't match! Please try again.")
                return redirect(url_for('profile'))

            # Validate the password if it containes especial characters and should 8 characters long
            if validate_password(password):
                flash_message = validate_password(password)
                flash(flash_message)
                return redirect(url_for('profile'))

            # generate_password_hash() will generate the "hash" of password, and then it will store the hash and not the password itself in db
            hashed_password = generate_password_hash(password)

            # UPDATES the hash value of old password
            if not db.execute("UPDATE users SET hash = ? WHERE id = ?", hashed_password, current_user_id):
                flash("Error Occurred while updating password! please contact support team!!")
                return redirect(url_for('profile'))

            # If password passed all checks then it will refresh the page and preview any flash messages stored
            flash("Password Changed successfully!")
            return redirect(url_for('profile'))

        # If "Add" button was submitted", only the input fields data in <form> of cash_amount will be submitted
        elif 'cash_amount' in request.form:

            try:
                # If any error raised refresh page and review flash message error
                entered_cash = float(request.form['cash_amount'])
            except ValueError:
                flash("Please write a valid number!")
                return redirect(url_for('profile'))

            # If user submits an empty field
            if not entered_cash:
                flash("Please enter an amount to be added to account!")
                return redirect(url_for('profile'))

            current_cash = extract_current_cash_from_db(current_user_id)

            total_cash = entered_cash + current_cash

            # UPDATE cash in account with the total_cash
            if not db.execute("UPDATE users SET cash = ? WHERE id = ?", total_cash, current_user_id):
                flash("Error Occurred while updating password! please contact support team!!")
                return redirect(url_for('profile'))

            flash(f"{usd(entered_cash)} added successfully to your account !")
            return redirect(url_for('profile'))

    elif request.method == "GET":
        # this stores the current client id from the session into "current_user_id"
        current_user_id = session["user_id"]

        # grand total = total cash in account + total holding values (each tocks shares time price per share)
        account_cash, total_holdings_value, grand_total = calculate_current_account_totals(current_user_id)

        # This will be used in the shared html page across pages that would preview ther account cash, total holdings value and grand total
        username = db.execute("SELECT username FROM users WHERE id = ?", current_user_id)[0]['username']

        return render_template("user_profile.html", username=username, account_cash=account_cash, total_holdings_value=total_holdings_value, grand_total=grand_total)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():

    # this stores the current client id from the session into "current_user_id"
    current_user_id = session["user_id"]

    if request.method == "POST":

        # Retrives the stock quote symbol entered by the user
        q = request.form.get('symbol')

        # checks if user left the "symbol" input field empty
        if not q:
            quote = {}
            error_message = "Please Enter a Stock Quote Symbol!"
            return  apology(error_message)

        # Fetchs the stock info (name, price, symbol)
        quote = lookup(q)
        # If "q" were not valid lookup() will return "None"
        if not quote:
            quote = {}
            error_message = "Please Enter a Valid Stock Quote Symbol!"
            return  apology(error_message)

        try:
            # It will try to convert the shares number of shares inputed by user to "int", if an error raised then it will return an apology
            recently_acquired_shares = int(request.form.get('shares'))

            quote_symbol = quote["symbol"]
            quote_price = quote["price"]

            # Cehck whether the user entered a valid shares number or not , if not valid an ValueError raise and will activate the "except" statement down below and an apology will be returned to user
            if recently_acquired_shares < 0:
                raise ValueError

            # Setting the type of transaction
            transaction_type = "Acquiring"

            # I would assinged it a genuine number but check50 doesnot reconize it
            trading_fees = 0

            # holding value = price per share * total number of shares
            holding_value = float(quote_price * recently_acquired_shares)


            # payment total = holding value + trading fees
            total_amount =  float(holding_value + trading_fees)

            # This extract amount of cash in user account
            current_account_cash = extract_current_cash_from_db(current_user_id)

            # Check whether the user has enough cash in account in complete the purchase of the stock
            if not current_account_cash >= total_amount:
                error_message = f"Not enough cash to proceed with payment ! cash in account is {usd(current_account_cash)} and total payment amount is {usd(total_amount)}"
                return apology(error_message)

            # This will deducte the payment total from the user's cash, but will not yet update his cash in db (this will be done after he presses on (make purchase that will make a POST request to this route /checkout_for_buying))
            cash_after_transaction = current_account_cash - total_amount

            # Check if user has bought shares from the same organization/company
            # if true then update total shares, if false, then insert new row containing stock info
            user_stock_row = db.execute("SELECT * FROM users_stocks WHERE user_id = ? AND stock_symbol = ?", current_user_id, quote_symbol)

            if len(user_stock_row) >= 1:

                previously_acquired_shares = user_stock_row[0]['shares']

                total_acquired_shares = previously_acquired_shares + recently_acquired_shares

                db.execute("UPDATE users_stocks SET shares = ? WHERE user_id = ? AND stock_symbol = ?", total_acquired_shares, current_user_id, quote_symbol)

            elif len(user_stock_row) != 1:

                previously_acquired_shares = 0

                total_acquired_shares = previously_acquired_shares + recently_acquired_shares

                # This will try to extract stock_id from "stocks" then use that stock_id in the insertion query of the newly purchased stock into "users_stocks"
                # if an exeption raises like "UnboundLocalError" or "runtimeError" then that will mean that the stock symbole doesn't exist in "stocks", so "except" will activate and then execute INSERT stock_symbol into stocks table(quote_symbol)", and use its return value "current_stock_id" in the insertion of the user's newly purchased stock into "users_stocks" after "finally" statement.
                try:
                    current_stock_id = db.execute("SELECT stock_id FROM stocks WHERE stock_symbol = ?", quote_symbol)[0]['stock_id']
                except Exception:
                    current_stock_id = db.execute("INSERT OR IGNORE INTO stocks (stock_symbol) VALUES (?)", (quote_symbol))
                finally:
                    db.execute("INSERT INTO users_stocks (user_id, stock_id, stock_symbol, shares) VALUES (?, ?, ?, ?)", current_user_id, current_stock_id, quote_symbol, total_acquired_shares)

            # deduct the payment amount from user's cash and update it
            db.execute("UPDATE users SET cash = ? WHERE id = ?", cash_after_transaction, current_user_id)

            current_stock_id = db.execute("SELECT stock_id FROM stocks WHERE stock_symbol = ?", quote_symbol)[0]['stock_id']

            # Get the current today's date and time, will be used in the insertion of the transaction
            current_date = datetime.now()

            # insert transaction into "transactions"
            # take the return value "transaction_id", and insert it in the flash message that will be displayed to the user
            # Notice that here we insert into transactions "recently_acquired_shares" and not "total_acquired_shares"
            transaction_id = db.execute("INSERT INTO transactions (user_id, quote_id, transaction_date, quote_symbol, price_per_share, shares, trading_fees, holding_value, total_amount, transaction_type) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", current_user_id, current_stock_id, current_date, quote_symbol, quote_price, recently_acquired_shares, trading_fees, holding_value, total_amount, transaction_type)

            # a flash message that will dispaly payment total/total_acquired_shares/quote_symbol/transaction_id
            flash(f'A total amount of {usd(total_amount)} have been paid to procur {recently_acquired_shares} shares from {quote_symbol} -- Transaction ID is {transaction_id}', 'success')
            # Finally redirect the user to index() route (homepage)
            # I didn't use "redirect("/")" because this will be sent to the JS code in "checkout_buying.html", so it needs to be in JSON (as the JS code in client side is waiting for a "response", and it has to in JSON), so i used the method redirect_route() instead
            return redirect(url_for("index"))

        except ValueError :
            error_message = "Please Enter a valid shares number !"
            return  apology(error_message)

        except Exception as e :
            # display an apology with error message if an exception raised that wasn't into consideration
            error_message = f'Error occured: {str(e)}'
            print(error_message)
            return apology(error_message)

    elif request.method == "GET":

        # extract username from database using current_user_id = session["user_id"], to be used in the shared user info html page
        username = db.execute("SELECT username FROM users WHERE id = ?", current_user_id)[0]['username']

        # This will calculate the current total cash in account, total holdings value and  grand total, the it will be placed in the shared html of cash_shared_div.html (at the top right corner of screen, in all pages except login and register)
        account_cash, total_holdings_value, grand_total = calculate_current_account_totals(current_user_id)

        return render_template("buy.html", username=username, account_cash=account_cash, total_holdings_value=total_holdings_value, grand_total=grand_total)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    current_user_id = session["user_id"]

    # extract username from database using current_user_id = session["user_id"], to be used in the shared user info html page
    username = db.execute("SELECT username FROM users WHERE id = ?", current_user_id)[0]['username']

    # This will calculate the current total cash in account, total holdings value and  grand total, the it will be placed in the shared html of cash_shared_div.html (at the top right corner of screen, in all pages except login and register)
    account_cash, total_holdings_value, grand_total = calculate_current_account_totals(current_user_id)

    # extract the latest five transactions, ORDER BY date
    user_transactions = db.execute("SELECT ROW_NUMBER() OVER (ORDER BY transaction_date DESC) AS RowNumber, transaction_id, transaction_date, quote_symbol, price_per_share, shares, holding_value, trading_fees, total_amount, transaction_type FROM transactions WHERE user_id = ? LIMIT 5", current_user_id)

    return render_template("history.html", user_transactions=user_transactions, username=username, account_cash=account_cash, total_holdings_value=total_holdings_value, grand_total=grand_total)


# This will get activated if user presses on "show all transactions" button found in history.html
@app.route("/show_all_history")
@login_required
def show_all_history():

    current_user_id = session["user_id"]

    # extract all transactions ORDER BY date
    user_transactions = db.execute("SELECT ROW_NUMBER() OVER (ORDER BY transaction_date DESC) AS RowNumber, transaction_id, transaction_date, quote_symbol, price_per_share, shares, holding_value, trading_fees, total_amount, transaction_type FROM transactions WHERE user_id = ?", current_user_id)

    return render_template("show_all_history.html", user_transactions=user_transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():

    # this stores the current client id from the session into "current_user_id"
    current_user_id = session["user_id"]

    if request.method == "POST":

        # Retrives the stock quote symbol entered by the user
        q = request.form.get("symbol")

        # checks if user left the "symbol" input field empty
        if not q:
            quote = {}
            error_message = "Please Enter a Stock Quote Symbol!"
            return  apology(error_message)

        # Fetchs the stock info (name, price, symbol)
        quote = lookup(q)
        # If "q" were not valid lookup() will return "None"
        if not quote:
            quote = {}
            error_message = "Please Enter a Valid Stock Quote Symbol!"
            return  apology(error_message)

        return render_template("quoted.html", quote=quote)

    elif request.method == "GET":

        # extract username from database using current_user_id = session["user_id"], to be used in the shared user info html page
        username = db.execute("SELECT username FROM users WHERE id = ?", current_user_id)[0]['username']

        # This will calculate the current total cash in account, total holdings value and  grand total, the it will be placed in the shared html of cash_shared_div.html (at the top right corner of screen, in all pages except login and register)
        account_cash, total_holdings_value, grand_total = calculate_current_account_totals(current_user_id)

        return render_template("quote.html", username=username, account_cash=account_cash, total_holdings_value=total_holdings_value, grand_total=grand_total)


@app.route("/register", methods=["GET", "POST"])
def register():

    if request.method == "POST":

        username = request.form.get("username").strip()
        password = request.form.get("password").strip()
        password_confirm = request.form.get("confirmation").strip()

        # generate_password_hash() will generate the "hash" of password, and then it will stroe the hash and not the password itself
        hashed_password = generate_password_hash(password)


        # If left any field empty
        if not username or not password or not password_confirm:
            error_message = "Please fill in all fields!"
            return apology(error_message)

        # IF two passwords fields don't match
        if password != password_confirm:
            error_message = "Passwords don't match! Please try again."
            return apology(error_message)

        # Validate the password if it containes especial characters and should 8 characters long
        if validate_password(password):
            flash_message = validate_password(password)
            flash(flash_message)
            return redirect(url_for('register'))

        # Checks if username already exists in database
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        if len(rows) == 1:
            error_message = "Username already exists! Please choose a new one."
            return apology(error_message)

        # Finally if all is good, insert the new user info into database
        current_user_id = db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hashed_password)

        session["user_id"] = current_user_id

        return redirect("/")

    # If usser reloads "register" by pressing on its link, it will make a GET request and reloads the page
    elif request.method == "GET":
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():

    # this stores the current client id from the session into "current_user_id"
    current_user_id = session["user_id"]

    if request.method == "POST":

        try:

            # Retrives the stock quote symbol choosen by the user from the select input list
            quote_symbol = request.form.get('symbol')

            # Checks of the user didinot choose any symbol from the list and presses on "sell" then it display an apology
            if not quote_symbol:
                # error_message = "Unfortunately you don't own any stocks !"
                error_message = "Please select a stock from the list !"
                return  apology(error_message)

            # # It will try to convert the shares number of shares inputed by user to "int", if an error raised then it will return an apology
            shares_to_sell = int(request.form.get('shares'))

            # Fetch the current stock price using lookup()
            quote_price = lookup(quote_symbol)["price"]

            # Checks if the shares number entered by user is not positive
            if shares_to_sell < 0:
                raise ValueError

            # Extract shares from databse, to be used in the comparison between the entered shares the actual stock's shares user owns
            shares_owned = db.execute("SELECT shares FROM users_stocks WHERE user_id = ? AND stock_symbol = ?", current_user_id, quote_symbol)[0]['shares']

            # compare shares in users account with entered shares
            if shares_to_sell <= 0 or shares_to_sell > shares_owned:
                # If the shares inputed by user is not a positive number or is bigger than shares he/she owns
                error_message = "Number of shares must be less than or equal to shares you own!"
                raise ValueError

            # Setting the type of transaction
            transaction_type = "Dissolsving"

            trading_fees = 0

            # holding value = price per share * total number of shares
            holding_value = float(quote_price * shares_to_sell)

            # payment total = holding value + trading fees
            total_amount =  float(holding_value - trading_fees)

            # This extract amount of cash in user account
            current_account_cash = extract_current_cash_from_db(current_user_id)

            # This will deducted the payment total from the user's cash, but will not yet update his cash in db (this will be done after he presses on (make purchase that will make a POST request to this route /checkout_for_buying))
            cash_after_transaction = current_account_cash + total_amount

            # extract total previously bought shares from database
            previously_acquired_shares = db.execute("SELECT shares FROM users_stocks WHERE user_id = ? AND stock_symbol = ?", current_user_id, quote_symbol)[0]['shares']

            # Calculate the remaining shares of the stock after the selling process
            remaining_shares = previously_acquired_shares - shares_to_sell

            # Checks whether user has selled all shares of the stock or some of it
            if remaining_shares > 0:

                # If the remaining shares is is more than zero that means the hther is still left shares, so just UPDATE the shars in database
                db.execute("UPDATE users_stocks SET shares = ? WHERE user_id = ? AND stock_symbol = ?", remaining_shares, current_user_id, quote_symbol)

                flash_message = (f'A total amount of {usd(total_amount)} have been added to your cash as a result of selling {shares_to_sell} shares from {quote_symbol}')

            else:

                # However, if the remaining shares is zero, DELETE the whole row of the stock from users_stocks
                db.execute("DELETE FROM users_stocks WHERE user_id = ? AND stock_symbol = ?", current_user_id, quote_symbol)

                # flash_message var holds the message that will be inserted into flash() Fn at the end, then it will be displayed at the top of page as a feedback after redirceting the user to index.html page
                flash_message = (f'Your account has been credited with {usd(total_amount)} due to the successful sale of all {shares_to_sell} shares of the {quote_symbol} stock')

            # Add total amount of income from the selling process to user's cash
            db.execute("UPDATE users SET cash = ? WHERE id = ?", cash_after_transaction, current_user_id)

            # Selecting the unique stock_symbol id from stocks, that will be used in the insertion query of the transaction into "transactions"
            current_stock_id = db.execute("SELECT stock_id FROM stocks WHERE stock_symbol = ?", quote_symbol)[0]['stock_id']

            # defining the type of transaction, this will be used in the INSERT query to distinguish buying shares from selling shares.
            transaction_type = "Dissolsving"

            # Fetch the current today's date
            current_date = datetime.now().date()

            # insert the recent transaction into "transactions"
            # take the return value "transaction_id", and insert it in the flash message that will be displayed to the user
            # Notice that here that we inserted into "transactions" "shares_to_sell" and not "remaining_shares"

            # insert transaction into "transactions"
            # take the return value "transaction_id", and insert it in the flash message that will be displayed to the user
            # Notice that here we insert into transactions "shares_to_sell" and not "total_acquired_shares"
            transaction_id = db.execute("INSERT INTO transactions (user_id, quote_id, transaction_date, quote_symbol, price_per_share, shares, trading_fees, holding_value, total_amount, transaction_type) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", current_user_id, current_stock_id, current_date, quote_symbol, quote_price, shares_to_sell, trading_fees, holding_value, total_amount, transaction_type)

            # A flash message that will dispaly total amount acquired/selled shares/quote_symbol/transaction_id
            flash(f'{flash_message} -- Transaction ID is {transaction_id}', 'success')

            return redirect(url_for("index"))

        # A ValueError will raise, if shares were not a number
        except ValueError :
            return  apology(error_message)

        except Exception as e :
            # display an apology with error message if an exception raised that wasn't into consideration
            error_message = f'Error occured: {str(e)}'
            print(error_message)
            return apology(error_message)

    elif request.method == "GET":

        # extract username from database using current_user_id = session["user_id"], to be used in the shared user info html page
        username = db.execute("SELECT username FROM users WHERE id = ?", current_user_id)[0]['username']

        # This will calculate the current total cash in account, total holdings value and  grand total, the it will be placed in the shared html of cash_shared_div.html (at the top right corner of screen, in all pages except login and register)
        account_cash, total_holdings_value, grand_total = calculate_current_account_totals(current_user_id)

        # Extracts all the stocks symbols user owns from users_stocks table
        quote_symbols = db.execute("SELECT stock_symbol FROM users_stocks WHERE user_id = ?", current_user_id)

        return render_template("sell.html", quote_symbols=quote_symbols, username=username, account_cash=account_cash, total_holdings_value=total_holdings_value, grand_total=grand_total)

