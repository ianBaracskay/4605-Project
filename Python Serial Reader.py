import serial
import csv
import time

PORT = "COM13"
BAUD = 115200

ser = serial.Serial(PORT, BAUD, timeout=1)
time.sleep(2)   # wait for ESP32 reboot

outfile = "magnetometer_data.csv"

print("Starting logging... (Ctrl+C to stop)")

with open(outfile, "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow([
        "timestamp_ms",
        "x0","y0","z0",
        "x1","y1","z1",
        "x2","y2","z2"
    ])

    while True:
        line = ser.readline().decode("utf-8").strip()
        if not line:
            continue

        values = line.split(",")
        if len(values) == 10:    # 1 timestamp + 9 axis values
            writer.writerow(values)
            print(values)
