import thingspeak
import time
 
 
#lectura de sensores
lectura_ph = 11.54
lectura_turbidez = 3.26
 
 
#Se ponen los ids y las keys que correspondan a cada canal
channel_id = 1006531 
write_key  = '49BYVNFYXCNCIM9Y' 
read_key   = '3BV8ZQ76RFG28J78' 

 
def measure(channel):
    try:
        #PH TURBIDEZ
        ph = lectura_ph
        turbidez = lectura_turbidez
        
        # write
        response = channel.update({'field1': ph, 'field2': turbidez})
        # read
        read = channel.get({})
        print("Read:", read)
        
    except:
        print("connection failed")
 
 
if __name__ == "__main__":
    channel = thingspeak.Channel(id=channel_id, write_key=write_key, api_key=read_key)
    while True:
        measure(channel)
        time.sleep(15) #la version gratuita solo deja actualizar datos cada 15s
        