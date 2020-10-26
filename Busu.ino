#include <LiquidCrystal_I2C.h>
#include <DeHasTeu.h>
#define DHT_pin 2
LiquidCrystal_I2C LCD(0x27, 16, 2);//Adresa I2C, coloane, linii
DHT dht(DHT_pin);
volatile int s = 10, m=10, h=20;// secunde, minute, ora
volatile bool one_second = false;// flag pentru o secunda
volatile bool two_second = false;// flag pentru doua secunde
volatile float temperatura;
void setup() {
  // put your setup code here, to run once:
  DDRB |= 1 << PB5;
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << WGM12) | (1 << CS12) | _BV(CS10) ;
  TIMSK1 |= (1 << OCIE1A);
  OCR1A = 15624;
  sei();// generam o intrerupere la fiecare secunda
  LCD.init();
  LCD.backlight();
  Serial.begin(9600);
}
ISR(TIMER1_COMPA_vect)
{
  PORTB ^= _BV(PB5);
  if (digitalRead(13) == 0)
    two_second = true;
  //operatiuni pentru ora
  s++;
  if (s == 60)
  {
    m++;
    s = 0;
    if (m == 60)
    {
      h++;
      m = 0;
    }
  }
  Serial.print(h);
  Serial.print(" ");
  Serial.print(m);
  Serial.print(" ");
  Serial.println(s);
  one_second = true;
}
void loop() {
  // daca a trecut o secunda, se afiseaza temperatura
  if (one_second)
  {
    //daca au trecut 2 secunde, citim temperatura
    //DHT11 are maxim 0.5Hz
    if (two_second)
    {
      temperatura = dht.Busu();
      LCD.clear();
      two_second = false;
    }
    //verificam temperatura citita
    //-1 si -3 sunt erori din biblioteca
    LCD.setCursor(0,0);
    LCD.print("ORA ");
    LCD.setCursor(4,0);
    LCD.print(h);
    LCD.setCursor(7,0);
    LCD.print(m);
    LCD.setCursor(10,0);
    LCD.print(s);
    LCD.setCursor(0,1);
    LCD.print("Temp= ");
    if (temperatura > 0)
    {
      LCD.setCursor(7, 1);
      LCD.print(temperatura);
      LCD.print(" C");
    }
    else
    {
      LCD.setCursor(0, 1);
      LCD.print("Eroare de senzor");
    }
  }
}
