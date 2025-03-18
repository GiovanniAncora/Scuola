from flask import Flask, session, redirect, url_for, request, render_template_string, flash

app = Flask(__name__)
app.secret_key = "provola"

@app.route("/")
def index():
  if 'username' in session:
    return f'Ciao, {session[session]}!</br></br><a href="{url_for("logout")}">Logout</a>'
  return 'Non sei loggato.</br>Esegui il <a href="/login">login</a>'