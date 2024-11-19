import test_responses

import requests
from io import BytesIO
import pycurl
import os

# 200 OK

def index_html_request():
    w_res = requests.get(test_responses.WEBSERV_URL)
    n_res = requests.get(test_responses.NGINX_URL)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Length"] == n_res.headers["Content-Length"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert w_res.content == n_res.content

def upload_html_request():
    UPLOAD = "/upload.html"
    w_res = requests.get(test_responses.WEBSERV_URL + UPLOAD)
    n_res = requests.get(test_responses.NGINX_URL + UPLOAD)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Length"] == n_res.headers["Content-Length"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert w_res.content == n_res.content

def upload_php_request():
    UPLOAD = "/php/upload.php"
    files = {"image": ("penguin.png", open("images/penguin.png", "rb"), "image/png")}
    w_res = requests.post(test_responses.WEBSERV_URL + UPLOAD, files=files)
    n_res = requests.post(test_responses.NGINX_URL + UPLOAD, files=files)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"uploading from webserv" in w_res.content
    assert b"penguin.png" in w_res.content

def login_html_request():
    LOGIN = "/login.html"
    w_res = requests.get(test_responses.WEBSERV_URL + LOGIN)
    n_res = requests.get(test_responses.NGINX_URL + LOGIN)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Length"] == n_res.headers["Content-Length"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert w_res.content == n_res.content

def signup_html_request():
    SIGNUP = "/signup.html"
    w_res = requests.get(test_responses.WEBSERV_URL + SIGNUP)
    n_res = requests.get(test_responses.NGINX_URL + SIGNUP)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Length"] == n_res.headers["Content-Length"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert w_res.content == n_res.content

def autoindex_request():
    IMAGES = "/uploaded_images/"
    w_res = requests.get(test_responses.WEBSERV_URL + IMAGES)
    n_res = requests.get(test_responses.NGINX_URL + IMAGES)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"Index of /uploaded_images/" in w_res.content

def chmod_upload_php():
    UPLOAD = "/php/upload.php"
    files = {"image": ("penguin.png", open("images/penguin.png", "rb"), "image/png")}
    PATH = test_responses.ROOT + "cgi" + UPLOAD
    os.chmod(PATH, 0o000)
    w_res = requests.post(test_responses.WEBSERV_URL + UPLOAD, files=files)
    n_res = requests.post(test_responses.NGINX_URL + UPLOAD, files=files)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] in n_res.headers["Content-Type"]
    assert b"Access denied.\n" in w_res.content
    os.chmod(PATH, 0o755)

def delete_request_to_upload_php():
    UPLOAD = "/php/upload.php"
    w_res = requests.delete(test_responses.WEBSERV_URL + UPLOAD)
    n_res = requests.delete(test_responses.NGINX_URL + UPLOAD)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"uploading from webserv" in w_res.content
    assert b"Possible file upload attack!" in w_res.content

def upload_php_request_with_chunked():
    UPLOAD = "/php/upload.php"
    w_res = BytesIO()
    w_c = pycurl.Curl()
    w_c.setopt(w_c.URL, test_responses.WEBSERV_URL + UPLOAD)
    w_c.setopt(w_c.HTTPPOST, [("image", (w_c.FORM_FILE, "images/penguin.png")),])
    w_c.setopt(w_c.HTTPHEADER, ["Transfer-Encoding: chunked"])
    w_c.setopt(w_c.WRITEDATA, w_res)
    w_c.perform()
    w_c.close()
    assert b"uploading from webserv" in w_res.getvalue()
    assert b"File is valid, and was successfully uploaded." in w_res.getvalue()
    n_res = BytesIO()
    n_c = pycurl.Curl()
    n_c.setopt(n_c.URL, test_responses.NGINX_URL + UPLOAD)
    n_c.setopt(n_c.HTTPPOST, [("image", (n_c.FORM_FILE, "images/penguin.png")),])
    n_c.setopt(n_c.HTTPHEADER, ["Transfer-Encoding: chunked"])
    n_c.setopt(n_c.WRITEDATA, n_res)
    n_c.perform()
    n_c.close()
    assert b"uploading from nginx" in n_res.getvalue()
    # assert b"File is valid, and was successfully uploaded." in n_res.getvalue()

def signup_php():
    CSV = "../cgi/php/users.csv"
    # clear file content.
    if os.access(CSV, os.F_OK):
        os.remove(CSV)
    f = os.open(CSV, os.O_WRONLY | os.O_CREAT)
    os.chmod(CSV, 0o777)
    os.close(f)
    payload1 = {"submit": "", "username": "kisobe", "password": "abc"}
    payload2 = {"submit": "", "username": "kowatan", "password": "def"}
    SIGNUP = "/php/signup.php"
    w_res = requests.post(test_responses.WEBSERV_URL + SIGNUP, data=payload1)
    n_res = requests.post(test_responses.NGINX_URL + SIGNUP, data=payload2)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"Registration successful!" in w_res.content
    assert b"Registration successful!" in n_res.content

def login_php():
    payload1 = {"submit": "", "username": "kisobe", "password": "abc"}
    payload2 = {"submit": "", "username": "kowatan", "password": "def"}
    LOGIN = "/php/login.php"
    w_res = requests.post(test_responses.WEBSERV_URL + LOGIN, data=payload1)
    n_res = requests.post(test_responses.NGINX_URL + LOGIN, data=payload2)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert w_res.headers["Set-Cookie"]
    session_id1 = w_res.headers["Set-Cookie"]
    assert n_res.headers["Set-Cookie"]
    session_id2 = n_res.headers["Set-Cookie"]
    assert b"Go To My Page" in w_res.content
    assert b"Go To My Page" in n_res.content
    private_index_php(session_id1, session_id2)

def private_index_php(session_id1, session_id2):
    INDEX = "/php/private_index.php"
    headers1 = {"Cookie": session_id1}
    headers2 = {"Cookie": session_id2}
    w_res = requests.get(test_responses.WEBSERV_URL + INDEX, headers=headers1)
    n_res = requests.get(test_responses.NGINX_URL + INDEX, headers=headers2)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"Hello, kisobe" in w_res.content
    assert b"Hello, kowatan" in n_res.content
    logout_php(session_id1, session_id2)

def logout_php(session_id1, session_id2):
    INDEX = "/php/logout.php"
    headers1 = {"Cookie": session_id1}
    headers2 = {"Cookie": session_id2}
    w_res = requests.get(test_responses.WEBSERV_URL + INDEX, headers=headers1)
    n_res = requests.get(test_responses.NGINX_URL + INDEX, headers=headers2)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"Go To Top Page" in w_res.content
    assert b"Go To Top Page" in n_res.content
    after_logout_php(session_id1, session_id2)

def after_logout_php(session_id1, session_id2):
    INDEX = "/php/private_index.php"
    headers1 = {"Cookie": session_id1}
    headers2 = {"Cookie": session_id2}
    w_res = requests.get(test_responses.WEBSERV_URL + INDEX, headers=headers1)
    n_res = requests.get(test_responses.NGINX_URL + INDEX, headers=headers2)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"Invalid credentials!" in w_res.content
    assert b"Invalid credentials!" in n_res.content

def private_index_php_without_cookie():
    INDEX = "/php/private_index.php"
    # no cookie headers
    w_res = requests.get(test_responses.WEBSERV_URL + INDEX)
    n_res = requests.get(test_responses.NGINX_URL + INDEX)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"Invalid credentials!" in w_res.content
    assert b"Invalid credentials!" in n_res.content

def test_OK_response():
    index_html_request()
    upload_html_request()
    upload_php_request()
    login_html_request()
    signup_html_request()
    autoindex_request()
    chmod_upload_php()
    delete_request_to_upload_php()
    upload_php_request_with_chunked()
    signup_php()
    login_php()
    private_index_php_without_cookie()
