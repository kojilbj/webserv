import socket
import time

HOST = "172.17.0.2"
PORT = 80

def close_soon():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.close()
    time.sleep(0.1)

def send_bad_words():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall(b"hello world")
        data = s.recv(1024)
        assert b"HTTP/1.1 400 Bad Request\r\n" in data
        s.close()
    time.sleep(0.1)

def only_status_line():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall(b"GET / HTTP/1.1\r\n\r\n")
        data = s.recv(1024)
        assert b"HTTP/1.1 200 OK\r\n" in data
        s.close()
    time.sleep(0.1)

def bad_method():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall(b"COMEHERE / HTTP/1.1\r\n\r\n")
        data = s.recv(1024)
        assert b"HTTP/1.1 400 Bad Request\r\n" in data
        s.close()
    time.sleep(0.1)

def bad_uri():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall(b"GET iamnoturi HTTP/1.1\r\n\r\n")
        data = s.recv(1024)
        assert b"HTTP/1.1 400 Bad Request\r\n" in data
        s.close()
    time.sleep(0.1)

# def bad_version(): 
#     with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
#         s.connect((HOST, PORT))
#         s.sendall(b"GET / HTTP/1.0\r\n\r\n")
#         data = s.recv(1024)
#         assert b"HTTP/1.1 400 Bad Request\r\n" in data
#         s.close()
#     time.sleep(0.1)

def test_badclient():
    close_soon()
    send_bad_words()
    only_status_line()
    bad_method()
    bad_uri()
    # bad_version()
