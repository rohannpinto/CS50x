import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime
from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

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
    """Show portfolio of stocks"""
    user_purchase_rows = db.execute("SELECT SUM(purchases.shares) AS totalshares, purchases.symbol AS symbol, users.cash AS cash FROM purchases JOIN users ON purchases.username = users.id WHERE purchases.username = ? GROUP BY symbol", session["user_id"])
    rows = []
    try:
        grandtotal = float(user_purchase_rows[0]["cash"])
    except:
        grandtotal = 0
    for row in user_purchase_rows:
        rowinfo = dict()
        rowinfo["totalshares"] = row["totalshares"]
        lookupinfo = lookup(row["symbol"])
        rowinfo["price"] = usd(lookupinfo["price"])
        rowinfo["name"] = lookupinfo["name"]
        rowinfo["symbol"] = lookupinfo["symbol"]
        rowinfo["total"] = usd(round(float(lookupinfo["price"]) * float(row["totalshares"]), 2))
        grandtotal = grandtotal + round(float(lookupinfo["price"]) * float(row["totalshares"]), 2)
        rowinfo["grandtotal"] = grandtotal
        rowinfo["grandtotal"] = usd(rowinfo["grandtotal"])
        rowinfo["cash"] = row["cash"]
        rowinfo["cash"] = usd(rowinfo["cash"])
        rows.append(rowinfo)
    return render_template("index.html", rows = rows)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    #add symbol, shares to purchases table
    if request.method == "GET":
        return render_template("buy.html")
    elif request.method == "POST":
        symbol = request.form.get("symbol")
        try:
            shares = float(request.form.get("shares"))
        except:
            return apology("not a valid value")
        if not shares % 1 == 0:
            return apology("provide a symbol and share amount")
        if not symbol or not shares:
            return apology("provide a symbol and share amount")
        if not shares > 0:
            return apology("please provide a positive share amount")
        result = lookup(symbol)
        if result == None:
            return apology("provide a valid symbol")
        price = result["price"]
        if not result or not price:
            return apology("provide a valid symbol")
        amount = price * shares
        amount = round(amount, 2)
        if amount:
            cashrows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
            oldcash = cashrows[0]["cash"]
            newcash = float(oldcash) - float(amount)
            newcash = round(newcash, 2)
            if newcash < 0:
                return apology("need more cash")
            db.execute("UPDATE users SET cash = ? WHERE id = ?", newcash, session["user_id"])
            current_time = datetime.now()
            db.execute("INSERT INTO purchases (username, amount, symbol, shares, datetime) VALUES (?,?,?,?,?)", session["user_id"], amount, symbol, shares, current_time)
            return redirect("/")
        else:
            return apology("price doesn't exist or share amount not valid")
    return apology("TODO")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_purchase_rows = db.execute("SELECT shares , symbol , amount, datetime FROM purchases WHERE username = ?", session["user_id"])
    rows = []
    for row in user_purchase_rows:
        rowinfo = dict()
        rowinfo["shares"] = row["shares"]
        rowinfo["symbol"] = row["symbol"]
        rowinfo["amount"] = row["amount"]
        rowinfo["amount"] = usd(rowinfo["amount"])
        if int(row["amount"]) > 0:
            rowinfo["type"] = "Buy"
        else:
            rowinfo["type"] = "Sell"
        rowinfo["datetime"] = row["datetime"]
        rows.append(rowinfo)
    return render_template("history.html", rows = rows)


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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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
    if request.method == "GET":
        return render_template("quote.html")
    elif request.method == "POST":
        try:
            symbol = request.form.get("symbol")
            if not symbol:
                return apology("price or symbol doesn't exist")
            result = lookup(symbol)
            price = result["price"]
            price = usd(float(price))
            if price:
                return render_template("quoted.html", price=price )
            else:
                return apology("price or symbol doesn't exist")
        except:
            return apology("price or symbol doesn't exist")

    return apology("quote TODO")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    elif request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if not username or not password or not confirmation or not password == confirmation:
            return apology("username and password details are not correct")
        try:
            hashpw = generate_password_hash(password)
            db.execute("INSERT INTO users (username, hash) VALUES (?,?)", username, hashpw)
            return redirect("/")
        except:
            return apology("username already exists")
    return apology("REGISTER TODO")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        stocks = db.execute("SELECT symbol FROM purchases WHERE username = ? GROUP BY symbol", session["user_id"])
        return render_template("sell.html", stocks = stocks)
    elif request.method == "POST":
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        if not symbol or not shares:
            return apology("provide a symbol and share amount")
        if not shares > 0:
            return apology("please provide a positive share amount")
        shares = -shares
        if shares:
            totalsharesdb = db.execute("SELECT SUM(shares) AS totalshares FROM purchases WHERE username = ? AND symbol = ?", session["user_id"], symbol)
            try:
                totalshares = totalsharesdb[0]["totalshares"]
            except:
                return apology("not a valid symbol")
            if totalshares + shares < 0:
                return apology("need more shares")
        result = lookup(symbol)
        price = result["price"]
        if not result or not price:
            return apology("provide a valid symbol")
        amount = price * shares
        if amount:
            cashrows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
            oldcash = cashrows[0]["cash"]
            newcash = oldcash - amount
            db.execute("UPDATE users SET cash = ? WHERE id = ?", newcash, session["user_id"])
            current_time = datetime.now()
            db.execute("INSERT INTO purchases (username, amount, symbol, shares, datetime) VALUES (?,?,?,?,?)", session["user_id"], amount, symbol, shares, current_time)
            return redirect("/")
        else:
            return apology("amount calculation error")
    return apology("SELL TODO")

@app.route("/balance", methods=["GET", "POST"])
@login_required
def balance():
    """Add cash to balance"""
    if request.method == "GET":
        balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        balance = int(balance[0]["cash"])
        return render_template("balance.html", balance = balance)
    elif request.method == "POST":
        add = request.form.get("add")
        add = int(add)
        balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        balance = int(balance[0]["cash"])
        if not add:
            return apology("provide an amount to add")
        if not add > 0:
            return apology("please provide a positive add amount")
        if add:
            newbalance = balance + add
            db.execute("UPDATE users SET cash = ? WHERE id = ?", newbalance, session["user_id"])
            return redirect("/")
        else:
            return apology("add amount not valid")
    return apology("Balance TODO")
