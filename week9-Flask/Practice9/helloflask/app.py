from flask import Flask, render_template, request

app = Flask(__name__)

# Define a list of valid color values
VALID_COLORS = ["red", "blue"]

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "GET":
        return render_template("index.html")
    else:
        print("Form submitted!")
        color = request.form.get("color")

        # Validate the color input
        if color in VALID_COLORS:
            return render_template("color.html", color=color)
        else:
            # If the color is not valid, handle the error (display an error message)
            error_message = "Invalid color selection. Please choose a valid color."
            return render_template("error.html", message=error_message)
