import thingspeak
import telegram
from telegram.update import Update
from telegram.ext.callbackcontext import CallbackContext
from telegram.bot import Bot
from telegram.parsemode import ParseMode
import time

token = '1245262637:AAGFfXDyh3wM4cFpnAiYa8cIM8bKXRXjecs'
bot = telegram.Bot(token)

state_fuente_1 = 0
state_fuente_2 = 0

channel_id_1 = 1006531
write_key_1 = '49BYVNFYXCNCIM9Y'

channel_id_2 = 1006531
write_key_2 = '49BYVNFYXCNCIM9Y'


def process_d(data):
    data_proc = data.split()
    device_id = int(data_proc[1])
    sensor_id = int(data_proc[2])
    data_measure = float(data_proc[3])
    data_alarm = int(data_proc[4])
    data_error = int(data_proc[5])

    print("valores recibidos")
    print(device_id)
    print(sensor_id)
    print(data_measure)
    print(data_alarm)
    print(data_error)

    update_device(device_id, sensor_id, data_measure, data_alarm, data_error)


def update_device(id_device, id_sensor, measure, data_alarm, data_error):
    if id_device == 1:
        one(channel_id_1, write_key_1, id_sensor, measure, data_alarm, data_error)
    elif id_device == 2:
        two(channel_id_2, write_key_2, id_sensor, measure, data_alarm, data_error)
    else:
        print("Fuente no existente")


def one(id_channel, api_key_channel, id_sensor, measure, data_alarm, data_error):
    global state_fuente_1
    channel = thingspeak.Channel(id=id_channel, api_key=api_key_channel)
    if data_error == 1:
        print('Error de lectura')
    else:
        if id_sensor == 1:
            if data_alarm == 1:
                bot.send_message(chat_id='1083405023',
                                 text="Fuente 1 con valores de pH fuera de rango")
                state_fuente_1 = 0
                channel.update({'field1': measure})
            else:
                channel.update({'field1': measure})
                state_fuente_1 = 1
        elif id_sensor == 2:
            if data_alarm == 1:
                bot.send_message(chat_id='1083405023',
                                 text="Fuente 1 con valores de turbidez fuera de rango")
                state_fuente_1 = 0
                channel.update({'field2': measure})
            else:
                channel.update({'field2': measure})
                state_fuente_1 = 1



def two(id_channel, api_key_channel, id_sensor, measure, data_alarm, data_error):
    global state_fuente_2
    channel = thingspeak.Channel(id=id_channel, api_key=api_key_channel)
    if data_error == 1:
        print('Error de lectura')
    else:
        if id_sensor == 1:
            if data_alarm == 1:
                bot.send_message(chat_id='1083405023',
                                 text="Fuente 2 con valores de pH fuera de rango")
                state_fuente_2 = 0
                channel.update({'field1': measure})
            else:
                channel.update({'field1': measure})
                state_fuente_2 = 1
        elif id_sensor == 2:
            if data_alarm == 1:
                bot.send_message(chat_id='1083405023',
                                 text="Fuente 2 con valores de turbidez fuera de rango")
                state_fuente_2 = 0
                channel.update({'field2': measure})
            else:
                channel.update({'field2': measure})
                state_fuente_2 = 1


def fuente_1(update: Update, context: CallbackContext):
    bot: Bot = context.bot
    if state_fuente_1 == 1:
        bot.send_message(
            chat_id=update.effective_chat.id,
            text="Agua apta para el consumo",
            parse_mode=ParseMode.HTML,
        )
    elif state_fuente_1 == 0:
        bot.send_message(
            chat_id=update.effective_chat.id,
            text="Agua NO apta para el consumo",
            parse_mode=ParseMode.HTML,
        )


def fuente_2(update: Update, context: CallbackContext):
    bot: Bot = context.bot
    if state_fuente_2 == 1:
        bot.send_message(
            chat_id=update.effective_chat.id,
            text="Agua apta para el consumo",
            parse_mode=ParseMode.HTML,
        )
    elif state_fuente_2 == 0:
        bot.send_message(
            chat_id=update.effective_chat.id,
            text="Agua NO apta para el consumo",
            parse_mode=ParseMode.HTML,
        )
