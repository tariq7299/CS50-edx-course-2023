import os
import helpers

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for, jsonify
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime
from helpers import apology, login_required, lookup, usd, _apology_

# Configure application
app = Flask(__name__)

# Needed for flash messages
app.secret_key = "secret_key"

# Custom filter
app.jinja_env.filters["usd"] = usd

# Adding lookup() from helpers.py in jinja2 in order to be able to use it in the jinja templates
app.jinja_env.globals.update(lookup=lookup, usd=usd)

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


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

    """Show portfolio of stocks"""

    # This will calculate the current total cash in account, total holdings value and  grand total, the it will be placed in the shared html of cash_shared_div.html (at the top right corner of screen, in all pages except login and register)
    account_cash, total_holdings_value, grand_total  = helpers.calculate_current_account_totals(current_user_id)

    # extract username from database using current_user_id = session["user_id"], to be used in the shared user info html page
    username = helpers.extract_username_from_db(current_user_id)

    # Extracts all the user_stocks the user owns to be displayed in a table
    user_stocks = helpers.extract_user_stocks_from_db(current_user_id)

    return render_template('index.html', username=username, account_cash=account_cash, total_holdings_value=total_holdings_value, grand_total=grand_total, user_stocks=user_stocks)


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
            if helpers.validate_password(password):
                flash_message = helpers.validate_password(password)
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

            current_cash = helpers.extract_current_cash_from_db(current_user_id)

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
        account_cash, total_holdings_value, grand_total = helpers.calculate_current_account_totals(current_user_id)

        # This will be used in the shared html page across pages that would preview ther account cash, total holdings value and grand total
        username = db.execute("SELECT username FROM users WHERE id = ?", current_user_id)[0]['username']

        return render_template("user_profile.html", username=username, account_cash=account_cash, total_holdings_value=total_holdings_value, grand_total=grand_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "GET":
        current_user_id = session["user_id"]

        # extract username from database using current_user_id = session["user_id"], to be used in the shared_div of user Info
        username = helpers.extract_username_from_db(current_user_id)

        # This will calculate the current total cash in account, total holdings value and  grand total, the it will be placed in the shared html of cash_shared_div.html (at the top right corner of screen, in all pages except login and register)

        # grand total = total cash in account + total holding values (each tocks shares time price per share)
        account_cash, total_holdings_value, grand_total  = helpers.calculate_current_account_totals(current_user_id)
        return render_template("buy.html", username=username, account_cash=account_cash, total_holdings_value=total_holdings_value, grand_total=grand_total)


@app.route("/buy_shares")
@login_required
def buy_shares():

    # This will fetch a stock dict (name, price, symbol), and also check for user errors like empty feild or invalid stock symbol, if all is good and user entered a valid symbol, then an ajax request will happen and shares input feild will appear to take number of shares from user
    # This will happen after a GET request, no POST requests here
    q = request.args.get('quote')
    quote = lookup(q)
    if q:
        if quote is not None:
            return render_template("buy_shares.html")
        else:
            quote = {}
            error_message = "Please Enter a Valid Stock Quote Symbol!"
            return  _apology_(error_message, "0x00")
    else:
        quote = {}
        error_message = "Please Enter a Stock Quote Symbol!"
        return  _apology_(error_message, "0x01")


# This route will be activated via GET and POST
# if GET : display checkout_buying.html via an AJAX request to process client purhacey
# If POST : add stock to his stocks/deduct the payment amount from his cash/save the transaction to database
@app.route("/checkout_buying", methods=["GET", "POST"])
@login_required
def checkout_buying():

    current_user_id = session["user_id"]

    # This will calculate payment info and then validate data (positive number of shares entered by user/enough cash in user account) then make an ajax request and display checkout_buying.html
    if request.method == "GET":
        try:
            # I Won't check again if "q" contain a value or not as it already has been checked for in the previous function ("/buy_shares") so checkout() depends on share() and if shares() was successful and "q" is good then "q" must aslo contain a value and checkout() will be executed, and on the other hand if "q" didnot contain a value in share() (or contain an invalid value) checkout() won't be executed in the first place
            q = request.args.get('quote')
            # "share" is an input from user, so there is might be user errors, that is why that is the only variable I will check for any potentioal input error from users
            shares_to_buy = int(request.args.get('shares'))
            quote = lookup(q)
            quote_symbol = quote["symbol"]
            quote_price = quote["price"]

            if shares_to_buy > 0:

                # Setting the type of transaction
                transaction_type = "Acquiring"

                trading_fees = 6
                # holding value = price per share * total number of shares
                holding_value = helpers.calculate_holding_value(quote_price, shares_to_buy)
                # payment total = holding value + trading fees
                total_amount =  helpers.calculate_total_amount(holding_value, trading_fees, transaction_type)
                # This extract amount of cash in user account
                current_account_cash = helpers.extract_current_cash_from_db(current_user_id)

                if not helpers.has_enough_cash(current_account_cash, total_amount):
                    error_message = f"Not enough cash to proceed with payment ! cash in account is {usd(current_account_cash)} and total payment amount is {usd(total_amount)}"
                    return _apology_(error_message, "0x02")

                # This will deducted the payment total from the user's cash, but will not yet update his cash in db (this will be done after he presses on (make purchase that will make a POST request to this route /checkout_for_buying))
                cash_after_transaction = helpers.calculate_cash_after_transaction(total_amount, current_account_cash, transaction_type)

                # Return a html template called checkout_buying.html and fill the paymnet info place holders in it with the necessary data (this is an AJAX request)
                # Notice that I used usd() on the variable that fills placeholders in the page, to make the number be currency
                return render_template("checkout_buying.html", quote_symbol=quote_symbol, quote_price=usd(quote_price), shares=shares_to_buy, holding_value=usd(holding_value), trading_fees=usd(trading_fees), total_amount=usd(total_amount), cash_after_transaction=usd(cash_after_transaction))
            # If the shares inputed by user is not a positive number
            else:
                raise ValueError

        except ValueError :
            error_message = "Please Enter a valid shares number !"
            return  _apology_(error_message, "0x03")

    # THis will handle the databse CRUD querires, (add user's newly purchased stock to his/her stocks/deduct the payment amount from his/her cash/save the transaction to database/update the cash on user's account/add stock symbol to stocks table with a unoque ID for each individual symbol
    if request.method == "POST":
        try:

            # This is a json data sent from the JS code located in checkout_buying.html(contains payment info, that was taken from text inside tags in the html page, then coverted to json and sent to server side)
            JSON_data = request.get_json()
            quote_symbol, quote_price, recently_acquired_shares, holding_value, trading_fees, total_amount, remaining_cash = helpers.extract_checkout_info(JSON_data)
            # helpers.extract_checkout_info() a function located in helpers.py that will extract the payment info from json data sent from client side by the javascript code in checkout_buying.html

            # Check if user has bought shares from the same organization/company
            # if true then update total shares, if false, then insert new row containing stock info
            user_stock_row = helpers.extract_user_stock_row_from_db(current_user_id, quote_symbol)

            if len(user_stock_row) >= 1:

                previously_acquired_shares = user_stock_row[0]['shares']

                total_acquired_shares = previously_acquired_shares + recently_acquired_shares

                helpers.update_current_user_stocks(total_acquired_shares, current_user_id, quote_symbol)

            elif len(user_stock_row) != 1:

                previously_acquired_shares = 0

                total_acquired_shares = previously_acquired_shares + recently_acquired_shares

                # This will try to extract stock_id from "stocks" then use that stock_id in insertion of the newly purchased stock into "users_stocks"
                # if an exeption raises like "UnboundLocalError" or "runtimeError" then that will mean that the stock symbole doesn't exist in "stocks", so after "except" execute "helpers.insert_stock_symbol_into_stocks_table(quote_symbol)", and use its return value "current_stock_id" in the insertion of the user's newly purchased stock into "users_stocks"
                try:
                    current_stock_id = helpers.select_stock_id_from_stocks_table(quote_symbol)
                except Exception:
                    current_stock_id = helpers.insert_stock_symbol_into_stocks_table(quote_symbol)
                finally:
                    helpers.insert_stock_into_users_stocks_table(current_user_id, current_stock_id, quote_symbol, total_acquired_shares)

            # deduct the payment amount from user's cash and update it
            helpers.update_cash_in_user_account(remaining_cash, current_user_id)

            # Select the unique stock symbol id from stocks, that will be used in the insetion query of the transaction into "transactions"
            current_stock_id = helpers.select_stock_id_from_stocks_table(quote_symbol)

            # Setting the type of transaction
            transaction_type = "Acquiring"

            # Get the current today's date, will be used in the insertion of the transaction
            current_date = datetime.now().date()

            # insert transaction into "transactions"
            # take the return value "transaction_id", and insert it in the flash message that will be displayed to the user
            # Notice that here we insert into transactions "recently_acquired_shares" and not "total_acquired_shares"
            transaction_id = helpers.insert_recent_transaction_into_transactions(current_user_id, current_stock_id, current_date, quote_symbol, quote_price, recently_acquired_shares, trading_fees, holding_value, total_amount, transaction_type)

            # a flash message that will dispaly payment total/total_acquired_shares/quote_symbol/transaction_id
            flash(f'A total amount of {usd(total_amount)} have been paid to procur {recently_acquired_shares} shares from {quote_symbol} -- Transaction ID is {transaction_id}', 'success')
            # Finally redirect the user to index() route (homepage)
            # I didn't use "redirect("/")" because this will be sent to the JS code in "checkout_buying.html", so it needs to be in JSON (as the JS code in client side is waiting for a "response", and it has to in JSON), so i used the method redirect_route() instead
            return jsonify({'success': True, 'redirect_route': url_for('index')})

        except Exception as e :
            # display an apology with error message if an exception raised that wasn't into consideration
            error_message = f'Error occured: {str(e)}'
            print(error_message)
            return jsonify({'success': False, 'erro_message': _apology_(error_message, "0x03")})


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    current_user_id = session["user_id"]

    # This will calculate the current total cash in account, total holdings value and  grand total, the it will be placed in the shared html of cash_shared_div.html (at the top right corner of screen, in all pages except login and register)

    # extract username from database using current_user_id = session["user_id"], to be used in the shared_div of user Info
    username = helpers.extract_username_from_db(current_user_id)

    # grand total = total cash in account + total holding values (each tocks shares time price per share)
    account_cash, total_holdings_value, grand_total  = helpers.calculate_current_account_totals(current_user_id)



    # extract the latest five transactions, ORDER BY date
    user_transactions = helpers.extract_latest_five_user_transactions(current_user_id)


    return render_template("history.html", user_transactions=user_transactions, username=username, account_cash=account_cash, total_holdings_value=total_holdings_value, grand_total=grand_total)


@app.route("/show_all_history")
@login_required
def show_all_history():

    current_user_id = session["user_id"]

    # extract all transactions ORDER BY date
    user_transactions = helpers.extract_all_user_transactions(current_user_id)

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
        # THis fetche the value of "success" var found in the url of the page (it was sent from register(), this is reposible of the alert() message)
        # If "success" was not found nothing will happen and it render the login.html without the alert() message
        success = request.args.get("success")
        return render_template("login.html", success=success)


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote")
@login_required
def quote():

    current_user_id = session["user_id"]

    # extract username from database using current_user_id = session["user_id"], to be used in the shared_div of user Info
    username = helpers.extract_username_from_db(current_user_id)

    # This will calculate the current total cash in account, total holdings value and  grand total, the it will be placed in the shared html of cash_shared_div.html (at the top right corner of screen, in all pages except login and register)

    # grand total = total cash in account + total holding values (each tocks shares time price per share)
    account_cash, total_holdings_value, grand_total  = helpers.calculate_current_account_totals(current_user_id)

    return render_template("quote.html", username=username, account_cash=account_cash, total_holdings_value=total_holdings_value, grand_total=grand_total)


@app.route("/search_quote")
@login_required
def search():
    q = request.args.get('q')
    # Assinging an empty {} to "q" will make the program run better, as it will didn't exist, the program will raise an error in the background, as it will not be defined on case if the user leaves the search filed in quote empty
    if q:
        quote = lookup(q)
        if quote:
            # I changed the value of quote[price] key, using usd() to review it as currency
            # I did this here becasue i coudlnot do it inside html or in JS (here is much easier)
            quote['price'] = usd(quote['price'])
    else:
        quote = {}

    return jsonify(quote)


@app.route("/register", methods=["GET", "POST"])
def register():
    # If usser reloads "register" by pressing on its link, it will , make a GET request and reloads the page
    if request.method == "GET":
        return render_template("register.html")

    # If user submits after filling account info, by the submit button found in form in "register.html", it will then make a POST request and start to validate the info of user
    elif request.method == "POST":
        username = request.form.get("username").strip()
        password = request.form.get("password").strip()
        password_confirm = request.form.get("confirmation").strip()
        # generate_password_hash() will generate the "hash" of password, and then it will stroe the hash and not the password itself
        hashed_password = generate_password_hash(password)

        # If left any field empty
        if not username or not password or not password_confirm:
            flash("Please fill in all fields!", "error")
            return redirect(url_for("register"))

        # IF two passwords fields don't match
        if password != password_confirm:
            flash("Passwords don't match! Please try again.", "error")
            return redirect(url_for("register"))

        # Checks if username already exists in database
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        if len(rows) == 1:
            flash("Username already exists! Please choose a new one.", "error")
            return redirect(url_for("register"))

        # Finally if all is good, insert the new user info into database
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hashed_password)

        # I couldn't use flash() becasue all flash messages will get erased in the beggining of /login --> session.clear(), So i got to find another way!, so i used alert() in JS instead
        # flash("Account has been created successfully! Please log in!", "success")

        # success="true" get added as a query in the url of login.html
        # this will activate alert() function in JS
        return redirect(url_for("login", success="true"))



@app.route("/sell")
@login_required
def sell():
    if request.method == "GET":

        current_user_id = session["user_id"]

        # extract username from database using current_user_id = session["user_id"], to be used in the shared_div of user Info
        username = helpers.extract_username_from_db(current_user_id)

        # This will calculate the current total cash in account hodlings value total and grand total, the it will be placed in the shared html of cash_shared_div.html (at the top right corner of screen, in all pages except login and register)
        account_cash, total_holdings_value, grand_total  = helpers.calculate_current_account_totals(current_user_id)

        return render_template("sell.html", username=username, account_cash=account_cash, total_holdings_value=total_holdings_value, grand_total=grand_total)


@app.route("/sell_shares")
@login_required
def sell_shares():

    current_user_id = session["user_id"]

    # This will fetch a stock dict (name, price, symbol), and also check for user errors like empty feild or invalid stock symbol, if all is good and user entered a valid symbol, then an ajax request will happen and shares input feild will appear to take number of shares from user
    # This will happen after a GET request, no POST requests here
    quote_symbol = request.args.get('quote').strip().upper()

    # To check if user didnot write anything
    if quote_symbol:

        # This will extract all the users stocks ids from database
        owned_stocks_ids_list = helpers.extract_user_stocks_ids_from_users_stocks_table(current_user_id)

        # This will extract the unique stock id of the entered stock symbol from our data base, to be used in the comparison between it and the user stocks ids in database
        entered_stock_id = helpers.select_stock_id_from_stocks_table(quote_symbol)

        # if the entered_stock_id is in owned_stocks_ids_list, then make an ajax request to display the shares input field
        if entered_stock_id:
            entered_stock_id = entered_stock_id
            for owned_stocks_id_dict in owned_stocks_ids_list:
                if owned_stocks_id_dict['stock_id'] == entered_stock_id:
                    return render_template("sell_shares.html")
            # If the loop finishes without accessing "return" then that would mean that the entered stock doesnot equal to stock_id of which the user owns !, so retrun an apology
            error_message = "Please Enter a Stock Quote Symbol that you own!"
            return  _apology_(error_message, "0x04")
        else:
            # if not then render an apology
            error_message = "Please Enter a Stock Quote Symbol that you own!"
            return  _apology_(error_message, "0x04")
    else:
        # if the user leaes the stock quote symbol input field empty
        error_message = "Please Enter a Stock Quote Symbol!"
        return  _apology_(error_message, "0x05")



# This route will be activated via GET and POST
# if GET : display checkout_selling.html via an AJAX request to process client selling
# If POST : Remove the entered number of shares from the total shares of the entered stock quote symbol in database, then update cash of account and finally redirect user to home page
@app.route("/checkout_selling", methods=["GET", "POST"])
@login_required
def checkout_selling():

    current_user_id = session["user_id"]


    # This will calculate payment info and then validate data (positive number of shares entered by user/shares number which is less than or equal to shares number that he owns) then finally make an ajax request and display checkout_buying.html
    if request.method == "GET":
        try:

            # This will be displayed in the "_apology_()" image, if a valueError raises
            error_message = "Please Enter a valid shares number !"

            q = request.args.get('quote').strip().upper()
            shares_to_sell = int(request.args.get('shares'))
            quote = lookup(q)
            quote_symbol = quote["symbol"]
            quote_price = quote["price"]

            # Extract shares from databse, to be used in the cmparison between the entered shares the actual shares user owns
            shares_owned = helpers.extract_shares_owned_by_user(current_user_id, quote_symbol)

            # compare shares in users account with entered shares
            if shares_to_sell > 0 and shares_to_sell <= shares_owned:

                # Setting the type of transaction
                transaction_type = "Dissolsving"

                trading_fees = 6
                # holding value = price per share * total number of shares
                holding_value = helpers.calculate_holding_value(quote_price, shares_to_sell)
                # total amount = holding value - trading fees
                total_amount =  helpers.calculate_total_amount(holding_value, trading_fees, transaction_type)
                # extract amount of cash in user account
                current_account_cash = helpers.extract_current_cash_from_db(current_user_id)

                cash_after_transaction = helpers.calculate_cash_after_transaction(total_amount, current_account_cash, transaction_type)

                # Return a html template called checkout_selling.html and fill the paymnet info place holders in it with the necessary data (this is an AJAX request)
                # Notice that I used usd() on the variable that fills placeholders in the page, to make the number be currency
                return render_template("checkout_selling.html", quote_symbol=quote_symbol, quote_price=usd(quote_price), shares=shares_to_sell, holding_value=usd(holding_value), trading_fees=usd(trading_fees), total_amount=usd(total_amount), cash_after_transaction=usd(cash_after_transaction))

            # If the shares inputed by user is not a positive number or is bigger than shares he/she owns
            else:
                error_message = "Number of shares must be less than or equal to shares you own!"
                raise ValueError

        # A ValueError will raise, if shares were not a number
        except ValueError :
            return  _apology_(error_message, "0x03")

    # THis will handle the databse CRUD querires, (add user's newly purchased stock to his/her stocks, add the total amount to his/her cash/save the transaction to database, update the cash on user's account.
    if request.method == "POST":
        try:

            # This is a json data sent from the JS code located in checkout_selling.html(contains checkout info, that was taken from text inside tags in the html page, then coverted to json and sent to server side)
            JSON_data = request.get_json()

            # helpers.extract_checkout_info() a function located in helpers.py that will extract the checkout info from json data sent from client side by the javascript code found in checkout_selling.html
            quote_symbol, quote_price, shares_to_sell, holding_value, trading_fees, total_amount, cash_after_selling = helpers.extract_checkout_info(JSON_data)

            # extract total previously bought shares from database
            previously_acquired_shares = helpers.extract_shares_owned_by_user(current_user_id, quote_symbol)

            # Calculate the remaining shares of the stock after the selling process
            remaining_shares = previously_acquired_shares - shares_to_sell

            # Checks whether user has selled all shares of the stock or some of it
            if remaining_shares > 0:
                # if the remaining shares is is more than zero that means the hther is still left shares, so just UPDATE the shars in database
                helpers.update_current_user_stocks(remaining_shares, current_user_id, quote_symbol)
                flash_message = (f'A total amount of {usd(total_amount)} have been added to your cash as a result of selling {shares_to_sell} shares from {quote_symbol}')
            else:
                # However, if the remaining shares is zero, DELETE the whole row of the stock from users_stocks
                db.execute("DELETE FROM users_stocks WHERE user_id = ? AND stock_symbol = ?", current_user_id, quote_symbol)

                # flash_message var holds the message that will be inserted into flash() Fn at the end, then it will be displayed at the top of page as a feedback after redirceting the user to index.html page
                flash_message = (f'Your account has been credited with {usd(total_amount)} due to the successful sale of all {shares_to_sell} shares of the {quote_symbol} stock')

            # Add total amount of income from the selling process to user's cash
            helpers.update_cash_in_user_account(cash_after_selling, current_user_id)

            # Selecting the unique stock_symbol id from stocks, that will be used in the insertion query of the transaction into "transactions"
            current_stock_id = helpers.select_stock_id_from_stocks_table(quote_symbol)

            # defining the type of transaction, this will be used in the INSERT query to distinguish buying shares from selling shares.
            transaction_type = "Dissolsving"

            # Fetch the current today's date
            current_date = datetime.now()

            # insert the recent transaction into "transactions"
            # take the return value "transaction_id", and insert it in the flash message that will be displayed to the user
            # Notice that here that we inserted into transactions "shares_to_sell" and not "remaining_shares"

            # chaneg recently_acquired shares -- to do
            transaction_id = helpers.insert_recent_transaction_into_transactions(current_user_id, current_stock_id, current_date, quote_symbol, quote_price, shares_to_sell, trading_fees, holding_value, total_amount, transaction_type)

            # A flash message that will dispaly total amount acquired/selled shares/quote_symbol/transaction_id
            flash(f'{flash_message} -- Transaction ID is {transaction_id}', 'success')

            # Finally redirect the user to index() route (homepage)
            # I didn't use "redirect("/")" because this will be sent to the JS code in "checkout_selling.html", so it needs to be in JSON (as the JS code in client side is waiting for a "response", and it has to in JSON), so i used the method redirect_route() instead
            return jsonify({'success': True, 'redirect_route': url_for('index')})

        except Exception as e :
            # display an apology with error message if an exception raised that wasn't into consideration
            error_message = f'Error occured: {str(e)}'
            print(error_message)
            return jsonify({'success': False, 'erro_message': _apology_(error_message, "0x03")})
