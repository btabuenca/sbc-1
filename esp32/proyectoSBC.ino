#include <ThingsBoard.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>


const char* host = "esp32-g4";
#define WIFI_AP_NAME        "SBC"
#define WIFI_PASSWORD       "sbc$18-maceta"
#define TOKEN               "Qoa6isBokGrd32m4zsYr"
#define THINGSBOARD_SERVER  "demo.thingsboard.io"
#define SERIAL_DEBUG_BAUD    9600

WebServer server(80);
WiFiClient espClient;
ThingsBoard tb(espClient);
int status = WL_IDLE_STATUS;


int iluminacion;
int peso;
int humedad_desbordamiento;
int humedad_suelo1;
int humedad_suelo2;


const int pinDetHum = 32;
const int pinLdr = 38;
const int pinPincho1 = 36;
const int pinPincho2 = 37;
const int pinPeso = 39;
const int pinRele = 15;

#define ROJO 18
#define AZUL 23
#define VERDE 19
#define RELE 15

int value = 0;

unsigned long interval = 500;
unsigned long previousMillis;



const char* loginIndex =
  "<form name='loginForm'>"
  "<table width='20%' bgcolor='A09F9F' align='center'>"
  "<tr>"
  "<td colspan=2>"
  "<center><font size=4><b>Acceso a la plataforma OTA del grupo 4</b></font></center>"
  "<br>"
  "</td>"
  "<br>"
  "<br>"
  "</tr>"
  "<td>Username:</td>"
  "<td><input type='text' size=25 name='userid'><br></td>"
  "</tr>"
  "<br>"
  "<br>"
  "<tr>"
  "<td>Password:</td>"
  "<td><input type='Password' size=25 name='pwd'><br></td>"
  "<br>"
  "<br>"
  "</tr>"
  "<tr>"
  "<td><input type='submit' onclick='check(this.form)' value='Login'></td>"
  "</tr>"
  "</table>"
  "</form>"
  "<script>"
  "function check(form)"
  "{"
  "if(form.userid.value=='root' && form.pwd.value=='toor')"
  "{"
  "window.open('/serverIndex')"
  "}"
  "else"
  "{"
  " alert('Error Password or Username')/*displays error message*/"
  "}"
  "}"
  "</script>";

/*
   Server Index Page
*/

const char* serverIndex =
  "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
  "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
  "<input type='file' name='update'>"
  "<input type='submit' value='Update'>"
  "</form>"
  "<div id='prg'>progress: 0%</div>"
  "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')"
  "},"
  "error: function (a, b, c) {"
  "}"
  "});"
  "});"
  "</script>";



void setup() {

  previousMillis = millis();
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin(WIFI_AP_NAME, WIFI_PASSWORD);
  InitWiFi();
  pinMode(pinDetHum, OUTPUT);
  pinMode(pinLdr, INPUT);
  pinMode(pinPincho1, OUTPUT);
  pinMode(pinPincho2, OUTPUT);
  pinMode(pinPeso, OUTPUT);
  pinMode(pinRele, OUTPUT);

     pinMode(ROJO, OUTPUT);
   pinMode(AZUL, OUTPUT);
   pinMode(VERDE, OUTPUT);
   pinMode(RELE, OUTPUT);

  // Connect to WiFi network
  //  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  //  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) { //http://esp32-g4.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  /*return index page which is stored in serverIndex */
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
  server.begin();
}







void InitWiFi() {
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP_NAME, WIFI_PASSWORD);
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
    WiFi.begin(WIFI_AP_NAME, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connected to AP");
  }
}
void sendTelemetry() {
  tb.sendTelemetryInt("LDR", analogRead(pinLdr));
  Serial.println("luminosidad:");
  Serial.println(analogRead(pinLdr));
  tb.sendTelemetryInt("Desbordamiento", analogRead(pinDetHum));
  Serial.println("Desbordamiento:");
  Serial.println(analogRead(pinDetHum));
  tb.sendTelemetryInt("h_super", analogRead(pinPincho1));
  Serial.println("Hmedad superficie");
  Serial.println(analogRead(pinDetHum));
  tb.sendTelemetryInt("h_medio", analogRead(pinPincho2));
  Serial.println("Humedad medio");
  Serial.println(analogRead(pinPincho2));
  //tb.sendTelemetryInt("Relé",analogRead(pinRele));
  tb.sendTelemetryInt("Peso", analogRead(pinPeso));
  Serial.println("Peso");
  Serial.println(analogRead(pinPeso));

}

void actuadores() {


  if (iluminacion < 1500)
  {
    digitalWrite(ROJO, HIGH);
    digitalWrite(VERDE, HIGH);
  }
  else
  {
    digitalWrite(ROJO, LOW);
    digitalWrite(VERDE, LOW);
  }

  if (pinPincho1 > 2500 && pinDetHum > 1000)
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

void sensores() {

  humedad_suelo1 = analogRead(pinPincho1);
  peso = analogRead(pinPeso);
  /*
    peso = 8 * (peso - 228);
    Serial.println(" ");
    Serial.println(" ");

    Serial.print(peso);
    Serial.println("gramos");
  */

  humedad_desbordamiento = analogRead(pinDetHum);

  if (pinDetHum < 300)
{
  Serial.println("Peligro desboramiento");
    digitalWrite(RELE, LOW);
  }
  else
    digitalWrite(RELE, HIGH);


    iluminacion = analogRead(pinLdr);


  }

void loop() {




  unsigned long currentMillis = millis();
  if ((unsigned long)(currentMillis - previousMillis) >= interval)
  {
      sensores();
  actuadores();
  
    if (WiFi.status() != WL_CONNECTED) {
      reconnect();
      return;
    }
    // Reconnect to ThingsBoard, if needed
    if (!tb.connected())
    {
      bool subscribed = false;
      Serial.print("Connecting to: ");
      Serial.print(THINGSBOARD_SERVER);
      Serial.print(" with token ");
      Serial.println(TOKEN);
      if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
        Serial.println("Failed to connect");
        return;
      }
      else
      {
        Serial.println("connected");
      }
    }
    sendTelemetry();
    tb.loop();
    previousMillis = millis();
  }


  server.handleClient();

  delay(1);
}
