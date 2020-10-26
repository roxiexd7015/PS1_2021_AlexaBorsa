#include "Arduino.h"
#include "DeHasTeu.h"

//Daca putem cu C++, de ce sa nu lucram cu obiecte?

DHT::DHT(int _pin)
{
	pin = _pin;
}

//DHT11 comunica pe un singur fir
void DHT::start()
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delay(18);
    digitalWrite(pin, HIGH);
    //Setam pinul ca iesire si il tragem la 0 pentru 18 microsecunde
    pinMode(pin, INPUT);
    //Acum ne pregatim sa citim datele de la senzor
    //digitalWrite(dht11_pin, HIGH); 
}

float DHT::Busu()
{
    uint16_t rawHumidity = 0;
    uint16_t rawTemperature = 0;
    uint8_t checkSum = 0;
    uint16_t data = 0;

    uint8_t humi;
    uint8_t humd;
    uint8_t tempi;
    uint8_t tempd;

    unsigned long startTime;
    
    start();

    for (int8_t i = -3; i < 80; i++) {
        //ca sa ne dam seama de ce valoare primim
        //masuram mereu lungimea semnalului
        byte live;
        startTime = micros();

        do {
            live = (unsigned long)(micros() - startTime);
            if (live > 90)
                //asta veti primi in cazul in care nu functioneaza senzorul
                return -1.0;
        } while (digitalRead(pin) == (i & 1) ? HIGH : LOW);

        //facem loc unui nou bit
        if (i >= 0 && (i & 1)) {
            data <<= 1;

            // semnalul pentru 0 are maxim 30 us
            //iar cel pentru 1 are cel putin 68
            if (live > 30) {
                data |= 1; // daca semnalul tine mai mult de 30 us, avem un 1
                //daca nu, nu facem nimic pentru ca deja avem un 0
            }
        }

        switch (i) {
        case 31:
            //primii 2 octeti sunt umiditatea
            rawHumidity = data;
            break;
        case 63:
            //urmatorii 2 sunt temperatura
            rawTemperature = data;
        case 79:
            //ultimul octet e checksum
            checkSum = data;
            data = 0;
            break;
        }
    }
    humi = rawHumidity >> 8;
    rawHumidity = rawHumidity << 8;
    humd = rawHumidity >> 8;
    tempi = rawTemperature >> 8;
    rawTemperature = rawTemperature << 8;
    tempd = rawTemperature >> 8;

    //verificam integritatea datelor receptionate
    if ((byte)checkSum == (byte)(tempi + tempd + humi + humd ))
        return (tempi + (float(tempd) / 10));
    else
        //am pus -3 ca asa mi-a venit, faceti ce vreti din valoarea asta
        //important e sa stiti ca asta va fi afisat cand datele trimise de DHT11 nu sunt bune
        return -3.0;
}
