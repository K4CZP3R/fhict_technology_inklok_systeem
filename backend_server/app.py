from flask import Flask, session, redirect, url_for, render_template
from flask_session import Session
from werkzeug.utils import secure_filename

from views.attendance import attendance_blueprint
from views.admin import admin_blueprint
import KspReturns

UPLOAD_FOLDER = 'uploads'
ALLOWED_EXTENSIONS = set(['.png', '.jpeg', '.jpg', '.bmp'])
app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

app.register_blueprint(attendance_blueprint)
app.register_blueprint(admin_blueprint)

SESSION_TYPE = 'mongodb'
SESSION_PERMANENT = True
SESSION_KEY_PREFIX = "ksp" 
app.config.from_object(__name__)
Session(app)


@app.route("/")
def index():
    return KspReturns.return_front_message("Title", "Content", 5, url_for('index'))

if __name__ == "__main__":
    app.run(
        "127.0.0.1",
        8080,
        debug=True,
        ssl_context='adhoc'
    )