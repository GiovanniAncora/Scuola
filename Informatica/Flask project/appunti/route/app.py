from flask import Flask
from markupsafe import escape

app = Flask(__name__)

@app.route("/")
def index():
  return "Main page."

@app.route("/<nome>")
def ciao(nome):
  return f"Ciao {escape(nome)}"

if __name__ == '__main__':
  app.run(debug=True)