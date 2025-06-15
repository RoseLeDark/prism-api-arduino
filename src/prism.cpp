#include "prism/prism.h"

// #include "prism/prism_arch.h"

#include "Arduino.h"
#include <Wire.h>

typedef struct prism_send_data {
  uint16_t op;
  ui8 arg;
  ui8 type;
  timeout_t timeout;
} prism_send_data_t;

prism_err _prism_arch_send_opcode(const prdev_t *dev, const uint16_t op,
                                  const ui8 type, timeout_t timeout) {
  if (dev == 0) {
    return PR_ERR_INVALID_ARGUMENT;
  }
  if (op == 0) {
    return PR_ERR_INVALID_ARGUMENT;
  } // Ensure op is valid
  if (type == 0) {
    return PR_ERR_INVALID_ARGUMENT;
  } // Ensure type is valid

  return _prism_arch_send_opcode_arg1(dev, op, type, 255,
                                      timeout); // Assume success for now
}
prism_err _prism_arch_send_opcode_arg1(const prdev_t *dev, const uint16_t op,
                                       const ui8 type, const ui8 arg,
                                       timeout_t timeout) {
  if (dev == 0) {
    return PR_ERR_INVALID_ARGUMENT;
  }

  prism_send_data data = {
      .op = op, .arg = arg, .type = type, .timeout = timeout};

  Wire.beginTransmission(dev->address); // Start Übertragung zum PCF8574
  Wire.write((const uint8_t *)&data,
             sizeof(prism_send_data_t)); // Wert schreiben
  uint8_t err = Wire.endTransmission();  // End transmission

  if (err != 0) {
    return PR_ERR_UNKNOWN; // Transmission error
  }

  delay(50); // Wait for the dev to process the command

  Wire.requestFrom(dev->address,
                   sizeof(uint8_t)); // Request 1 byte from the dev
  if (Wire.available() == 0) {
    return PR_ERR_UNKNOWN; // Device not responding
  }
  uint8_t response = Wire.read(); // Read the response byte
  if (response != 0x01) {  // Assuming 0x01 is the expected response for a
                           // successful operation
    return PR_ERR_UNKNOWN; // Device did not respond as expected
  }

  return PR_OK; // Assume success for now
}

uint8_t _prism_arch_get_variable(const prdev_t *dev, const uint16_t op) {
  if (dev == 0) {
    return 0;
  }

  prism_send_data data = {
      .op = op, .arg = 0, .type = PRISM_OPCODE_TYPE_UI8, .timeout = UINT16_MAX};

  Wire.beginTransmission(dev->address); // Start Übertragung zum PCF8574
  Wire.write((uint8_t *)&data, sizeof(prism_send_data_t)); // Wert schreiben
  Wire.endTransmission();                                  // Ende

  delay(50); // Wait for the dev to process the command

  Wire.requestFrom(dev->address,
                   sizeof(uint8_t)); // Request 1 byte from the dev
  if (Wire.available() == 0) {
    return PR_ERR_UNKNOWN; // Device not responding
  }
  return Wire.read(); // Read the response byte
}

void __prism_send_byte(const prdev_t *dev, uint8_t byte, bool new_entry) {
  // CLK auf HIGH (vorbereitend für Fallende Flanke)
  digitalWrite(dev->config.pin5Time, HIGH);
  // ENTRY_FLAG (Pin 6)
  digitalWrite(dev->config.pin6Next, new_entry ? HIGH : LOW);

  // Daten setzen
  digitalWrite(dev->config.pin1low, byte & 0x01);
  digitalWrite(dev->config.pin2low, (byte >> 1) & 0x01);
  digitalWrite(dev->config.pin3low, (byte >> 2) & 0x01);
  digitalWrite(dev->config.pin4low, (byte >> 3) & 0x01);
  digitalWrite(dev->config.pin7high, (byte >> 4) & 0x01);
  digitalWrite(dev->config.pin8high, (byte >> 5) & 0x01);
  digitalWrite(dev->config.pin9high, (byte >> 6) & 0x01);
  digitalWrite(dev->config.pin10high, (byte >> 7) & 0x01);

  delayMicroseconds(2);

  // Fallende Flanke erzeugen
  digitalWrite(dev->config.pin5Time, LOW);

  if (new_entry == true) {
    // ENTRY_FLAG (Pin 6)
    digitalWrite(dev->config.pin6Next, LOW);
  }

  // Hold-Zeit (nach der Flanke)
  delayMicroseconds(4);
}
void __prism_send_uint32(const prdev_t *dev, ui32 entry) {

  // Send each byte of the entry to the dev
  __prism_send_byte(dev, entry & 0xFF, true); // First byte is a new entry
  __prism_send_byte(dev, (entry >> 8) & 0xFF, false);
  __prism_send_byte(dev, (entry >> 16) & 0xFF, false);
  __prism_send_byte(dev, (entry >> 24) & 0xFF, false);
}

prism_err _prism_send_bank_x(const prdev_t *dev, const _v256i vec,
                             const bank_t bank, timeout_t timeout) {

  prism_err err = _prism_arch_send_opcode(
      dev, bank == PRISM_BANK_A ? PRISM_OPCODE_STORE_A : PRISM_OPCODE_STORE_B,
      PRISM_OPCODE_TYPE_UI32, timeout);

  if (err != PR_OK) {
    return err;
  }

  for (uint8_t i = 0; i < 8; i++) {
    __prism_send_uint32(dev, vec.ui[i]);
  }

  _prism_arch_send_opcode(dev, PRISM_OPCODE_END, PRISM_OPCODE_TYPE_UI8,
                          timeout);

  return PR_OK; // Assume success for now
}

uint8_t __prism_recv_byte(const prdev_t *dev, bool new_entry) {
  // ENTRY_FLAG (Pin 6)
  digitalWrite(dev->config.pin6Next, new_entry ? HIGH : LOW);

  // Takt vorbereiten
  digitalWrite(dev->config.pin5Time, HIGH);
  delayMicroseconds(2); // Setup-Zeit für Empfänger

  // Taktflanke erzeugen (Fallend)
  digitalWrite(dev->config.pin5Time, LOW);
  delayMicroseconds(2); // Hold-Zeit

  if (new_entry) {
    digitalWrite(dev->config.pin6Next, LOW);
  }

  // Daten einlesen (nach der Fallenden Flanke)
  uint8_t byte = 0;
  byte |= digitalRead(dev->config.pin1low) << 0;
  byte |= digitalRead(dev->config.pin2low) << 1;
  byte |= digitalRead(dev->config.pin3low) << 2;
  byte |= digitalRead(dev->config.pin4low) << 3;
  byte |= digitalRead(dev->config.pin7high) << 4;
  byte |= digitalRead(dev->config.pin8high) << 5;
  byte |= digitalRead(dev->config.pin9high) << 6;
  byte |= digitalRead(dev->config.pin10high) << 7;

  return byte;
}

uint32_t __prism_recv_uint32(const prdev_t *dev) {
  uint32_t val = 0;
  val |= ((uint32_t)__prism_recv_byte(dev, true)) << 0;
  val |= ((uint32_t)__prism_recv_byte(dev, false)) << 8;
  val |= ((uint32_t)__prism_recv_byte(dev, false)) << 16;
  val |= ((uint32_t)__prism_recv_byte(dev, false)) << 24;
  return val;
}

prism_err _prism_load_bank_x(const prdev_t *dev, const bank_t bank, _v256i *out,
                             timeout_t timeout) {

  prism_err _ret = PR_OK;

  if (bank == PRISM_BANK_A) {
    _ret = _prism_arch_send_opcode(dev, PRISM_OPCODE_LOAD_A,
                                   PRISM_OPCODE_TYPE_UI8, timeout);
  } else if (bank == PRISM_BANK_B) {
    _ret = _prism_arch_send_opcode(dev, PRISM_OPCODE_LOAD_B,
                                   PRISM_OPCODE_TYPE_UI8, timeout);
  } else if (bank == PRISM_BANK_C) {
    _ret = _prism_arch_send_opcode(dev, PRISM_OPCODE_LOAD_C,
                                   PRISM_OPCODE_TYPE_UI8, timeout);
  } else if (bank == PRISM_BANK_D) {
    _ret = _prism_arch_send_opcode(dev, PRISM_OPCODE_LOAD_D,
                                   PRISM_OPCODE_TYPE_UI8, timeout);
  } else {
    _ret = PR_ERR_INVALID_ARGUMENT;
  }

  if (_ret != PR_OK) {
    return _ret;
  }

  // 2. Lese alle 8 Einträge (32 Bit × 8)
  for (uint8_t i = 0; i < 8; i++) {
    out->ui[i] = __prism_recv_uint32(dev);
  }

  // 3.  END-Opcode für Abschluss
  _prism_arch_send_opcode(dev, PRISM_OPCODE_END, PRISM_OPCODE_TYPE_UI32,
                          timeout);

  return PR_OK;
}

// PUBLIC API

prism_err prism_device_create(const uint8_t address, bool wireInit,
                              const prism_dev_config_t *config, prdev_t *dev) {
  if (dev == 0) {
    return PR_ERR_INVALID_ARGUMENT;
  } // Ensure dev pointer is valid

  if (address == 0 || address > 127) {
    return PR_ERR_INVALID_ARGUMENT;
  } // Ensure address is within valid I2C range

  prism_err _err = PR_OK;
  dev->address = address;
  if (wireInit)
    Wire.begin();
  delay(100); // Wait for the I2C bus to stabilize

  if (config == 0) {
    // If no config is provided, use default values
    dev->config.pin1low = PRISM_PIN_1LOW_DEFAULT;
    dev->config.pin2low = PRISM_PIN_2LOW_DEFAULT;
    dev->config.pin3low = PRISM_PIN_4LOW_DEFAULT;
    dev->config.pin4low = PRISM_PIN_3LOW_DEFAULT;
    dev->config.pin5Time = PRISM_PIN_CLK_DEFAULT;
    dev->config.pin6Next = PRISM_PIN_NXT_DEFAULT;
    dev->config.pin7high = PRISM_PIN_7HIGH_DEFAULT;
    dev->config.pin8high = PRISM_PIN_8HIGH_DEFAULT;
    dev->config.pin9high = PRISM_PIN_9HIGH_DEFAULT;
    dev->config.pin10high = PRISM_PIN_10HIGH_DEFAULT;
  } else {
    // Copy the provided configuration
    dev->config = *config;
  }
  // Initialize the I2C communication
  _err = _prism_arch_send_opcode(dev, PRISM_OPCODE_ARCH_INIT,
                                 PRISM_OPCODE_TYPE_UI32,
                                 255); // Send initialization opcode

  dev->flank = _prism_arch_get_variable(
      dev, PRISM_OPCODE_ARCH_GET_FLANK); // Get the flank speed variable

  dev->major =
      _prism_arch_get_variable(dev, PRISM_OPCODE_ARCH_GET_VERSION_MAJOR);
  dev->minor =
      _prism_arch_get_variable(dev, PRISM_OPCODE_ARCH_GET_VERSION_MINOR);
  dev->patch =
      _prism_arch_get_variable(dev, PRISM_OPCODE_ARCH_GET_VERSION_PATCH);

  if ((dev->major != 0 && dev->minor != 0 && dev->patch != 0)) {

    // Print the version information
    Serial.print("Prism Device Version: ");
    Serial.print(dev->major);
    Serial.print(".");
    Serial.print(dev->minor);
    Serial.print(".");
    Serial.println(dev->patch);
  }

  return PR_OK;
}

prism_err prism_device_stop(const prdev_t *dev) {
  if (dev == 0) {
    return PR_ERR_INVALID_ARGUMENT;
  } // Ensure dev pointer is valid

  // Send the stop command to the dev
  prism_err err = _prism_arch_send_opcode(dev, PRISM_OPCODE_ARCH_END,
                                          PRISM_OPCODE_TYPE_UI8, 255);
  if (err != PR_OK) {
    return err; // Return error if sending stop command fails
  }

  delay(50); // Wait for the dev to process the stop command

  return PR_OK; // Assume success for now
}
prism_err prism_device_reset(const prdev_t *dev) {
  if (dev == 0) {
    return PR_ERR_INVALID_ARGUMENT;
  } // Ensure dev pointer is valid

  // Send the reset command to the dev
  prism_err err = _prism_arch_send_opcode(dev, PRISM_OPCODE_ARCH_RESET,
                                          PRISM_OPCODE_TYPE_UI8, 255);
  if (err != PR_OK) {
    return err; // Return error if sending reset command fails
  }

  delay(50); // Wait for the dev to process the reset command

  return PR_OK; // Assume success for now
}
