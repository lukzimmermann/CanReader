#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <CAN.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

#include "SPIFFS.h"

#define MAX_MESSAGES 200

struct CanMessage {
    long id;
    int length;
    uint8_t data[8];
    bool isRTR;
    bool isExtended;
    unsigned long time;
    float frequency;
};

CanMessage messages[MAX_MESSAGES];
int messageLength = 0;
unsigned int counter = 0;

const char* ssid = "SchoggiPopcorn";
const char* password = "C2!GP28Q$ZTWcJ";

int getIndex(long canId) {
    for (int i = 0; i < MAX_MESSAGES; i++) {
        if (messages[i].id == -1) return -1;
        if (messages[i].id == canId) return i;
    }
    return -1;
}

void canMessageToString(CanMessage message) {
    Serial.println();
    Serial.println("-----------------");
    Serial.print("ID:         0x");
    Serial.println(message.id, HEX);
    Serial.println("-----------------");
    Serial.print("Length:     ");
    Serial.println(message.length);
    Serial.print("IsExtended: ");
    Serial.println(message.isExtended);
    Serial.print("IsRTR:      ");
    Serial.println(message.isRTR);
    Serial.print("Frequency:  ");
    Serial.println(message.frequency);
}

void printCanMessage(long canId, CanMessage canMessage) {
    if (canMessage.id == canId) {
        canMessageToString(canMessage);
    }
}

String getRawCanDataJson() {
    String json = "[";
    for (int i = 0; i < messageLength; i++) {
        json.concat("{");
        json.concat("\"id\": " + String(messages[i].id) + ",");
        json.concat("\"length\": " + String(messages[i].length) + ",");
        json.concat("\"data\": [");
        for (int j = 0; j < messages[i].length; j++) {
            json.concat(String(messages[i].data[j]) + ",");
        }
        json.remove(json.length() - 1);
        json.concat("],");
        json.concat("\"isRTR\": " + String(messages[i].isRTR) + ",");
        json.concat("\"isExtended\": " + String(messages[i].isExtended) + ",");
        json.concat("\"frequency\": " + String(messages[i].frequency));
        json.concat("},");
    }
    json.remove(json.length() - 1);
    json.concat("]");
    return json;
}

AsyncWebServer server(80);

void setup() {
    Serial.begin(921600);
    CAN.setPins(4, 5);
    if (!CAN.begin(1000E3)) {
        Serial.println("Starting CAN failed!");
        while (1);
    }

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    Serial.print("IP:  ");
    Serial.println(WiFi.localIP());
    Serial.print("Mac: ");
    Serial.println(WiFi.macAddress());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/index.html", String());
    });

    server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/styles.css", "text/css");
    });

    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/script.js", "application/javascript");
    });

    server.on("/raw-can", HTTP_GET, [](AsyncWebServerRequest* request) {
        AsyncWebServerResponse* response = request->beginResponse(
            200, "application/json", getRawCanDataJson());
        response->addHeader("Access-Control-Allow-Origin", "*");
        request->send(response);
    });

    server.on(
        "/upload", HTTP_POST,
        [](AsyncWebServerRequest* request) {
            request->send(200, "text/plain", "File uploaded successfully!");
        },
        [](AsyncWebServerRequest* request, String filename, size_t index,
           uint8_t* data, size_t len, bool final) {
            static File file;

            if (index == 0) {
                if (!SPIFFS.exists("/dbc")) {
                    SPIFFS.mkdir("/dbc");
                }

                String filePath = "/dbc/" + filename;
                file = SPIFFS.open(filePath, "w");
                if (!file) {
                    Serial.println("Failed to open file for writing");
                    return;
                }
                Serial.printf("Starting upload: %s\n", filePath.c_str());
            }

            if (file) {
                file.write(data, len);
            }

            if (final) {
                if (file) {
                    file.close();
                    Serial.println("File upload complete");
                } else {
                    Serial.println("File upload failed");
                }
            }
        });

    server.on("/list-dbc-files", HTTP_GET, [](AsyncWebServerRequest* request) {
        DynamicJsonDocument jsonDoc(1024);
        JsonArray fileList = jsonDoc.createNestedArray("files");

        File root = SPIFFS.open("/dbc");
        if (!root || !root.isDirectory()) {
            request->send(500, "application/json",
                          "{\"error\":\"Failed to open /dbc directory\"}");
            return;
        }

        File file = root.openNextFile();
        while (file) {
            fileList.add(file.name());
            file = root.openNextFile();
        }

        String jsonString;
        serializeJson(jsonDoc, jsonString);

        // Send the JSON response
        request->send(200, "application/json", jsonString);
    });

    server.on("/delete-file", HTTP_POST, [](AsyncWebServerRequest* request) {
        // Check if the filename parameter is provided
        if (!request->hasParam("filename", true)) {
            request->send(400, "application/json",
                          "{\"error\":\"Filename not provided\"}");
            return;
        }

        // Get the filename from the request
        String filename = request->getParam("filename", true)->value();

        // Ensure the filename is in the /dbc directory
        if (!filename.startsWith("/dbc/")) {
            request->send(
                400, "application/json",
                "{\"error\":\"Invalid filename. Must be in /dbc directory.\"}");
            return;
        }

        // Attempt to delete the file
        if (SPIFFS.exists(filename)) {
            if (SPIFFS.remove(filename)) {
                request->send(200, "application/json",
                              "{\"message\":\"File deleted successfully\"}");
            } else {
                request->send(500, "application/json",
                              "{\"error\":\"Failed to delete file\"}");
            }
        } else {
            request->send(404, "application/json",
                          "{\"error\":\"File not found\"}");
        }
    });

    server.on("/get-file", HTTP_GET, [](AsyncWebServerRequest* request) {
        // Check if the filename parameter is provided
        if (!request->hasParam("filename")) {
            request->send(400, "application/json",
                          "{\"error\":\"Filename not provided\"}");
            return;
        }

        // Get the filename from the request
        String filename = request->getParam("filename")->value();

        // Ensure the filename is in the /dbc directory
        if (!filename.startsWith("/dbc/")) {
            request->send(
                400, "application/json",
                "{\"error\":\"Invalid filename. Must be in /dbc directory.\"}");
            return;
        }

        // Open the file for reading
        if (SPIFFS.exists(filename)) {
            File file = SPIFFS.open(filename, "r");
            if (!file) {
                request->send(500, "application/json",
                              "{\"error\":\"Failed to open file\"}");
                return;
            }

            // Read the file content
            String fileContent;
            while (file.available()) {
                fileContent += char(file.read());
            }
            file.close();

            // Send the file content as the response
            request->send(200, "text/plain", fileContent);
        } else {
            request->send(404, "application/json",
                          "{\"error\":\"File not found\"}");
        }
    });

    server.on("/free-space", HTTP_GET, [](AsyncWebServerRequest* request) {
        // ESP32 uses the following API
        size_t totalBytes = SPIFFS.totalBytes();
        size_t usedBytes = SPIFFS.usedBytes();
        size_t freeBytes = totalBytes - usedBytes;

        // Convert to kilobytes
        size_t freeKB = freeBytes / 1024;

        // Create a JSON response
        String jsonResponse = "{\"free_space_kb\": " + String(freeKB) + "}";
        request->send(200, "application/json", jsonResponse);
    });

    server.begin();
}

CanMessage createCanMessage(long id, int length, bool isRtr, bool isExtended) {
    CanMessage canMessage;
    canMessage.id = id;
    canMessage.length = length;
    canMessage.isRTR = isRtr;
    canMessage.isExtended = isExtended;
    canMessage.time = micros();
    return canMessage;
}

void loop() {
    int packetSize = CAN.parsePacket();
    if (packetSize) {
        int index = getIndex(CAN.packetId());

        if (index == -1) {
            messages[messageLength] =
                createCanMessage(CAN.packetId(), packetSize, CAN.packetRtr(),
                                 CAN.packetExtended());

            messageLength++;
            Serial.print("ID: 0x");
            Serial.println(CAN.packetId(), HEX);
        } else {
            messages[index].id = CAN.packetId();
            messages[index].length = packetSize;
            messages[index].isRTR = CAN.packetRtr();
            messages[index].isExtended = CAN.packetExtended();
            messages[index].frequency =
                double(1E6) / double(micros() - messages[index].time);
            messages[index].time = micros();

            byte bytes[packetSize];
            for (int i = 0; i < packetSize; i++) {
                messages[index].data[i] = CAN.read();
            }
        }
    }
}
