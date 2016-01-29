# test_blinking_cyan
Test case for "blink cyan" problem


How to reproduce the problem:

- Upload the user firmware

- Optionnal: particle call <device> sys leddebug:1

    This take control of the RGB led during disconnection to show where it get
    stuck in the code of PhotonLib/blink_cyan.cpp

- particle call <device> often 500

    This sets the period of the "often" timer to 500 ms.
    The "often" callback is just here to flood the cloud 

- particle call <device> sometimes 1000

    This sets the period of the "sometimes" timer to 1000 ms.
    The "sometimes" callback publishes the System.freeMemory()


The D7 led lights up when the photon is disconnected, and the RGB led (if
controlled) will blink in blue/red.


After a few seconds/minutes, we can see disconnections.

Sometimes, the photon couldn't reconnect and get stuck on blinking cyan, (or
blue/red blinking if "leddebug:1").

Adjusting the timers "often" and "sometimes" to lower values helps with the
disconnections.


Using the following one-liner right after booting it, my photon get stuck in less than 30 seconds:

particle call <device> sys leddebug:1 && particle call <device> often 500 &&
particle call <device> sometimes 1000



Another useful function is:

- particle call <device> flood 5

    This makes the "often" timer callback to (try to) send 5 messages instead
    of only one.



