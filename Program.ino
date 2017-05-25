// This program allows the ESP-8266 to read data from GP2Y0A41SK0F distance measuring sensor

// Sensor calibration - You can change these values
const int minimum_position_analog = 818;  // Your sensor analog reading at 4 cm (typical value is 822)
const int maximum_position_analog = 102;  // Your sensor analog reading at 30 cm (typical value is 124)

// Sensor constants
const float minimum_position_meter = 0.04;  // Sensor minimum position (fixed at 4 cm)
const float maximum_position_meter = 0.30;  // Sensor maximum position (fixed at 30 cm)
const float sensor_offset = 0.0042;         // Sensor offset position (fixed at 0.42 cm)
const float sensor_slope = ((1.0/(minimum_position_meter + sensor_offset)) - (1.0/(maximum_position_meter + sensor_offset))) / (minimum_position_analog - maximum_position_analog);
const float sensor_yintercept = (1.0/(minimum_position_meter + sensor_offset)) - (sensor_slope * minimum_position_analog);

// Global variables
int current_position_analog = 0;
float current_position_meter = 0;
float current_position_volt = 0;

// Setup code to run once
void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

// Convert analog reading (0..1023) to position in meter
float convertPosition(int analogPosition) {
  // Local variables
  float realPosition = 0;

  // Too far
  if (analogPosition < maximum_position_analog) {
    realPosition = maximum_position_meter;
  }

  // Too close
  else if (analogPosition > minimum_position_analog) {
    realPosition = minimum_position_meter;
  }

  // Within sensor range
  else {
    realPosition = (1.0 / ((sensor_slope * analogPosition) + sensor_yintercept)) - sensor_offset;
  }

  // Return value
  return realPosition;
}

// Read sensor output analog voltage 
void readSensorData() {
  current_position_analog = (int)analogRead(A0);
  current_position_volt = 3.3 * current_position_analog / 1023;
  current_position_meter = convertPosition(current_position_analog);
}

// Print sensor readings to serial monitor
void printSensorData() {
  Serial.print("Distance: ");
  Serial.print(100.0 * current_position_meter);
  Serial.print(" cm = ");
  Serial.print(current_position_analog);
  Serial.print("/1023 = ");
  Serial.print(current_position_volt);
  Serial.println(" V");
}

// Main code to run repeatedly
void loop() {
  readSensorData();
  printSensorData();
}
