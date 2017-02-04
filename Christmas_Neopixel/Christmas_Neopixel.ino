/*
   Christmas Neopixel for THE ARDUINO INTERNET OF HOLIDAY THINGS contest
   More info: https://create.arduino.cc/projecthub/contests/arduino-holiday-contest

   A web server that lets you configure a 8x8 neopixel LED matrix via the web.
   This sketch will create a new access point (with no password).
   It will then launch a new server and print out the IP address
   to the Serial monitor.
   From the embeded web can be selected de image, colors, etc...

   created 1 Jan 2017
   by jecrespo (aka aprendiendoarduino)

*/

#include <SPI.h>
#include <WiFi101.h>
#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      64

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const long int a = pixels.Color(0, 50, 0);  //initial color

long pixelMatrix[NUMPIXELS];

long christmas_tree [] = {
  0, 0, 0, a, a, 0, 0, 0,
  0, 0, 0, a, a, 0, 0, 0,
  0, 0, a, a, a, a, 0, 0,
  0, 0, a, a, a, a, 0, 0,
  0, a, a, a, a, a, a, 0,
  a, a, a, a, a, a, a, a,
  0, 0, 0, a, a, 0, 0, 0,
  0, 0, 0, a, a, 0, 0, 0
};

long face [] = {
  0, 0, a, a, a, a, 0, 0,
  0, a, 0, 0, 0, 0, a, 0,
  a, 0, 0, 0, 0, 0, 0, a,
  a, 0, 0, 0, 0, 0, 0, a,
  a, 0, 0, 0, 0, 0, 0, a,
  a, 0, 0, 0, 0, 0, 0, a,
  0, a, 0, 0, 0, 0, a, 0,
  0, 0, a, a, a, a, 0, 0
};

int tree_lights[] = {3, 4, 18, 21, 33, 38};

int face_ligths[][8] = {{18, 21, 43, 44, 34, 37}, {18, 21, 43, 44, 42, 45}, {18, 21, 43, 44, 50, 53}};

char ssid[] = "wifi101-network"; // created AP name
char pass[] = "1234567890";      // AP password (needed only for WEP, must be exactly 10 or 26 characters in length)
int keyIndex = 0;                // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;  // initial wifi state
WiFiServer server(80);

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  /*
    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
    }
  */
  delay(5000);  // wait 10 seconds for connection

  Serial.println("Christmas Neopixel");

  Serial.println("Access Point Web Server");

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // by default the local IP address of will be 192.168.1.1
  // you can override it with the following:
  // WiFi.config(IPAddress(10, 0, 0, 1));

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
  printWifiStatus();

  pixels.begin(); // This initializes the NeoPixel library.

  // Neopixel test
  for (int i = 0; i < NUMPIXELS; i++) { //Initialize all pixels to 0 swith off
    pixels.setPixelColor(i, random(0, 16777215));
    pixels.setBrightness(25);
    pixels.show();
    delay(100);
  }
  /*
    for (int i = 0; i < NUMPIXELS; i++) { //Initialize all pixels to 0 swith off
      pixels.setPixelColor(i, 0);
    }

    pixels.show();
  */
}


void loop() {
  // compare the previous status to the current status
  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      byte remoteMac[6];

      // a device has connected to the AP
      Serial.print("Device connected to AP, MAC address: ");
      WiFi.APClientMacAddress(remoteMac);
      Serial.print(remoteMac[5], HEX);
      Serial.print(":");
      Serial.print(remoteMac[4], HEX);
      Serial.print(":");
      Serial.print(remoteMac[3], HEX);
      Serial.print(":");
      Serial.print(remoteMac[2], HEX);
      Serial.print(":");
      Serial.print(remoteMac[1], HEX);
      Serial.print(":");
      Serial.println(remoteMac[0], HEX);
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }

  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            //send web page
            printWeb(client);
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        //Actions depending on recieved string
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED_BUILTIN, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED_BUILTIN, LOW);                // GET /L turns the LED off
        }
        if (currentLine.endsWith("GET /FACE")) {        //Show face
          static int i = 0;

          for (int i = 0; i < NUMPIXELS; i++) {
            pixels.setPixelColor(i, face[i]);
          }

          for (int j = 0; j < 6; j++) {
            pixels.setPixelColor(face_ligths[i][j], pixels.Color(0, 0, 50));
          }

          (i < 2) ? i++ : i = 0;

          pixels.setBrightness(25);
          pixels.show();

          //delay(delayval);  //configurar el parpadeo un timer
        }
        if (currentLine.endsWith("GET /TREE")) {  // show a tree

          for (int i = 0; i < NUMPIXELS; i++) {
            pixels.setPixelColor(i, christmas_tree[i]);
          }

          for (int j = 0; j < 6; j++) {
            pixels.setPixelColor(christmas_tree[j], pixels.Color(0, 0, 50));
          }
          pixels.setBrightness(25);
          pixels.show();

          //delay(delayval);  //configurar el parpadeo un timer poniendo a 0 el color
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }

}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}

void printWeb(WiFiClient client) {

  client.print("Click <a href=\"/H\">here</a> turn the LED on<br>");
  client.print("Click <a href=\"/L\">here</a> turn the LED off<br>");
  client.print("Click <a href=\"/FACE\">here</a> show a face<br>");
  client.print("Click <a href=\"/TREE\">here</a> show a tree<br>");

}

