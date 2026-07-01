import json
import time
import random
import math
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque

MAX_POINTS = 60        # 60 seconds of rolling data

# Data buffers
timestamps  = deque(maxlen=MAX_POINTS)
temps       = deque(maxlen=MAX_POINTS)
humidities  = deque(maxlen=MAX_POINTS)
pressures   = deque(maxlen=MAX_POINTS)
accel_z     = deque(maxlen=MAX_POINTS)

# Setup Matplotlib Figure
fig, axes = plt.subplots(2, 2, figsize=(12, 7))
fig.suptitle("STM32 Multi-Sensor Dashboard (SIMULATION MODE)", fontsize=14, fontweight='bold', color='darkred')

start_time = time.time()

def generate_mock_data():
    """Artificially generates the JSON string that the STM32 would send."""
    t = time.time() - start_time
    ts = int(t * 1000)
    
    # Simulate realistic physical data
    temp = 25.0 + math.sin(t / 5.0) * 2.0 + random.uniform(-0.1, 0.1)
    hum = 50.0 + math.cos(t / 10.0) * 5.0 + random.uniform(-0.5, 0.5)
    press = 1013.25 + random.uniform(-0.2, 0.2)
    
    # MPU6050 Z-axis usually reads ~16384 when resting flat (1g of gravity)
    az = 16384 + random.randint(-100, 100) 

    data_dict = {
        "ts": ts,
        "T": round(temp, 2),
        "H": round(hum, 1),
        "P": round(press, 2),
        "ax": random.randint(-50, 50),
        "ay": random.randint(-50, 50),
        "az": az,
        "gx": random.randint(-10, 10),
        "gy": random.randint(-10, 10),
        "gz": random.randint(-10, 10),
        "crc": "DEADBEEF" # Fake CRC hash
    }
    
    # Return exactly as the STM32 UART would output it
    return json.dumps(data_dict) + "\r\n"

def parse_line(line):
    try:
        return json.loads(line.strip())
    except (json.JSONDecodeError, ValueError):
        return None

def animate(frame):
    try:
        # Instead of reading from the serial port, we call our mock generator
        raw = generate_mock_data() 
        data = parse_line(raw)
        
        if data:
            ts = data.get("ts", 0) / 1000.0  # convert ms to seconds
            timestamps.append(ts)
            temps.append(data.get("T", 0))
            humidities.append(data.get("H", 0))
            pressures.append(data.get("P", 0))
            accel_z.append(data.get("az", 0))

            for ax in axes.flat:
                ax.clear()

            axes[0,0].plot(list(timestamps), list(temps), color="tomato", linewidth=2)
            axes[0,0].set_title("Temperature (°C)")
            axes[0,0].set_ylabel("°C")
            axes[0,0].grid(True, linestyle='--', alpha=0.6)

            axes[0,1].plot(list(timestamps), list(humidities), color="steelblue", linewidth=2)
            axes[0,1].set_title("Humidity (%RH)")
            axes[0,1].set_ylabel("%")
            axes[0,1].grid(True, linestyle='--', alpha=0.6)

            axes[1,0].plot(list(timestamps), list(pressures), color="mediumseagreen", linewidth=2)
            axes[1,0].set_title("Pressure (hPa)")
            axes[1,0].set_ylabel("hPa")
            axes[1,0].grid(True, linestyle='--', alpha=0.6)

            axes[1,1].plot(list(timestamps), list(accel_z), color="mediumpurple", linewidth=2)
            axes[1,1].set_title("Accel Z (Raw Gravity)")
            axes[1,1].set_ylabel("ADC counts")
            axes[1,1].grid(True, linestyle='--', alpha=0.6)

            fig.tight_layout()

    except Exception as e:
        print(f"Error: {e}")

# Run the animation loop at 1000ms intervals (matching the FreeRTOS 1-second delay)
ani = animation.FuncAnimation(fig, animate, interval=1000)
plt.show()