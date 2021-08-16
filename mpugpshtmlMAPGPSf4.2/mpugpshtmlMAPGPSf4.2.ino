#include <Wire.h>
#include <WiFi.h>
#include <SoftwareSerial.h>//incluimos SoftwareSerial
#include <TinyGPS.h>//incluimos TinyGPS

TinyGPS gps;//Declaramos el objeto gps
SoftwareSerial serialgps(16,17);//Declaramos pines Rx y Tx


// A = MPU, B = WIFI, C = GPS, D = PYTHON COMMUNICATION, E = GPS MAP HTML



// 01. LECTURA DE DATOS MPU (GRAFICAS) + ENVIO WIFI + GPS  LONG, LAT, HORA


//A
const int MPU = 0x68;   //Default address of I2C for MPU 6050
int16_t AcX, AcY, AcZ;
float g = 9.78; //gravedad Arequipa
float buff_X[9000];
float buff_Y[9000];
float buff_Z[9000];
int16_t ax_m_s2_c, ay_m_s2_c, az_m_s2_c; 
int16_t valor1, valor2, valor3;


//B
// WiFi network info.
const char *ssid =  "ROS_Wireless";     // Enter your WiFi Name
const char *pass =  "A542256b7"; // Enter your WiFi Password
WiFiServer server(80);
IPAddress ip(192,168,1,10); //IP dir




//C
//Declaramos la variables para la obtención de datos
int year;
byte month, day, hour, minute, second, hundredths;
float plat,plong;
unsigned long chars;
unsigned short sentences, failed_checksum;


//D
unsigned long lastTime,sampleTime;

//E 
String yazi;

void setup() {
  //A Y B
  Wire.begin();                   // Wire library initialization
  Wire.beginTransmission(MPU);    // Begin transmission to MPU
  Wire.write(0x6B);               // PWR_MGMT_1 register
  Wire.write(0);                  // MPU-6050 to start mode
  Wire.endTransmission(true);
  Serial.begin(115200);           //Iniciamos el puerto serie





   Serial.println("Wrote to IMU"); //MEMS
  Serial.println("Connecting to "); //conexion
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");              // print ... till not connected
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address is : ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("Server started");


// C
  serialgps.begin(9600);//Iniciamos el puerto serie del gps
  //Imprimimos:
  Serial.println("");
  Serial.println("GPS GY-GPS6MV2 Leantec");
  Serial.println(" ---Buscando senal--- ");
  Serial.println("");

//D
  ax_m_s2_c = 0;
  ay_m_s2_c = 0;
  az_m_s2_c = 0;

 
  
}
  


void loop() {
  
  //smartdelay(1000);//E

//******
    read_GPS();
    read_data();
    WiFiClient client = server.available();  // listen for incoming clients

      float flat, flon;//E
      unsigned long age;//E
      gps.f_get_position(&flat, &flon, &age);//E
    
    if (client){
                                   
        Serial.println("new client");          
        String currentLine = "";                   //Storing the incoming data in the string
        while (client.connected()) 
        {            
          if (client.available())                  //if there is some client data available
          {                
            char c = client.read();                // read a byte
              if (c == '\n') {                     // check for newline character, 
                if (currentLine.length() == 0){     //if line is blank it means its the end of the client HTTP request


                yazi="<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><meta http-equiv='X-UA-Compatible' content='ie=edge'><title>My Google Map</title><style>#map{height:400px;width:100%;}</style></head> <body><h1>My Google Map</h1><div id='map'></div><script>function initMap(){var options = {zoom:8,center:{lat:";
                    yazi+=flat;
                    yazi+=",lng:";
                    yazi+=flon;
                    yazi+="}};var map = new google.maps.Map(document.getElementById('map'), options);google.maps.event.addListener(map, 'click', function(event){addMarker({coords:event.latLng});});var markers = [{coords:{lat:";
                yazi+=flat;
                yazi+=",lng:";
                yazi+=flon;
                yazi+="}}];for(var i = 0;i < markers.length;i++){addMarker(markers[i]);}function addMarker(props){var marker = new google.maps.Marker({position:props.coords,map:map,});if(props.iconImage){marker.setIcon(props.iconImage);}if(props.content){var infoWindow = new google.maps.InfoWindow({content:props.content});marker.addListener('click', function(){infoWindow.open(map, marker);});}}}</script><script async defer src='https://maps.googleapis.com/maps/api/js?key=AIzaSyDHNUG9E870MPZ38LzijxoPyPgtiUFYjTM&callback=initMap'></script></body></html>";
                

               
                client.print("<html>");    
                client.print("<head>");    
                client.print("<meta charset='utf-8' />");    
                client.print("<META HTTP-EQUIV='Refresh' CONTENT='1'>"); //Actualizacion pag   
                client.print("<br><html><title> ESP32 WebServer</title></html>");


                client.print("<body bgcolor=\"#E6E6FA\"><h1 style=\"text-align: center; color: FF5D5D\"> Sismógrafo ESP32 WebServer </h1>");
                //client.print("<a href='https://twitter.com/TwitterDev?ref_src=twsrc%5Etfw' class='twitter-follow-button' data-show-count='false'>Follow @TwitterDev</a><script async src='https://platform.twitter.com/widgets.js' charset='utf-8'></script>");
                


////////////////////////////////

                client.print("<center>");
                client.print("    <table border='0' width='90%' bgcolor='#F5F2FF'>");
                client.print("        <tr>");
                client.print("            <td colspan='2'>");
                client.print("  <center>");
                client.print("      <font color ='63496C' face='verdana'>");
                client.print("          <h1>Sismógrafo electrónico de medición y detección de movimientos telúricos con comunicación inalámbrica wifi portátil de bajo costo. </h1>");
                client.print("      </font>");
                client.print("  </center>");
                client.print("</td>");
                client.print("</tr>");
                client.print("<tr>");
                client.print("<td>  ");
                client.print("<p> La finalidad del trabajo de grado presentado, es el diseño y la construcción de un              hacia la red mediante Wi-Fi. </p>");

                 //client.print(" <h2><font color ='green' face = 'Impact'> Magnitud Registrada (Escala Richter)</h2></font>");


                //client.print("<br><hr><br> <p style=\"text-align: left; color: red; font-size:150% \">Valores Acelerometro (m/s^2): ");
                client.print("<hr> <p style=\"text-align: left; color: red; font-size:150% \">Valores Acelerometro (m/s^2): ");
                client.print("<p style=\"text-align: left; font-size:150% \">AcX: ");
                client.print(AcX* (g/16384.0) -0.18);
                client.print("<br/>AcY: ");
                client.print(AcY* (g/16384.0) +0.25);
                client.print("<br/>AcZ: ");
                client.print(AcZ* (g/16384.0) -0.70);

                client.print("<hr> <p style=\"text-align: left; color: red; font-size:150% \">Ubicación: ");
                client.print("<p style=\"text-align: left; font-size:150% \">Latitud/Longitud: ");
                client.print(plat);
                client.print(",");
                client.print(plong);
                client.print("<br/>Fecha: ");
                client.print(day);
                client.print("/");
                client.print(month);
                client.print("/");
                client.print(year);
                client.print("<br/>Hora: ");
                client.print(hour-5);
                client.print(":");
                client.print(minute);
                client.print(" ");
                client.print("horas");
                //client.print(second);

                
                client.print("<p style=\"text-align: left; color: red; font-size:150% \">JHOEL_ALDER: ");
                client.print("<p style=\"text-align: left; font-size:150% \">ESCUELA: ");
                client.print("EPIE UNSA");
                client.print("<br/>CURSO: ");
                client.print("PROYECTOS");
                client.print("<br/>PRESENTACIÓN: ");
                client.print("SISMOGRAFO");

                client.print("<p> </p>");












                
                 //client.print("  <center><img src='boxx.jpg' width='170' height='110' /></center>");
                                
                 client.print(" <h1><font color ='red' face = 'Lucida Console'> Medidas (IMU MEMS)</h1></font>");
                 client.print(" <h2><font color ='green' face = 'Impact'> Magnitud Registrada (Escala Richter)</h2></font>");
                 client.print(" <h3>Ubicación (lat,lon)</h3>");
                
                 client.print(" <p> </p>");
                
                
                  client.print(" <p>El software será el medio por el cual se le dará la inteligencia suficiente al sismógrafo para que desempeñe sus roles de manera eficaz, por medio de la programación de algoritmos destinados a dicho propósito. Además del desarrollo  para que se almacene y procese los datos registrados por la estación sismográfica.  Esto permitirá la transferencia de información entre la estación y el dispositivo a comunicar (PC, móvil, etc).</p>    ");
                
                
                
                  client.print("</td>");
                  client.print("<td>");
                  client.print(yazi);
                  client.print("</td>");
                  client.print("</tr>");
                        
                        
                  client.print("</table>");
                client.print("</center>");




                client.print("<center><a href='https://github.com/JhoelRN'> Git Hub Codes </a></center>");
                
                
                client.print("<p> </p>");
                client.print("<p> </p>");
                
                client.print("<center>");
                client.print("    <a href='inicio.html'> Inicio</a> -");
                client.print("    <a href='sismografo.html'> Sismografo</a> -");
                client.print("    <a href='nosotros.html'> Nosotros</a> -");
                client.print("    <a href='contactanos.html'> Contactanos</a> <br>Copyright 2021");
                client.print("    Mandanos un mensaje via email a ");
                client.print("    <a href = 'mailto:jhmamanih@unsa.edu.pe' >jhmamanih@unsa.edu.pe</a>");
                client.print("     y ");
                client.print("     <a href = 'mailto:aanahue@unsa.edu.pe' >aanahue@unsa.edu.pe</a>");
                     
                
                
                
                client.print("</center>");




/////////////////////////////////////
                
                //client.print("<a class='twitter-timeline' data-lang='es' data-width='280' data-height='500' href='https://twitter.com/TwitterDev?ref_src=twsrc%5Etfw'>Tweets by TwitterDev</a> <script async src='https://platform.twitter.com/widgets.js' charset='utf-8'></script>");
                
                client.print("</p></body>");        
                break;  // break out of the while loop:
              } 
               else
              {    // if you got a newline, then clear currentLine:
                currentLine = "";
              }
             } 
             else if (c != '\r') 
             {  // if you got anything else but a carriage return character,
              currentLine += c;       // add it to the end of the currentLine
             }
            }
          }
      }

//******

  

}


// F. Lectura de datos MPU6050 + Grafica Serial
void read_data(){
  Wire.beginTransmission(MPU);      // Start transfer
  Wire.write(0x3B);                 // register 0x3B (ACCEL_XOUT_H), records data in queue
  Wire.endTransmission(false);      // Maintain connection
  Wire.requestFrom(MPU, 14, true);  // Request data to MPU

  //************** ingreso de datos *******************


/*
  for(int j=0;j<9000;j++){
    buff_X[j] = buff_X[j+1];
    buff_Y[j] = buff_Y[j+1];
    buff_Z[j] = buff_Z[j+1];
  }
  
*/
  
  //Reads byte by byte
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)





// conversiones
  float ax_m_s2 = AcX * (g/16384.0);
  float ay_m_s2 = AcY * (g/16384.0);
  float az_m_s2 = AcZ * (g/16384.0);


// conversiones changed
  float ax_m_s2_c = ax_m_s2-0.18;
  float ay_m_s2_c = ay_m_s2+0.25;
  float az_m_s2_c = az_m_s2-0.70;






  //Prints values on Serial
 /* Serial.print(AcX);
  Serial.print(","); 
  Serial.print(AcY);
  Serial.print(","); 
  Serial.println(AcZ);
*/
  //Prints valores m/s
  Serial.print(ax_m_s2_c);
  Serial.print(","); 
  Serial.print(ay_m_s2_c);
  Serial.print(","); 
  Serial.println(az_m_s2_c);
  
  delay(20);
}

void read_GPS(){
  while(serialgps.available()) 
  {
    int c = serialgps.read();
 
    if(gps.encode(c))  
    {
      float latitude, longitude;
      gps.f_get_position(&latitude, &longitude);
      Serial.print("Latitud/Longitud: "); 
      Serial.print(latitude,5); 
      Serial.print(", "); 
      Serial.println(longitude,5);
      //agreg me
      plat = latitude;
      plong = longitude;

  gps.crack_datetime(&year,&month,&day,&hour,&minute,&second,&hundredths);
      Serial.print("Fecha: "); Serial.print(day, DEC); Serial.print("/"); 
      Serial.print(month, DEC); Serial.print("/"); Serial.print(year);
      Serial.print(" Hora: "); Serial.print(hour-5, DEC); Serial.print(":"); //peru -5
      Serial.print(minute, DEC); Serial.print(":"); Serial.print(second, DEC); 
      Serial.print("."); Serial.println(hundredths, DEC);
      Serial.print("Altitud (metros): ");
      Serial.println(gps.f_altitude()); 
      Serial.println();
      gps.stats(&chars, &sentences, &failed_checksum);  
    }
  }
}




//D 
float scaling(float x, float in_min, float in_max, float out_min, float out_max) 
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



/*
void sta_lta(){
  float lta_sum=0.0;
  for(int j=3999;j<=10000-1;j++){
    float tmp=buff_Z[j]*conv-11.52985;
    if(tmp<0){
      tmp=tmp*(-1);
      }
      lta_sum=lta_sum+tmp;
    }
    lta=lta_sum/6000;
    
  float sta_sum=0.0;
  for(int j=9799;j<=10000-1;j++){
    float tmp1=buff_Z[j]*conv-11.52985;
    if(tmp1<0){
      tmp1=tmp1*(-1);
      }
      sta_sum=sta_sum+tmp1;
    }
    sta=sta_sum/200;
    sta_to_lta=sta/lta;
}

*/
