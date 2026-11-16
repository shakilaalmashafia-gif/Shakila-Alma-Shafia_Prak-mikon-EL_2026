#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>

const char* ssid = "cil";
const char* password = "luspmpruy";

WebServer server(80);
Adafruit_MPU6050 mpu;  // SDA ke GPIO 25, SCL ke GPIO 26

unsigned long waktuSebelumnya = 0;
const long interval = 2000;

// Membaca sudut kemiringan dalam derajat
float bacaKemiringan(float &nilaiAccelY) {
    sensors_event_t a, g, temp;

    mpu.getEvent(&a, &g, &temp);

    nilaiAccelY = a.acceleration.y;

    float roll = atan2(
        a.acceleration.y,
        a.acceleration.z
    ) * 180.0 / PI;

    return fabs(roll);
}

// Menentukan status berdasarkan sudut kemiringan
String tentukanStatus(float sudut) {
    if (sudut > 35) {
        return "BAHAYA";
    } else if (sudut >= 15) {
        return "SEDANG";
    } else {
        return "DATAR";
    }
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Tugas 2 - Sensor Kemiringan MPU6050</title>

    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            margin-top: 50px;
        }

        .badge {
            padding: 15px 30px;
            color: white;
            font-weight: bold;
            border-radius: 8px;
            display: inline-block;
            font-size: 24px;
        }

        .DATAR {
            background-color: #6c757d;
        }

        .SEDANG {
            background-color: #28a745;
        }

        .BAHAYA {
            background-color: #dc3545;
        }
    </style>
</head>

<body>
    <h2>Pemantauan Sudut Kemiringan (MPU6050)</h2>

    <h3>
        Sudut Kemiringan:
        <span id="sudut">--</span> &deg;
        (Accel Y:
        <span id="accY">--</span>
        m/s&sup2;)
    </h3>

    <div id="statusBox" class="badge SEDANG">MEMUAT...</div>

    <script>
        function ambilData() {
            fetch("/data")
                .then(res => res.json())
                .then(data => {
                    document.getElementById("sudut").innerText =
                        data.sudut.toFixed(1);

                    document.getElementById("accY").innerText =
                        data.accY.toFixed(2);

                    let box = document.getElementById("statusBox");

                    box.innerText = data.status;
                    box.className = "badge " + data.status;
                });
        }

        setInterval(ambilData, 2000);
        ambilData();
    </script>
</body>
</html>
)rawliteral";

// Mengirim data sensor dalam format JSON
void handleData() {
    float accY = 0;
    float sudut = bacaKemiringan(accY);
    String status = tentukanStatus(sudut);

    String json = "{\"sudut\":" + String(sudut)
                + ",\"accY\":" + String(accY)
                + ",\"status\":\"" + status + "\"}";

    server.send(200, "application/json", json);
}

void setup() {
    Serial.begin(115200);

    // Mengatur pin I2C khusus untuk ESP32 (SDA = 25, SCL = 26)
    Wire.begin(25, 26);

    // Memulai sensor MPU6050
    if (!mpu.begin()) {
        Serial.println(
            "Gagal menemukan sensor MPU6050! Cek kabel SDA(25) & SCL(26)."
        );

        while (1) {
            delay(10);
        }
    }

    Serial.println("MPU6050 Siap pada pin SDA:25, SCL:26!");

    WiFi.begin(ssid, password);
    Serial.print("Menghubungkan ke WiFi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println(
        "\nTerhubung! IP Address: " + WiFi.localIP().toString()
    );

    server.on("/", []() {
        server.send(200, "text/html", index_html);
    });

    server.on("/data", handleData);

    server.begin();
}

void loop() {
    server.handleClient();

    unsigned long waktuSekarang = millis();

    if (waktuSekarang - waktuSebelumnya >= interval) {
        waktuSebelumnya = waktuSekarang;

        float accY = 0;
        float sudut = bacaKemiringan(accY);
        String status = tentukanStatus(sudut);

        Serial.print("Accel Y: ");
        Serial.print(accY, 2);

        Serial.print(" m/s2 | Sudut Miring: ");
        Serial.print(sudut, 1);

        Serial.print("° | Status: ");
        Serial.println(status);
    }
}