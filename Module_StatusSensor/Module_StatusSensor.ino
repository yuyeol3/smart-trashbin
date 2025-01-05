
#include "Ultrasonic.h"
#include "Loadcell.h"
#include "WiFiEsp.h"
#include "Tof.h"
// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

char ssid[] = "";            // your network SSID (name)
char pass[] = "";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char server[] = "43.201.56.68";
int port = 80;
long sendInterval = 30;  // in seconds
// long sendInterval = 5;

// Initialize the Ethernet client object
WiFiEspClient client;
long prevTime, curTime;
void sendDataToServer(String postData);

void setup() {
  Serial.begin(9600);
  loadcell_setup();
  Serial.println("START");
  // initialize serial for ESP module
  Serial1.begin(115200);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");
  
  printWifiStatus();

  Serial.println();
  prevTime = millis() / 1000;
}




void sendDataToServer(String postData) {
  Serial.println("Connecting to server...");

  if (client.connect(server, port)) {
    Serial.println("Connected to server");

    // HTTP POST 요청 작성
    // String postData = "key1=value1&key2=value2"; // 전송할 데이터
    client.println("POST /trashbin/data HTTP/1.1");
    client.println("Host: httpbin.org");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println(); // 헤더 끝 표시
    client.println(postData); // 실제 데이터 전송

    Serial.println("Data sent to server");

    // 서버 응답 읽기
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }

    client.stop();
    Serial.println("\nDisconnected from server");
  } else {
    Serial.println("Connection to server failed");
  }
}

int  i = 0;

void loop()
{
  curTime = millis() / 1000;
  if (curTime - prevTime >= sendInterval) {
    prevTime = curTime;
    String sendData = "";
    sendData += String("name=") + "adventure design" + "&";
    sendData += String("key=") + "1234" + "&";
    sendData += String("pet=") + (( (56.0 - tof.getDistance())/38.0 ) * 100) + "&";
    sendData += String("can=") + max(((16.0 - us1.getDistance())/16.0 * 100), 0) + "&";
    sendData += String("others=") + 0 + "&";
    // sendData += String("dist3=") + us2.getDistance() + "&";
    sendData += String("weight=") + getWeight();
  // Serial.println(String("TOF: ") + tof.getDistance());
  // Serial.println(String("Ultrasonic1: ") + us1.getDistance());
  // Serial.println(String("Ultrasonic2: ") + us2.getDistance());
  // Serial.println(String("Loadcell: ") + getWeight() + "g");
    sendDataToServer(sendData);
  }

}




void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
