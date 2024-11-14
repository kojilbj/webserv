import requests

WEBSERV_URL = "http://172.17.0.2:80"
NGINX_URL = "http://172.17.0.2:8080"

# 100 Continue

def continue_when_get_request():
    headers = {"Expect": "100-continue"}
    w_res = requests.get(WEBSERV_URL, headers=headers)
    n_res = requests.get(NGINX_URL, headers=headers)
    assert w_res.status_code == n_res.status_code 
    assert w_res.content == n_res.content

def Continue_request():
    continue_when_get_request()

# 200 OK

def index_html_request():
    w_res = requests.get(WEBSERV_URL)
    n_res = requests.get(NGINX_URL)
    assert w_res.status_code == n_res.status_code 
    assert w_res.headers["Content-Length"] == n_res.headers["Content-Length"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert w_res.content == n_res.content

def upload_html_request():
    UPLOAD = "/upload.html"
    w_res = requests.get(WEBSERV_URL + UPLOAD)
    n_res = requests.get(NGINX_URL + UPLOAD)
    assert w_res.status_code == n_res.status_code 
    assert w_res.headers["Content-Length"] == n_res.headers["Content-Length"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert w_res.content == n_res.content

def upload_php_request():
    UPLOAD = "/php/upload.php"
    files = {"image": ("penguin.png", open("images/penguin.png", "rb"), "image/png")}
    # payload = {"image": "penguin.png"}
    w_res = requests.post(WEBSERV_URL + UPLOAD, files=files)
    n_res = requests.post(NGINX_URL + UPLOAD, files=files)
    assert w_res.status_code == n_res.status_code 
    # assert w_res.headers["Content-Length"] == n_res.headers["Content-Length"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]

def OK_request():
    index_html_request()
    upload_html_request()
    upload_php_request()

# 404 Not Found

def Not_Found_request():
    ABSENT = "/absent.html"
    w_res = requests.get(WEBSERV_URL + ABSENT)
    n_res = requests.get(NGINX_URL + ABSENT)
    assert w_res.status_code == n_res.status_code 
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"404 Not Found" in w_res.content

def test_responses():
    Continue_request()
    OK_request()
    Not_Found_request()
