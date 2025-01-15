#include <Quectel_BC660_custom.h>
#include <BME280I2C.h>
#include <Wire.h>

#define SERIAL_PORT Serial1 // Define the serial port for the Quectel module
#define MQTT_IP       test  // Put your mqtt ip here
#define MQTT_PORT     test  // Put your mqtt port here
#define MQTT_USERNAME test  // Put your mqtt username here
#define MQTT_PASSWORD test  // Put your mqtt password here
#define MQTT_TOPIC    test  // Put your mqtt topix here

const int potmeterPin = 34; // Pin connected to the potentiometer
const int ledPin = 4;       // Pin connected to the LED

QuectelBC660 quectel = QuectelBC660(5, true); // Initialize Quectel BC660 object
BME280I2C bme;                                // Initialize BME280 sensor object

float temp, hum, pres; // Variables for temperature, humidity, and pressure readings

void setup() 
{
  Serial.begin(115200); // Initialize serial communication
  Wire.begin();         // Initialize I2C communication

  pinMode(ledPin, OUTPUT); // Set the LED pin as output
  digitalWrite(ledPin, LOW); // Ensure the LED is off at the start

  Serial.println("Quectel MQTT client example");
  Serial.println("===================");

  delay(5000);

  // Attempt to initialize the BME280 sensor
  while(!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }

  // Identify and report the type of sensor detected
  switch(bme.chipModel())
  {
    case BME280::ChipModel_BME280:
      Serial.println("Found BME280 sensor! Success.");
      break;
    case BME280::ChipModel_BMP280:
      Serial.println("Found BMP280 sensor! No Humidity available.");
      break;
    default:
      Serial.println("Found UNKNOWN sensor! Error!");
  }

  // Initialize the Quectel BC660 module
  quectel.begin(&SERIAL_PORT);

  // Wait for network registration
  while(!quectel.getRegistrationStatus(5))
  {                    
    Serial.println("Waiting for network registration...");
    delay(2500);
  } 

  Serial.println("Module is successfully registered to network");

  temp = 21; // Placeholder temperature value
  Serial.println("\nTemp: " + String(temp) + " °C"); // Print initial temperature value

  delay(2500);

  // Disable deep sleep mode on the Quectel module
  Serial.println("\nTurn off deep sleep mode");
  if(quectel.setDeepSleep())
  {
    Serial.println("\tDeep sleep mode turned off");
  }
  else
  {
    Serial.println("\tDeep sleep mode turn off failed");
  }

  delay(2500);

  // Close any existing MQTT connection
  if(quectel.closeMQTT())
  {
    Serial.println("\tMQTT connection closed");
  }
  else
  {
    Serial.println("\tFailed to close MQTT connection");
  }

  delay(2500);
  
  // Enable SSL for MQTT client 0
  if (quectel.configureMQTTSSL(0, true, 0, 0))
  {
    Serial.println("SSL configured successfully.");
  }
  else
  {
    Serial.println("Failed to configure SSL.");
  }

  delay(2500);

  // Open a new MQTT connection
  Serial.println("\nOpen MQTT connection");
  if(quectel.openMQTT(0, MQTT_IP, MQTT_PORT))
  {
    Serial.println("\tMQTT connection opened");
  }
  else
  {
    Serial.println("\tFailed to open MQTT connection, trying again");
  }

  delay(5000);

  // Connect to the MQTT broker
  Serial.println("\nConnect to MQTT broker");
  if(quectel.connectMQTT(MQTT_USERNAME, MQTT_USERNAME, MQTT_PASSWORD))
  {
    Serial.println("\tConnected to MQTT broker");
  }
  else 
  {
    Serial.println("\tFailed to connect to MQTT broker");
  }

  delay(5000);

  // Publish the first sensor readings to the MQTT broker
  Serial.println("\nPublish first readings to MQTT broker");
  String jsonData = getClimateData(potmeterPin); // Get sensor data as JSON
  Serial.println(jsonData);

  int stringLength = jsonData.length();
  if(quectel.publishMQTT(jsonData.c_str(), stringLength, MQTT_TOPIC))
  { 
    Serial.println("\tTemperature reading published to MQTT broker");
  }
  else
  {
    Serial.println("\tFailed to publish temperature reading to MQTT broker");
  }

  delay(5000); 
}

void loop() {
  // Periodically get sensor data and publish it to the MQTT broker
  String jsonData = getClimateData(potmeterPin);
  Serial.println(jsonData);

  int stringLength = jsonData.length();
  if(quectel.publishMQTT(jsonData.c_str(), stringLength, "Robbert/Greenhouse"))
  { 
    Serial.println("\tTemperature reading published to MQTT broker");
  }
  else
  {
    Serial.println("\tFailed to publish temperature reading to MQTT broker");
  }
  delay(10000);
}

String getClimateData(int potentiometerPin)
{
    // Variables to store sensor readings
    float temp = NAN, hum = NAN, pres = NAN;

    // Units for the BME280 sensor
    BME280::TempUnit tempUnit = BME280::TempUnit_Celsius;
    BME280::PresUnit presUnit = BME280::PresUnit_Pa;

    // Read data from the BME280 sensor
    bme.read(pres, temp, hum, tempUnit, presUnit);

    // Read the raw analog value from the potentiometer
    int potValue = analogRead(potentiometerPin);

    // Convert the raw value to percentage (assuming a 3.3V reference)
    float potVoltage = (potValue / 4095.0) * 100.0;

    // Control LED based on potentiometer value
    if (potVoltage < 25)
    {
      digitalWrite(ledPin, HIGH); // Turn LED on if moisture is low
    }
    else
    {
      digitalWrite(ledPin, LOW); // Turn LED off if moisture is sufficient
    }

    // Create a buffer to hold the JSON string
    char jsonBuffer[150];

    // Format the data into the JSON string
    snprintf(jsonBuffer, sizeof(jsonBuffer),
             "{\"temperature\": %.2f, \"humidity\": %.2f, \"pressure\": %.2f, \"moisture\": %.2f}",
             temp, hum, pres, potVoltage);

    // Return the JSON string
    return String(jsonBuffer);
}
