#include "ThingsBoard.h"
#include "WiFi.h"
#include "Adafruit_VEML7700.h"

Adafruit_VEML7700 veml = Adafruit_VEML7700();


#define WIFI_AP             "SBC"
#define WIFI_PASSWORD       "sbc$18-maceta"

// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token
#define TOKEN               "cIJQDpi21oDem3gnDDdz"
#define THINGSBOARD_SERVER  "demo.thingsboard.io"

// Baud rate for debug serial

// Initialize ThingsBoard client
WiFiClient espClient;
// Initialize ThingsBoard instance
ThingsBoard tb(espClient);
// the Wifi radio's status
int status = WL_IDLE_STATUS;

//SENSORES
#define SENSOR_HUMEDAD_TIERRA 4
#define LDR 27
#define SENSOR_AGUA 13
#define PESO 12

//ACTUADORES
#define ROJO 18
#define AZUL 23
#define VERDE 19
#define RELE 15


int V;
int ilum;
int temperatura;
float peso;
int humedad;

void setup() {
   pinMode(SENSOR_AGUA, INPUT);
   pinMode(PESO, INPUT);
   
   pinMode(LDR, INPUT);
   pinMode(ROJO, OUTPUT);
   pinMode(AZUL, OUTPUT);
   pinMode(VERDE, OUTPUT);
   pinMode(RELE, OUTPUT);
   
  // initialize serial for debugging
  //WiFi.begin(WIFI_AP, WIFI_PASSWORD);


//////////////////////////////////////////////////I2C - Iluminacion

while (!Serial) { delay(10); }
  Serial.begin(115200);
  Serial.println("Adafruit VEML7700 Test");

  if (!veml.begin()) {
    Serial.println("Sensor not found");
    while (1);
  }
  Serial.println("Sensor found");

  veml.setGain(VEML7700_GAIN_1);
  veml.setIntegrationTime(VEML7700_IT_800MS);

  Serial.print(F("Gain: "));
  switch (veml.getGain()) {
   // case VEML7700_GAIN_1: Serial.println("1"); break;
    //case VEML7700_GAIN_2: Serial.println("2"); break;
    case VEML7700_GAIN_1_4: Serial.println("1/4"); break;
    //case VEML7700_GAIN_1_8: Serial.println("1/8"); break;
  }

  Serial.print(F("Integration Time (ms): "));
  switch (veml.getIntegrationTime()) {
    //case VEML7700_IT_25MS: Serial.println("25"); break;
    //case VEML7700_IT_50MS: Serial.println("50"); break;
    case VEML7700_IT_100MS: Serial.println("100"); break;
    //case VEML7700_IT_200MS: Serial.println("200"); break;
    //case VEML7700_IT_400MS: Serial.println("400"); break;
    //case VEML7700_IT_800MS: Serial.println("800"); break;
  }

  //veml.powerSaveEnable(true);
  //veml.setPowerSaveMode(VEML7700_POWERSAVE_MODE4);

  veml.setLowThreshold(10000);
  veml.setHighThreshold(20000);
  veml.interruptEnable(true);
   
////////////////////////////////////////////////////////////////

}


void sensores(){
  
  humedad = analogRead(SENSOR_HUMEDAD_TIERRA);

  peso = analogRead(PESO);
  peso = 8*(peso - 228);
  Serial.println(" ");
  Serial.println(" ");

  Serial.print(peso);
  Serial.println("gramos");


  Serial.print("Humedad desbordamiento: ");
  Serial.println(analogRead(SENSOR_AGUA));  

  if(analogRead(SENSOR_AGUA) < 300)
  {
    Serial.println("Peligro desboramiento");
    digitalWrite(RELE, LOW);   
  }
  else 
    digitalWrite(RELE, HIGH);   

    
  ilum = analogRead(LDR);    
 
   
}

void actuadores(){


  if(ilum < 1500)
  {
    digitalWrite(ROJO, HIGH); 
    digitalWrite(VERDE, HIGH);
  }
  else
  {
    digitalWrite(ROJO, LOW); 
    digitalWrite(VERDE, LOW);   
  }

   Serial.print("Iluminación LDR: ");   
   Serial.println(ilum);  

   
   Serial.print("Humedad del suelo: ");
   Serial.println(humedad);
  
  if(humedad > 2500 && analogRead(SENSOR_AGUA) > 1000)
   {
    Serial.println("Regando");  
    digitalWrite(RELE, HIGH);   // turn the LED on (HIGH is the voltage level)
   }
   else 
   {
    Serial.println("Cortando riego");  
    digitalWrite(RELE, LOW);   // turn the LED on (HIGH is the voltage level)
   }
  
}

void loop() {
 
  sensores();
  actuadores();

    Serial.print("Lux: "); Serial.println(veml.readLux());
  //Serial.print("White: "); Serial.println(veml.readWhite());
  //Serial.print("Raw ALS: "); Serial.println(veml.readALS());

  uint16_t irq = veml.interruptStatus();
  if (irq & VEML7700_INTERRUPT_LOW) {
    //Serial.println("** Low threshold"); 
  }
  if (irq & VEML7700_INTERRUPT_HIGH) {
    //Serial.println("** High threshold"); 
  }
  delay(1000);


/*

  // Reconnect to ThingsBoard, if needed
  if (!tb.connected()) {

    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect");
      return;
    }
  }



  

  // Check if it is a time to send DHT22 temperature and humidity
    Serial.println("Sending data...");

    // Uploads new telemetry to ThingsBoard using MQTT. 
    // See https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api 
    // for more details

      tb.sendTelemetryInt("ldr", ilum);
    

  

  // Process messages
  tb.loop();*/
}

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void reconnect() {
  // Loop until we're reconnected
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connected to AP");
  }
}
