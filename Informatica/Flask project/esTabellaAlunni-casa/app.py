from flask import Flask, render_template

app = Flask(__name__)


@app.route('/')
def home():
  intestazioni = ['Matricola', 'Nome', 'Cognome', 'Data di nascita']
  alunni = [
    ['1001', 'Giovanni', 'Ancora', '2006-08-24'],
    ['1002', 'Mattia', 'Arseni', 'NULL']
  ]

  return render_template('index.html', alunni=alunni, intestazioni=intestazioni)

@app.route('/alunni/<matricola>')
def alunni(matricola):
  return f'Qui ci andrà la pagina dell\'alunno {matricola}'

if __name__ == '__main__':
  app.run(debug=True)