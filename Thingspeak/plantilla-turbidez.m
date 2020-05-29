%PLANTILLA PARA REPRESENTAR GR�FICAS DE LOS VALORES DE LOS SENSORES EN
%THINGSPEAK

% Lectura del canal (sensor de temperatura)
readChannelID = 1006531;
% Campo que guarda la temperatura:
turbidez = 2; 

% Lectura en nuestro canal: 
readAPIKey = '3BV8ZQ76RFG28J78'; 

%% Lectura de los datos %%

samples = 15;
lim_sup = 80;
lim_inf = 10;
[data, time] = thingSpeakRead(readChannelID, 'Field', turbidez, 'NumPoints', samples, 'ReadKey', readAPIKey)

%% Representaci�n de los datos %%

figure
plot(time, data, '-.k.')
ylim([5 140])
xlabel('Time')
ylabel('Turbidez')

yline(lim_sup, '--r*')
yline(lim_inf, '--m*')


%%Hacer la media de los valores para representar la recta

media = nanmean(data)

yline(media, 'b')

legend('Turbidez','L�mite potabilidad sup','L�mite potabilidad inf','Media valores')