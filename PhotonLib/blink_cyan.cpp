#include <Particle.h>

static uint32_t lastdisconnect = 0;
static uint32_t firstdisconnect = 0;
static uint32_t disconnected = 0;
static uint32_t connected = 0;

extern Timer tm_often;
extern Timer tm_sometimes;

bool blink_cyan(bool take_led_control, bool stop_timers) {
    if (!Particle.connected()) {
        if(stop_timers) {
            tm_often.stop();
            tm_sometimes.stop();
        }
        RGB.control(take_led_control);
        digitalWrite(D7, true);

        RGB.color(255, 255, 0); // ORANGE
        Particle.publish("crash", "test", 60, PRIVATE);

        //record when photon lost connection      
        disconnected = millis();

        if(millis() > (firstdisconnect + 1000)) {
            //try reconnect if disconnected > 6 secs

            RGB.color(0, 255, 0); // GREEN

            Particle.connect();
            while ((!Particle.connected())) { // && (millis()-lastdisconnect < 30000) ) {
                RGB.color(0, 0, 255); // BLUE
                Particle.process();
                delay(100);
                RGB.color(255, 0, 0); // RED
                delay(100);
            }
            RGB.color(128, 128, 128); // WHITE
            if(firstdisconnect == 0) {
                firstdisconnect = millis();
            }
        }
    }

    if(Particle.connected()) {
        digitalWrite(D7, false);
        if(disconnected) {
            RGB.control(false); // BREATHING CYAN
            char stmp[128];
            sprintf(stmp, "Disconnected %ld ms (previous connection: %ld ms)", millis() - disconnected, disconnected - connected);
            Particle.publish("reconnect", stmp);
            disconnected = 0;
            connected = millis();
            if(stop_timers) {
                tm_often.start();
                tm_sometimes.start();
            }
        }
        lastdisconnect = millis();
        return false;
    }
    return true;
}
