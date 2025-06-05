#define ROUND_NUM 10
#define READY_CODE 127
#define MIN_CHUNK_TEST 8
#define MAX_CHUNK_TEST 256

static uint8_t reset_flag = 0;
const uint16_t buf_size = 20480;
uint16_t buffer[buf_size];

void setup() {
    Serial.begin(2000000);
    while (!Serial) {
        delay(1);
    }

    buffer[0] = 0;
    for (uint16_t i = 1; i < buf_size; i++) {
        buffer[i] = buffer[i-1] + 1;
    }
}

void loop() {
    // prepare matlab sync
    wait_trigger();
    if (reset_flag == 1) {
        Serial.write(READY_CODE);
        reset_flag = 0;
    }

    // trial #1
    wait_trigger();
    for (uint16_t i = 0; i < ROUND_NUM; i++) {
        transfer_serial_data_basic(buffer, buf_size);
    }
    
    // trial #2
    for (uint16_t chunk = 8; chunk <= MAX_CHUNK_TEST; chunk *= 2) {
        wait_trigger();
        for (uint16_t i = 0; i < ROUND_NUM; i++) {
            transfer_serial_data_advanced(buffer, buf_size, chunk);
        }
    }
}

void wait_trigger() {
    static char serial_buf = 0;
    while (1) {
        while (Serial.available() == 0);
        serial_buf = Serial.read();
        if (serial_buf == 's') {
            return;
        }
        if (serial_buf == 'r') {
            reset_flag = 1;
            return;
        }
    }
}

void transfer_serial_data_basic(const uint16_t* data, const uint16_t size) {
    for (uint16_t i = 0; i < size; i++) {
        Serial.write((const uint8_t *) data + 2*i, 2);
    }
}

void transfer_serial_data_advanced(const uint16_t* data, const uint16_t data_length, const uint16_t chunk_size_byte) {
    for (uint16_t i = 0; i < (data_length*2); i += chunk_size_byte) {
        uint16_t transfer_size = ((data_length*2) - i < chunk_size_byte) ? (data_length*2 - i) : chunk_size_byte;
        Serial.write((const uint8_t *) data + i, transfer_size);
    }
}