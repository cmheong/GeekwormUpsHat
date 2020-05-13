# GeekwormUpsHat
updated main.c I2C code for Geekworm Raspberry Pi B+ UPS Hat V1.0

To compile,
gcc main2.c -o ups_read2

Sample run:
# ./ups_read2 -a
MODE 10 00
CFG 97 1c
CRATE ff f9
VER 00 12
STAT 00 ff
VRST 00 0c
VCELL cf f0
CREG 60 65
4.158750V 96.394531%

Original main.c is here:
http://raspberrypiwiki.com/index.php?title=Raspi_UPS_HAT_Board
