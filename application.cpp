#include "PhotonLib/PhotonLib.h"

void sometimes();
void often();

int msgsec = 1; // be nice
int sometimes_period = 10000;
int often_period = 1000;

Timer tm_sometimes(sometimes_period, sometimes);
Timer tm_often(often_period, often);

int set_flood(String command) {
    int ms = atoi(command);
    if(ms) {
        msgsec = ms;
    }
    return msgsec;
}

int set_sometimes_period(String command) {
    int o = atoi(command);
    if(o) {
        sometimes_period = o;
        tm_sometimes.changePeriod(sometimes_period);
    }
    return sometimes_period;
}

int set_often_period(String command) {
    int o = atoi(command);
    if(o) {
        often_period = o;
        tm_often.changePeriod(often_period);
    }
    return often_period;
}

void setup() {
    pinMode(D7, OUTPUT);
    digitalWrite(D7, HIGH);

    photon_setup();

    Particle.function("flood", set_flood);
    Particle.function("often", set_often_period);
    Particle.function("sometimes", set_sometimes_period);

    tm_often.start();     // often
    tm_sometimes.start();    // sometimes

}

void loop() {
    photon_loop();
}

uint32_t count = 0;

void sometimes() {
    char stmp[128];
    sprintf(stmp, "%lums mem:%lu", millis(), System.freeMemory());
    Particle.publish("Publishing/sometimes", stmp, 60, PRIVATE);
}

void often() {
    static int count = 0;
    if(++count == 1) {
        count = 0;
        char stmp[128];
        for(int i = 0; i < msgsec; i++) {
            sprintf(stmp, "%d %lu", i, millis());
            Particle.publish("Publishing/often", stmp, 60, PRIVATE);
        }
    }
}

