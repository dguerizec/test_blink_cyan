#include "PhotonLib.h"
#include "build.h"

bool leddebug = false;
bool stoptimers = false;

void enter_safe_mode() {
    System.enterSafeMode();
}

Timer tm_sm(1000, enter_safe_mode, true);

int photon_sys(String command) {
    const char *c = command.c_str();
    if(!strcmp("sm:" PHOTONLIB_PASSWORD, command)) {
        tm_sm.start(); // Use a timer to ensure an answer to the function call
        return 1;
    } else if(!strncmp("leddebug:", command, 5) && c[5]) {
        leddebug = c[5] != '0';
        return leddebug;
    } else if(!strncmp("stoptimers:", command, 11) && c[11]) {
        stoptimers = c[11] != '0';
        return stoptimers;
    }
    return 0;
}

void photon_setup() {
    Particle.publish("photonlib/system_version", System.version().c_str(), 60, PRIVATE);
    Particle.publish("photonlib/build_date", BUILD_DATE, 60, PRIVATE);


    /* defines system functions:
     * sys sm:<password>
     *      Put the photon in safe mode
     * sys leddebug:<0|1>
     *      Use RGB LED to debug
     */
    Particle.function("sys", photon_sys);
}

void photon_loop() {
    blink_cyan(leddebug, stoptimers);
}

