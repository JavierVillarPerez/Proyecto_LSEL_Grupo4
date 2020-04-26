import socket
from process_data import *

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((socket.gethostname(), 80))
server_socket.listen(5)

while True:
    connection, address = server_socket.accept()
    print("Conexión establecida")
    print(address)

    data = connection.recv(1024)
    process_d(data)
    connection.send("OK")

# connection.close()
