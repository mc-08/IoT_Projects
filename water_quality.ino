#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#define SensorPin A1            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00             //deviation compensate
unsigned long int avgValue;     //Store the average value of the sensor feedback

#define ONE_WIRE_BUS 6
SoftwareSerial mySerial(7, 8); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS); // Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
int blue = 2;
int red = 3;
int green = 5;
int tempblue = 7;
int tempred = 8;
int tempgreen = 9;
int turbblue = 11;
int turbred = 12;
int turbgreen = 13;
float phValue;
float temperature;
void setup(void)
{
  // start serial port
  Serial.begin(9600);
  sensors.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement
  lcd.begin(16, 2); //initilise lcd with num of coloums 16 ,by row 2.
  lcd.clear(); //clears lcd just incase there is anytin been displayed
  pinMode(blue, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(tempblue, OUTPUT);
  pinMode(tempred, OUTPUT);
  pinMode(tempgreen, OUTPUT);
  pinMode(turbblue, OUTPUT);
  pinMode(turbred, OUTPUT);
  pinMode(turbgreen, OUTPUT);
}
void loop() {
  Serial.println(" TAKING READINGS ");
  lcd.clear( );
  erial.println(" ");
  digitalWrite(turbblue, LOW);
  digitalWrite(turbgreen, LOW);
  digitalWrite(turbred, LOW);
  lcd.clear( );
  lcd.setCursor(1, 0); //set cursor (colum by row) indexing from 0
  lcd.print("READINGS FROM");
  lcd.setCursor(0, 1);
  lcd.print("TURBIDITY SENSOR");
  Serial.println("Taking Readings from turbidity Sensor");
  TURBblink();
  int turbidityValue = analogRead(A2);
  float turbidityV = turbidityValue / 100;
  Serial.print("Turbidity level: ");
  Serial.println(turbidityV);

  if ( turbidityV > 9) {
    digitalWrite(turbblue, LOW);
    digitalWrite(turbgreen, HIGH);
    digitalWrite(turbred, LOW );
    lcd.clear( );
    lcd.setCursor(0, 0); //set cursor (colum by row) indexing from 0
    lcd.print("TURBI LEV:");
    lcd.setCursor(11, 0);
    lcd.print(turbidityV);
    lcd.setCursor(14, 0);
    lcd.print("NTU");
    lcd.setCursor(0, 1);
    Serial.print("Turbidity Level: ");
    Serial.print(turbidityV);
    Serial.println("NTU");
    lcd.setCursor(0, 1); //set cursor (colum by row) indexing from 0
    lcd.print("WATER VERY CLEAN");
    Serial.println("Water Very Clean ");
    delay(3000);
    lcd.clear( );
    delay(3000);
    sensors.requestTemperatures(); // Send the command to get temperature
    Serial.print("Water Temperature: ");
    Serial.println(sensors.getTempCByIndex(0));
    delay(3000);
    lcd.clear( );
    delay(3000);
    int buf[10];                //buffer for read analog
    for (int i = 0; i < 10; i++) //Get 10 sample value from the sensor for smooth the value
    {
      buf[i] = analogRead(SensorPin);
      delay(10);
    }
    for (int i = 0; i < 9; i++) //sort the analog from small to large
    {
      for (int j = i + 1; j < 10; j++)
      {
        if (buf[i] > buf[j])
        {
          int temp = buf[i];
          buf[i] = buf[j];
          buf[j] = temp;
        }
      }
    }
    avgValue = 0;
    for (int i = 2; i < 8; i++)               //take the average value of 6 center sample
      avgValue += buf[i];
    float phValue = (float)avgValue * 3.8 / 1030 / 6; //convert the analog into millivolt
    phValue = 3.3 * phValue + Offset;                //convert the millivolt into pH value
    Serial.print("pH:");
    Serial.print(phValue, 2);
    Serial.println(" ");

    if (phValue >= 7.30) {
      lcd.clear();
      digitalWrite(blue, LOW);
      digitalWrite(green, LOW);
      digitalWrite(red, HIGH);
      lcd.setCursor(1, 0); //set cursor (colum by row) indexing from 0
      lcd.print("PH VALUE:");
      lcd.setCursor(10, 0);
      lcd.print(phValue);
      lcd.setCursor(0, 1);
      Serial.print("PH VALUE: ");
      Serial.println(phValue);
      lcd.setCursor(0, 1); //set cursor (colum by row) indexing from 0
      lcd.print("ALKALINITY HIGH");
      Serial.println("Water Alkalinity high");
      delay(3000);
    }
  }

  if (phValue >= 6.90 && phValue <= 7.19) {
    digitalWrite(blue, HIGH);
    digitalWrite(green, LOW);
    digitalWrite(red, LOW);
    lcd.clear();
    lcd.setCursor(1, 0); //set cursor (colum by row) indexing from 0
    lcd.print("PH VALUE:");
    lcd.setCursor(10, 0);
    lcd.print(phValue);
    lcd.setCursor(0, 1);
    Serial.print("PH VALUE: ");
    Serial.println(phValue);
    lcd.setCursor(1, 1); //set cursor (colum by row) indexing from 0
    lcd.print("WATER IS SAFE");
    Serial.println("Water Is  neutral (safe)");
    delay(3000);
    lcd.clear( );
    delay(3000);
    sensors.requestTemperatures(); // Send the command to get temperature
    Serial.print("Water Temperature: ");
    Serial.println(sensors.getTempCByIndex(0));
    delay(3000);
    lcd.clear( );
    delay(3000);
    int buf[10];                //buffer for read analog
    for (int i = 0; i < 10; i++) //Get 10 sample value from the sensor for smooth the value
    {
      buf[i] = analogRead(SensorPin);
      delay(10);
    }
    for (int i = 0; i < 9; i++) //sort the analog from small to large
    {
      for (int j = i + 1; j < 10; j++)
      {
        if (buf[i] > buf[j])
        {
          int temp = buf[i];
          buf[i] = buf[j];
          buf[j] = temp;
        }
      }
    }
    avgValue = 0;
    for (int i = 2; i < 8; i++)               //take the average value of 6 center sample
      avgValue += buf[i];
    float phValue = (float)avgValue * 3.8 / 1030 / 6; //convert the analog into millivolt
    phValue = 3.3 * phValue + Offset;                //convert the millivolt into pH value
    Serial.print("pH:");
    Serial.print(phValue, 2);
    Serial.println(" ");

    if (phValue >= 7.30) {
      lcd.clear();
      digitalWrite(blue, LOW);
      digitalWrite(green, LOW);
      digitalWrite(red, HIGH);
      lcd.setCursor(1, 0); //set cursor (colum by row) indexing from 0
      lcd.print("PH VALUE:");
      lcd.setCursor(10, 0);
      lcd.print(phValue);
      lcd.setCursor(0, 1);
      Serial.print("PH VALUE: ");
      Serial.println(phValue);
      lcd.setCursor(0, 1); //set cursor (colum by row) indexing from 0
      lcd.print("ALKALINITY HIGH");
      Serial.println("Water Alkalinity high");
      delay(3000);
    }
  }

  if (phValue < 6.89) {
    lcd.clear();
    digitalWrite(blue, LOW);
    digitalWrite(green, LOW);
    digitalWrite(red, HIGH);
    lcd.setCursor(1, 0); //set cursor (colum by row) indexing from 0
    lcd.print("PH VALUE:");
    lcd.setCursor(10, 0);
    lcd.print(phValue);
    lcd.setCursor(0, 1);
    Serial.print("PH VALUE: ");
    Serial.println(phValue);
    lcd.setCursor(2, 1); //set cursor (colum by row) indexing from 0
    lcd.print("ACIDITY HIGH");
    Serial.println("Water Acidity High");
    delay(3000);
  }



  if ( turbidityV >= 6 && turbidityValue / 100 <= 9 ) {
    digitalWrite(turbblue, HIGH);
    digitalWrite(turbgreen, LOW);
    digitalWrite(turbred, LOW);
    lcd.clear( );
    lcd.setCursor(0, 0); //set cursor (colum by row) indexing from 0
    lcd.print("TURBI LEV:");
    lcd.setCursor(10, 0);
    lcd.print(turbidityV);
    lcd.setCursor(13, 0);
    lcd.print("NTU");
    lcd.setCursor(0, 1);
    Serial.print("Turbidity Level: ");
    Serial.print(turbidityV);
    Serial.println("NTU");
    lcd.setCursor(0, 1); //set cursor (colum by row) indexing from 0
    lcd.print("WATER NORM CLEAN");
    Serial.println("Water Clean ");
    delay(3000);
    Serial.println(" ");

    if (phValue >= 7.30) {
      lcd.clear();
      digitalWrite(blue, LOW);
      digitalWrite(green, LOW);
      digitalWrite(red, HIGH);
      lcd.setCursor(1, 0); //set cursor (colum by row) indexing from 0
      lcd.print("PH VALUE:");
      lcd.setCursor(10, 0);
      lcd.print(phValue);
      lcd.setCursor(0, 1);
      Serial.print("PH VALUE: ");
      Serial.println(phValue);
      lcd.setCursor(0, 1); //set cursor (colum by row) indexing from 0
      lcd.print("ALKALINITY HIGH");
      Serial.println("Water Alkalinity high");
      delay(3000);
    }

    if (phValue >= 6.90 && phValue <= 7.19) {
      digitalWrite(blue, HIGH);
      digitalWrite(green, LOW);
      digitalWrite(red, LOW);
      lcd.clear();
      lcd.setCursor(1, 0); //set cursor (colum by row) indexing from 0
      lcd.print("PH VALUE:");
      lcd.setCursor(10, 0);
      lcd.print(phValue);
      lcd.setCursor(0, 1);
      Serial.print("PH VALUE: ");
      Serial.println(phValue);
      lcd.setCursor(1, 1); //set cursor (colum by row) indexing from 0
      lcd.print("WATER IS SAFE");
      Serial.println("Water Is  neutral (safe)");
    }

    if (phValue < 6.89) {
      lcd.clear();
      digitalWrite(blue, LOW);
      digitalWrite(green, LOW);
      digitalWrite(red, HIGH);
      lcd.setCursor(1, 0); //set cursor (colum by row) indexing from 0
      lcd.print("PH VALUE:");
      lcd.setCursor(10, 0);
      lcd.print(phValue);
      lcd.setCursor(0, 1);
      Serial.print("PH VALUE: ");
      Serial.println(phValue);
      lcd.setCursor(2, 1); //set cursor (colum by row) indexing from 0
      lcd.print("ACIDITY HIGH");
      Serial.println("Water Acidity High");
      delay(3000);
    }
  }

  if ( turbidityV < 6) {
    digitalWrite(turbblue, LOW);
    digitalWrite(turbgreen, LOW);
    digitalWrite(turbred, HIGH);
    lcd.clear( );
    lcd.setCursor(0, 0); //set cursor (colum by row) indexing from 0
    lcd.print("TURBI LEV:");
    lcd.setCursor(11, 0);
    lcd.print(turbidityV);
    lcd.setCursor(14, 0);
    lcd.print("NTU");
    lcd.setCursor(0, 1);
    Serial.print("Turbidity Level: ");
    Serial.print(turbidityV);
    Serial.println("NTU");
    lcd.setCursor(0, 1); //set cursor (colum by row) indexing from 0
    lcd.print("WATER VERY DIRTY");
    Serial.println("Water Very Dirty ");
    delay(3000);
    Serial.println(" ");

    if (phValue >= 7.30) {
      lcd.clear();
      digitalWrite(blue, LOW);
      digitalWrite(green, LOW);
      digitalWrite(red, HIGH);
      lcd.setCursor(1, 0); //set cursor (colum by row) indexing from 0
      lcd.print("PH VALUE:");
      lcd.setCursor(10, 0);
      lcd.print(phValue);
      lcd.setCursor(0, 1);
      Serial.print("PH VALUE: ");
      Serial.println(phValue);
      lcd.setCursor(0, 1); //set cursor (colum by row) indexing from 0
      lcd.print("ALKALINITY HIGH");
      Serial.println("Water Alkalinity high");
      delay(3000);
    }

    if (phValue >= 6.90 && phValue <= 7.19) {
      digitalWrite(blue, HIGH);
      digitalWrite(green, LOW);
      digitalWrite(red, LOW);
      lcd.clear();
      lcd.setCursor(1, 0); //set cursor (colum by row) indexing from 0
      lcd.print("PH VALUE:");
      lcd.setCursor(10, 0);
      lcd.print(phValue);
      lcd.setCursor(0, 1);
      Serial.print("PH VALUE: ");
      Serial.println(phValue);
      lcd.setCursor(1, 1); //set cursor (colum by row) indexing from 0
      lcd.print("WATER IS SAFE");
      Serial.println("Water Is  neutral (safe)");
    }

    if (phValue < 6.89) {
      lcd.clear();
      digitalWrite(blue, LOW);
      digitalWrite(green, LOW);
      digitalWrite(red, HIGH);
      lcd.setCursor(1, 0); //set cursor (colum by row) indexing from 0
      lcd.print("PH VALUE:");
      lcd.setCursor(10, 0);
      lcd.print(phValue);
      lcd.setCursor(0, 1);
      Serial.print("PH VALUE: ");
      Serial.println(phValue);
      lcd.setCursor(2, 1); //set cursor (colum by row) indexing from 0
      lcd.print("ACIDITY HIGH");
      Serial.println("Water Acidity High");
      delay(3000);
    }
  }
  delay (8000);
}
