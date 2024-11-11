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
        assert b"Bad Request" in data
        s.close()
        
def test_badclient():
    close_soon()
    send_bad_words()
