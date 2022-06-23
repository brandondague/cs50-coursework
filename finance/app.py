import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


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
    """Show portfolio of stocks"""
    user = session["user_id"]
    user_data = db.execute("SELECT * FROM users JOIN stocks ON users.id=stocks.id WHERE users.id=?", user)
    if not user_data:
        user_data = db.execute("SELECT * FROM users WHERE id=?", user)
        user_data[0]["cash"] = usd(user_data[0]["cash"])
        return render_template("index.html", user_data=user_data)
    account_total = user_data[0]["cash"]
    for row in user_data:
        account_total+=row["total"]
        row["cash"] = usd(row["cash"])
        row["price"] = usd(row["price"])
        row["total"] = usd(row["total"])
    account_total = usd(account_total)
    return render_template("index.html", user_data=user_data, account_total=account_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must enter a stock symbol", 400)
        elif not request.form.get("shares"):
            return apology("must enter the number of shares you want to purchase", 400)

        #query the input shares
        symbol = request.form.get("symbol").upper()
        quote = lookup(symbol)
        user_id = session["user_id"]
        price = quote["price"] * float(request.form.get("shares"))

        #if no results returned return apology
        if not quote:
            return apology("there are no stocks with that symbol", 400)

        #check if there's enough funds in the users account
        funds = db.execute("SELECT cash FROM users WHERE id=?", user_id)
        if quote["price"] > funds[0]["cash"]:
            return apology("insufficient funds", 400)

        #update tables appropriately
        if not db.execute("SELECT * FROM stocks WHERE id=? AND name=?", user_id, quote["name"]):
            db.execute("INSERT INTO stocks (id, name, symbol, price, shares, total) VALUES (?, ?, ?, ?, ?, ?)", user_id, quote["name"], quote["symbol"], quote["price"], request.form.get("shares"), price)
            db.execute("INSERT INTO transact (id, symbol, change, price) VALUES (?, ?, ?, ?)", user_id, quote["symbol"], request.form.get("shares"), quote["price"])
            db.execute("UPDATE users SET cash=cash-? WHERE id=?", price, user_id)
            flash("Bought!")
            return redirect("/")
        else:
            db.execute("UPDATE stocks SET price=?, shares=shares+?, total=total+? WHERE id=? AND name=?", quote["price"], request.form.get("shares"), price, user_id, quote["name"])
            db.execute("INSERT INTO transact (id, symbol, change, price) VALUES (?, ?, ?, ?)", user_id, quote["symbol"], request.form.get("shares"), quote["price"])
            db.execute("UPDATE users SET cash=cash-? WHERE id=?", price, user_id)
            flash("Bought!")
            return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    history = db.execute("SELECT * FROM transact WHERE id=?", user_id)
    for row in history:
        row["price"] = usd(row["price"])
    return render_template("history.html", history=history)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username").lower())

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
    """Get stock quote."""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide a stock symbol", 400)

        symbol = request.form.get("symbol").upper()
        quote = lookup(symbol)

        if not quote:
            return apology("there are no stocks with that symbol", 400)

        quote["price"] = usd(quote["price"])

        return render_template("quoted.html", quote=quote)

    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not username:
            return apology("must provide username", 400)
        elif not password:
            return apology("must provide password", 400)
        elif not confirmation:
            return apology("must re-enter password", 400)
        elif password != confirmation:
            return apology("passwords must match", 400)
        elif db.execute("SELECT * FROM users WHERE username=?", username):
            return apology("that username is taken", 400)


        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username.lower(), generate_password_hash(password))

        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username").lower())

        session["user_id"] = rows[0]["id"]

        flash("You account was successfully registered!")

        return redirect("/")

    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user = session["user_id"]
    user_data = db.execute("SELECT * FROM users JOIN stocks ON users.id=stocks.id WHERE users.id=?", user)
    if not user_data:
        user_data = db.execute("SELECT * FROM users WHERE id=?", user)
    if request.method == "POST":
        sold = float(request.form.get("shares"))
        signed_sold = sold - (sold * 2)
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("enter a symbol", 400)
        if not sold:
            return apology("enter an amount", 400)
        for row in user_data:
            if row["symbol"] == symbol:
                price = sold * row["price"]
                if sold > row["shares"]:
                    return apology("you entered more shares than are in your account", 400)
                elif sold < row["shares"]:
                    db.execute("UPDATE stocks SET shares=shares-?, total=total-? WHERE symbol=? AND id=?", sold, price, symbol, user)
                    db.execute("INSERT INTO transact (id, symbol, change, price) VALUES (?, ?, ?, ?)", user, symbol, signed_sold, price)
                    db.execute("UPDATE users SET cash=cash+? WHERE id=?", price, user)
                    flash("Sold!")
                    return redirect("/")
                elif sold == row["shares"]:
                    db.execute("UPDATE users SET cash=cash+? WHERE id=?", price, user)
                    db.execute("INSERT INTO transact (id, symbol, change, price) VALUES (?, ?, ?, ?)", user, symbol, signed_sold, price)
                    db.execute("DELETE FROM stocks WHERE id=? AND symbol=?", user, symbol)
                    flash("Sold!")
                    return redirect("/")
                else:
                    return apology("there was an error, please try again", 400)
            else:
                return apology("enter a symbol", 400)
    else:
        return render_template("sell.html", user_data=user_data)