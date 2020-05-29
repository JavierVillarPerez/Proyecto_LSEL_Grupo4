import socket
from process_data import *
from telegram.ext.commandhandler import CommandHandler
from telegram.ext.updater import Updater
from telegram.ext.dispatcher import Dispatcher


server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((socket.gethostname(), 80))
server_socket.listen(5)

TOKEN = '1245262637:AAGFfXDyh3wM4cFpnAiYa8cIM8bKXRXjecs'
updater = Updater("1245262637:AAGFfXDyh3wM4cFpnAiYa8cIM8bKXRXjecs", use_context=True)
dispatcher: Dispatcher = updater.dispatcher
dispatcher.add_handler(CommandHandler("fuente_1", fuente_1))
dispatcher.add_handler(CommandHandler("fuente_2", fuente_2))

while True:
    updater.start_polling()
    connection, address = server_socket.accept()
    print("Conexión establecida")
    print(address)
    data = connection.recv(1024)
    process_d(data)
    updater.start_polling()
    connection.send("OK")



# connection.close()


