// Mengimpor liblary yang diperlukan
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Menginisiasi wifi
const char* ssid = "REDACTED";
const char* password = "REDACTED";

// Menginisiasi sensor DHT22 pada pin digital 5
#define DHTPIN 5
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// Menginisiasi web server pada port 80
AsyncWebServer server(80);

// Menginisiasi nilai awal
float minTemp = 1000;
float maxTemp = -1000;
float totalTemp = 0;
int countTemp = 0;

float minHum = 1000;
float maxHum = -1000;
float totalHum = 0;
int countHum = 0;

String readDHTTemperature() {
  // Membaca temperatur dengan satuan celcius
  float t = dht.readTemperature();
  // Mengecek jika gagal membaca temperatur dari sensor, program akan keluar dari loop
  if (isnan(t)) {    
    Serial.println("Gagal membaca data dari sensor DHT!");
    return "--";
  }
  else {
    Serial.println(t);
    // Mengecek apakah temperatur yang terbaca kurang dari minHum, jika iya minTemp di set menjadi kelembaban saat ini.
    if(t < minTemp) minTemp = t;
    // Mengecek apakah temperatur yang terbaca lebih dari maxHum, jika iya maxTemp di set menjadi kelembaban saat ini.
    if(t > maxTemp) maxTemp = t;
    // Mentotalkan temperatur
    totalTemp += t;
    // Banyaknya temperatur yang sudah terbaca
    countTemp++;
    return String(t);
  }
}

String readDHTHumidity() {
  float h = dht.readHumidity();
  // Mengecek jika gagal membaca kelembapan dari sensor, program akan keluar dari loop
  if (isnan(h)) {
    Serial.println("Gagal membaca data dari sensor DHT!");
    return "--";
  }
  else {
    Serial.println(h);
    // Mengecek apakah kelembaban yang terbaca kurang dari minHum, jika iya minHum di set menjadi kelembaban saat ini.
    if(h < minHum) minHum = h;
    // Mengecek apakah kelembaban yang terbaca lebih dari maxHum, jika iya maxHum di set menjadi kelembaban saat ini.
    if(h > maxHum) maxHum = h;
    // Mentotalkan kelembaban
    totalHum += h;
    // Banyaknya kelembaban yang sudah terbaca
    countHum++;
    return String(h);
  }
}

const char index_html[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="en">
<head>
    <title>ESP 32 DHT Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <style>@import url('https://fonts.googleapis.com/css2?family=Quicksand:wght@300..700&display=swap');</style>
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
        <link href="https://cdn.jsdelivr.net/npm/remixicon@4.2.0/fonts/remixicon.css" rel="stylesheet"/>
        <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/4.4.1/chart.min.js" integrity="sha512-L0Shl7nXXzIlBSUUPpxrokqq4ojqgZFQczTYlGjzONGTDAcLremjwaWv5A+EDLnxhQzY5xUZPWLOLqYRkY0Cbw==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
        <script type="text/JavaScript" src="https://MomentJS.com/downloads/moment.js"></script>
        <style>
          html, body {
            font-family: "Quicksand", Arial, sans-serif;
            background: #0D0D2B;
            color: #333;
            margin: 0;
            padding: 0;
          }
          .container {
            padding-top: 20px;
          }
          h1 {
            font-size: 4em;
            color: #ffffff;
            margin-bottom: 20px;
          }
          .card {
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
            height: 100%%;
          }
          .card h2 {
            font-size: 1.8em;
            color: #059e8a;
          }
          .card .icon {
            font-size: 1.5em;
            margin-right: 10px;
          }
          .card p {
            font-size: 1.5em;
            margin: 10px 0;
          }
          .card .sub-info {
            font-size: 1.2em;
            color: #666;
          }
          .units {
            font-size: 1rem;
          }
          .angka {
            font-size: 2em;
            font-weight: bold;
          }
          .angka-kecil {
            font-size: 0.8em;
          }
          .units-kecil { 
            font-size: 0.6rem;
          }
          .card-content {
            display: flex;
            justify-content: space-between;
            align-items: center;
          }
        </style>
    </head>
    <body>
        <div class="container">
          <h1 class="text-center">ESP32 DHT Server</h1>
          <div class="row">
            <div class="col-md-6 mb-3">
              <div class="card p-3 h-100">
                <div class="text-center">
                  <h2><i class="fas fa-thermometer-half icon" style="color:#059e8a;"></i>Temperatur</h2>
                </div>
                <div class="card-content">
                  <div class="text-center w-50">
                    <p><span class="angka" id="temperature">%TEMPERATURE%</span><sup class="units">&deg;C</sup></p>
                  </div>
                  <div class="w-50">
                    <p class="sub-info"><i class="fas fa-temperature-low" style="color: #006eff;"></i> Min: <span class="angka-kecil" id="minTemperature">%MINTEMPERATURE%</span><sup class="units-kecil">&deg;C</sup></p>
                    <p class="sub-info"><i class="fas fa-temperature-high" style="color: #ff0000;"></i> Max: <span class="angka-kecil" id="maxTemperature">%MAXTEMPERATURE%</span><sup class="units-kecil">&deg;C</sup></p>
                    <p class="sub-info"><i class="fas fa-thermometer-three-quarters" style="color:#00FD27;"></i>&nbsp;&nbsp;Avg: <span class="angka-kecil" id="avgTemperature">%AVGTEMPERATURE%</span><sup class="units-kecil">&deg;C</sup></p>
                  </div>
                </div>
              </div>
            </div>
            <div class="col-md-6 mb-3">
              <div class="card p-3 h-100">
                <div class="text-center">
                  <h2 style="color: #00add6;"><i class="ri-water-percent-fill icon" style="color:#00add6;"></i>Kelembapan</h2>
                </div>
                <div class="card-content">
                  <div class="text-center w-50">
                    <p><span class="angka" id="humidity">%HUMIDITY%</span><sup class="units">&#37;</sup></p>
                  </div>
                  <div class="w-50">
                    <p class="sub-info"><i class="ri-drop-line" style="color: #006eff;"></i> Min: <span class="angka-kecil" id="minHumidity">%MINHUMIDITY%</span><sup class="units-kecil">&#37;</sup></p>
                    <p class="sub-info"><i class="ri-drop-fill" style="color: #ff0000;"></i> Max: <span class="angka-kecil" id="maxHumidity">%MAXHUMIDITY%</span><sup class="units-kecil">&#37;</sup></p>
                    <p class="sub-info"><i class="ri-contrast-drop-2-fill" style="color:#00FD27"></i> Avg: <span class="angka-kecil" id="avgHumidity">%AVGHUMIDITY%</span><sup class="units-kecil">&#37;</sup></p>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
    <script>
    setInterval(function () {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("temperature").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "/temperature", true);
      xhttp.send();
    }, 10000);

    setInterval(function () {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("humidity").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "/humidity", true);
      xhttp.send();
    }, 10000);

    setInterval(function () {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("minTemperature").innerHTML =
            this.responseText;
        }
      };
      xhttp.open("GET", "/mintemp", true);
      xhttp.send();
    }, 10000);

    setInterval(function () {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("maxTemperature").innerHTML =
            this.responseText;
        }
      };
      xhttp.open("GET", "/maxtemp", true);
      xhttp.send();
    }, 10000);

    setInterval(function () {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("avgTemperature").innerHTML =
            this.responseText;
        }
      };
      xhttp.open("GET", "/avgtemp", true);
      xhttp.send();
    }, 10000);

    setInterval(function () {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("minHumidity").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "/minhum", true);
      xhttp.send();
    }, 10000);

    setInterval(function () {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("maxHumidity").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "/maxhum", true);
      xhttp.send();
    }, 10000);

    setInterval(function () {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("avgHumidity").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "/avghum", true);
      xhttp.send();
    }, 10000);
  </script>
</html>)rawliteral";

// Merubah nilai placeholder menjadi nilai yang sebenarnya
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
  // Inisiasi port serial untuk menampilkan console esp32 di komputer
  Serial.begin(115200);

  dht.begin();
  
  // Fungsi konek ke wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Mengeprint IP ESP32 di consol
  Serial.println(WiFi.localIP());

  // Rute untuk root/halaman web
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

  // Memulai server
  server.begin();
}
 
void loop(){
  
}
