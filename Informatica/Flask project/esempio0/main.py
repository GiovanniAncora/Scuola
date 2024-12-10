from flask import Flask

app = Flask(__name__)

@app.route('/')
def index():
    return 'Hello from Flask!'

@app.route('/prova')
def prova():
    return '<h1>Pagina di prova.</h1>'

app.run(host='localhost', debug=True)