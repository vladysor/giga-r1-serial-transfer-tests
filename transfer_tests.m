%% transfer_tests.m
clear;
close all;
clc;

%% Settings
ARDUINO_PORT = 'COM8';
ARDUINO_BAUDRATE = 2000000;

BUF_SIZE = 20480;
ROUND_NUM = 10;
READY_CODE = 127;
MAX_CHUNK_TEST = 256;
MIN_CHUNK_TEST = 8;

%% Setup
arduino = serialport(ARDUINO_PORT, ARDUINO_BAUDRATE);
arduino.Timeout = 10;
buf = zeros(2, BUF_SIZE);
write(arduino, 'r', "char"); % start
code = read(arduino, 1, 'uint8');
if code ~= READY_CODE
    error("Arduino Synchronization Failed! " + ...
        "Expected 'r' symbol from Arduino. " + ...
        "It happens if you shut down MATLAB code manually, " + ...
        "restart MATLAB script couple of times to fix it.");
end

%% Trials
trial_num = 1 + (log2(MAX_CHUNK_TEST)-log2(MIN_CHUNK_TEST)+1);
trial_time = zeros(1, trial_num);
trial_speed_kbps = zeros(1, trial_num);

% Trial #1
write(arduino, 's', "char"); % start
tic;
for j = 1:ROUND_NUM
    for i = 1:BUF_SIZE
        read(arduino, 2, 'uint8');
    end
end
trial_time(1) = toc;
trial_speed_kbps(1) = round(calculate_speed_kbps (trial_time(1), BUF_SIZE, ROUND_NUM));
fprintf("Trial #1 (Basic sample by sample): " + trial_speed_kbps(1) + "kbps\n");

% Trial #2
for trial = 2:trial_num
    chunk = MIN_CHUNK_TEST* 2^(trial - 2);
    chunk_num = (BUF_SIZE*2)/chunk;

    write(arduino, 's', "char"); % start
    tic;
    for j = 1:ROUND_NUM
        for i = 1:chunk_num
            read(arduino, chunk, 'uint8');
        end
    end

    trial_time(trial) = toc;
    trial_speed_kbps(trial) = round(calculate_speed_kbps (trial_time(trial), BUF_SIZE, ROUND_NUM));
    fprintf("Trial #" + trial + " (By " + chunk + "byte chunks): " + trial_speed_kbps(trial) + "kbps\n");
end

arduino = [];

%% Functions
function time_kbps = calculate_speed_kbps(time_sec, buf_size, round_num)
    total_bits = buf_size * round_num * 16;
    time_kbps = (total_bits / time_sec) / 1000;
end
