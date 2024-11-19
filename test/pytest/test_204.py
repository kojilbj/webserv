import test_responses

import requests
import os

# 204 No Content

def delete_file():
    PATH = test_responses.ROOT + "/uploaded_images/"
    FILE1 = "test1.png"
    FILE2 = "test2.png"
    f1 = os.open(PATH + FILE1, os.O_RDONLY | os.O_CREAT)
    os.close(f1)
    f2 = os.open(PATH + FILE2, os.O_RDONLY | os.O_CREAT)
    os.close(f2)
    w_res = requests.delete(test_responses.WEBSERV_URL + "/uploaded_images/" + FILE1)
    n_res = requests.delete(test_responses.NGINX_URL + "/uploaded_images/" + FILE2)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert os.access(PATH + FILE1, os.F_OK) == os.access(PATH + FILE2, os.F_OK)

def delete_chmod_file():
    PATH = test_responses.ROOT + "/uploaded_images/"
    FILE1 = "test1.png"
    FILE2 = "test2.png"
    f1 = os.open(PATH + FILE1, os.O_RDONLY | os.O_CREAT)
    os.close(f1)
    f2 = os.open(PATH + FILE2, os.O_RDONLY | os.O_CREAT)
    os.close(f2)
    os.chmod(PATH + FILE1, 0o000)
    os.chmod(PATH + FILE2, 0o000)
    w_res = requests.delete(test_responses.WEBSERV_URL + "/uploaded_images/" + FILE1)
    n_res = requests.delete(test_responses.NGINX_URL + "/uploaded_images/" + FILE2)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert os.access(PATH + FILE1, os.F_OK) == os.access(PATH + FILE2, os.F_OK)

def test_No_Content_response():
    delete_file()
    delete_chmod_file()