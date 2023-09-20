
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define TRIGGER_PIN D5
#define ECHO_PIN D6
const char* ssid = "YOU";
const char* password = "sbvizag2019"; 
const int LDR=A0;// Defining LDR PIN 
int input_val=0;  // Varible to store Input values
int Led1=D7;
int Led2=D8;
#define ORG "j0usg3"
#define DEVICE_TYPE "srivalli"
#define DEVICE_ID "srivalli99"
#define TOKEN "srivalli99"
String command;
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/cmd/home/fmt/String";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
WiFiClient wifiClient;
void callback(char* topic, byte* payload, unsigned intpayloadLength);
PubSubClient client(server,1883,callback,wifiClient);
const int pin1 = D0;
const int pin2 = D1;
const int pin3 = D2;
const int pin4 = D3;

double duration;
double distance;


void setup() {
Serial.begin(115200);
Serial.println();
pinMode(Led1,OUTPUT);
pinMode(Led2,OUTPUT);

pinMode(TRIGGER_PIN, OUTPUT);
pinMode(ECHO_PIN, INPUT);

  //Serial.begin(9600);
pinMode(pin1,OUTPUT);
pinMode(pin2,OUTPUT);
pinMode(pin3,OUTPUT);
pinMode(pin4,OUTPUT);

wifiConnect();
mqttConnect();
}

void loop() {

if (!client.loop()) {
mqttConnect();
}
delay(100);
ultra();
ldrled();
}
void ultra()
{
digitalWrite(TRIGGER_PIN, LOW);  // Get Start
delayMicroseconds(2); // stable the line 
digitalWrite(TRIGGER_PIN, HIGH); // sending 10 us pulse
delayMicroseconds(10); // delay 
digitalWrite(TRIGGER_PIN, LOW); // after sending pulse wating to receive signals 
duration = pulseIn(ECHO_PIN, HIGH); // calculating time 
distance = (duration/2) / 29.1; // single path 
Serial.print(distance);
Serial.println(" cm");
if(distance<10&&distance>0.7)
{
digitalWrite(pin1,LOW);
digitalWrite(pin2,HIGH);
digitalWrite(pin3,LOW);
digitalWrite(pin4,HIGH);
Serial.println("left");

}

}
void ldrled()
{
input_val=analogRead(LDR);      // Reading Input
   Serial.print("LDR value is : ");                        
   Serial.println(input_val);        // Writing input on serial monitor.

if(input_val>1000)
   {
digitalWrite(D7,HIGH);
digitalWrite(D8,HIGH);
delay(1000);
} 
else
{
  digitalWrite(D7,LOW);
  digitalWrite(D8,LOW);
}
}
void wifiConnect() {
Serial.print("Connecting to "); Serial.print(ssid);
WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
Serial.print(".");
  }
Serial.print("nWiFi connected, IP address: "); Serial.println(WiFi.localIP());
}

void mqttConnect() {
  if (!client.connected()) {
Serial.print("Reconnecting MQTT client to "); 
Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) {
Serial.print(".");
      delay(500);
    }
initManagedDevice();
Serial.println();
  }
}
void initManagedDevice() {
  if (client.subscribe(topic)) {
Serial.println("subscribe to cmd OK");
  } else {
Serial.println("subscribe to cmd FAILED");
  }
}
void callback(char* topic, byte* payload, unsigned int payloadLength) {
Serial.print("callback invoked for topic: "); Serial.println(topic);

  for (int i = 0; i <payloadLength; i++) {
    //Serial.println((char)payload[i]);
    command += (char)payload[i];
  }
  Serial.println(command);

if(command == "BACKWARD"){
digitalWrite(pin1,LOW);
digitalWrite(pin2,HIGH);
digitalWrite(pin3,HIGH);
digitalWrite(pin4,LOW);
Serial.println("BACKWAD");
}
else if(command == "LEFT"){

digitalWrite(pin1,LOW);
digitalWrite(pin2,HIGH);
digitalWrite(pin3,LOW);
digitalWrite(pin4,HIGH);
Serial.println("left");

}

else if(command == "RIGHT"){
digitalWrite(pin1,HIGH);
digitalWrite(pin2,LOW);
digitalWrite(pin3,HIGH);
digitalWrite(pin4,LOW);
Serial.println("RIGHT");

}
else if(command == "FORWARD"){

Serial.println("FORWARD");
digitalWrite(pin1,HIGH);
digitalWrite(pin2,LOW);
digitalWrite(pin3,LOW);
digitalWrite(pin4,HIGH);
}
command ="";
}
