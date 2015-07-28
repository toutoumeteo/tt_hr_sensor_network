#include "Wire.h"
#include "DHT.h"
#include "Adafruit_HTU21DF.h"
#define SLAVE_ADDRESS 0x04 //Addresse sur bus i2c
#define SENSOR_LED_RED   5
#define SENSOR_LED_GREEN 6
#define SENSOR_LED_BLUE  7
#define RS485_LED_RED    8
#define RS485_LED_GREEN  9
#define RS485_LED_BLUE   10
#define DHTPIN           11 // what pin DHT22 is connected to
#define DHTTYPE DHT22       // DHT 22  (AM2302)
#
#define DEBUG 1
#
int data[4];

// Pour rs485 
#include "RS485_protocol.h"
#include <SoftwareSerial.h>
const byte ENABLE_PIN = 4;
SoftwareSerial rs485 (2, 3);  // receive pin, transmit pin
byte received = 0;
byte buf [10];
DHT dht(DHTPIN, DHTTYPE);

//HTU21D-F breakout board
// Connect Vin to 3-5VDC
// Connect GND to ground
// Connect SCL to I2C clock pin (A5 on UNO)
// Connect SDA to I2C data pin (A4 on UNO)
Adafruit_HTU21DF htu = Adafruit_HTU21DF();

// callback routines
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

void sensor_led_rgb_common_anode(String color){
    digitalWrite(SENSOR_LED_RED, HIGH);
    digitalWrite(SENSOR_LED_GREEN, HIGH);
    digitalWrite(SENSOR_LED_BLUE, HIGH);
    if(color == "red"){
       digitalWrite(SENSOR_LED_RED, LOW);
    }else if(color == "green"){
       digitalWrite(SENSOR_LED_GREEN, LOW);  
    }else if(color == "blue"){
       digitalWrite(SENSOR_LED_BLUE, LOW);  
    }
}

void rs485_led_rgb_common_anode(String color){
    digitalWrite(RS485_LED_RED, HIGH);
    digitalWrite(RS485_LED_GREEN, HIGH);
    digitalWrite(RS485_LED_BLUE, HIGH);
    if(color == "red"){
       digitalWrite(RS485_LED_RED, LOW);
    }else if(color == "green"){
       digitalWrite(RS485_LED_GREEN, LOW);  
    }else if(color == "blue"){
       digitalWrite(RS485_LED_BLUE, LOW);  
    }
}

void get_temp_DHT22(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  if(DEBUG){Serial.println("In get_temp_DHT22");}
  int trial=1;
  sensor_led_rgb_common_anode("blue");   
  float hr = dht.readHumidity();
  float tt = dht.readTemperature();
  while(isnan(tt) || isnan(hr) || trial < 10){
    hr = dht.readHumidity();
    tt = dht.readTemperature();
    trial++;
  }
  if(isnan(tt) || isnan(hr)){
     sensor_led_rgb_common_anode("red");
  }else{
     sensor_led_rgb_common_anode("green");  
  }   
  tt=tt;
  data[0]=round(tt);
  data[1]=round(hr);
}

void get_temp_HTU21D(){
  if(DEBUG){Serial.println("In get_temp_HTU21D");}
  int trial=1;
  sensor_led_rgb_common_anode("blue");
  float hr = htu.readHumidity();
  float tt = htu.readTemperature();
  while(isnan(tt) || isnan(hr) || trial < 10){
    hr = htu.readHumidity();
    tt = htu.readTemperature();
    trial++;
  }
  if(isnan(tt) || isnan(hr)){
     sensor_led_rgb_common_anode("red");
  }else{
     sensor_led_rgb_common_anode("green");  
  }   
  tt=tt;
  data[0]=round(tt);
  data[1]=round(hr);
}

void setup(){
  Serial.begin(9600);
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  //dht.begin();
  if (!htu.begin()) {
    Serial.println("Couldn't find sensor on breakout board HTU21D-F!");
    while (1);
  }
  rs485.begin (28800);
  pinMode (ENABLE_PIN, OUTPUT);  // driver output enable
  pinMode (SENSOR_LED_RED, OUTPUT);
  pinMode (SENSOR_LED_GREEN, OUTPUT);
  pinMode (SENSOR_LED_BLUE, OUTPUT);
  pinMode (RS485_LED_RED, OUTPUT);
  pinMode (RS485_LED_GREEN, OUTPUT);
  pinMode (RS485_LED_BLUE, OUTPUT);
}
 
void loop(){
  get_temp_DHT22();
  //get_temp_HTU21D();
  for (int slave = 1; slave <= 1 ; slave++) { 
    // assemble message
    byte msg [] = { 
      slave, // device
      1,     // get data
    };
    // send to slave
    // Note from Toutou: RGB LED with common anode is control by reverse LOW HIGH state
    rs485_led_rgb_common_anode("blue");
    digitalWrite (ENABLE_PIN, HIGH);  // enable sending
    sendMsg (fWrite, msg, sizeof msg);
    digitalWrite (ENABLE_PIN, LOW);  // disable sending
    // receive response  
    buf[2]=255;
    buf[3]=255;
    received = 0;
    int trial=0;
    while( (trial < 20) & (received == 0) ){
       trial=trial+1;
       Serial.print("Communication with slave trial ");
       Serial.println(trial);
       rs485_led_rgb_common_anode("blue");
       received = recvMsg (fAvailable, fRead, buf, sizeof buf);
       delay(1000);
    }
    if(received != 0){
      if(buf[3] != 255){
         buf[3]=buf[3]-128;
	 rs485_led_rgb_common_anode("green");
      }else{
	 rs485_led_rgb_common_anode("red");
      }
    } else {
       rs485_led_rgb_common_anode("red");
    }
    data[slave*2]  =buf[3];
    data[slave*2+1]=buf[2];
    
    delay(1000);

  }
  Serial.println("DEBUT MESSAGE");
  Serial.println("Coin SE : temperature "+String(data[0])+" C, humidite "+String(data[1])+" %");
  Serial.println("Coin SO : temperature "+String(data[2])+" C, humidite "+String(data[3])+" %");
  Serial.println("FIN MESSAGE");
  if(DEBUG){
     delay(1000);
  }else{
     delay(60000);
  }
}

