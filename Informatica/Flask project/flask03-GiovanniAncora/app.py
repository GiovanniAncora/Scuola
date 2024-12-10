from flask import Flask, render_template

app = Flask(__name__)

@app.route("/")
def hello():
    return render_template("index.html")

@app.route("/piloti")
def piloti():
    listaPiloti = ['Max Verstappen','Charles Leclerc', 'Fernando Alonso',
                   'Oscar Piastri', 'Danel Ricciardo', 'Esteban Ocon',
                   'Lewis Hamilton', 'Pierre Gasly', 'Lance Stroll',
                   'Carlos Sainz', 'Oliver Bearman', 'Kevin Magnussen',
                   'Nico Hülkenberg', 'Zhou Guanyu', 'Valtteri Bottas',
                   'Lando Norris', 'George Russel', 'Liam Lawson',
                   'Yuki Tsunoda', 'Sergio Pérez', 'Logan Sargeant',
                   'Franco Colapinto', 'Alexander Albon'
                   ]
    return render_template("piloti.html", piloti=listaPiloti)

@app.route('/squadre')
def squadre():
    listaSquadre = ['Red Bull Racing', 'Ferrari',
                    'Aston Martin', 'Mercedes',
                    'McLaren', 'Williams Racing',
                    'Alpine', 'Haas', 'Sauber']
    return render_template('squadre.html', squadre=listaSquadre)

@app.route('/granPremi')
def granPremi():
    listaGranPremi = ['Bahrain', 'Arabia Saudita',
                      'Australia', 'Giappone',
                      'Cina', 'Miami',
                      'Emilia Romagna', 'Monaco',
                      'Canada', 'Spagna',
                      'Austria', 'Regno Unito',
                      'Ungheria', 'Belgio',
                      'Paesi Bassi', 'Italia',
                      'Azerbaijan', 'Singapore',
                      'Stati Uniti', 'Mexico City',
                      'São Paulo', 'Las Vegas',
                      'Qatar', 'Abu Dhabi']
    return render_template('granPremi.html', granPremi=listaGranPremi)

@app.route('/prova') 
def prova(): 
    users = ['Alice', 'Bob', 'Charlie'] 
    return render_template('prova.html', users=users)

app.run(debug=True)
