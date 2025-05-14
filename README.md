# ❄️ Air Conditioning Controller using Fuzzy Logic and Arduino

This research focuses on implementing a Fuzzy Logic-based temperature control system using an Arduino board. The objective is to optimize comfort and energy efficiency. ---

## 🧠 Overview

The system's Fuzzy Inference System maps the inputs to the desired temperature output using membership functions and Fuzzy Logic Rules. The implementation process includes sensor connection, code development, and evaluation based on accuracy. The research concludes that the system offers potential benefits in terms of comfort and energy efficiency, applicable to environments like lecture halls.

---

## 🔧 Components Used

- Arduino Uno 
- DHT11 Temperature & Humidity Sensor
- Jumper wires

---

## 📐 Fuzzy Logic Description
temperature, humidity,
occupancy, and eco
- **Input Variables:**
  - Temperature (Cold, Warm, Hot)
  - Humidity (Low, Comfortable, High)
  - Occupancy (Low, Normal, High)
  - Eco (Low, Moderate, High)

- **Output Variable:**
  - Temperature (Cold, Warm, Hot)

- **Rules :**
Since there are three membership functions for temperature, three for humidity, three for eco and three for occupancy; 81 Fuzzy Rules will suffice. The combination of these input membership functions will determine the appropriate Rules. Meanwhile the number of Rules is
reduced or compacted to 30 Rules according to data obtained from several times of testing that I made.

---
