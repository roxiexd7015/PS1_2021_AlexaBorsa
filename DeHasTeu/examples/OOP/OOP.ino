#include <DeHasTeu.h>
//aici va puneti un pin in functie de montajul vostru
#define DHT_Pin 2

void setup()
{
  Serial.begin(9600); 
}

DHT merge(DHT_Pin);

void loop() {
  //N-am ce face...
  //Are nevoie sa stea 2 secunde inainte sa transmita iar.
  //Asta e vina mea.
  delay(2000);
  merge.start();
  //Metoda lui Busu o sa va returneze temperatura sub forma de float
  //Sau o eroare
  //Ce faceti cu informatia e problema voastra
  float temperatura = merge.Busu();
  //Aveti 2 tipuri de erori:
  //-1 daca nu raspunde senzorul - in general, ati gresit montajul
  //-3 daca datele sunt corupte
  if(temperatura > 0)
  {
  Serial.print(temperatura);
  Serial.println(" C");
  }
  else
  Serial.println("Cum ai trecut la FIE?");
}
