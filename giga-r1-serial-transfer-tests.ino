#define ROUND_NUM 5

const uint16_t buf_size = 20000;
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
    // trial #1
    wait_trigger();
    for (uint16_t i = 0; i < ROUND_NUM; i++) {
        transfer_serial_data_basic(buffer, buf_size);
    }
    
    // trial #2
}

void wait_trigger() {
    static char serial_buf = 0;
    while (1) {
        while (Serial.available() == 0);
        serial_buf = Serial.read();
        if (serial_buf == 's') {
            return;
        }
    }
}

void transfer_serial_data_basic(const uint16_t* data, const uint16_t size) {
    for (uint16_t i = 0; i < size; i++) {
        Serial.write((const uint8_t *) data + 2*i, 2);
    }
}

void transfer_serial_data_advanced(const uint16_t* data, const uint16_t size, const uint16_t chunk_size) {
    return;
}