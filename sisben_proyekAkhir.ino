#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>

const char* ssid = "LARON GAMING";
const char* password = "ARDRA4404";

#define DHTPIN 5
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
AsyncWebServer server(80);

float minTemp = 1000;
float maxTemp = -1000;
float totalTemp = 0;
int countTemp = 0;

float minHum = 1000;
float maxHum = -1000;
float totalHum = 0;
int countHum = 0;

String readDHTTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) {    
    Serial.println("Gagal membaca data dari sensor DHT!");
    return "--";
  }
  else {
    Serial.println(t);
    if(t < minTemp) minTemp = t;
    if(t > maxTemp) maxTemp = t;
    totalTemp += t;
    countTemp++;
    return String(t);
  }
}

String readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Gagal membaca data dari sensor DHT!");
    return "--";
  }
  else {
    Serial.println(h);
    if(h < minHum) minHum = h;
    if(h > maxHum) maxHum = h;
    totalHum += h;
    countHum++;
    return String(h);
  }
}

const char index_html[] PROGMEM = R"rawliteral(
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
    <link href="https://cdn.jsdelivr.net/npm/remixicon@4.2.0/fonts/remixicon.css" rel="stylesheet"/>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/4.4.1/chart.min.js" integrity="sha512-L0Shl7nXXzIlBSUUPpxrokqq4ojqgZFQczTYlGjzONGTDAcLremjwaWv5A+EDLnxhQzY5xUZPWLOLqYRkY0Cbw==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
    <script type = "text/JavaScript" src = " https://MomentJS.com/downloads/moment.js"></script>
    <style>
      html {
       font-family: Arial;
       display: inline-block;
       margin: 0px auto;
       text-align: center;
      }
    h1 {
    font-size:4em;
    }
    h2 {
    font-size:2em;
    }
    h3 {
    font-size:1em;
    }
    h4 {
    font-size:0.5em;
    }
      p { font-size: 3.0rem; }
      .units { font-size: 1.2rem; }
      .dht-labels{
        /* font-size: 3.5rem; */
        vertical-align:middle;
        padding-bottom: 15px;
      }
      .angka{
        font-size: 3.1rem;
        vertical-align:middle;
      }
      .angka-kecil{
        font-size: 1.3rem;
        vertical-align:middle;
      }
      .units-kecil { 
        font-size: 0.8rem; 
        vertical-align:middle;
    }
    </style>
  </head>
  <body>
    <h1>ESP32 DHT Server</h1>
      <h2>
        <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
        <span class="dht-labels">Suhu: </span> 
        <span class="angka" id="temperature">%TEMPERATURE%</span>
        <sup class="units">&deg;C</sup>
        </h2>
    <h3><i class="fas fa-temperature-low" style="color: #006eff;"></i> 
      <span class="dht-labels">Min: </span><span class="angka-kecil" id="minTemperature">%MINTEMPERATURE%</span><sup class="units-kecil">&deg;C</sup> 
      <i class="fas fa-temperature-high" style="color: #ff0000;"></i> 
      <span class="dht-labels">Max: </span><span class="angka-kecil" id="maxTemperature">%MAXTEMPERATURE%</span><sup class="units-kecil">&deg;C</sup> 
      <i class="fas fa-thermometer-three-quarters" style="color:#00FD27;"></i> 
      <span class="dht-labels">Avg: </span><span class="angka-kecil" id="avgTemperature">%AVGTEMPERATURE%</span><sup class="units-kecil">&deg;C</sup> 
      </h3>
    </br>
    <h2>
      <i class="ri-water-percent-fill" style="color:#00add6;"></i> 
      <span class="dht-labels">Kelembapan: </span>
      <span class="angka" id="humidity">%HUMIDITY%</span>
      <sup class="units">&#37;</sup>
      </h2>
    <h3>
      <i class="ri-drop-line" style="color: #006eff;"></i> 
      <span class="dht-labels">Min: </span><span class="angka-kecil" id="minHumidity">%MINHUMIDITY%</span><sup class="units-kecil">&#37;</sup> 
      <i class="ri-drop-fill" style="color: #ff0000;"></i> 
      <span class="dht-labels">Max: </span><span class="angka-kecil" id="maxHumidity">%MAXHUMIDITY%</span><sup class="units-kecil">&#37;</sup> 
      <i class="ri-contrast-drop-2-fill" style="color:#00FD27"></i> 
      <span class="dht-labels">Avg: </span><span class="angka-kecil" id="avgHumidity">%AVGHUMIDITY%</span><sup class="units-kecil">&#37;</sup> 
      </h3>
    </br>
  </body>
  <script>
  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("temperature").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/temperature", true);
    xhttp.send();
  }, 10000 ) ;
  
  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("humidity").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/humidity", true);
    xhttp.send();
  }, 10000 ) ;

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("minTemperature").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/mintemp", true);
    xhttp.send();
  }, 10000 ) ;

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("maxTemperature").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/maxtemp", true);
    xhttp.send();
  }, 10000 ) ;

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("avgTemperature").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/avgtemp", true);
    xhttp.send();
  }, 10000 ) ;

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("minHumidity").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/minhum", true);
    xhttp.send();
  }, 10000 ) ;

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("maxHumidity").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/maxhum", true);
    xhttp.send();
  }, 10000 ) ;

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("avgHumidity").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/avghum", true);
    xhttp.send();
  }, 10000 ) ;
  </script>
  </html>)rawliteral";

// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
    return readDHTHumidity();
  }
  else if(var == "MINTEMPERATURE"){
    return String(minTemp);
  }
  else if(var == "MAXTEMPERATURE"){
    return String(maxTemp);
  }
  else if(var == "AVGTEMPERATURE"){
    return String(totalTemp/countTemp);
  }
  else if(var == "MINHUMIDITY"){
    return String(minHum);
  }
  else if(var == "MAXHUMIDITY"){
    return String(maxHum);
  }
  else if(var == "AVGHUMIDITY"){
    return String(totalHum/countHum);
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  dht.begin();
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });
  server.on("/mintemp", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", String(minTemp).c_str());
  });
  server.on("/maxtemp", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", String(maxTemp).c_str());
  });
  server.on("/avgtemp", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", String(totalTemp/countTemp).c_str());
  });
  server.on("/minhum", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", String(minHum).c_str());
  });
  server.on("/maxhum", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", String(maxHum).c_str());
  });
  server.on("/avghum", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", String(totalHum/countHum).c_str());
  });

  // Start server
  server.begin();
}
 
void loop(){
  
}
