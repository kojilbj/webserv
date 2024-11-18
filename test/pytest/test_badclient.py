import socket
import time

HOST = "172.17.0.2"
PORT = 8080
NGINX_PORT = 8000

def close_soon():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.close()
    time.sleep(0.1)

def close_after_1s():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall(b"GET / HTTP/1.1\r\n\r\n")
        time.sleep(1)
        s.close()
    time.sleep(0.1)

def close_after_request_line():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall(b"GET / HTTP/1.1\r\n\r\n")
        s.close()
    time.sleep(0.1)

def close_after_request_header():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall(b"GET / HTTP/1.1\r\nHOST: localhost\r\n\r\n")
        s.close()
    time.sleep(0.1)

def close_after_request_body():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall(b"GET / HTTP/1.1\r\nHOST: localhost\r\n\r\nid=kisobe&name=keisei")
        s.close()
    time.sleep(0.1)

def close_after_status_line():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall(b"GET / HTTP/1.1\r\n\r\n")
        data = s.recv(30)
        assert b"HTTP/1.1 200 OK\r\n" in data
        s.close()
    time.sleep(0.1)

def close_during_response():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall(b"GET / HTTP/1.1\r\n\r\n")
        data = s.recv(1024)
        assert b"HTTP/1.1 200 OK\r\n" in data
        s.close()
    time.sleep(0.1)

def test_badclient():
    for i in range(5):
        close_soon()
        close_after_1s()
        close_after_request_line()
        close_after_request_header()
        close_after_request_body()
        close_during_response()