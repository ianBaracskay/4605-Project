#include <Wire.h>
#include <Adafruit_MLX90393.h>

// PCA9548A I2C multiplexer
#define PCA9548A_ADDR 0x70

Adafruit_MLX90393 mlx[3];

void i2cSelect(uint8_t channel) {
  Wire.beginTransmission(PCA9548A_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);   // ESP32 I2C pins

  for (int i = 0; i < 3; i++) {
    i2cSelect(i);

    if (!mlx[i].begin_I2C()) {
      Serial.print("Sensor ");
      Serial.print(i);
      Serial.println(" not found!");
    } else {
      Serial.print("Sensor ");
      Serial.print(i);
      Serial.println(" initialized.");
    }
  }
  delay(5000);
}

void loop() {
  static unsigned long lastTime = 0;
  unsigned long now = millis();

  // Read all 3 sensors across the multiplexer
  float x[3], y[3], z[3];

  for (int i = 0; i < 3; i++) {
    i2cSelect(i);
    mlx[i].readData(&x[i], &y[i], &z[i]);
  }

  // CSV OUTPUT (one line per loop)
  Serial.print(now);
  Serial.print(",");

  for (int i = 0; i < 3; i++) {
    Serial.print(x[i]); Serial.print(",");
    Serial.print(y[i]); Serial.print(",");
    Serial.print(z[i]);
    if (i < 2) Serial.print(",");
  }

  Serial.println();

  delay(10);   // ~100 Hz sampling
}
