import test_responses

import requests

# 405 Not Allowed 

def post_request_to_index_html():
    w_res = requests.post(test_responses.WEBSERV_URL)
    n_res = requests.post(test_responses.NGINX_URL)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert w_res.headers["Allow"]
    assert b"405 Not Allowed" in w_res.content

def delete_request_to_index_html():
    w_res = requests.delete(test_responses.WEBSERV_URL)
    n_res = requests.delete(test_responses.NGINX_URL)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert w_res.headers["Allow"]
    assert b"405 Not Allowed" in w_res.content

def post_request_to_upload_html():
    UPLOAD = "/upload.html"
    w_res = requests.post(test_responses.WEBSERV_URL + UPLOAD)
    n_res = requests.post(test_responses.NGINX_URL + UPLOAD)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert w_res.headers["Allow"]
    assert b"405 Not Allowed" in w_res.content

def delete_request_to_upload_html():
    UPLOAD = "/upload.html"
    w_res = requests.delete(test_responses.WEBSERV_URL + UPLOAD)
    n_res = requests.delete(test_responses.NGINX_URL + UPLOAD)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert w_res.headers["Allow"]
    assert b"405 Not Allowed" in w_res.content

def Not_allowed_response(): 
    post_request_to_index_html()
    delete_request_to_index_html()
    post_request_to_upload_html()
    delete_request_to_upload_html()