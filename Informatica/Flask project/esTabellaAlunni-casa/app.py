from flask import Flask

app = Flask(__name__)

@app.route('/')
def home():
  return 'Qui ci andrà la pagina di home. Ci andrà la tabella degli alunni'

@app.route('/alunni/<alunno>')
def alunni(alunno):
  return f'Qui ci andrà la pagina dell\'alunno {alunno}'

if __name__ == '__main__':
  app.run(debug=True)