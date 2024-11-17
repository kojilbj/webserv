import test_responses

import requests
import os

# 403 Forbidden

def chmod_index_html():
    PATH = test_responses.ROOT + "html/index.html"
    os.chmod(PATH, 0o200)
    w_res = requests.get(test_responses.WEBSERV_URL)
    n_res = requests.get(test_responses.NGINX_URL)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"403 Forbidden" in w_res.content
    os.chmod(PATH, 0o755)

def chmod_html_dir():
    PATH = test_responses.ROOT + "html"
    os.chmod(PATH, 0o200)
    w_res = requests.get(test_responses.WEBSERV_URL)
    n_res = requests.get(test_responses.NGINX_URL)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"403 Forbidden" in w_res.content
    os.chmod(PATH, 0o755)

def Forbidden_response():
    chmod_index_html()
    chmod_html_dir()

