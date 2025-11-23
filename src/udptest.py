import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# sock.connect(('localhost', 6000))
while (True):
    msg = input("> ").encode()
    sock.sendto(msg, ('127.0.0.1', 6001))