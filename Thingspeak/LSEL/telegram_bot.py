from telegram.ext import Updater, CommandHandler
import logging


def fuente_1(bot, update, estado_fuente_1):
    logger.info('He recibido un comando')
    if estado_fuente_1 == 1:
        bot.send_message(chat_id=update.message.chat_id, text="Estado del agua adecuado")
    else:
        bot.send_message(chat_id=update.message.chat_id, text="Estado del agua no adecuado")


def fuente_2(bot, update, estado_fuente_2):
    logger.info('He recibido un comando')
    if estado_fuente_2 == 1:
        bot.send_message(chat_id=update.message.chat_id, text="Estado del agua adecuado")
    else:
        bot.send_message(chat_id=update.message.chat_id, text="Estado del agua no adecuado")

