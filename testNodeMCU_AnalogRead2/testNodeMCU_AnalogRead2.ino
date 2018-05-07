/*
 * 31 mar 2015
 * This sketch display UDP packets coming from an UDP client.
 * On a Mac the NC command can be used to send UDP. (nc -u 192.168.1.101 2390). 
 *
 * Configuration : Enter the ssid and password of your Wifi AP. Enter the port number your server is listening on.
 *
 */
 /*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO 
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products
  
  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
  
  modified 2 Sep 2016
  by Arturo Guadalupi
  
  modified 8 Sep 2016
  by Colby Newman
*/
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#define COMM_LEN 1001
byte serial_buf[COMM_LEN];

int status = WL_IDLE_STATUS;
const char* ssid = "WeWork";  //  your network SSID (name)
const char* pass = "P@ssw0rd";       // your network password

unsigned int localPort = 12345;      // local port to listen for UDP packets

byte packetBuffer[512]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;

int sensorPin = A0;    // select the input pin for the potentiometer


// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  delay(10);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println('\n');

  // setting up Station AP
  WiFi.begin(ssid, pass);
  
  // Wait for connect to AP
  Serial.print(" Connecting to");
  Serial.print(ssid);
  Serial.println("...");
  
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }
  
  Serial.println('\n');
  Serial.println("Connected to wifi");
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer

  printWifiStatus();
  Serial.println(localPort);
  Udp.begin(localPort);
}
int sensorValue;
// the loop function runs over and over again forever
void loop() {
  int i=0;

  serial_buf[0]=0xFE;
  serial_buf[1]=0xAB;
  serial_buf[2]=0xCD;
  serial_buf[3]=0xEF;
  serial_buf[4]=0x12;
  serial_buf[5]=0x34;

  Udp.beginPacket("10.25.104.157", 4321);
  Udp.write(serial_buf,sizeof(char)*6);
  Udp.endPacket();
  
  serial_buf[0]=0xFF;
  for (i=1;i<=500;i++){
    sensorValue=analogRead(A0);
    serial_buf[i*2-1] = highByte(sensorValue);
    serial_buf[i*2] = lowByte(sensorValue);
    delayMicroseconds(100);
  }
  Serial.println("read analog data");
  
  Udp.beginPacket("10.25.104.157", 4321);
  Udp.write(serial_buf,sizeof(char)*1001);
  Udp.endPacket();
  
  int noBytes = Udp.parsePacket();
  /*while (~noBytes){
    noBytes = Udp.parsePacket();
    delay(100);
  }
  Udp.read(packetBuffer,noBytes); 
  
  */  
  String received_command = "";
  
  if ( noBytes ) {
    Serial.print(millis() / 1000);
    Serial.print(":Packet of ");
    Serial.print(noBytes);
    Serial.print(" received from ");
    Serial.print(Udp.remoteIP());
    Serial.print(":");
    Serial.println(Udp.remotePort());
    // We've received a packet, read the data from it
    Udp.read(packetBuffer,noBytes); // read the packet into the buffer

    // display the packet contents in HEX
    for (int i=1;i<=noBytes;i++)
    {
      Serial.print(packetBuffer[i-1],HEX);
      received_command = received_command + char(packetBuffer[i - 1]);
      if (i % 32 == 0)
      {
        Serial.println();
      }
      else Serial.print(' ');
    } // end for
    Serial.println();
    
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write("Answer from ESP8266 ChipID#");
    Udp.write("#IP of ESP8266#");
    Udp.println(WiFi.localIP());
    Udp.endPacket();
    
    Serial.println(received_command);
    Serial.println();
  
    
  } // end if
  /*
  if (false){
    Udp.beginPacket("10.25.104.157", 4321);
    Udp.write(23);
    Udp.endPacket();
    }
  //Serial.println(analogRead(A0));
  */
  delay(50);                       // wait
}



void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}
/////////////////////////


