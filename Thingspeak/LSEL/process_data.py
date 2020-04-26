import thingspeak
import time
import ctypes
import struct
import datetime


class MyStruct(ctypes.Structure):
    _fields_ = [
        ("ID", ctypes.c_ushort),
        ("measure", ctypes.c_ushort),
        ("alarm", ctypes.c_bool),
        ("error", ctypes.c_bool),
        ("threshold_L", ctypes.c_ushort),
        ("threshold_H", ctypes.c_ushort),
        ("year", ctypes.c_int),
        ("month", ctypes.c_int),
        ("day", ctypes.c_int),
        ("hour", ctypes.c_int),
        ("minutes", ctypes.c_int),
    ]


def process_d(data):
    channel_id = 1006531
    write_key = '49BYVNFYXCNCIM9Y'
    # read_key = '3BV8ZQ76RFG28J78'
    x = MyStruct()
    fmt_size = struct.calcsize("HH??HHiiiii")
    x.ID, x.measure, x.alarm, x.error, x.threshold_L, x.threshold_H, x.year, x.month, x.day, x.hour, x.minutes = struct.unpack("HH??HHiiiii", data[:fmt_size])
    update_channel(x.ID, channel_id, write_key, x.measure)


def update_channel(id_sensor, id_channel, api_key_channel, measure):
    switcher = {
        1: one(id_channel, api_key_channel, measure),
        2: two(id_channel, api_key_channel, measure),
        3: three(id_channel, api_key_channel, measure)
    }
    switcher.get(id_sensor)


def one(id_channel, api_key_channel, measure):
    channel = thingspeak.Channel(id=id_channel, api_key=api_key_channel)
    channel.update({'field1': measure})
    time.sleep(15)


def two(id_channel, api_key_channel, measure):
    channel = thingspeak.Channel(id=id_channel, api_key=api_key_channel)
    channel.update({'field2': measure})
    time.sleep(15)


def three(id_channel, api_key_channel, measure):
    channel = thingspeak.Channel(id=id_channel, api_key=api_key_channel)
    channel.update({'field3': measure})
    time.sleep(15)