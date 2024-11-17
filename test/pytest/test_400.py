import test_badclient
import test_responses

import requests
import socket
import time

# 400 Bad Request

def imcomplete_request():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((test_badclient.HOST, test_badclient.PORT))
        s.sendall(b"GET / HTTP/1.1\r\n")
        webserv_data = s.recv(1024)
        s.close()
    time.sleep(0.1)
    assert b"400 Bad Request" in webserv_data

def unsupported_version():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((test_badclient.HOST, test_badclient.PORT))
        s.sendall(b"GET / HTTP/0.0\r\n\r\n")
        webserv_data = s.recv(1024)
        s.close()
    time.sleep(0.1)
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((test_badclient.HOST, test_badclient.NGINX_PORT))
        s.sendall(b"GET / HTTP/0.0\r\n\r\n")
        nginx_data = s.recv(1024)
        s.close()
    time.sleep(0.1)
    assert b"400 Bad Request" in nginx_data
    assert b"400 Bad Request" in webserv_data

def too_long_header():
    LONG_STR = "x" * 10000
    headers = {"Accept": LONG_STR}
    w_res = requests.get(test_responses.WEBSERV_URL, headers=headers)
    n_res = requests.get(test_responses.NGINX_URL, headers=headers)
    assert w_res.status_code == n_res.status_code
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert b"400 Bad Request" in w_res.content

def Bad_request_response():
    imcomplete_request()
    unsupported_version()
    too_long_header()