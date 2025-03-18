from flask import Flask, session, redirect, url_for, request, render_template_string, flash

app = Flask(__name__)
app.secret_key = "provola"