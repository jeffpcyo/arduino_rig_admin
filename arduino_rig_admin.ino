//ARDUINO 1.0+ ONLY
//ARDUINO 1.0+ ONLY
//Jeffpcyo & antho281
// Temperature sensor and I/O Board
// V0.0.2a BETA 

#include <Ethernet.h>
#include <SPI.h>
boolean reading = false;

boolean secondExe = false;

////////////////////////////////////////////////////////////////////////
//CONFIGURE
////////////////////////////////////////////////////////////////////////
  //byte ip[] = { 192, 168, 0, 199 };   //Manual setup only
  //byte gateway[] = { 192, 168, 0, 1 }; //Manual setup only
  //byte subnet[] = { 255, 255, 255, 0 }; //Manual setup only

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0x78, 0x4B, 0x87, 0xFE, 0x70, 0x56
};
/*byte ip[] = { 192, 168, 1, 177 }; // ip in lan
byte gateway[] = { 192, 168, 1, 1 }; // internet access via router
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask
//IPAddress ip(192, 168, 1, 177);*/

  EthernetServer server = EthernetServer(6979); //port 80
////////////////////////////////////////////////////////////////////////

int TempPins[] = {A0};
int TempPinsQty = 1;

void setup(){
  Serial.begin(9600);

  //Pins 10,11,12 & 13 are used by the ethernet shield

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

//    Ethernet.begin(mac, ip, gateway, subnet);
 Ethernet.begin(mac);
  //Ethernet.begin(mac, ip, gateway, subnet); //for manual setup

  server.begin();
  Serial.println(Ethernet.localIP());

}

void loop(){

  // listen for incoming clients, and process request.
  checkForClient();

}

void checkForClient(){

  EthernetClient client = server.available();

  if (client) {

    int pageContent = 0;

    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    boolean sentHeader = false;
    boolean rssPage = false;
    boolean sentFooter = false;
         
    while (client.connected()) {
      if (client.available()) {

        char c = client.read();

        if(reading && c == ' ') reading = false;
        if(c == '?') reading = true; //found the ?, begin reading the info

        if(reading){
          Serial.print(c);

           switch (c) {
            case 'r':
              rssPage = true;
              rssFeed(client);
              break;
            case '2':
              //add code here to trigger on 2
              pageContent = 2;
              break;
            case '3':
            //add code here to trigger on 3
              pageContent = 3;
              break;
            case '4':
            //add code here to trigger on 4
              pageContent = 4;
              break;
            case '5':
            //add code here to trigger on 5
              pageContent = 5;
              break;
            case '6':
            //add code here to trigger on 6
              pageContent = 6;
              break;
            case '7':
            //add code here to trigger on 7
              pageContent = 7;
              break;
            case '8':
            //add code here to trigger on 8
              pageContent = 8;
              break;
            case '9':
            //add code here to trigger on 9
              pageContent = 9;
              break;
            /*default: 
         // if nothing else matches, do the default
          // default is optional
          //client.println("<p>Hello World</p>");
          sensTemp(A0, client);
               break;*/
          }

        }

        if (c == '\n' && currentLineIsBlank)  break;

        if (c == '\n') {
          currentLineIsBlank = true;
        }else if (c != '\r') {
          currentLineIsBlank = false;
        }

      }
    }
        if(!sentHeader && !rssPage){
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();


        client.println("<table align='center' border='0'><tr id='Menu'><td><table border='1'><tr><th><a href='http://127.0.0.1.com:6979/?'>Temp</th><th>Rig 1</th><th>Rig 2</th><th> !! EXECUTE !! </th></tr>");
        client.println("<tr><td></td><td><a href='http://127.0.0.1:6979/?2'>ON</a></td><td><a href='http://127.0.0.1:6979/?4'>ON</a></td></tr>");
        client.println("<tr><td></td><td><a href='http://127.0.0.1:6979/?3'>OFF</a></td><td><a href='http://127.0.0.1:6979/?5'>OFF</a></td></tr></table></td></tr>");
        client.println("<tr id='content'><td>");

        
          sentHeader = true;
        }
             if(pageContent == 2){
              triggerPinOn(2, client);
             }
            else if(pageContent == 3){
              triggerPinOff(2, client);
             }
            else if(pageContent == 4){
              triggerPinOn(3, client);
             }
             else if(pageContent == 5){
              triggerPinOff(3, client);
             }
             else if(pageContent == 6){
              triggerPinOn(4, client);
             }
             else if(pageContent == 7){
              triggerPinOff(4, client);
             }
             else if(pageContent == 8){
              triggerPinOn(5, client);
             }
             else if(pageContent == 9){
              triggerPinOff(5, client);
             }
             // sensTemp(A0, client);
             
          if(!sentFooter && !rssPage){
          client.println("</td></tr></table>");

          sentFooter = true;
        }
    delay(1); // give the web browser time to receive the data
    client.stop(); // close the connection:

  } 

}

void triggerPinOn(int pin, EthernetClient client){
  if(secondExecution()) {
//blink a pin - Client needed just for HTML output purposes.  
  client.print("<p>Turning on Rig </p>");
  client.println(pin);
  client.println("<br>");

  digitalWrite(pin, HIGH);
  }
}

void triggerPinOff(int pin, EthernetClient client){
  if(secondExecution()) {
//blink a pin - Client needed just for HTML output purposes.  
  client.print("<p>Turning Off Rig </p>");
  client.println(pin);
  client.println("<br>");

  digitalWrite(pin, LOW);
  }
}

void sensTemp(int sensorPin, EthernetClient client){
  if(secondExecution()) {
        int sensorValue = analogRead(sensorPin); // getting input from analog pin 0
        float tempC = modTempC(sensorValue); // converting input values from the temperature sensor
        float tempF = modTempF(sensorValue); // converting input values from the temperature sensor

        // Afficher la température
          client.print("<p>Temperature Ambiante: ");
          client.print(tempC);
          client.print("°C</p><br />");
          client.print("<p>Temperature Ambiante: ");
          client.print(tempF);
          client.println("°F</p><br />");
}
}

// converting analog input value to ℃
float modTempC(int analog_val){
float v = 4.25; // base voltage (V)
float tempC = ((v * analog_val) / 1024) * 100; // conversion to Celsius
return tempC;
}
// converting analog input value to F
float modTempF(int analog_val){
float v = 4.25; // base voltage (V)
float tempC = ((v * analog_val) / 1024) * 100; // conversion to Celsius
float temperatureF = (tempC * 9.0 / 5.0) + 32.0; // conversion to F
return temperatureF;
}
void rssFeed(EthernetClient client){
          // send a standard http response header
          client.println("<?xml version="1.0" encoding="UTF-8"?>");
          client.println("<infos>");
          client.println();

          for (int i=0; i < TempPinsQty; i++){

            int sensorValue = analogRead(TempPins[i]); // getting input from analog pin 0
        float tempC = modTempC(sensorValue); // converting input values from the temperature sensor
        float tempF = modTempF(sensorValue); // converting input values from the temperature sensor
         
            client.print("<TempPinsQty>");
            client.print(TempPinsQty);
            client.println("</TempPinsQty>");
            client.println("<item>");
            client.print("<TempPin>");
            client.print(TempPins[i]);
            client.println("</TempPin>");
            client.print("<tempC>");
            client.print(tempC);
            client.println("</tempC>");
            client.print("<tempF>");
            client.print(tempF);
            client.println("</tempF>");
            client.println("</item>");
          }

          client.println("</infos>");
}
/*
void loop()
{
client.print( "GET /add.php?");
client.print("ip=");
client.print( ip );
client.print("&");
client.print("temp=");
client.print( tempC );
client.println( " HTTP/1.1");
client.println( "Host: WEBSERVER_HERE" );
client.println( "Content-Type: application/x-www-form-urlencoded" );
client.println( "Connection: close" );
client.println();
client.println();
client.stop();
delay( 60 );
}*/

boolean secondExecution() {
  if(secondExe == false) {
    secondExe = true;
  }
  else {
    secondExe = false;
  }

 return secondExe;
}


