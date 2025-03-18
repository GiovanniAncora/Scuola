from flask import Flask, session, redirect, url_for, request, render_template_string, flash

app = Flask(__name__)
app.secret_key = "provola"

@app.route("/")
def index():
  if 'username' in session:
    return f'Ciao, {session[session]}!</br></br><a href="{url_for("logout")}">Logout</a>'
  return 'Non sei loggato.</br>Esegui il <a href="/login">login</a>'

@app.route("/login", methods=['GET', 'POST'])
def login():
  if request.method == 'POST':
    username = request.form.get('username')
    password = request.form.get('password')

    if username == 'admin' and password == 'pawssord':
      session['username'] = username
      flash('Login effettuato con successo.', 'success')
      return redirect(url_for('index'))
    else:
      flash('Credenziali errate!', 'error')
      return redirect(url_for('login'))
    
  return render_template_string(LOGIN_TEMPLATE)

@app.route("/logout")
def logout():
  session.pop('username', None)
  flash('Logout effettuato.', 'info')
  return redirect(url_for('index'))

if __name__ == '__main__':
  app.run(debug=True)