import test_responses

import requests
import os

# 403 Forbidden

def chmod_index_html():
    PATH = test_responses.ROOT + "html/index.html"
    # write and execute permission only
    os.chmod(PATH, 0o300)
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
    # write permission only
    os.chmod(PATH, 0o200)
    w_res = requests.get(test_responses.WEBSERV_URL)
    n_res = requests.get(test_responses.NGINX_URL)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"403 Forbidden" in w_res.content
    os.chmod(PATH, 0o755)

def chmod_test_dir():
    PATH = test_responses.ROOT
    # read and write permission only
    os.chmod(PATH, 0o600)
    w_res = requests.get(test_responses.WEBSERV_URL)
    n_res = requests.get(test_responses.NGINX_URL)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"403 Forbidden" in w_res.content
    os.chmod(PATH, 0o755)

def chmod_dir_of_delete_content():
    PATH = test_responses.ROOT
    URI1 = "/uploaded_images/test1.png"
    URI2 = "/uploaded_images/test2.png"
    DIR = PATH + "/uploaded_images"
    f1 = os.open(PATH + URI1, os.O_RDONLY | os.O_CREAT)
    f2 = os.open(PATH + URI2, os.O_RDONLY | os.O_CREAT)
    os.close(f1)
    os.close(f2)
    # read and execute permission only
    os.chmod(DIR, 0o500)
    w_res = requests.delete(test_responses.WEBSERV_URL + URI1)
    n_res = requests.delete(test_responses.NGINX_URL + URI2)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"403 Forbidden" in w_res.content
    os.chmod(DIR, 0o777)
    os.remove(PATH + URI1)
    os.remove(PATH + URI2)

def chmod_dir_of_dir_of_delete_content():
    PATH = test_responses.ROOT
    URI1 = "/uploaded_images/test1.png"
    URI2 = "/uploaded_images/test2.png"
    DIR = PATH + "/uploaded_images"
    f1 = os.open(PATH + URI1, os.O_RDONLY | os.O_CREAT)
    f2 = os.open(PATH + URI2, os.O_RDONLY | os.O_CREAT)
    os.close(f1)
    os.close(f2)
    # read and write permission only
    os.chmod(PATH, 0o600)
    w_res = requests.delete(test_responses.WEBSERV_URL + URI1)
    n_res = requests.delete(test_responses.NGINX_URL + URI2)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"403 Forbidden" in w_res.content
    os.chmod(PATH, 0o777)
    os.remove(PATH + URI1)
    os.remove(PATH + URI2)

def Forbidden_response():
    chmod_index_html()
    chmod_html_dir()
    chmod_test_dir()
    chmod_dir_of_delete_content()
    chmod_dir_of_dir_of_delete_content()