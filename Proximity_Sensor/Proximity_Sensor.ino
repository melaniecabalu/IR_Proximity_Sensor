/* 
 * Proximity_Sensor reads the analog values from the Sharp IR Proximity 
 * Sensor and uses an exponential smoothing filter to remove noise from 
 * the sensor values.
 * 
 * This file is part of IR Proximity Sensor (https://github.com/melaniecabalu/IR_Proximity_Sensor).
 * Copyright (c) 2018 Melanie Cabalu
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

int sensorPin = 0;
float currentValue;
float weight = 0.2;
boolean firstIteration = true;

void setup(){
  // Set data rate to 115200 baud
  Serial.begin(115200);  
}

void loop(){  
  // Unfiltered sensor value
  int sensorValue = analogRead(sensorPin);
  Serial.print("Unfiltered sensor value: ");
  Serial.print(sensorValue);
  
  // Filtered sensor value
  currentValue = filter(currentValue, sensorValue);
  Serial.print(" | Filtered sensor value: ");
  Serial.print(currentValue);

  // Voltage based on unfiltered sensor value
  float voltage = sensorValue * .0049;
  Serial.print(" | Voltage: ");
  Serial.print(voltage);

  // Distance based on unfiltered sensor value (in centimeters)
  float distance = 12343.85 * pow(sensorValue,-1.15);
  Serial.print(" | Distance: ");
  Serial.print(distance);

  // Distance based on filtered sensor value (in centimeters)
  float filteredDistance = 12343.85 * pow(currentValue,-1.15);
  Serial.print(" | Filtered distance: ");
  Serial.println(filteredDistance);
  
  // Change firstIteration flag to false once first iteration is complete
  firstIteration = false;
  
  // Transmit sensor data at a rate of 10 Hz (equals 100 ms)
  delay(10);
}

// Filter that uses exponential smoothing (as a low-pass filter) to remove noise
float filter(float currentValue, int sensorValue){
  if (firstIteration == true)
    currentValue = weight * sensorValue + (1.0 - weight) * 0;
  else
    currentValue = weight * sensorValue + (1.0 - weight) * currentValue;    

  return currentValue;
}
