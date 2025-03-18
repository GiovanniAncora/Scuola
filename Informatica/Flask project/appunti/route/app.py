from flask import Flask
from markupsafe import escape

app = Flask(__name__)

@app.route("/")
def index():
  return "Main page."

@app.route("/saluti/<nome>")
def ciao(nome):
  return f"Ciao <b>{escape(nome)}</b>"

if __name__ == '__main__':
  app.run(debug=True)