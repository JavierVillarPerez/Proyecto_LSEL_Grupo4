%PLANTILLA PARA REPRESENTAR GRÁFICAS DE LOS VALORES DE LOS SENSORES EN
%THINGSPEAK

% Lectura del canal (sensor de temperatura)
readChannelID = 12397;
% Campo que guarda la temperatura:
TemperatureFieldID = 4; 

% Lectura en nuestro canal: 
readAPIKey = '3BV8ZQ76RFG28J78'; 

%% Lectura de los datos %%

[data, time] = thingSpeakRead(readChannelID, 'Field', TemperatureFieldID, 'NumPoints', 30, 'ReadKey', readAPIKey);

%% Representación de los datos %%

figure
plot(time, data, 'ko-')
ylim([30 48])
xlabel('Time')
ylabel('Conductividad (mS/m)')

yline(42, '--r*')
yline(32, '--m*')

suma = 0;

%%Hacer la media de los valores para representar la recta

for i = 1:30
    suma = data(i) + suma;
end

media = suma/30;

yline(media, 'b')

legend('Conductividad','Límite potabilidad sup','Límite potabilidad inf','Media valores')


