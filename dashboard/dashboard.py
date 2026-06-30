import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque
import csv
import datetime
import sys

# --- Configuration ---
SERIAL_PORT = 'COM3' 
BAUD_RATE = 115200
MAX_POINTS = 50
CSV_FILENAME = "sensor_log.csv"

# --- Setup CSV File ---
# Open the file in write mode and add a header
with open(CSV_FILENAME, mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(["Timestamp", "Temperature_C", "Pressure_hPa", "Humidity_%"])

# --- Data Structures ---
temps, pressures, humidities = deque(maxlen=MAX_POINTS), deque(maxlen=MAX_POINTS), deque(maxlen=MAX_POINTS)

# --- Plot Setup (Same as before) ---
fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(8, 8))
line_temp, line_press, line_hum = ax1.plot([], [], 'r-')[0], ax2.plot([], [], 'g-')[0], ax3.plot([], [], 'b-')[0]
# ... [Keep your existing axis formatting code here] ...

# --- Serial Connection ---
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)

def update(frame):
    if ser.in_waiting > 0:
        try:
            raw_line = ser.readline().decode('utf-8').strip()
            
            # --- NEW: Robust Error Handling ---
            if "ERROR" in raw_line:
                print(f"!!! HARDWARE ALERT: {raw_line} !!!")
                # Display the error clearly in the Temperature plot area
                ax1.text(0.5, 0.5, 'SENSOR DISCONNECTED', color='red', 
                         fontsize=16, ha='center', weight='bold')
                plt.draw()
                return line_temp, line_press, line_hum

            # --- Existing Data Processing ---
            data = raw_line.split(',')
            if len(data) == 3:
                temp, press, hum = float(data[0]), float(data[1]), float(data[2])
                
                # Append to CSV and Update Plots (as before)
                # ... [Keep your existing CSV and line_set_data logic here] ...
                
        except ValueError:
            pass # Ignore non-numeric startup strings
            
    return line_temp, line_press, line_hum

ani = animation.FuncAnimation(fig, update, interval=500, cache_frame_data=False)
plt.show()
ser.close()