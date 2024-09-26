## Introduction
GreenCode is a Smart Plant Monitoring and Automatic Irrigation system. GreenCode allows users to monitor and control their plants via a mobile app, using real-time sensor data for soil moisture, temperature, light intensity, and more.

## Technologies Used
![technologies used](https://github.com/user-attachments/assets/41a38798-1c56-46cd-a053-f62158d0394e)


## Project App


https://github.com/user-attachments/assets/a3d346ed-b010-44bc-8ff2-128213023933

## Circuit Diagram

![circuit-diagram](https://github.com/user-attachments/assets/f0f93c19-e92d-4b4b-8f23-159d4d0b57ad)

## Key Features:
 1. Real-time Sensor Data and Health Monitoring: View live sensor readings and assess your plant’s health status in real time. 🌱

 2. Automatic and Manual Irrigation: Irrigation is triggered either automatically based on soil moisture levels or manually controlled via the app or hardware interface. 💧

 3. Smart Notifications: Receive alerts through a buzzer, LED, and in-app notifications for issues like low water levels or insufficient light. 🔔

 4. Data Analytics and Visualizations: Track plant health over time with insightful charts and trends. 📊

## Main Frameworks and Data Flow:

 1. ESP32 Sensor Publishing: The ESP32 sends sensor readings in JSON format to a HiveMQ broker using the MQTT protocol. 📡

 2. Node-RED Integration: Node-RED subscribes to the MQTT topic to collect sensor data, automating data logging into Google Sheets for historical tracking and visualization through Grafana dashboards. 🔗

 3. Flutter App Functionality: The app subscribes to HiveMQ for real-time sensor data display, offering live insights into the plant's health. 📱

 4. Historical Data Visualization: The app integrates with the Google Sheets API (via the gsheets package) to retrieve historical data for visualizations on the Stats screen using the syncfusion_flutter_charts package. 📊

 5. Firebase Authentication: The app supports user authentication for SignIn, SignUp, and password recovery using Firebase. 🔒

### System Architecture
![system-architecture](https://github.com/user-attachments/assets/5ac4537c-0394-4232-9814-5e9a39804af6)


## Real-Time Operating System
![RTOS Handler](https://github.com/user-attachments/assets/7a623395-eaf4-4870-aa36-775d0419be35)

### Event-Driven Architecture

**Responsiveness**: The system can react quickly to events as they occur. It doesn't rely on constant polling, making it efficient for real-time applications like your plant monitoring system.

**Modularity**: New sensors, actuators, or functionalities can be added with minimal impact on existing code. You simply define new events and corresponding actions within the eventHandlerTask.

**Flexibility**: The order and timing of events are not predetermined. The system adapts dynamically to changing conditions and user inputs.
