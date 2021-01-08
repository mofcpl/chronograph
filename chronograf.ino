


//Chronograf mofcpl@gmail.com

//---Biblioteki
#include <SPI.h>
//---Stałe
#define CS 10     //Chip select
#define NOS 10    //Liczba moĹĽliwych do wyĹ›wietlenia cyfr + kropka (0-9 + "kropka" daje 10 moĹĽliwoĹ›ci)
#define DIG1 8
#define DIG2 7
#define DIG3 6
#define BUTTON 4
#define DISPLAY_TIME 1

#define M_DISTANCE 0.072
#define F_DISTANCE 0.23622047244


byte displayArray[NOS] = { B00000011,   // 0  "jedynka" bitowa = "jedynka" dla włączenia segmentu
                           B10011111,   // 1  kolejno w kodzie => a-b-c-d-e-f-g-DP
                           B00100101,   // 2
                           B00001101,   // 3
                           B10011001,   // 4
                           B01001001,   // 5
                           B01000001,   // 6
                           B00011111,   // 7
                           B00000001,   // 8
                           B00001001    // 9
                         };

unsigned long Time1;
unsigned long Time2;
int wynik;

void write_number(int x)
{
  int Digit3 = x % 10;
  int Digit2 = ((int)(x / 10)) % 10;
  int Digit1 = (int)x / 100;

  
  digitalWrite(CS, LOW);
  SPI.transfer (displayArray[Digit1]);
  digitalWrite(CS, HIGH);
  digitalWrite(DIG1, HIGH);
  delay(DISPLAY_TIME);
  digitalWrite(DIG1, LOW);

  
  digitalWrite(CS, LOW);
  SPI.transfer (displayArray[Digit2]);
  digitalWrite(CS, HIGH);
  digitalWrite(DIG2, HIGH);
  delay(DISPLAY_TIME);
  digitalWrite(DIG2, LOW);

  
  digitalWrite(CS, LOW);
  SPI.transfer (displayArray[Digit3]);
  digitalWrite(CS, HIGH);
  digitalWrite(DIG3, HIGH);
  delay(DISPLAY_TIME);
  digitalWrite(DIG3, LOW);
}

void setup()
{
  wynik = 0;

  pinMode (BUTTON,INPUT);
  digitalWrite(BUTTON, HIGH);

  pinMode (DIG1, OUTPUT);
  pinMode (DIG2, OUTPUT);
  pinMode (DIG3, OUTPUT);
  pinMode (CS, OUTPUT);
  SPI.begin();

  Serial.begin(9600);
  while (!Serial);
  attachInterrupt(digitalPinToInterrupt(3), Sensor1Time, RISING);
  attachInterrupt(digitalPinToInterrupt(2), Sensor2Time, RISING);

  Serial.println("Start");

}

void Sensor2Time()
{
  Time2 = micros();
  //Serial.println("Wykonano przerwanie 2");
}

void Sensor1Time()
{
  Time1 = micros();
  //Serial.println("Wykonano przerwanie 1");
}


void loop()
{
  double result=0;
  //Serial.println(analogRead(5));
  //Serial.println(analogRead(5) * (5.0 / 1023.0));
  //  if (digitalRead(Sensor1) == HIGH) Serial.println("Wykryto stan wysoki");


  if ( Time1 > 0 && Time2 > 0)
  {

    Serial.print("Time1: ");
    Serial.println(Time1);

    Serial.print("Time2: ");
    Serial.println(Time2);

    //float Speed = 0.06 / (((double)Time2 - (double)Time1) / 1000000 );

    if (digitalRead(BUTTON) == LOW)
    {
      result = (double)F_DISTANCE / ((double)(Time2 - Time1) / 1000000 );
    }
    if (digitalRead(BUTTON) == HIGH)
    {
      result = (double)M_DISTANCE / ((double)(Time2 - Time1) / 1000000 );
    }

    Serial.print("predkosc: ");
    Serial.print(result,5);

    
    if (digitalRead(BUTTON) == LOW) Serial.println("fps");
    if (digitalRead(BUTTON) == HIGH) Serial.println("m/s");



    Time1 = 0;
    Time2 = 0;

 
  }

  if (result > 0) wynik = result;

  write_number(wynik);

}



