import socket

HOST = "172.17.0.2"
PORT = 80

def close_soon():
    print("close_soon test")
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
    s.close()

def send_bad_words():
    print("send_bad_words test")
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall(b"hello world")
        data = s.recv(1024)
    print("Received: ", data)
    assert b"Bad Request" in data
    s.close()
        
def test():
    print("bad client test")
    close_soon()
    send_bad_words()
