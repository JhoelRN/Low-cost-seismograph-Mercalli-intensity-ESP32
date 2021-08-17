#include <Wire.h>
#include <WiFi.h>

// 01. LECTURA DE DATOS MPU (GRAFICAS) + ENVIO WIFI


const int MPU = 0x68;   //Default address of I2C for MPU 6050
int16_t AcX, AcY, AcZ;
float g = 9.78; //gravedad Arequipa
float buff_X[9000];
float buff_Y[9000];
float buff_Z[9000];




// WiFi network info.
const char *ssid =  "ROS_Wireless";     // Enter your WiFi Name
const char *pass =  "A542256b7"; // Enter your WiFi Password
WiFiServer server(80);
//IPAddress ip(192,168,1,10); //IP dir



void setup() {
  Wire.begin();                   // Wire library initialization
  Wire.beginTransmission(MPU);    // Begin transmission to MPU
  Wire.write(0x6B);               // PWR_MGMT_1 register
  Wire.write(0);                  // MPU-6050 to start mode
  Wire.endTransmission(true);
  Serial.begin(115200);





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
}
  


void loop() {
  read_data();
  WiFiClient client = server.available();
  if (client) 
    {                             
      Serial.println("new client");          
      String currentLine = "";                   //Storing the incoming data in the string
      while (client.connected()) 
      {            
        if (client.available())                  //if there is some client data available
        {                
          char c = client.read();                // read a byte
            if (c == '\n')                       // check for newline character, 
            {                     
            if (currentLine.length() == 0)      //if line is blank it means its the end of the client HTTP request
            { 
              client.print("<html>");    
              client.print("<head>");    
              client.print("<meta charset='utf-8' />");    
              client.print("<META HTTP-EQUIV='Refresh' CONTENT='0.5'>"); //Actualizacion pag 
              client.print("<title>Sismografo ESP32 </title>");
              client.print("</head>");
              client.print("<body background='backgroundpage.jpg' text='#0d0d07'>");
              client.print("<center>");
              client.print("<img src='sismografoloogo.jpg' width='400' height='150' />");
              client.print("<img src='menu/m1.jpg' width='100' height='40'>");
              client.print("<img src='menu/m2.jpg' width='100' height='40'>");
              client.print("<img src='menu/m3.jpg' width='100' height='40'>");
              client.print("<img src='menu/m4.jpg' width='100' height='40'>");
              client.print("</center>");
              client.print("<p> </p>");
              client.print("<center>");
              client.print("<img src='PORTADA.jpg' width='85%'>");
              client.print("</center>");
              client.print("</body>");
              client.print("</html>");

              client.print("<br><html><title> ESP32 WebServer</title></html>");
              client.print("<body bgcolor=\"#E6E6FA\"><h1 style=\"text-align: center; color: blue\"> Sismografo ESP32 WebServer </h1>");
              client.print("<br><hr><br> <p style=\"text-align: left; color: red; font-size:150% \">Valores Acelerometro: ");
              client.print("<p style=\"text-align: left; font-size:150% \">AcX: ");
              client.print(AcX);
              client.print("<br/>AcY: ");
              client.print(AcY);
              client.print("<br/>AcZ: ");
              client.print(AcZ);
              client.print("<p style=\"text-align: left; color: red; font-size:150% \">MAMANI_ANAHUE: ");
              client.print("<p style=\"text-align: left; font-size:150% \">ESCUELA: ");
              client.print("EPIE UNSA");
              client.print("<br/>CURSO: ");
              client.print("PROYECTOS");
              client.print("<br/>PRESENTACIÓN: ");
              client.print("SISMOGRAFO");
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

  //Prints values on Serial
 /* Serial.print(AcX);
  Serial.print(","); 
  Serial.print(AcY);
  Serial.print(","); 
  Serial.println(AcZ);
*/
  //Prints valores m/s
  Serial.print(ax_m_s2);
  Serial.print(","); 
  Serial.print(ay_m_s2);
  Serial.print(","); 
  Serial.println(az_m_s2);
  
  delay(20);
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
