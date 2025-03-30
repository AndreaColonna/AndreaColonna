# Phyphox Physics Analyzer 📱🔬

A  toolkit for analyzing Phyphox smartphone sensor data in physics experiments, featuring statistical analysis and Gaussian distribution verification.

![Phyphox Integration Demo](screenshots/phyphox_connection.gif) 

## 🌐 Phyphox Integration
- **Direct Data Streaming** via WiFi/Network
- **Supported Phyphox Experiments**:
  - Accelerometer (RAW data)
  - Magnetic Field
  - Pressure
- **Automatic Metadata Parsing**:
  - Sample rate detection
  - Sensor resolution
  - Timestamp synchronization

## 🛠️ Quick Start: Installation

1. **Phyphox Setup**:
   - Install [Phyphox](https://phyphox.org/) on your smartphone
     
2. **Generate QR Code**
   - Open your Phyphox experiment XML file on GitHub
   -  Click "Raw" button to get direct file URL
   - A[Right-click page] --> B[Select 'Create QR Code']
   - B --> C[Download QR Image]
2. **Scan QR Code in Phyphox**:
   - Open Phyphox app
   - Tap "+" → "Scan QR Code"
   - Point camera at project QR code

## 📂 Repository Structure

```bash
main/
├── Cal_stat_G_x.phyphox      # Acceleration (X-axis)
├── Cal_stat_G_y.phyphox      # Acceleration (Y-axis)
├── Cal_stat_G_z.phyphox      # Acceleration (Z-axis)
├── Cal_stat_Mg.phyphox       # Magnetometer (Total)
├── Cal_stat_Mg_x.phyphox     # Magnetometer (X-axis)
├── Cal_stat_Mg_y.phyphox     # Magnetometer (Y-axis)
├── Cal_stat_Mg_z.phyphox     # Magnetometer (Z-axis)
├── Cal_stat_lin_a_x.phyphox  # Linear Acceleration (X)
├── Cal_stat_lin_a_y.phyphox  # Linear Acceleration (Y)
└── Cal_stat_lin_a_z.phyphox  # Linear Acceleration (Z)
