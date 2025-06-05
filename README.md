## Arduino GIGA R1 Data Transfer Speed via USB
The Arduino GIGA R1 WiFi board does not use a physical UART for communication when connected to a PC; instead, it uses a virtual COM port over USB, which is implemented through USB-CDC. It's USB communication abstracted to behave like a serial link.

USB CDC is baud rate independent. The number you will set in `Serial.begin()` has no effect on the actual transfer speed.

[Forum on communication speed](https://forum.arduino.cc/t/portenta-h7-uart-as-a-fast-interface-to-a-host/1007978).

## Communication Results
Below are the results of tests conducted to measure the data transfer speed from the Arduino GIGA R1 to a PC (using MATLAB) while sending data in different chunk sizes.

| **Trial**              | **Chunk Size**         | **Speed**   |
|------------------------|------------------------|-------------|
| Trial #1               | Basic sample-by-sample (2 bytes) | 253 kbps    |
| Trial #2               | 8-byte chunks          | 973 kbps    |
| Trial #3               | 16-byte chunks         | 1849 kbps   |
| Trial #4               | 32-byte chunks         | 3225 kbps   |
| Trial #5               | 64-byte chunks         | 5525 kbps   |
| Trial #6               | 128-byte chunks        | 5432 kbps   |
| Trial #7               | 256-byte chunks        | 5522 kbps   |

The fastest configuration was achieved with 64-byte chunks, which aligns with the USB Full-Speed protocol's maximum packet size for bulk transfers.

For larger chunk sizes or high data rates, the transfer buffer may overflow if the receiving side (e.g., MATLAB) cannot process incoming data fast enough. This could result in data loss or transfer failures (it can be simulated by increasing `ROUND_NUM`).
