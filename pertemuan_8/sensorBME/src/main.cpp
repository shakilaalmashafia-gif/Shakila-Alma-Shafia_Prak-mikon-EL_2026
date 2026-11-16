#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// ================= WIFI =================
const char* ssid = "hermaN";
const char* password = "herma220807";

// ================= MQTT =================
const char* mqtt_server =
    "2d5cd8d3bcbd425a8865b60469b32931.s1.eu.hivemq.cloud";

const int mqtt_port = 8883;

const char* mqtt_user = "Praktikan25";
const char* mqtt_pass = "Mikrokontroler2026";

// ================= MQTT TOPIC =================
const char* topic_cmd = "ugm/mikon/kel14/cmd"; // SESUAIKAN KELOMPOK
const char* topic_status = "ugm/mikon/kel14/status";
const char* topic_temp = "ugm/mikon/kel14/temp";

// ================= PIN SENSOR =================
const int pinSDA = 21; // ESP32-S3: 8
const int pinSCL = 22; // ESP32-S3: 9

// ================= BME280 =================
Adafruit_BME280 bme;

// ================= DATA PUBLISH =================
unsigned long interval = 5000; // periode kirim data (ms)
unsigned long lastPub = 0;

// ================= MQTT CLIENT =================
WiFiClientSecure espClient;
PubSubClient client(espClient);

// ================= KIRIM DATA SENSOR =================
void kirimData() {
    float t = bme.readTemperature();

    if (isnan(t)) {
        Serial.println("BME280 gagal membaca");
        return;
    }

    char buf[16];
    dtostrf(t, 1, 2, buf);

    client.publish(topic_temp, buf);
}

// ================= MQTT CALLBACK =================
void callback(char* topic, byte* payload, unsigned int length) {
    char message[32];

    if (length >= sizeof(message)) {
        length = sizeof(message) - 1;
    }

    memcpy(message, payload, length);
    message[length] = '\0';

    Serial.print("Pesan masuk: ");
    Serial.println(message);
}

// ================= WIFI SETUP =================
void setup_wifi() {
    Serial.print("Koneksi WiFi...");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi Terhubung!");
}

// ================= MQTT RECONNECT =================
void reconnect() {
    while (!client.connected()) {
        Serial.print("Koneksi MQTT...");

        String clientId =
            "ESP32-BME-Kel-" + String(random(1000, 9999));

        if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
            Serial.println(" Terhubung!");

            client.subscribe(topic_cmd);
            client.publish(topic_status, "Tugas 1: Ready");
        }

        else {
            delay(3000);
        }
    }
}

// ================= SETUP =================
void setup() {
    Serial.begin(115200);

    Wire.begin(pinSDA, pinSCL);

    if (!bme.begin(0x76)) { // coba 0x77 jika tidak terdeteksi
        Serial.println("BME280 tidak terdeteksi!");

        while (1) {
            delay(1000);
        }
    }

    setup_wifi();

    espClient.setInsecure();

    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
}

// ================= LOOP =================
void loop() {
    if (!client.connected()) {
        reconnect();
    }

    client.loop();

    if (millis() - lastPub >= interval) {
        lastPub = millis();

        kirimData();
    }
}
