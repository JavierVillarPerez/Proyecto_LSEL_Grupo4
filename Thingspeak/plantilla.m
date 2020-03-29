%PLANTILLA PARA REPRESENTAR GRÁFICAS DE LOS VALORES DE LOS SENSORES EN
%THINGSPEAK

% Lectura del canal (sensor de temperatura)
readChannelID = 12397;
% Campo que guarda la temperatura:
TemperatureFieldID = 4; 

% Lectura en nuestro canal: 
readAPIKey = '3BV8ZQ76RFG28J78'; 

%% Lectura de los datos %%
%% 60 muestras => 1 hora
tiempo = 12; %En horas
samples = 60*tiempo;
lim_sup = 80;
lim_inf = 10;
[data, time] = thingSpeakRead(readChannelID, 'Field', TemperatureFieldID, 'NumPoints', samples, 'ReadKey', readAPIKey);

%% Representación de los datos %%

figure
plot(time, data, 'k')
ylim([5 120])
xlabel('Time')
ylabel('pH')

yline(lim_sup, '--r*')
yline(lim_inf, '--m*')

suma = 0;

%%Hacer la media de los valores para representar la recta

for i = 1:samples
    suma = data(i) + suma;
end

media = suma/samples;

yline(media, 'b')

legend('pH','Límite potabilidad sup','Límite potabilidad inf','Media valores')