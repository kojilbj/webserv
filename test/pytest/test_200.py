import test_responses

import requests
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
    # os.rename(PATH, ROOT + "cgi" + "/php/tmp.php")
    os.chmod(PATH, 0o000)
    w_res = requests.post(test_responses.WEBSERV_URL + UPLOAD, files=files)
    n_res = requests.post(test_responses.NGINX_URL + UPLOAD, files=files)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] in n_res.headers["Content-Type"]
    # os.rename(test_responses.ROOT + "cgi" + "/php/tmp.php", PATH)
    os.chmod(PATH, 0o755)

def OK_request():
    index_html_request()
    upload_html_request()
    upload_php_request()
    login_html_request()
    signup_html_request()
    autoindex_request()
    chmod_upload_php()
