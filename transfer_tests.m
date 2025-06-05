%% transfer_tests.m
clear;
close all;
clc;

%% Settings
ARDUINO_PORT = 'COM8';
ARDUINO_BAUDRATE = 115200;

BUF_SIZE = 20000;
ROUND_NUM = 5;

%% Setup
arduino = serialport(ARDUINO_PORT, ARDUINO_BAUDRATE);
buf = zeros(2, BUF_SIZE);

%% Trials
write(arduino, 's', "char"); % start
tic;
for j = 1:ROUND_NUM
    for i = 1:BUF_SIZE
        buf(:,i) = read(arduino, 2, 'uint8');
    end
end
trial1_time = toc;
fprintf("Trial #1 (Basic sample by sample): " + round(calculate_time(trial1_time, BUF_SIZE, ROUND_NUM)) + "kbps");

arduino = [];

%% Functions
function time_kbps = calculate_time(time_sec, buf_size, round_num)
    total_bits = buf_size * round_num * 16;
    time_kbps = (total_bits / time_sec) / 1000;
end
