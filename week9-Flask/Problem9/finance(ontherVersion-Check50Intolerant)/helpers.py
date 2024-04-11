import csv
# This used to get the current date-time
import datetime
# This used to get time zones
import pytz
import requests
import subprocess
# We will be using .parse.quote_plus() method from that module, which will make any str as URL friendly str (it will replace specific characters with other friendly characters in order to put that str inside a URL)
import urllib
# This is used in cookies={"session": str(uuid.uuid4()) to create a random 128 bit number, that will unique cookies session ID
import uuid
from cs50 import SQL

from flask import redirect, render_template, session
# This is used inside of @ deceorator functions, to not not make the deceorated functins lose its __name__
from functools import wraps

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

def apology(message, code=400):
    """Render message as an apology to user."""
    # This functions will replace some unfreindly URL characters with other friendly URL characters (if found in the error message) as the error messsage will be written inside of a URL
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
                         ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=code, bottom=escape(message)), code

def login_required(f):
    """
    Decorate routes to require login.

    http://flask.pocoo.org/docs/0.12/patterns/viewdecorators/
    """
    # This supposed to be @functools.wraps(f), this keeps the name and docstring of "f" function passed in decorated_function(*args, **kwargs), so without @functools.wraps(f), f.__name__ would change completely to decorated_function() instead of its correct __name__ which is in our case (login() or index(), history() ...etc)
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function


def lookup(symbol):
    """Look up quote for symbol."""

    # Prepare API request
    symbol = symbol.upper()
    # This will get the current date-time object of the US/Eastern time zone
    end = datetime.datetime.now(pytz.timezone("US/Eastern"))
    # datetime.timedelta(days=7) will return # 7 days, 0:00:00
    # And when you substract it from "end" (current date) it will get the date of the day from a week ago
    start = end - datetime.timedelta(days=7)

    # Yahoo Finance API
    url = (
        # .parse.quote_plus() method from that module, which will make any str as URL friendly str (it will replace specific characters with other friendly characters in order to put that str inside a URL)
        f"https://query1.finance.yahoo.com/v7/finance/download/{urllib.parse.quote_plus(symbol)}"
        # .timestamp() converts the date-time into seconds (seconds that have been elapsed since the Unix epoch (January 1, 1970))
        # Eaiser to work the Unix formate (sorting, comparing, storing ...etc)
        f"?period1={int(start.timestamp())}"
        f"&period2={int(end.timestamp())}"
        f"&interval=1d&events=history&includeAdjustedClose=true"
    )

    # Query API
    try:
        # str(uuid.uuid4()) creates  a unique 128 bit number, that  wil  be the cookies session ID
        # 1.	"User-Agent": "python-requests":
        # •	The User-Agent header is used to identify the client (the user agent) making the request.
        # •	In this case, the value "python-requests" is used as the user agent, indicating that the request is coming from the requests library in Python.
        # 2.	"Accept": "*/*":
        # •	The Accept header specifies the media types (content types) that the client (Python script) can handle or understand in the response.
        # •	The value "*/*" is a wildcard and indicates that the client can accept any content type.
        response = requests.get(url, cookies={"session": str(uuid.uuid4())}, headers={"User-Agent": "python-requests", "Accept": "*/*"})
        response.raise_for_status()

        # CSV header: Date,Open,High,Low,Close,Adj Close,Volume
        quotes = list(csv.DictReader(response.content.decode("utf-8").splitlines()))
        quotes.reverse()
        price = round(float(quotes[0]["Adj Close"]), 2)
        return {
            "name": symbol,
            "price": price,
            "symbol": symbol
        }
    except (requests.RequestException, ValueError, KeyError, IndexError):
        return None


def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"


                                                # CUSTOM MADE FUNCTION CREATED BY ME (Tariq Sarhan)

# I used revert_usd() (custom made function by me ), to make numbers formated as currency revert back to normal numbers (numbers without "$" and ","), and that is necessary becasue in the JS code in "checkout.html", as I take the TEXT of html <>tags as my values in the server side, so if I take the text as currency formated numbers like for ex: ($1,275.10) then in app.py (server side) I try to covert that string to float it will raise an error !, becasue it cannot convert '$' and ',' into float numbers !, so this custom made revert_usd() removes '$'and ',' from string inputed into it
def revert_usd(value):
    """Format value as a reguler number."""
    return value.replace('$', '').replace(',', '')

def calculate_holding_value(quote_price, recently_acquired_shares):
    return float(quote_price * recently_acquired_shares)

def calculate_total_amount(holding_value, trading_fees, transaction_type):
    if transaction_type == "Acquiring":
        return float(holding_value + trading_fees)
    elif transaction_type == "Dissolsving":
        return float(holding_value - trading_fees)

def extract_username_from_db(current_user_id):
    return db.execute("SELECT username FROM users WHERE id = ?", current_user_id)[0]['username']

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

def has_enough_cash(current_account_cash, total_amount):
    return current_account_cash > total_amount

def calculate_cash_after_transaction(total_amount, current_account_cash, transaction_type):
    if transaction_type == "Acquiring":
        return current_account_cash - total_amount
    elif transaction_type == "Dissolsving":
        return current_account_cash + total_amount

def extract_checkout_info(JSON_data):
    quote_symbol =  JSON_data['quoteSymbol'].strip()
    # Refere to revert_usd() definition in above for explaination
    quote_price = float(revert_usd(JSON_data['quotePrice']))
    recently_acquired_shares = int(revert_usd(JSON_data['shares']))
    holding_value = float(revert_usd(JSON_data['holdingValue']))
    trading_fees = float(revert_usd(JSON_data['tradingFees']))
    total_amount = float(revert_usd(JSON_data['totalAmount']))
    cash_after_transaction = float(revert_usd(JSON_data['cashAfterTransaction']))

    return quote_symbol, quote_price, recently_acquired_shares, holding_value, trading_fees, total_amount, cash_after_transaction


def extract_user_stock_row_from_db(current_user_id, quote_symbol):
    return db.execute("SELECT * FROM users_stocks WHERE user_id = ? AND stock_symbol = ?", current_user_id, quote_symbol)

def update_cash_in_user_account(remaining_cash, current_user_id):
    db.execute("UPDATE users SET cash = ? WHERE id = ?", remaining_cash, current_user_id)

def update_current_user_stocks(total_shares, current_user_id, quote_symbol):
    db.execute("UPDATE users_stocks SET shares = ? WHERE user_id = ? AND stock_symbol = ?", total_shares, current_user_id, quote_symbol)

def insert_stock_symbol_into_stocks_table(quote_symbol):
    return db.execute("INSERT OR IGNORE INTO stocks (stock_symbol) VALUES (?)", (quote_symbol))

def insert_stock_into_users_stocks_table(current_user_id, current_stock_id, quote_symbol, shares):
    db.execute("INSERT INTO users_stocks (user_id, stock_id, stock_symbol, shares) VALUES (?, ?, ?, ?)", current_user_id, current_stock_id, quote_symbol, shares)

def select_stock_id_from_stocks_table(quote_symbol):
    try:
        return db.execute("SELECT stock_id FROM stocks WHERE stock_symbol = ?", quote_symbol)[0]['stock_id']
    except IndexError:
        return None

def insert_recent_transaction_into_transactions(current_user_id, current_stock_id, current_date, quote_symbol, quote_price, recently_acquired_shares, trading_fees, holding_value, total_amount, transaction_type):
    return db.execute("INSERT INTO transactions (user_id, quote_id, transaction_date, quote_symbol, price_per_share, shares, trading_fees, holding_value, total_amount, transaction_type) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", current_user_id, current_stock_id, current_date, quote_symbol, quote_price, recently_acquired_shares, trading_fees, holding_value, total_amount, transaction_type)

def extract_user_stocks_ids_from_users_stocks_table(current_user_id):
    return db.execute("SELECT stock_id FROM users_stocks WHERE user_id = ?", current_user_id)

def _apology_(message, code=400):
    """Render message as an apology to user."""
    # This functions will replace some unfreindly URL characters with other friendly URL characters (if found in the error message) as the error messsage will be written inside of a URL
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
                         ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("shared_apology.html", top=code, bottom=escape(message))

def extract_user_stocks_from_db(current_user_id):
    return db.execute("SELECT ROW_NUMBER() OVER (ORDER BY shares DESC) AS RowNumber, stock_symbol, shares FROM users_stocks WHERE user_id = ?", current_user_id)

def extract_shares_owned_by_user(current_user_id, quote_symbol):
    return db.execute("SELECT shares FROM users_stocks WHERE user_id = ? AND stock_symbol = ?", current_user_id, quote_symbol)[0]['shares']

def extract_latest_five_user_transactions(current_user_id):
    return db.execute("SELECT ROW_NUMBER() OVER (ORDER BY transaction_date DESC) AS RowNumber, transaction_id, transaction_date, quote_symbol, price_per_share, shares, holding_value, trading_fees, total_amount, transaction_type FROM transactions WHERE user_id = ? LIMIT 5", current_user_id)

def extract_all_user_transactions(current_user_id):
    return db.execute("SELECT ROW_NUMBER() OVER (ORDER BY transaction_date DESC) AS RowNumber, transaction_id, transaction_date, quote_symbol, price_per_share, shares, holding_value, trading_fees, total_amount, transaction_type FROM transactions WHERE user_id = ?", current_user_id)

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