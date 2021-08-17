


# Low-cost-seismograph-Mercalli-intensity-ESP32
Sistema de medición intensidada Mercalli de ultra bajo costo, construido con componentes accesibles. Este proyecto busca acercar estudios de sismología a más estudiantes, haciendo que el costo no sea una barrera.

![implementasis](https://user-images.githubusercontent.com/62358739/129647080-57a27355-7e05-47a8-afac-6daac7f7d188.png)


## Planteamiento inicial
Arequipa se encuentra en una de las regiones de potencial sísmico, donde es frecuente la ocurrencia de sismos de diversas magnitudes a diferentes niveles de profundidad con un  volcán Misti que pertenece al Cinturón de Fuego del Pacífico, alineado en el borde de la interacción de las placas tectónicas donde se registra principalmente el proceso geodinámico de la subducción. En este proceso se tiene al rozamiento que ocurre entre la placa oceánica y la continental y es donde se generan fuertes sismos. \citep{tavera_volc} 

En defecto, en la región de Arequipa se ve que existe deficiencia de sistema que informe directamente el estado actual de algún movimiento de la tierra durante un sismo o terremoto hacia la población. La población carece de herramientas para revisar datos de un sismo. Para tener acceso a esta información se tiene opciones como realizar una consulta telefónica o referenciarse a la publicación que el Instituto Geofísico del Perú(IGP) realiza luego de un sismo via Internet. Además muchos de los sismos no son tomados en cuenta, esto se debe principalmente a la distancia epicentral y la magnitud del sismo.

Es por eso que se requiere un diseño factible para un sismógrafo bajo costo que sea capaz de captar magnitudes y parámetros de un sismo con acceso a portabilidad e información compartida mediante wifi.






## Documentación
En este espacio se describirá la documentación del proyecto y en el sitio web: [GitHub](https://github.com/JhoelRN/Low-cost-seismograph-Mercalli-intensity-ESP32).

En este espacio se encuentran los archivos como son los programas .ino y programas html. 
También se puede encontrar documentación del informe final del trabajo y en modo resumen un artículo v1.
Implementación: [Youtube](https://www.youtube.com/channel/UCplPBdMY127yMaeyeEgBMew).


## Diseño Funcional
![funcionamiento](https://user-images.githubusercontent.com/62358739/129647294-a2c7bdb7-6818-445f-a763-217d2706413c.jpeg)
  
  ![diagramaseismograph](https://user-images.githubusercontent.com/62358739/129647404-38d81415-31fa-47ac-9ed7-30d5c9b00278.png)
El algoritmo implementado se describe en la figura mostrada, de tal manera que inicia en la sensación de la magnitud de la aceleración. Con esta, al momento de pasar un umbral (valor especificado) genera un registro almacenado en un arreglo unidimensional de tamaño 'n'. Al momento de que se registre el evento de ondas, los datos son enviados a detectarse una amplitud de valor máximo. Éste último valor describirá la máxima sensación en el movimiento telúrico, y por medio de escala de Mercalli se registra un valor de salida regulada por valores Mercalli. Esta funcionalidad se realiza en un ciclo repetitivo loop, de tal manera que cada vez que se registre un movimiento se tenga un valor de salida y a la vez, siempre se anda verificando que se sobrepase el umbral para detección de su amplitud.
  
  
### Página web server
![paginaRes](https://user-images.githubusercontent.com/62358739/129647362-17fdb7f1-7c1e-4726-9cef-00c1a0dc4e49.PNG)



## IOT Open Source
Estes un proyecto de software y hardware opensource provisto bajo licencia Generic Public Licence v3.8.








