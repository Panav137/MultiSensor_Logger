import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque
import random
import math

# --- Configuration ---
MAX_POINTS = 50  # How many data points to show on the screen at once

# --- Data Structures ---
temps = deque(maxlen=MAX_POINTS)
pressures = deque(maxlen=MAX_POINTS)
humidities = deque(maxlen=MAX_POINTS)

# --- Plot Setup ---
fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(8, 8))
fig.canvas.manager.set_window_title('BME280 Live Telemetry (SIMULATION MODE)')
fig.tight_layout(pad=4.0)

# Temperature Axis
ax1.set_title('Temperature (°C)')
ax1.set_ylabel('°C')
line_temp, = ax1.plot([], [], 'r-', lw=2)
ax1.grid(True, linestyle='--', alpha=0.6)

# Pressure Axis
ax2.set_title('Atmospheric Pressure (hPa)')
ax2.set_ylabel('hPa')
line_press, = ax2.plot([], [], 'g-', lw=2)
ax2.grid(True, linestyle='--', alpha=0.6)

# Humidity Axis
ax3.set_title('Relative Humidity (%)')
ax3.set_ylabel('%')
ax3.set_xlabel('Recent Samples')
line_hum, = ax3.plot([], [], 'b-', lw=2)
ax3.grid(True, linestyle='--', alpha=0.6)

time_counter = 0

# --- Animation Update Function ---
def update(frame):
    global time_counter
    time_counter += 1
    
    # Generate fake realistic environmental data using sine waves and noise
    temp = 24.5 + math.sin(time_counter * 0.1) * 0.5 + random.uniform(-0.05, 0.05)
    press = 1013.25 + math.cos(time_counter * 0.05) * 1.5 + random.uniform(-0.2, 0.2)
    hum = 45.0 + math.sin(time_counter * 0.08) * 3.0 + random.uniform(-0.5, 0.5)
    
    # Append to our scrolling windows
    temps.append(temp)
    pressures.append(press)
    humidities.append(hum)
    
    # Create a dynamic X-axis array based on current length
    x_data = range(len(temps))
    
    # Update the lines
    line_temp.set_data(x_data, temps)
    line_press.set_data(x_data, pressures)
    line_hum.set_data(x_data, humidities)
    
    # Dynamically scale the X and Y axes
    ax1.set_xlim(0, MAX_POINTS)
    if len(temps) > 1:
        ax1.set_ylim(min(temps) - 0.5, max(temps) + 0.5)
    
    ax2.set_xlim(0, MAX_POINTS)
    if len(pressures) > 1:
        ax2.set_ylim(min(pressures) - 1, max(pressures) + 1)
    
    ax3.set_xlim(0, MAX_POINTS)
    if len(humidities) > 1:
        ax3.set_ylim(min(humidities) - 2, max(humidities) + 2)

    return line_temp, line_press, line_hum

# --- Run the Dashboard ---
print("Starting BME280 Simulator...")
# Update every 500ms to mimic the STM32's real-time output
ani = animation.FuncAnimation(fig, update, interval=500, cache_frame_data=False)
plt.show()