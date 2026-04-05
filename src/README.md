# GTK Calculator

A simple animated scientific calculator built using C and GTK4.

## Features
- Basic operations (+, -, *, /)
- Scientific functions (sin, cos, tan, log, sqrt)
- Power (^)
- Animated UI

## Run

gcc -o calculator src/calculator.c `pkg-config --cflags --libs gtk4`
./calculator
