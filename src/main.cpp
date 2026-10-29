#include <Arduino.h>
#include <HX711.h>

constexpr uint8_t LOADCELL_DOUT_PIN = 2;
constexpr uint8_t LOADCELL_SCK_PIN = 3;
constexpr uint8_t CALIBRATION_READINGS = 10;
constexpr uint8_t WEIGHT_READINGS = 10;
constexpr float KNOWN_WEIGHT_OUNCES = 7.8f;

HX711 scale;

void setup() {
  Serial.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();

  while (!scale.wait_ready_timeout(1000)) {
    Serial.println("HX711 not found. Check the wiring.");
  }

  Serial.println("Remove all weight from the scale.");
  delay(5000);
  scale.tare();
  Serial.println("Tare complete.");

  Serial.println("Place the 7.8 ounce known weight on the scale.");
  delay(5000);

  const float rawReading = scale.get_units(CALIBRATION_READINGS);
  if (rawReading == 0) {
    Serial.println("No change detected. Calibration failed; restart and try again.");
    while (true) {
      delay(1000);
    }
  }

  const float calibrationFactor = rawReading / KNOWN_WEIGHT_OUNCES;
  scale.set_scale(calibrationFactor);

  Serial.print("Calibration factor: ");
  Serial.println(calibrationFactor, 2);
  Serial.println("Calibrated weight readings in ounces:");
}

void loop() {
  if (scale.is_ready()) {
    Serial.println(scale.get_units(WEIGHT_READINGS), 2);
  } else {
    Serial.println("HX711 not found.");
  }

  delay(1000);
}
