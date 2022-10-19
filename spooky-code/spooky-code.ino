// Spooky Painting
// an IPDL project, School of Industrial Design at Georgia Tech
// painting by CJ Manabat, electronics by Sara Inani and Tynan Purdy, code by Tynan Purdy

#include "Adafruit_VL53L1X.h"
#include <Adafruit_DotStar.h>

#define IRQ_PIN 2
#define XSHUT_PIN 3
#define DATAPIN 14
#define CLOCKPIN 13

#define NUMPIXELS 23
#define COLOR strip.Color(75,75,200)

Adafruit_VL53L1X vl53 = Adafruit_VL53L1X(XSHUT_PIN, IRQ_PIN);
Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);

#if defined(ARDUINO_SAMD_ZERO) && defined(SERIAL_PORT_USBVIRTUAL)
  // Required for Serial on Zero based boards
  #define Serial SERIAL_PORT_USBVIRTUAL
#endif

void setup() {
  Serial.begin(115200);
//  while (!Serial) delay(10);

  Wire.begin();
  if (! vl53.begin(0x29, &Wire)) {
    Serial.print(F("Error on init of VL sensor: "));
    Serial.println(vl53.vl_status);
    while (1)       delay(10);
  }
  Serial.println(F("VL53L1X sensor OK!"));

  Serial.print(F("Sensor ID: 0x"));
  Serial.println(vl53.sensorID(), HEX);

  if (! vl53.startRanging()) {
    Serial.print(F("Couldn't start ranging: "));
    Serial.println(vl53.vl_status);
    while (1)       delay(10);
  }
  Serial.println(F("Ranging started"));

  // Valid timing budgets: 15, 20, 33, 50, 100, 200 and 500ms!
  vl53.setTimingBudget(50);
  Serial.print(F("Timing budget (ms): "));
  Serial.println(vl53.getTimingBudget());

  strip.begin(); // Initialize pins for output
  strip.setBrightness(75);
  strip.show();  // Turn all LEDs off ASAP
}

void loop() {
  int16_t distance;
  int brightness;
  int faces;

  if (vl53.dataReady()) {
    // new measurement for the taking!
    distance = vl53.distance();
    if (distance == -1) {
      // something went wrong!
      Serial.print(F("Couldn't get distance: "));
      Serial.println(vl53.vl_status);
      return;
    }
    Serial.print(F("Distance: "));
    Serial.print(distance);
    Serial.println(" mm");

    strip.clear();

    if (distance < 3500) {strip.fill(COLOR,  0, 2);}
    if (distance < 2600) {strip.fill(COLOR,  2, 2);}
    if (distance < 2500) {strip.fill(COLOR,  4, 2);}
    if (distance < 2400) {strip.fill(COLOR,  6, 2);}
    if (distance < 2300) {strip.fill(COLOR,  8, 2);}
    if (distance < 2100) {strip.fill(COLOR, 10, 2);}
    if (distance < 2000) {strip.fill(COLOR, 12, 2);}
    if (distance < 1900) {strip.fill(COLOR, 14, 2);}
    if (distance < 1600) {strip.fill(COLOR, 16, 2);}
    if (distance < 1400) {strip.fill(COLOR, 18, 2);}
    if (distance < 1200) {strip.fill(COLOR, 20, 1);} // (0)
    if (distance < 1000) {strip.fill(COLOR, 21, 2);}

    strip.show();

    // data is read out, time for another reading!
    vl53.clearInterrupt();
  }
}
