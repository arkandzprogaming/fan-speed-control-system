// Pin Definitions
const int thermistorPin = A3;  // Analog pin connected to the thermistor voltage divider
const int fanPin = 3;         // PWM pin to control the fan speed

const float R0 = 10000.0;       // Resistance at 25°C (10kΩ)
const float B = 3950.0;         // B-value (25°C-50°C)
const float T0 = 298.15;        // Reference temperature in Kelvin (25°C)
const float Vcc = 5.0;          // Supply voltage
const float Rf = 10000.0;       // Fixed resistor in voltage divider

// Temperature-to-PWM mapping
const float tempMin = 0.0;   // Minimum temperature (°C) for fan control
const float tempMax = 100.0;  // Maximum temperature (°C) for fan control

void setup() {
  pinMode(thermistorPin, INPUT);
  pinMode(fanPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Step 1: Read thermistor voltage
  int adcValue = analogRead(thermistorPin);               // Read ADC value (0–1023)
  float Vout = (adcValue / 1023.0) * Vcc;                 // Convert to voltage
  
  // Step 2: Calculate thermistor resistance
  float Rt = Rf * (Vout / (Vcc - Vout));                     // Resistance of the thermistor
  
  // Step 3: Calculate temperature in Celsius using B-value equation
  float temperature = calculateTemperature(Rt);           // Convert resistance to temperature
  
  // Step 4: Map temperature to PWM duty cycle (0–255)
  int D = mapTemperatureToPWM(temperature);       // Convert temperature to fan speed
  
  // Step 5: Write PWM signal to fan
  analogWrite(fanPin, D);

  int percent_D = (D * 100) / 255;

  // Print temperature
  Serial.print("adcValue: ");
  Serial.print(adcValue);
  Serial.print(", Vout: ");
  Serial.print(Vout);
  Serial.print(" V, Rt: ");
  Serial.print(Rt);
  Serial.print(" Ohm, Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, PWM: ");
  Serial.println(percent_D);

  delay(500); // Delay for readability
}

float calculateTemperature(float Rt) {
  // Calculate temperature in Kelvin using the simplified thermistor resistance equation
  float tempK = 1.0 / ( (1.0 / T0) + (log(Rt / R0) / B) );

  // Convert Kelvin to Celsius
  return tempK - 273.15;
}

// Function to map temperature to PWM duty cycle
int mapTemperatureToPWM(float temperature) {
  // Clamp temperature to range [tempMin, tempMax]
  if (temperature < tempMin) temperature = tempMin;
  if (temperature > tempMax) temperature = tempMax;

  // Map temperature range [tempMin, tempMax] to PWM range [0, 255]
  return map(temperature, tempMin, tempMax, 0, 255);
}
