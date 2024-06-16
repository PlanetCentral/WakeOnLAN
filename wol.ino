//Adjust to match your Blynk configuration
#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKEN"

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <BlynkSimpleEthernet.h>

// Ethernet Shield MAC address
//Enter your Shield's MAC address like 0xAA, 0xBB, etc...
byte mac[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// PC's MAC address for WOL
//Enter your PC's MAC address like 0xAA, 0xBB, etc...
byte pcMac[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// Network settings
//Replace the 0's in Arduino IP and Router IP with your IP address, do NOT adjust any commas!
IPAddress ip(000, 000, 0, 000);  // Arduino's IP address
IPAddress gateway(000, 000, 0, 000);  // Your router's IP address
IPAddress subnet(255, 255, 255, 0);

EthernetUDP Udp;
WidgetTerminal terminal(V1);

void sendWOL() {
    byte wolPacket[102]; // Magic Packet

    // First 6 bytes should be 0xFF
    for (int i = 0; i < 6; i++) {
        wolPacket[i] = 0xFF;
    }

    // Repeat MAC address 16 times
    for (int i = 1; i <= 16; i++) {
        for (int j = 0; j < 6; j++) {
            wolPacket[i * 6 + j] = pcMac[j];
        }
    }

    // Send WOL packet 
    Udp.beginPacket(IPAddress(255, 255, 255, 255), 9); // Broadcast address and port
    Udp.write(wolPacket, sizeof(wolPacket));
    Udp.endPacket();
}

BLYNK_WRITE(V0) {
    if (param.asInt() == 1) {
        sendWOL(); // Send WOL Packet
        terminal.println("WOL Packet Sent");
        terminal.flush();
    }
}

void setup() {
    Ethernet.begin(mac, ip, gateway, subnet);
    Udp.begin(8888); // Random port for UDP
    Blynk.begin(BLYNK_AUTH_TOKEN);
}

void loop() {
    Blynk.run();
}

