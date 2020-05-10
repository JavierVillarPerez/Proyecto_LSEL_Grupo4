import socket
from process_data import *
from telegram.ext import Updater, CommandHandler
from telegram_bot import *
import logging


server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((socket.gethostname(), 80))
server_socket.listen(5)

token = '1245262637:AAGFfXDyh3wM4cFpnAiYa8cIM8bKXRXjecs'
logging.basicConfig(format='%(asctime)s - %(name)s - %(levelname)s -%(message)s', level=logging.INFO)
logger = logging.getLogger('ControlAguas_bot')

updater = Updater(token, use_context=True)
dp = updater.dispatcher

dp.add_handler(CommandHandler('fuente_1', fuente_1))
dp.add_handler(CommandHandler('fuente_2', fuente_2))
updater.start_polling()
updater.idle()

while True:
    connection, address = server_socket.accept()
    print("Conexión establecida")
    print(address)

    data = connection.recv(1024)
    fuente, potable = process_d(data)
    connection.send("OK")

    if fuente == 1:
        estado_fuente_1 = potable
    elif fuente == 2:
        estado_fuente_2 = potable


# connection.close()
