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
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 2
#define RST_PIN 20
#define CURR_COMM_LEN 1001
#define ID_COMM_LEN 5
byte serial_buf_curr[CURR_COMM_LEN];
byte serial_buf_id[ID_COMM_LEN];

int status = WL_IDLE_STATUS;
const char* ssid = "WeWork";  //  your network SSID (name)
const char* pass = "P@ssw0rd";       // your network password

unsigned int localPort = 12345;      // local port to listen for UDP packets

byte packetBuffer[512]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
byte readDevice[4] = {0,0,0,0};   // Stores scanned ID read from RFID Module
bool IsDevice = false;
bool IsDeviceOld = false;
bool IsDevicePrev = false;
bool OldDevice = false;
bool DeviceApproved = true;
int currValue;
int count = 1;
int i = 0;

int analogInPin = A0;    // select the input pin for the potentiometer
const int rlyPin = 15; //D8

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
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  
  pinMode(rlyPin, OUTPUT);
  digitalWrite(rlyPin,LOW);
  pinMode(analogInPin, INPUT);
  serial_buf_curr[0] = 0xFF;
  serial_buf_id[0] = 0xFE;
}

// the loop function runs over and over again forever
void loop() {
  int N_Bytes = Udp.parsePacket();
  String received_command = "";
  
//  if (N_Bytes){
//    /*Serial.print(millis() / 1000);
//    Serial.print(":Packet of ");
//    Serial.print(noBytes);
//    Serial.print(" received from ");
//    Serial.print(Udp.remoteIP());
//    Serial.print(":");
//    Serial.println(Udp.remotePort());*/
//    // We've received a packet, read the data from it
//    Udp.read(packetBuffer,N_Bytes); // read the packet into the buffer
//
//    // display the packet contents in HEX
//    for (int i=1;i<=N_Bytes;i++)
//    {
//      received_command = received_command + char(packetBuffer[i - 1]);
//    } // end for
//    if (received_command == "1"){DeviceApproved = true;}
//    else {DeviceApproved = false;}
//    
//    /*Serial.println();
//    
//    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
//    Udp.write("Answer from ESP8266 ChipID#");
//    Udp.write("#IP of ESP8266#");
//    Udp.println(WiFi.localIP());
//    Udp.endPacket();
//    
//    Serial.println(received_command);
//    Serial.println();*/
//  
//    
//  } // end if
  
//  bool IsDevice = mfrc522.PICC_IsNewCardPresent(); //Check for connected device
//  bool IsDeviceOld = IsDevice||IsDevicePrev; //For skipping handling the same device again
//  if (IsDevice){ //Device exists
//    if (IsDeviceOld){ //Already handled the ID
      if (DeviceApproved){ //Already approved the device - Measure current
        //digitalWrite(rlyPin,HIGH);
        for (i=1;i<=500;i++){
          currValue=analogRead(A0);
          serial_buf_curr[i*2-1] = highByte(currValue);
          serial_buf_curr[i*2] = lowByte(currValue);
          delayMicroseconds(100);
        }
        //currValue = analogRead(analogInPin);
        //serial_buf_curr[count*2-1] = highByte(currValue); serial_buf_curr[count*2] = lowByte(currValue);
        //count++;
        //Serial.println(currValue,DEC);
        //Serial.println(count);
        //if (count>=499){
          Udp.beginPacket("10.25.104.157", 4321);
          Udp.write(serial_buf_curr,sizeof(char)*CURR_COMM_LEN);
          Udp.endPacket();
          //c/ount = 1;
          //Serial.println(count);
        //}
      }
//      else{
//        digitalWrite(rlyPin,LOW);
//      }
//    }
//    else{ //Hande the new connected ID
//      if(mfrc522.PICC_ReadCardSerial()){
//        for (i = 1; i < 4; i++) 
//        { 
//          serial_buf_id[i] = mfrc522.uid.uidByte[i];
//        }
//        Udp.beginPacket("10.25.104.157", 4321);
//        Udp.write(serial_buf_id,sizeof(char)*ID_COMM_LEN);
//        Udp.endPacket();
//      }
//    }
//  }
//  else{
//    if (IsDevicePrev){//Device was just now disconnected
//      serial_buf_id[0] = 0xFE; serial_buf_id[1] = 0x00; serial_buf_id[2] = 0x00; serial_buf_id[3] = 0x00; serial_buf_id[4] = 0x00;
//      digitalWrite(rlyPin,LOW);
//      Udp.beginPacket("10.25.104.157", 4321);
//      Udp.write(serial_buf_id,sizeof(char)*ID_COMM_LEN);
//      Udp.endPacket();
//    }
//  }
//  IsDevicePrev = IsDevice;

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


