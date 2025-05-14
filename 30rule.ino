#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 13       // The pin to which the DHT11 sensor is connected
#define DHTTYPE DHT11  // DHT11 sensor type

DHT dht(DHTPIN, DHTTYPE);

// Define your fuzzy sets and variables
float temperature;
float humidity;
float eco;
float myeco;


float calculateEco(float temp) {
  eco = temp / 25 * 100;
  return eco;
}

// Define triangular and trapezoidal membership functions
float triangular(float x, float a, float b, float c) {
  if (x <= a || x >= c) return 0;
  if (x >= b) return (c - x) / (c - b);
  if (x < b) return (x - a) / (b - a);
  return 0;
}

float trapezoidal(float x, float a, float b, float c, float d) {
  if (x <= a || x >= d) return 0;
  if (x >= b && x <= c) return 1;
  if (x > a && x < b) return (x - a) / (b - a);
  if (x > c && x < d) return (d - x) / (d - c);
  return 0;
}

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  dht.begin();
}

void loop() {

      // Read temperature and humidity from the DHT11 sensor
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    
    // Check if any reads failed and exit early (to try again)
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

      // input
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  
  myeco = calculateEco(temperature);//calling func for eco

  Serial.print("Eco: ");
  Serial.print(myeco);
  Serial.println(" °C");

  int occupancy = 90;
  Serial.print("Occupancy: ");
  Serial.print(occupancy);
  Serial.println(" people");

  // Fuzzy logic rules with triangular and trapezoidal membership functions
  float Otemp = 0; // Default output temperature
  float coldtemp, warmtemp, hottemp, lowhumi, comfortablehumi, highhumi, loweco, normaleco, higheco, lowocu, moderateocu, highocu;

  // Rule 1 cold
  coldtemp = trapezoidal(temperature, 18, 18, 20, 22);
  lowhumi = trapezoidal(humidity, 50, 50, 58, 60);
  loweco = trapezoidal(myeco, 72, 72, 84, 88);
  lowocu = trapezoidal(occupancy, 30, 30, 41, 54);
  Otemp = max(Otemp, min(min(min(coldtemp, lowhumi), loweco), lowocu) * 22);

    // Rule 2 hot
  coldtemp = trapezoidal(temperature, 18, 18, 20, 22);
  lowhumi = trapezoidal(humidity, 50, 50, 58, 60);
  loweco = trapezoidal(myeco, 72, 72, 84, 88);
  highocu = trapezoidal(occupancy, 78, 84, 95, 95);
  Otemp = max(Otemp, min(min(min(coldtemp, lowhumi), loweco), highocu) * 22);

      // Rule 3 cold
  coldtemp = trapezoidal(temperature, 18, 18, 20, 22);
  lowhumi = trapezoidal(humidity, 50, 50, 58, 60);
  normaleco = triangular(myeco, 84, 92, 100);
  lowocu = trapezoidal(occupancy, 30, 30, 41, 54);
  Otemp = max(Otemp, min(min(min(coldtemp, lowhumi), normaleco), lowocu) * 22);

    // Rule 4 hot
  coldtemp = trapezoidal(temperature, 18, 18, 20, 22);
  lowhumi = trapezoidal(humidity, 50, 50, 58, 60);
  normaleco = triangular(myeco, 84, 92, 100);
  highocu = trapezoidal(occupancy, 78, 84, 95, 95);
  Otemp = max(Otemp, min(min(min(coldtemp, lowhumi), normaleco), highocu) * 22);

  // Rule 5 cold
  coldtemp = trapezoidal(temperature, 18, 18, 20, 22);
  comfortablehumi = triangular(humidity, 58, 65, 72);
  loweco = trapezoidal(myeco, 72, 72, 84, 88);
  lowocu = trapezoidal(occupancy, 30, 30, 41, 54);
  Otemp = max(Otemp, min(min(min(coldtemp, comfortablehumi), loweco), lowocu) * 22);

    // Rule 6 warm
  coldtemp = trapezoidal(temperature, 18, 18, 20, 22);
  comfortablehumi = triangular(humidity, 58, 65, 72);
  normaleco = triangular(myeco, 84, 92, 100);
  moderateocu = triangular(occupancy, 41, 66, 84);
  Otemp = max(Otemp, min(min(min(coldtemp, comfortablehumi), normaleco), moderateocu) * 22);

    // Rule 7 hot
  coldtemp = trapezoidal(temperature, 18, 18, 20, 22);
  comfortablehumi = triangular(humidity, 58, 65, 72);
  higheco = trapezoidal(myeco, 96, 100, 112, 112);
  highocu = trapezoidal(occupancy, 78, 84, 95, 95);
  Otemp = max(Otemp, min(min(min(coldtemp, comfortablehumi), higheco), highocu) * 22);

  // Rule 8 cold
  coldtemp = trapezoidal(temperature, 18, 18, 20, 22);
  highhumi = trapezoidal(humidity, 67, 72, 100, 100);
  loweco = trapezoidal(myeco, 72, 72, 84, 88);
  lowocu = trapezoidal(occupancy, 30, 30, 41, 54);
  Otemp = max(Otemp, min(min(min(coldtemp, highhumi), loweco), lowocu) * 22);

    // Rule 9 cold
  coldtemp = trapezoidal(temperature, 18, 18, 20, 22);
  highhumi = trapezoidal(humidity, 67, 72, 100, 100);
  normaleco = triangular(myeco, 84, 92, 100);
  lowocu = trapezoidal(occupancy, 30, 30, 41, 54);
  Otemp = max(Otemp, min(min(min(coldtemp, highhumi), normaleco), lowocu) * 22);

  // Rule 10 cold
  warmtemp = triangular(temperature, 20, 23, 25);
  lowhumi = trapezoidal(humidity, 50, 50, 58, 60);
  normaleco = triangular(myeco, 84, 92, 100);
  lowocu = trapezoidal(occupancy, 30, 30, 41, 54);
  Otemp = max(Otemp, min(min(min(warmtemp, lowhumi), normaleco), lowocu) * 25);

  // Rule 11 hot
  warmtemp = triangular(temperature, 20, 23, 25);
  lowhumi = trapezoidal(humidity, 50, 50, 58, 60);
  normaleco = triangular(myeco, 84, 92, 100);
  highocu = trapezoidal(occupancy, 78, 84, 95, 95);
  Otemp = max(Otemp, min(min(min(warmtemp, lowhumi), normaleco), highocu) * 25);

    // Rule 12 cold
  warmtemp = triangular(temperature, 20, 23, 25);
  comfortablehumi = triangular(humidity, 58, 65, 72);
  normaleco = triangular(myeco, 84, 92, 100);
  lowocu = trapezoidal(occupancy, 40, 40, 70, 80);
  Otemp = max(Otemp, min(min(min(warmtemp, comfortablehumi), normaleco), lowocu) * 25);

  // Rule 13 hot
  warmtemp = triangular(temperature, 20, 23, 25);
  comfortablehumi = triangular(humidity, 58, 65, 72);
  normaleco = triangular(myeco, 84, 92, 100);
  highocu = trapezoidal(occupancy, 78, 84, 95, 95);
  Otemp = max(Otemp, min(min(min(warmtemp, comfortablehumi), normaleco), highocu) * 25);

  // Rule 14 cold
  warmtemp = triangular(temperature, 20, 23, 25);
  lowhumi = trapezoidal(humidity, 50, 50, 58, 60);
  higheco = trapezoidal(myeco, 96, 100, 112, 112);
  lowocu = trapezoidal(occupancy, 30, 30, 41, 54);
  Otemp = max(Otemp, min(min(min(warmtemp, lowhumi), higheco), lowocu) * 25);

  // Rule 15 cold
  warmtemp = triangular(temperature, 20, 23, 25);
  highhumi = trapezoidal(humidity, 67, 72, 100, 100);
  loweco = trapezoidal(myeco, 72, 72, 84, 88);
  lowocu = trapezoidal(occupancy, 30, 30, 41, 54);
  Otemp = max(Otemp, min(min(min(warmtemp, highhumi), loweco), lowocu) * 25);

  // Rule 16 warm
  warmtemp = triangular(temperature, 20, 23, 25);
  highhumi = trapezoidal(humidity, 67, 72, 100, 100);
  loweco = trapezoidal(myeco, 72, 72, 84, 88);
  moderateocu = triangular(occupancy, 41, 66, 84);
  Otemp = max(Otemp, min(min(min(warmtemp, highhumi), loweco), moderateocu) * 25);

    // Rule 17 cold
  warmtemp = triangular(temperature, 20, 23, 25);
  highhumi = trapezoidal(humidity, 67, 72, 100, 100);
  normaleco = triangular(myeco, 84, 92, 100);
  lowocu = trapezoidal(occupancy, 30, 30, 41, 54);
  Otemp = max(Otemp, min(min(min(warmtemp, highhumi), normaleco), lowocu) * 25);

  // Rule 18 warm
  warmtemp = triangular(temperature, 20, 23, 25);
  highhumi = trapezoidal(humidity, 67, 72, 100, 100);
  normaleco = triangular(myeco, 84, 92, 100);
  moderateocu = triangular(occupancy, 41, 66, 84);
  Otemp = max(Otemp, min(min(min(warmtemp, highhumi), normaleco), moderateocu) * 25);

  // Rule 19 hot
  warmtemp = triangular(temperature, 20, 23, 25);
  highhumi = trapezoidal(humidity, 67, 72, 100, 100);
  normaleco = triangular(myeco, 84, 92, 100);
  highocu = trapezoidal(occupancy, 78, 84, 95, 95);
  Otemp = max(Otemp, min(min(min(warmtemp, highhumi), normaleco), highocu) * 25);

  // Rule 20 hot
  warmtemp = triangular(temperature, 20, 23, 25);
  highhumi = trapezoidal(humidity, 67, 72, 100, 100);
  higheco = trapezoidal(myeco, 96, 100, 112, 112);
  highocu = trapezoidal(occupancy, 78, 84, 95, 95);
  Otemp = max(Otemp, min(min(min(warmtemp, highhumi), higheco), highocu) * 25);

    // Rule 21 cold
  hottemp = trapezoidal(temperature, 24, 25, 28, 28);
  lowhumi = trapezoidal(humidity, 50, 50, 58, 60);
  loweco = trapezoidal(myeco, 72, 72, 84, 88);
  lowocu = trapezoidal(occupancy, 30, 30, 41, 54);
  Otemp = max(Otemp, min(min(min(hottemp, lowhumi), loweco), lowocu) * 28);

    // Rule 22 cold
  hottemp = trapezoidal(temperature, 24, 25, 28, 28);
  lowhumi = trapezoidal(humidity, 50, 50, 58, 60);
  normaleco = triangular(myeco, 84, 92, 100);
  lowocu = trapezoidal(occupancy, 30, 30, 41, 54);
  Otemp = max(Otemp, min(min(min(hottemp, lowhumi), normaleco), lowocu) * 28);

  // Rule 23 cold
  hottemp = trapezoidal(temperature, 24, 25, 28, 28);
  lowhumi = trapezoidal(humidity, 50, 50, 58, 60);
  higheco = trapezoidal(myeco, 96, 100, 112, 112);
  lowocu = trapezoidal(occupancy, 30, 30, 41, 54);
  Otemp = max(Otemp, min(min(min(hottemp, lowhumi), higheco), lowocu) * 28);

    // Rule 24 warm
  hottemp = trapezoidal(temperature, 24, 25, 28, 28);
  lowhumi = trapezoidal(humidity, 50, 50, 58, 60);
  higheco = trapezoidal(myeco, 96, 100, 112, 112);
  moderateocu = triangular(occupancy, 41, 66, 84);
  Otemp = max(Otemp, min(min(min(hottemp, lowhumi), higheco), moderateocu) * 28);

  // Rule 25 hot
  hottemp = trapezoidal(temperature, 24, 25, 28, 28);
  lowhumi = trapezoidal(humidity, 50, 50, 58, 60);
  higheco = trapezoidal(myeco, 96, 100, 112, 112);
  highocu = trapezoidal(occupancy, 78, 84, 95, 95);
  Otemp = max(Otemp, min(min(min(hottemp, lowhumi), higheco), highocu) * 28);

  // Rule 26 hot
  hottemp = trapezoidal(temperature, 24, 25, 28, 28);
  highhumi = trapezoidal(humidity, 67, 72, 100, 100);
  loweco = trapezoidal(myeco, 72, 72, 84, 88);
  highocu = trapezoidal(occupancy, 78, 84, 95, 95);
  Otemp = max(Otemp, min(min(min(hottemp, highhumi), loweco), highocu) * 28);

    // Rule 27 cold
  hottemp = trapezoidal(temperature, 24, 25, 28, 28);
  highhumi = trapezoidal(humidity, 67, 72, 100, 100);
  normaleco = triangular(myeco, 84, 92, 100);
  lowocu = trapezoidal(occupancy, 30, 30, 41, 54);
  Otemp = max(Otemp, min(min(min(hottemp, highhumi), normaleco), lowocu) * 28);

    // Rule 28 cold
  hottemp = trapezoidal(temperature, 24, 25, 28, 28);
  highhumi = trapezoidal(humidity, 67, 72, 100, 100);
  higheco = trapezoidal(myeco, 96, 100, 112, 112);
  lowocu = trapezoidal(occupancy, 30, 30, 41, 54);
  Otemp = max(Otemp, min(min(min(hottemp, highhumi), higheco), lowocu) * 28);

  // Rule 29 warm
  hottemp = trapezoidal(temperature, 24, 25, 28, 28);
  highhumi = trapezoidal(humidity, 67, 72, 100, 100);
  higheco = trapezoidal(myeco, 96, 100, 112, 112);
  moderateocu = triangular(occupancy, 41, 66, 84);
  Otemp = max(Otemp, min(min(min(hottemp, highhumi), higheco), moderateocu) * 28);

  // Rule 30 hot
  hottemp = trapezoidal(temperature, 24, 25, 28, 28);
  highhumi = trapezoidal(humidity, 67, 72, 100, 100);
  higheco = trapezoidal(myeco, 96, 100, 112, 112);
  highocu = trapezoidal(occupancy, 78, 84, 95, 95);
  Otemp = max(Otemp, min(min(min(hottemp, highhumi), higheco), highocu) * 28);

  // Minimum  output temperature
  if (Otemp < 16) 
  {
    Otemp = 16; 
  }


//output
  Serial.print("Optimum Temperature: ");
  Serial.println(Otemp);
  Serial.println("°C");

  delay(86400000); // Delay for 24 HOUR
}
