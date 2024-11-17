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

def absent_php_request():
    ABSENT = "/php/absent.php"
    w_res = requests.get(test_responses.WEBSERV_URL + ABSENT)
    n_res = requests.get(test_responses.NGINX_URL + ABSENT)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] in n_res.headers["Content-Type"]
    assert w_res.content == n_res.content

def Not_Found_request():
    absent_file_request()
    rename_html_dirname()
    absent_php_request()

