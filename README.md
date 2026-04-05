# GTK Calculator (C + GTK4)

An animated scientific calculator built using C and GTK4.

## Features
- Basic operations (+, -, *, /)
- Scientific functions (sin, cos, tan, log, sqrt)
- Power (^)
- Animated UI using GtkRevealer
- Dark theme

## How to Run

### Install dependencies
pacman -S mingw-w64-ucrt-x86_64-gtk4
pacman -S mingw-w64-ucrt-x86_64-gcc

### Compile
gcc -o calculator src/calculator.c `pkg-config --cflags --libs gtk4`

### Run
./calculator

## Author
Shruti Hore
