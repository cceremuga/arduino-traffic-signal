# arduino-traffic-signal

Controls a single triple-light 110VAC traffic signal based upon this premade controller board https://github.com/iotmug/3ch-7Segments-3PushButton-traffic-light-controller

- Runs 3 phases in serial: 35 seconds red, 30 seconds green, and 3 seconds yellow.
- Displays a countdown to the end of the current light phase on the 7 segment display.
- 7 segment display is multiplexed / non-blocking to the current phase's execution.
