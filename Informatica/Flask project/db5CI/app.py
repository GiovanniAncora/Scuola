import pymysql.cursors
from flask import Flask, render_template, session, request, redirect, url_for

conn = pymysql.connect(host='127.0.0.1', port=3306, user='root', password='24082006', database='db5CI')
cursore = conn.cursor()

# cursore.execute('SHOW COLUMNS FROM verifiche;')
# print(cursore.fetchall())

app = Flask(__name__)

app.secret_key = 'provola1234'


@app.route('/')
def index():
    query = 'SELECT matricola, nome, cognome, data_nascita FROM alunni'
    cursore.execute(query)
    risultato = cursore.fetchall()
    return render_template('index.html', alunni= risultato, campi= cursore.description)

@app.route('/voti/<studente>')
def voti(studente):
    if 'studente' in session and studente == session['studente']:
        queryVoti = 'SELECT * FROM verifiche WHERE studente = %s'
        cursore.execute(queryVoti, (studente,))
        risultatoVoti = cursore.fetchall()
        return render_template("voti.html", voti=risultatoVoti, campi= cursore.description)
    else:
        return redirect(url_for('login'))

@app.route('/medie')
def medie():
    queryMedie = 'SELECT cognome, nome, AVG(voto) AS media FROM alunni, verifiche WHERE alunni.matricola = verifiche.studente GROUP BY studente'
    cursore.execute(queryMedie)
    risultatoMedie = cursore.fetchall()
    # print(risultatoMedie)
    return render_template("medie.html", medie=risultatoMedie, campi= cursore.description)

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        studente = request.form['matricola']
        codice = request.form['codice']

        queryUtente = 'SELECT codice FROM alunni WHERE matricola=%s;'
        cursore.execute(queryUtente, (studente,))
        ris = cursore.fetchall()
        print(ris)
        if ris and ris[0][0] == codice:
            session['studente'] = studente
            session['codice'] = codice
            # print(f'Login effettuato come {session["studente"]}: {session["codice"]}')
            # print(type(session['studente']))
            return redirect(url_for('voti', studente=studente))
        else:
            print('Login fallito.')
            return redirect(url_for('login'))

    return render_template('login.html')

@app.route('/logout')
def logout():
    session.pop('studente', None)
    session.pop('codice', None)
    return redirect(url_for('index'))

app.run(debug=True)