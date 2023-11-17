#include <mcp_can.h>
#include <SPI.h>

const int SPI_CS_PIN = 2;
const int CAN_INT_PIN = 4;
MCP_CAN CAN0(SPI_CS_PIN);

float vehicleSpeed = 0.0;
float engineRPM = 0.0;
float coolantTemp = 0.0;

byte speedRequest[] = {0x02, 0x01, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00};
byte rpmRequest[] = {0x02, 0x01, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00};
byte coolantTempRequest[] = {0x02, 0x01, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00};

void sendMessage(byte* pidMsg) {
    if (CAN0.sendMsgBuf(0x7DF, 0, 8, pidMsg) == CAN_OK) {
        Serial.println("PID request sent successfully!");
    } else {
        Serial.println("Error sending PID request");
    }

    // Wait for a bit to receive the message

    // Read data, if available
    if (!digitalRead(CAN_INT_PIN)) {
        unsigned long replyId;
        unsigned char dlc, data[8];
        if (CAN0.readMsgBuf(&replyId, &dlc, data) == CAN_OK) {
            // Processing the received message
            Serial.print("Message received with ID: 0x");
            Serial.println(replyId, HEX);
            Serial.print("Data: ");
            for (int i = 0; i < dlc; i++) {
                Serial.print(data[i], HEX);
                Serial.print(" ");
            }
            Serial.println();

            // Filter out unexpected messages
            if (replyId != 0x7E8 || dlc < 3) {
                Serial.println("Unexpected reply ID or message length");
                return;
            }

            if (data[1] == 0x41 && data[2] == 0x0D) {
                vehicleSpeed = data[3];
                Serial.print("Vehicle speed = ");
                Serial.println(vehicleSpeed);
            } else if (data[1] == 0x41 && data[2] == 0x0C) {
                engineRPM = ((data[3] * 256) + data[4]) / 4.0;
                Serial.print("Engine RPM = ");
                Serial.println(engineRPM);
            } 
        } else {
            Serial.println("Error reading message");
        }
    } else {
        Serial.println("No message received");
    }

    if (!digitalRead(CAN_INT_PIN)) {
        unsigned long replyId;
        unsigned char dlc, data[8];
        if (CAN0.readMsgBuf(&replyId, &dlc, data) == CAN_OK && data[1] == 0x41 && data[2] == 0x05) {
            coolantTemp = data[3] - 40;
            Serial.print("Coolant temperature = ");
            Serial.println(coolantTemp);
        }
    }
    delay(100);
}

void setup() {
    Serial.begin(500000);
    while (!Serial);

    // Initialize MCP2515 running at 8MHz with a baudrate of 500kb/s
    if (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
        Serial.println("MCP2515 Initialized Successfully!");
    } else {
        Serial.println("Error Initializing MCP2515... Check your connections and try again.");
        while (1);
    }

    // Set normal operation mode
    CAN0.setMode(MCP_NORMAL);

    // Configure CAN interrupt pin
    pinMode(CAN_INT_PIN, INPUT);
    Serial.println("CAN OBD-II engine RPM");
}

void loop() {
    // Send PID request for RPM
    sendMessage(speedRequest);
    sendMessage(coolantTempRequest);
    sendMessage(rpmRequest);

    delay(5000); 
}
