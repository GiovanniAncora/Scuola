from flask import Flask

app = Flask(__name__)

@app.route("/")
def index():
  return "Main page."

@app.route("/<nome>")
def ciao(nome):
  return f"Ciao {nome}"

if __name__ == '__main__':
  app.run(debug=True)