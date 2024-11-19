#include "CanWebServer.h"

CanWebServer::CanWebServer(uint16_t port)
    : server(port), messages(nullptr), messageLength(0) {}

void CanWebServer::begin() {
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
    setUpWebServer();
    setUpBackendServer();
    server.begin();
}

void CanWebServer::updateData(CanMessage* messages, int length) {
    this->messages = messages;
    this->messageLength = length;
}

void CanWebServer::setUpWebServer() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/index.html", String());
    });

    server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/styles.css", "text/css");
    });

    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/script.js", "application/javascript");
    });
}

void CanWebServer::setUpBackendServer() {
    server.on("/get-can-data", HTTP_GET,
              [this](AsyncWebServerRequest* request) {
                  String canMessageJson = canDataToJson();
                  if (canMessageJson.length() > 0) {
                      // Send the stored CAN message as JSON
                      AsyncWebServerResponse* response = request->beginResponse(
                          200, "application/json", canMessageJson);
                      response->addHeader("Access-Control-Allow-Origin", "*");
                      request->send(response);
                  } else {
                      // Send a 404 error if no data is available
                      request->send(404, "application/json",
                                    "{\"error\":\"No CAN data available\"}");
                  }
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

    server.on("/delete-file", HTTP_DELETE, [](AsyncWebServerRequest* request) {
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
            request->send(400, "application/json",
                          "{\"error\":\"Invalid filename. Must be in "
                          "/dbc directory.\"}");
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
            request->send(400, "application/json",
                          "{\"error\":\"Invalid filename. Must be in /dbc "
                          "directory.\"}");
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
}

String CanWebServer::canDataToJson() {
    const unsigned long startTime = micros();

    const int bufferSize = 8192;
    char json[bufferSize];
    int pos = 0;

    pos += snprintf(json + pos, bufferSize - pos, "[");
    for (int i = 0; i < messageLength; i++) {
        pos += snprintf(json + pos, bufferSize - pos, "{");
        pos += snprintf(json + pos, bufferSize - pos, "\"id\":%lu,",
                        messages[i].id);
        pos += snprintf(json + pos, bufferSize - pos, "\"length\":%d,",
                        messages[i].length);

        pos += snprintf(json + pos, bufferSize - pos, "\"data\":[");
        for (int j = 0; j < messages[i].length; j++) {
            pos += snprintf(json + pos, bufferSize - pos, "%u,",
                            messages[i].data[j]);
        }
        if (messages[i].length > 0) {
            pos--;  // Remove trailing comma
        }
        pos += snprintf(json + pos, bufferSize - pos, "],");

        pos += snprintf(json + pos, bufferSize - pos, "\"isRTR\":%s,",
                        messages[i].isRTR ? "true" : "false");
        pos += snprintf(json + pos, bufferSize - pos, "\"isExtended\":%s,",
                        messages[i].isExtended ? "true" : "false");
        pos += snprintf(json + pos, bufferSize - pos, "\"frequency\":%.2f",
                        messages[i].frequency);
        pos += snprintf(json + pos, bufferSize - pos, "},");
    }
    if (messageLength > 0) {
        pos--;  // Remove trailing comma
    }
    pos += snprintf(json + pos, bufferSize - pos, "]");

    Serial.print("CreateJson: ");
    Serial.println(micros() - startTime);

    return String(json);
}