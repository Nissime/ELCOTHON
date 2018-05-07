/*****
 
 All the resources for this project:
 https://rntlab.com/

 v3 changed to Serial.readBytes
*****/
// CHANGE HERE

// Loading the ESP8266WiFi library and the PubSubClient library
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>

#define MSG_SIZE 5
#define SS_PIN 4
#define RST_PIN 5
#define Inv_MeasResistor 0.1 //10 Ohm resistor

const int TTL_MSG_SZ  = MSG_SIZE*4+15;

extern "C" {
#include "user_interface.h"
}

// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "Pi3AccPnt";
const char* password = "061015616";
const char* emptyRes = "0;0";
// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "10.10.0.1";
char msg_buff_t0 [MSG_SIZE+1]="24.1";
char msg_buff_ttl [TTL_MSG_SZ+1];
int number_of_bytes_received;
int rlyMode=0;

// Initializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);

// Connect an LED to each GPIO of your ESP8266
const int ledGPIO2 = 2;
const int rlyPin = 15; //D8


MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
byte readDevice[4] = {0,0,0,0};   // Stores scanned ID read from RFID Module
byte readDevicePrev[4] = {0,0,0,0};   // Stores previous scanned ID read from RFID Module
bool new_ID = false;
bool IsDevicePrev = false;
bool OldDevice = false;
//byte cardDB[4][4] = {{0x04,0x52,0xD2,0x3A},{0x04,0x4A,0xD2,0x3A},{0x04,0x44,0xD0,0x3A},{0x04,0x3C,0xD0,0x60}};
byte cardDB[2][4] = {{0x04,0x52,0xD2,0x3A},{0x04,0x4A,0xD2,0x3A}};
uint8_t i;
int Device = 0;
int current_pin = A0;
int meas_value;
int count = 0;
float Vmeas;
float Ameas;
// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  //*forDebug*/Serial.println();
  //*forDebug*/Serial.print("Connecting to ");
  //*forDebug*/Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //*forDebug*/Serial.println("");
  //*forDebug*/Serial.print("WiFi connected - ESP IP address: ");
  //*forDebug*/Serial.println(WiFi.localIP());


}

// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
  //*forDebug*/Serial.print("Message arrived on topic: ");
  //*forDebug*/Serial.print(topic);
  //*forDebug*/Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    //*forDebug*/Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  //*forDebug*/Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic home/office/esp1/gpio2, you check if the message is either 1 or 0. Turns the ESP GPIO according to the message

  if(topic=="esp8266/tmp1"){
      //*forDebug*/Serial.println();
      //*forDebug*/Serial.print("msg rec:");
      //Serial.println(messageTemp); // send msg to arduino
      if (messageTemp=="on"){
        client.publish("esp8266/tmp1_rply","on");
        digitalWrite(rlyPin,HIGH);
        rlyMode=1;
      }else{
        client.publish("esp8266/tmp1_rply","off");
        digitalWrite(rlyPin,LOW);
        rlyMode=0;
      }
  }
  //*forDebug*/Serial.println();
}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  //digitalWrite(LED_BUILTIN, HIGH);
  // Loop until we're reconnected
  while (!client.connected()) {
    /*forDebug*/Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client_home")) {
      //*forDebug*/Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("esp8266_a/meas"); //for notification only (to phone) %CHANGE HERE
      client.subscribe("esp8266/tmp1_rply");
      //digitalWrite(LED_BUILTIN, LOW);
    } else {
      //*forDebug*/Serial.print("failed, rc=");
      //*forDebug*/Serial.print(client.state());
      //*forDebug*/Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(2000);
    }
  }
}

// The setup function sets your ESP GPIOs to Outputs, starts the serial communication at a baud rate of 115200
// Sets your mqtt broker and sets the callback function
// The callback function is what receives messages and actually controls the LEDs
void setup() {
  pinMode(rlyPin, OUTPUT);
  digitalWrite(rlyPin,LOW);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  //  /client.connect("ESP8266Client_home");
  strcpy(msg_buff_ttl,msg_buff_t0);
  strcat(msg_buff_ttl,",0,");
}

int flagPrevExist=0;
// For this project, you don't need to change anything in the loop function. 
// Basically it ensures that you ESP is connected to your broker
void loop() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  char tmp[3]="0;";
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop()){
    client.connect("ESP8266Client_home");
  }
     
  dtostrf(random(2,150), 6, 4, msg_buff_t0);
  msg_buff_ttl[0]='\0';
  if (1==1){
    tmp[0]='1';
  }else{
    tmp[0]='2';
  }
  strcpy(msg_buff_ttl,tmp);
  strcat(msg_buff_ttl,msg_buff_t0);
  strcat(msg_buff_ttl,";");
  client.publish("esp8266_a/meas",msg_buff_ttl);
  delay(1000);

}

