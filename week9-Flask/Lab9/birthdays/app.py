import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session, flash, url_for





                                    # The First solution (simpler and cleaner)
# Configure application
app = Flask(__name__)

# Needed for flash messages
app.secret_key = "secret_key"

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

# THis function will be used when user submit a birthday
# It will check whether the user has typed a text inside the "month" or "day" input fields (Note: He is unable to type a text in the input field without manipulating the page's HTML code using developer tools, so this is one way to validate user inputs and prevent manipulation.)
def is_valid_date(month, day):
    try:
        month = int(month)
        day = int(day)
        return 1 <= month <= 12 and 1 <= day <= 31
    except ValueError:
        return False

# This Will actully do one thing and that is the executiong the insertion query
def add_birthday_to_db(name, month, day):
    db.execute("INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)", name, month, day)


# This Will actully do one thing and that is the executiong the deletion query
def delete_birthday_from_db(birthday_id):
    db.execute("DELETE FROM birthdays WHERE id = ?", birthday_id)


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# I have change the GET and POST routing strurctue to be seperate @app.route(), so one for GET and the other for POST
# This will be called when the user reloads the page
@app.route("/", methods=["GET"])
def display_birthdays():
    # Display the entries in the database on index.html
    birthdays = db.execute("SELECT * FROM birthdays")
    return render_template("index.html", birthdays=birthdays)



# This will be called when the user presses on "Add" submit button found in the page
@app.route("/add_birthday", methods=["POST"])
def validate_birthday_then_add():
    # Validate and handle form submission
    name = request.form.get("name")
    month = request.form.get("month")
    day = request.form.get("day")

    # Checks if user leaves any of "name" or "month" or "day" empty
    # If he submited with an empty field the page willthen reloads and an error message will appear !
    # IF all is good then the birthday will be added and a success message will appear when the page reloads automaticaly
    if name and month and day:
        # This validates first that the user entered the correct formate a for every filed
        if is_valid_date(month, day):
            add_birthday_to_db(name, month, day)
            flash("Birthday entry added successfully.", "success")
        else:
            flash("Invalid month or day.", "error")
    else:
        flash("Please fill in all the fields.", "error")

    return redirect(url_for("display_birthdays"))


# If it was the "Remove" button to delete a birthday from the list, this will be called
@app.route("/remove_birthday", methods=["POST"])
def validate_birthday_then_delete():
    birthday_id = request.form.get("id")
    if birthday_id:
        delete_birthday_from_db(birthday_id)
        # A message will be displayed at the top of page after the pages reloads
        flash("Birthday entry deleted successfully.", "success")
        return redirect(url_for("display_birthdays"))





                                        # The second solution


# import os

# from cs50 import SQL
# from flask import Flask, flash, jsonify, redirect, render_template, request, session, render_template_string, flash

# # Configure application
# app = Flask(__name__)

# # Needed for flash messages
# app.secret_key = "secret_key"

# # Ensure templates are auto-reloaded
# app.config["TEMPLATES_AUTO_RELOAD"] = True

# # Configure CS50 Library to use SQLite database
# db = SQL("sqlite:///birthdays.db")


# @app.after_request
# def after_request(response):
#     """Ensure responses aren't cached"""
#     response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
#     response.headers["Expires"] = 0
#     response.headers["Pragma"] = "no-cache"
#     return response


# @app.route("/", methods=["GET", "POST"])
# def index():
#     if request.method == "POST":

#         # TODO: Add the user's entry into the database
#         # Validate submission

#         name = request.form.get("name")
#         month = request.form.get("month")
#         day = request.form.get("day")
#         id = request.form.get("id")

#         if id:
#             db.execute("DELETE FROM birthdays WHERE id = ?", id)
#             return redirect("/")

#         elif name and month and day:
#             # Assuming month and day should be integers between 1 and 12, and 1 and 31 respectively
#             try:
#                 month = int(month)
#                 day = int(day)

#                 if not 1 <= month <= 12 or not 1 <= day <= 31:
#                     flash("Invalid month or day.", "error")
#                     return redirect("/")

#             except ValueError:
#                 flash("Invalid input for month or day.", "error")
#                 return redirect("/")

#             db.execute("INSERT INTO birthdays (name, month, day) VALUES(?, ?, ?)", name, month, day)
#             return redirect("/")

#         elif not name or not month or not day:
#             flash("Please fill in all the fields.", "error")
#             return redirect("/")


#     else:

#         # TODO: Display the entries in the database on index.html
#         birthdays = db.execute("SELECT * FROM birthdays")
#         return render_template("index.html", birthdays=birthdays)


# # This will be called when the user presses on ony submit button found in the page
# @app.route("/add_birthday", methods=["POST"])
# def validate_birthday_then_add():
#     # Validate and handle form submission
#     name = request.form.get("name")
#     month = request.form.get("month")
#     day = request.form.get("day")

#     # And if all fi
#     if name and month and day:
#         if is_valid_date(month, day):
#             add_birthday_to_db(name, month, day)
#             flash("Birthday entry added successfully.", "success")
#         else:
#             flash("Invalid month or day.", "error")
#     else:
#         flash("Please fill in all the fields.", "error")

#     return redirect(url_for("display_birthdays"))



#     # If it was the "Remove" button to delete a birthday from the list, this will be called
# @app.route("/remove_birthday", methods=["POST"])
# def validate_birthday_then_delete():
#     birthday_id = request.form.get("id")
#     if birthday_id:
#         delete_birthday_from_db(birthday_id)
#         # A message will be displayed at the top of page after the pages reloads
#         flash("Birthday entry deleted successfully.", "success")
#         return redirect(url_for("diseldsplay_birthdays"))