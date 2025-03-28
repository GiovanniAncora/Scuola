import pymysql
import random

connessione = pymysql.connect(host='127.0.0.1', port=3306, user='root', password='24082006', database='db5CI')
cur = connessione.cursor()

query = "SHOW COLUMNS FROM alunni;"
cur.execute(query)
ris = cur.fetchall()

for riga in ris:
  print(riga)

discipline = ('Matematica', 'Informatica', 'Sistemi e Reti', 'TPSIT', 'Italiano', 'Storia', 'Inglese', 'Religione', 'Scienze Motorie')

query = "SELECT matricola FROM alunni;"
cur.execute(query)
ris = cur.fetchall()

for alunno in ris:
  print(f"Creo verifiche per {alunno[0]}...")
  for i in range(3):
    disciplina = random.sample(discipline, 1)
    data = f"2025-03-{random.randrange(1, 32)}"
    voto = random.randrange(2, 11)
    cur.execute('INSERT INTO verifiche VALUES (%s, %s, %s, %s);', (disciplina, data, alunno[0], voto))
  cur.execute('SELECT * FROM verifiche WHERE studente=%s;', (alunno[0],))
  print(cur.fetchall())
connessione.commit()