#include "math.h"

const byte slave = 1;

//============
// DHT22 stuff
//------------
#include "DHT.h"
#define DHTPIN 10       // what pin DHT22 is connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define LED_RED   5
#define LED_GREEN 6
#define LED_BLUE  7
#define LED_MESSAGE_FROM_MASTER 8
#define DEBUG 1
DHT dht(DHTPIN, DHTTYPE);
// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

//============
//max485 stuff
//------------
#include <SoftwareSerial.h>
#include "RS485_protocol.h"
SoftwareSerial rs485 (2, 3);  // receive pin, transmit pin
const byte ENABLE_PIN = 4;

byte buf [20];
byte received =0;
int nb=0;
int trial=0;
int reading_ok=0;
float hr=255.;
float tt=255.;
int count=0;

void fWrite (const byte what)
  {
  rs485.write (what);  
  }
  
int fAvailable ()
  {
  return rs485.available ();  
  }

int fRead ()
  {
  return rs485.read ();  
  }
  
void setup()
{
  Serial.begin(9600);
  dht.begin();
  rs485.begin (28800);
  pinMode (ENABLE_PIN, OUTPUT);  // driver output enable
  pinMode (LED_RED, OUTPUT);
  pinMode (LED_GREEN, OUTPUT);
  pinMode (LED_BLUE, OUTPUT);
  pinMode (LED_MESSAGE_FROM_MASTER, OUTPUT);
  
}

void led_rgb_common_anode(String color){
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, HIGH);
    if(color == "red"){
       digitalWrite(LED_RED, LOW);
    }else if(color == "green"){
       digitalWrite(LED_GREEN, LOW);  
    }else if(color == "blue"){
       digitalWrite(LED_BLUE, LOW);  
    }
}

void loop()
{

  Serial.print("This is slave ");
  Serial.println(slave);
  
  //Read sensor every 10 cycle
  if (count == 10){count=0;}
  trial=0;
  reading_ok = 0;
  while(trial < 10 & reading_ok == 0 & count == 0){ 
    trial++;
    Serial.print("DHT 22 reading trial ");
    Serial.println(trial);
    // Trun off master message led
    digitalWrite(LED_MESSAGE_FROM_MASTER, LOW); 
    led_rgb_common_anode("blue");
    Serial.print("Read DHT sensor HR ");
    hr = dht.readHumidity();
    Serial.println(hr);
    Serial.print("Read DHT sensor TT ");
    tt = dht.readTemperature();
    Serial.println(tt);
    // check if returns are valid, if they are NaN (not a number) then something went wrong!
    Serial.println("   Check sensor reading");
    if (isnan(tt) || isnan(hr)) {
      Serial.println("Failed to read from DHT");
      hr=255;
      tt=255-128;
      reading_ok=0;
      led_rgb_common_anode("red");
    } else {
      reading_ok=1;
      nb=nb+1;
      if(nb>32000){
         Serial.println("Reset number of readings");
         nb=1;
      }
      Serial.print("Number of readings ");
      Serial.println(nb);      Serial.print("Humidity: "); 
      Serial.print(hr);
      Serial.print(" %\t");
      Serial.print("Temperature: "); 
      Serial.print(tt);
      Serial.println(" *C");
      if( ( tt == 0 ) && (hr == 0) ){
         led_rgb_common_anode("red");
      } else {
         led_rgb_common_anode("green");
      }
    }
  }
  
  Serial.println("Check if master sent a message ");
  received = recvMsg (fAvailable, fRead, buf, sizeof (buf) - 1);
  //analogWrite (11, 255);
  if (received){
    digitalWrite(LED_MESSAGE_FROM_MASTER, HIGH);
    Serial.println(buf[0]);  
    
    if (buf [0] != slave){
      Serial.println("   Not responding to this request");
      return;  // not my device
    }
    if (buf [1] != 1)
      return;  // unknown command
      
    digitalWrite(LED_MESSAGE_FROM_MASTER, HIGH);
  
    byte msg [] = {
       0,  // device 0 (master)
       slave,  // from slave
       round(hr),
       round(tt)+128,
    };
    
    delay (1);  // give the master a moment to prepare to receive
    digitalWrite (ENABLE_PIN, HIGH);  // enable sending
    Serial.println("   Sending response to master ");
    sendMsg (fWrite, msg, sizeof msg);
    digitalWrite (ENABLE_PIN, LOW);  // disable sending
    digitalWrite(LED_MESSAGE_FROM_MASTER, LOW);
   }  // end if something received
   if(DEBUG){
       delay (1000);
   }else{
       delay (10000);
   }
   count++;
}  // end of loop
