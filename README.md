# Phyphox Physics Analyzer 📱🔬

A  toolkit for analyzing Phyphox smartphone sensor data in physics experiments, featuring statistical analysis and Gaussian distribution verification.

![Phyphox Integration Demo](screenshots/phyphox_connection.gif) *(example connection animation placeholder)*

## 🌐 Phyphox Integration
- **Direct Data Streaming** via WiFi/Network
- **Supported Phyphox Experiments**:
  - Accelerometer (RAW data)
  - Gyroscope
  - Magnetic Field
  - Light Sensor
  - Sound Frequency
- **Automatic Metadata Parsing**:
  - Sample rate detection
  - Sensor resolution
  - Timestamp synchronization

## 🛠️ Installation

1. **Phyphox Setup**:
   - Install [Phyphox](https://phyphox.org/) on your smartphone
   - Enable "Remote Access" in Phyphox settings

2. **Desktop Setup**:
   ```bash
   git clone https://github.com/yourusername/phyphox-physics-analyzer.git
   cd phyphox-physics-analyzer
   pip install -r requirements.txt
