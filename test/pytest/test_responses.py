import requests
import os 
import time

WEBSERV_URL = "http://172.17.0.2:80"
NGINX_URL = "http://172.17.0.2:8080"

ROOT = "/home/kisobe/webserv/test/"

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
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Length"] == n_res.headers["Content-Length"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert w_res.content == n_res.content

def upload_html_request():
    UPLOAD = "/upload.html"
    w_res = requests.get(WEBSERV_URL + UPLOAD)
    n_res = requests.get(NGINX_URL + UPLOAD)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Length"] == n_res.headers["Content-Length"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert w_res.content == n_res.content

def upload_php_request():
    UPLOAD = "/php/upload.php"
    files = {"image": ("penguin.png", open("images/penguin.png", "rb"), "image/png")}
    w_res = requests.post(WEBSERV_URL + UPLOAD, files=files)
    n_res = requests.post(NGINX_URL + UPLOAD, files=files)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"uploading from webserv" in w_res.content
    assert b"penguin.png" in w_res.content

def login_html_request():
    LOGIN = "/login.html"
    w_res = requests.get(WEBSERV_URL + LOGIN)
    n_res = requests.get(NGINX_URL + LOGIN)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Length"] == n_res.headers["Content-Length"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert w_res.content == n_res.content

def signup_html_request():
    SIGNUP = "/signup.html"
    w_res = requests.get(WEBSERV_URL + SIGNUP)
    n_res = requests.get(NGINX_URL + SIGNUP)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Length"] == n_res.headers["Content-Length"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert w_res.content == n_res.content

def autoindex_request():
    IMAGES = "/uploaded_images/"
    w_res = requests.get(WEBSERV_URL + IMAGES)
    n_res = requests.get(NGINX_URL + IMAGES)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"Index of /uploaded_images/" in w_res.content

def chmod_upload_php():
    UPLOAD = "/php/upload.php"
    files = {"image": ("penguin.png", open("images/penguin.png", "rb"), "image/png")}
    PATH = ROOT + "cgi" + UPLOAD
    # os.rename(PATH, ROOT + "cgi" + "/php/tmp.php")
    os.chmod(PATH, 0o000)
    w_res = requests.post(WEBSERV_URL + UPLOAD, files=files)
    n_res = requests.post(NGINX_URL + UPLOAD, files=files)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] in n_res.headers["Content-Type"]
    # os.rename(ROOT + "cgi" + "/php/tmp.php", PATH)
    os.chmod(PATH, 0o755)

def OK_request():
    index_html_request()
    upload_html_request()
    upload_php_request()
    login_html_request()
    signup_html_request()
    autoindex_request()
    chmod_upload_php()

# 204 No Content

def delete_file():
    PATH = ROOT + "/uploaded_images/"
    FILE1 = "test1.png"
    FILE2 = "test2.png"
    f1 = os.open(PATH + FILE1, os.O_RDONLY | os.O_CREAT)
    os.close(f1)
    f2 = os.open(PATH + FILE2, os.O_RDONLY | os.O_CREAT)
    os.close(f2)
    w_res = requests.delete(WEBSERV_URL + "/uploaded_images/" + FILE1)
    n_res = requests.delete(NGINX_URL + "/uploaded_images/" + FILE2)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert os.access(PATH + FILE1, os.F_OK) == os.access(PATH + FILE2, os.F_OK)

def delete_dir():
    PATH = ROOT + "/uploaded_images/"
    # DIR1 = "test1"
    DIR2 = "test2"
    # os.mkdir(PATH + DIR1)
    os.mkdir(PATH + DIR2)
    # w_res = requests.delete(WEBSERV_URL + "/uploaded_images/" + DIR1)
    # time.sleep(1)
    n_res = requests.delete(NGINX_URL + "/uploaded_images/" + DIR2)
    print(n_res.headers)
    print(n_res.content)
    # assert w_res.status_code == n_res.status_code 
    assert requests.codes.conflict == n_res.status_code 
    # assert "webserv/1.0" in w_res.headers["Server"]
    # assert w_res.headers["Date"]
    # assert os.access(PATH + DIR1, os.F_OK) == os.access(PATH + DIR2, os.F_OK)

def No_Content_request():
    delete_file()
    delete_dir()

# 403 Forbidden

def chmod_index_html():
    PATH = ROOT + "html/index.html"
    os.chmod(PATH, 0o200)
    w_res = requests.get(WEBSERV_URL)
    n_res = requests.get(NGINX_URL)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"403 Forbidden" in w_res.content
    os.chmod(PATH, 0o755)

def chmod_html_dir():
    PATH = ROOT + "html"
    os.chmod(PATH, 0o200)
    w_res = requests.get(WEBSERV_URL)
    n_res = requests.get(NGINX_URL)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"403 Forbidden" in w_res.content
    os.chmod(PATH, 0o755)

def Forbidden_request():
    chmod_index_html()
    chmod_html_dir()
    chmod_upload_php()

# 404 Not Found

def absent_file_request():
    ABSENT = "/absent.html"
    w_res = requests.get(WEBSERV_URL + ABSENT)
    n_res = requests.get(NGINX_URL + ABSENT)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"404 Not Found" in w_res.content

def rename_html_dirname():
    SRC = ROOT + "html"
    DST = ROOT + "non"
    os.rename(SRC, DST)
    w_res = requests.get(WEBSERV_URL)
    n_res = requests.get(NGINX_URL)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"404 Not Found" in w_res.content
    os.rename(DST, SRC)

def absent_php_request():
    ABSENT = "/php/absent.php"
    w_res = requests.get(WEBSERV_URL + ABSENT)
    n_res = requests.get(NGINX_URL + ABSENT)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] in n_res.headers["Content-Type"]
    assert w_res.content == n_res.content

def Not_Found_request():
    absent_file_request()
    rename_html_dirname()
    absent_php_request()

def test_responses():
    Continue_request()
    OK_request()
    No_Content_request()
    Forbidden_request()
    Not_Found_request()
