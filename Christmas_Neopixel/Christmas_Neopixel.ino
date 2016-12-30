//Christmas Neopixel
//For THE ARDUINO INTERNET OF HOLIDAY THINGS contest
//More info: https://create.arduino.cc/projecthub/contests/arduino-holiday-contest

#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      64

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const long int a = pixels.Color(0, 50, 0);

int delayval = 500; // delay for half a second

long pixelMatrix[NUMPIXELS];

long image [] = {
  0, 0, 0, a, a, 0, 0, 0,
  0, 0, 0, a, a, 0, 0, 0,
  0, 0, a, a, a, a, 0, 0,
  0, 0, a, a, a, a, 0, 0,
  0, a, a, a, a, a, a, 0,
  a, a, a, a, a, a, a, a,
  0, 0, 0, a, a, 0, 0, 0,
  0, 0, 0, a, a, 0, 0, 0
};

int random_pins[] = {3, 4, 18, 21, 33, 38};


void setup() {

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Christmas Neopixel");

  pixels.begin(); // This initializes the NeoPixel library.

  for (int i = 0; i < NUMPIXELS; i++) { //Initialize all pixels to 0 swith off
      pixels.setPixelColor(i, image[i]);
    }
  
  pixels.show();

}

void loop() {

  //pixelMatrix[random(0, 64)] = random(0, 16777215);

  //pixels.setPixelColor(i, pixelMatrix[i]);

  for (int i = 0; i < sizeof(random_pins) / 4; i++) { //Initialize all pixels to 0 swith off
    pixels.setPixelColor(random_pins[i], random(0, 16777215));
  }

  pixels.setBrightness(25);
  pixels.show();

  delay(delayval);
}
