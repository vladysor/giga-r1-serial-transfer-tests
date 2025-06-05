#include "SensEdu.h"

const uint16_t buf_size = 256;
uint16_t buffer[buf_size];

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        delay(1);
    }

    buffer[0] = 0;
    for (uint16_t i = 1; i < buf_size; i++) {
        buffer[i] = buffer[i-1] + 1;
    }
}

void loop() {
    transfer_serial_data(buffer, buf_size);
}

void transfer_serial_data(const uint16_t* data, const uint16_t size) {
    return;
}