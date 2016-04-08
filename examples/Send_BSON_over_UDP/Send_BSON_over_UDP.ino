/*
 Created
 April 8, 2016
 by Hugo Arganda (@argandas)
*/

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <ardubson.h>

// MAC address
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0x05, 0x04};   // ethernet interface MAC address

// Server IP
IPAddress destIp(192, 168, 40, 36);      // destination ip address (change this depending on your network configuration)
unsigned int destPort = 10001;           // destination port (change this depending on your network configuration)

// Client IP
IPAddress localIp(192, 168, 40, 33);     // local ip address (change this depending on your network configuration)
unsigned int localPort = 2009;           // local port (change this depending on your network configuration)

// UDP instance
EthernetUDP Udp;

// BSON Object builder
BSONObjBuilder bob;

// setup the arduino and shields
void setup() {

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // start ethernet and udp
  if (Ethernet.begin(mac) == 0) {    // dhcp version
    Serial.println("DHCP Failed, reset device to try again");
    // no point in carrying on, loop indefinitely:
    while (true);
  } else {
    Serial.println("Device connected to network using DHCP");
  }

  // Give the Ethernet shield a second to initialize:
  delay(1000);

  // open UDP port
  Udp.begin(localPort);

  // show the local ip address (useful for dhcp)
  Serial.print("Local IP: ");
  Serial.println(Ethernet.localIP());

  // Append BSON data
  bob.append("Hello", "world");
  bob.append("Number", 1984);
  bob.append("Boolean", true);
  bob.obj();
}

// do tasks
void loop() {
  static int counter = 0;
  // send udp message
  Udp.beginPacket(destIp, destPort);
  int n = Udp.write(bob.data(), bob.len());
  Udp.endPacket();

  Serial.print("[");
  Serial.print(++counter);
  Serial.print("] ");
  Serial.print("Sent ");
  Serial.print(n);
  Serial.println(" bytes to server");
  
  delay(10 * 1000);
}