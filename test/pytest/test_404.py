import test_responses

import requests
import os

# 404 Not Found

def absent_file_request():
    ABSENT = "/absent.html"
    w_res = requests.get(test_responses.WEBSERV_URL + ABSENT)
    n_res = requests.get(test_responses.NGINX_URL + ABSENT)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"404 Not Found" in w_res.content

def rename_html_dirname():
    SRC = test_responses.ROOT + "html"
    DST = test_responses.ROOT + "non"
    os.rename(SRC, DST)
    w_res = requests.get(test_responses.WEBSERV_URL)
    n_res = requests.get(test_responses.NGINX_URL)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"404 Not Found" in w_res.content
    os.rename(DST, SRC)

def absent_php_with_get_request():
    ABSENT = "/php/absent.php"
    w_res = requests.get(test_responses.WEBSERV_URL + ABSENT)
    n_res = requests.get(test_responses.NGINX_URL + ABSENT)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] in n_res.headers["Content-Type"]
    assert w_res.content == n_res.content

def absent_php_with_post_request():
    UPLOAD = "/php/absent.php"
    files = {"image": ("penguin.png", open("images/penguin.png", "rb"), "image/png")}
    PATH = test_responses.ROOT + "cgi" + UPLOAD
    w_res = requests.post(test_responses.WEBSERV_URL + UPLOAD, files=files)
    n_res = requests.post(test_responses.NGINX_URL + UPLOAD, files=files)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] in n_res.headers["Content-Type"]
    assert w_res.content == n_res.content

def error_page():
    headers1 = {"Host": "webserv"}
    headers2 = {"Host": "nginx"}
    INDEX = "/absent.html"
    w_res = requests.get(test_responses.WEBSERV_URL + INDEX, headers=headers1)
    n_res = requests.get(test_responses.NGINX_URL + INDEX, headers=headers2)
    assert w_res.status_code == n_res.status_code
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.content == n_res.content

def Not_Found_response():
    absent_file_request()
    rename_html_dirname()
    absent_php_with_get_request()
    absent_php_with_post_request()
    error_page()
