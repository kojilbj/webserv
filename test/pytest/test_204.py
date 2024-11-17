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

# def delete_dir():
#     PATH = test_responses.ROOT + "/uploaded_images/"
#     w_res = requests.delete(test_responses.WEBSERV_URL + "/uploaded_images/")
#     assert w_res.status_code == requests.codes.no_content
#     assert os.access(PATH, os.F_OK) == False
#     os.mkdir(PATH)
#     n_res = requests.delete(test_responses.NGINX_URL + "/uploaded_images/")
#     assert w_res.status_code == n_res.status_code 
#     assert "webserv/1.0" in w_res.headers["Server"]
#     assert w_res.headers["Date"]
#     assert os.access(PATH, os.F_OK) == False
#     os.mkdir(PATH)

# def delete_dir():
#     PATH = test_responses.ROOT + "/uploaded_images/"
#     DIR1 = "test1"
#     DIR2 = "test2"
#     os.mkdir(PATH + DIR1)
#     os.mkdir(PATH + DIR2)
#     w_res = requests.delete(test_responses.WEBSERV_URL + "/uploaded_images/" + DIR1)
#     n_res = requests.delete(test_responses.NGINX_URL + "/uploaded_images/" + DIR2)
#     assert w_res.status_code == n_res.status_code 
#     assert "webserv/1.0" in w_res.headers["Server"]
#     assert w_res.headers["Date"]
#     assert os.access(PATH + DIR1, os.F_OK) == os.access(PATH + DIR2, os.F_OK)

def No_Content_response():
    delete_file()
    # delete_dir()

