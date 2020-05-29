from telegram.ext.commandhandler import CommandHandler
from telegram.ext.updater import Updater
from telegram.ext.dispatcher import Dispatcher
from telegram.update import Update
from telegram.ext.callbackcontext import CallbackContext
from telegram.bot import Bot
from telegram.parsemode import ParseMode

state_fuente_1 = 0
state_fuente_2 = 1


def fuente_1(update: Update, context: CallbackContext):
    bot: Bot = context.bot
    if state_fuente_1 == 1:
        bot.send_message(
            chat_id=update.effective_chat.id,
            text="Agua SÍ apta para el consumo",
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


TOKEN = '1245262637:AAGFfXDyh3wM4cFpnAiYa8cIM8bKXRXjecs'
updater = Updater("1245262637:AAGFfXDyh3wM4cFpnAiYa8cIM8bKXRXjecs", use_context=True)
dispatcher: Dispatcher = updater.dispatcher
dispatcher.add_handler(CommandHandler("fuente_1", fuente_1))
dispatcher.add_handler(CommandHandler("fuente_2", fuente_2))

while True:
    updater.start_polling()
