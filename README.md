# Proyecto_LSEL_Grupo4
LSEL_Proyect Repositorio para el desarrollo del proyecto del control de aguas por telemetría para la asignatura LSEL. 

Rama master. Rama para subir ficheros de entrega.


------------------------------------------------------------------------------------------------------------------

¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡ATENCIÓN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

Procedimiento para descargarse el proyecto:
Debido a algún fallo en la generación del archivo freertos.c que genera
de manera automática el CUBE cuando se activa la casilla "Generate peripherial
initialization as a pair of '.c/.h' files per peripherial" en Project Manager => Code Generator,
se ha optado por situar el código generado de FreeRTOS en el archivo main.c donde funciona sin 
problemas. Por consiguiente, tras descargar el código del repositorio y abrirlo con la herramienta
STM32CubeIDE, el sistema hará una petición de generación de código que habrá que aceptar. Posteriormente
es necesario eliminar el archivo autogenerado freertos.c y copiar a mano el archivo main.c que se encuentra en
github (o simplemente sustituirlo).

------------------------------------------------------------------------------------------------------------------


Procedimiento para añadir un sensor nuevo al proyecto:

En el CUBE:
1. Crear una nueva tarea de FreeRTOS en para el nuevo sensor StarTaskSensorX
   asignándole:
	 	- Prioridad osPriorityLow2
		- Stack Size 512
		- Parameter fsm_sx (donde x se refiere al nº de sensor).

2. Habilitar un canal del ADC para adquirir las medidas. Es opcional utilizar 
   un User Label para este pin, pero si se quiere seguir con la nomenclatura 
   se le puede llamar "ADC_SensorX". 

3. Habilitar un GPIO para alimentar al sensor. Se le puede llamar "SensorX_Supply".


En el código:

En el main():
4. Actualizar el código autogenerado a la hora de crear nuevas tareas: Sustituir el (void*) fsm_sx por &fsm_sx. Ej:

	myTaskSensor2Handle = osThreadNew(StartTaskSensor2, (void*) fsm_s2, &myTaskSensor2_attributes);
						🢃
  	myTaskSensor2Handle = osThreadNew(StartTaskSensor2, &fsm_s2, &myTaskSensor2_attributes);

En cada tarea:
5. Crear una nueva máquina de estados para ese sensor a partir del argumento (&fsm_s2) proporcionado en la creación 
   de la tarea: 
    		fsm_sensor_t* fsm_s1 = (fsm_sensor_t*)argument;

6. Inicialiar los parámetros del sensor utilizando la función sensor_initialization, se recomienda utilizar macros para
   la inicialización de los parámetros:
	    sensor_initialization(&sensor1, ID_Device, ID_ph_sensor, Sensor1_Supply_Pin, range_ph_basico, range_ph_acido, 
			          range_ph_max, ph_setup_period, ph_sleep_period, ph_measure_period, ph_average);
	  
	    En donde sensor1 es una variable tipo sensor_t, que recoge todos los parámetros para hacer funcionar la máquina
	    de estados del sensor 1.
	
7. Inicializar la máquina de estados:
    	    fsm_sensor_init(fsm_s1, &sensor1);
	    
	    En donde fsm_s1 es un puntero a la máquina de estados creada y sensor1 la dirección donde se encuentran los parámetros
	    establecidos.

8. Dentro del bucle infinito de la tarea, antes de llamar a fsm_fire(), cambiar el canal del ADC en el que realizar la medición.

9. Ejecutar fsm_fire().

10. Revisar y entender el funcionamiento de la gestión de las tareas de los demás sensores, como pueden ser diferentes variables y colas
    que garantizan el correcto funcionamiento del sistema, como por ejemplo el hecho de impedir la medida simultánea de dos sensores ya que
    comparten ADC (con diferente canal) a través de la comprobación "if(fsm_sx->fsm.current_state <= 4 && sensorX_ON == FALSE && sensorY_ON == FALSE)"
		
	     






