#include <Arduino.h>
#include "prism/prism.h"

prdev_t device;

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("Prism Example");

    prism_err err = prism_device_create(0x52, true, NULL, &device); // Create a Prism device with default configuration
    if (err != PR_OK) {
        Serial.print("Error creating Prism device: ");
        Serial.println(err);
        return;
    }
    Serial.print("Prism device was found on I²C address: ");
    Serial.println(device.address, HEX);
    Serial.print("Device version: ");
    Serial.print(device.major);
    Serial.print(".");
    Serial.print(device.minor);
    Serial.print(".");
    Serial.println(device.patch);   
}

void loop() {
    _v256i a = _v256_set8_uiv(1, 2, 3, 4, 5, 6, 7, 8); // Create a 256-bit vector with values from 1 to 8
    _v256i b = _v256_set8_uiv(10, 20, 30, 40, 50, 60, 70, 80); // Create another vector with values from 10 to 80
    _v256i result;
    prism_err err;
    // Store vectors in bank A and B
    err = _v256_store_bank_a(&device, a, 1000);
    if (err != PR_OK) {
        Serial.print("Error storing vector A: ");
        Serial.println(err);
        return;
    }
    err = _v256_store_bank_b(&device, b, 1000);
    if (err != PR_OK) {
        Serial.print("Error storing vector B: ");
        Serial.println(err);
        return;
    }
    // Perform addition operation on the vectors in bank A and B
    err = _v256_add8_ui32(&device, 1000);
    if (err != PR_OK) {
        Serial.print("Error performing addition: ");
        Serial.println(err);
        return;
    }
    // Load the result from bank A
    err = _v256_load_bank_c(&device, &result, 1000);
    if (err != PR_OK) {
        Serial.print("Error loading result: ");
        Serial.println(err);
        return;
    }
    // Print the result
    Serial.println("Result of addition in bank C:");
    for (int i = 0; i < 8; i++) {
        Serial.print(_v256_extract_ui32(result, i));
        if (i < 7) {
            Serial.print(", ");
        }
    }   
    Serial.println();
    // Clear all banks
    err = _v256_run_clear_all(&device, 1000);
    if (err != PR_OK) {
        Serial.print("Error clearing banks: ");
        Serial.println(err);
        return;
    }
    Serial.println("All banks cleared successfully.");
    // Print a message indicating the end of the loop
    Serial.println("Loop completed. Waiting for the next iteration...");
    // Wait for a while before the next iteration   
    delay(1000);
}
