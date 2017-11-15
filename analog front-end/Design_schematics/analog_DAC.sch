EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:components_library
LIBS:teensy
LIBS:dc-dc
LIBS:Complete_analog_front-end-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 4
Title "DAC output analog frontend"
Date "2017-10-08"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L INA128 U9
U 1 1 59DA7513
P 3650 3150
F 0 "U9" H 3800 3275 50  0000 L CNN
F 1 "INA128" H 3800 3025 50  0000 L CNN
F 2 "" H 3750 3150 50  0000 C CNN
F 3 "" H 3750 3150 50  0000 C CNN
	1    3650 3150
	1    0    0    -1  
$EndComp
$Comp
L INA128 U10
U 1 1 59DA751A
P 7550 3150
F 0 "U10" H 7700 3275 50  0000 L CNN
F 1 "INA128" H 7700 3025 50  0000 L CNN
F 2 "" H 7650 3150 50  0000 C CNN
F 3 "" H 7650 3150 50  0000 C CNN
	1    7550 3150
	1    0    0    -1  
$EndComp
NoConn ~ 3350 3050
NoConn ~ 3350 3250
$Comp
L POT RV1
U 1 1 59DA7523
P 4700 3300
F 0 "RV1" H 4700 3220 50  0000 C CNN
F 1 "POT" H 4700 3300 50  0000 C CNN
F 2 "" H 4700 3300 50  0000 C CNN
F 3 "" H 4700 3300 50  0000 C CNN
	1    4700 3300
	0    1    1    0   
$EndComp
$Comp
L GNDA #PWR1
U 1 1 59DA752A
P 4700 3700
F 0 "#PWR1" H 4700 3450 50  0001 C CNN
F 1 "GNDA" H 4700 3550 50  0000 C CNN
F 2 "" H 4700 3700 50  0000 C CNN
F 3 "" H 4700 3700 50  0000 C CNN
	1    4700 3700
	1    0    0    -1  
$EndComp
$Comp
L BNC P6
U 1 1 59DA7530
P 9100 3150
F 0 "P6" H 9110 3270 50  0000 C CNN
F 1 "BNC" V 9210 3090 50  0000 C CNN
F 2 "" H 9100 3150 50  0000 C CNN
F 3 "" H 9100 3150 50  0000 C CNN
	1    9100 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 3150 4700 3150
Wire Wire Line
	4700 3450 4700 3700
Wire Wire Line
	8950 3150 7950 3150
Wire Wire Line
	7650 3450 9100 3450
Wire Wire Line
	9100 3450 9100 3350
Wire Wire Line
	3750 3450 3750 3600
Connection ~ 4700 3600
$Comp
L Q_NPN_BCE Q1
U 1 1 59DA753E
P 7100 4000
F 0 "Q1" H 7400 4050 50  0000 R CNN
F 1 "Q_NPN_BCE" H 7700 3950 50  0000 R CNN
F 2 "" H 7300 4100 50  0000 C CNN
F 3 "" H 7100 4000 50  0000 C CNN
	1    7100 4000
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7000 2950 7000 3200
Wire Wire Line
	7000 2950 7250 2950
Wire Wire Line
	7250 3350 7000 3350
$Comp
L RELAY_2RT K1
U 1 1 59DA7548
P 5950 3450
F 0 "K1" H 5900 3850 50  0000 C CNN
F 1 "RELAY_2RT" H 6100 2950 50  0000 C CNN
F 2 "" H 5950 3450 50  0000 C CNN
F 3 "" H 5950 3450 50  0000 C CNN
	1    5950 3450
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3200 2500 7550 2500
Wire Wire Line
	3650 2500 3650 2850
Wire Wire Line
	7550 2500 7550 2850
Connection ~ 3650 2500
$Comp
L GND #PWR4
U 1 1 59DA7553
P 7000 4550
F 0 "#PWR4" H 7000 4300 50  0001 C CNN
F 1 "GND" H 7000 4400 50  0000 C CNN
F 2 "" H 7000 4550 50  0000 C CNN
F 3 "" H 7000 4550 50  0000 C CNN
	1    7000 4550
	1    0    0    -1  
$EndComp
$Comp
L GNDA #PWR2
U 1 1 59DA7559
P 4950 2750
F 0 "#PWR2" H 4950 2500 50  0001 C CNN
F 1 "GNDA" H 4950 2600 50  0000 C CNN
F 2 "" H 4950 2750 50  0000 C CNN
F 3 "" H 4950 2750 50  0000 C CNN
	1    4950 2750
	-1   0    0    1   
$EndComp
Wire Wire Line
	8200 4000 7300 4000
Wire Wire Line
	8150 3700 6350 3700
Wire Wire Line
	6350 3800 7000 3800
Wire Wire Line
	4850 3300 5550 3300
Wire Wire Line
	5550 3100 4950 3100
Wire Wire Line
	4950 3100 4950 2750
Wire Wire Line
	7000 3200 6350 3200
Wire Wire Line
	7000 4200 7000 4550
$Comp
L GNDA #PWR3
U 1 1 59DA7567
P 7000 3450
F 0 "#PWR3" H 7000 3200 50  0001 C CNN
F 1 "GNDA" H 7000 3300 50  0000 C CNN
F 2 "" H 7000 3450 50  0000 C CNN
F 3 "" H 7000 3450 50  0000 C CNN
	1    7000 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 3350 7000 3450
Wire Wire Line
	3650 3450 3650 5250
Wire Wire Line
	3200 5250 7550 5250
Wire Wire Line
	7550 5250 7550 3450
Connection ~ 3650 5250
NoConn ~ 7250 3050
NoConn ~ 7250 3250
NoConn ~ 5550 3400
NoConn ~ 5550 3600
NoConn ~ 6350 3500
Wire Wire Line
	3750 3600 5000 3600
Wire Wire Line
	2800 3350 3350 3350
Wire Wire Line
	3350 2950 2800 2950
Text HLabel 3200 2500 0    60   Input ~ 0
PowerStage_VDD
Text HLabel 3200 5250 0    60   Input ~ 0
PowerStage_VSS
Text HLabel 8150 3700 2    60   Input ~ 0
PowerStage_VDD
Text HLabel 8200 4000 2    60   Input ~ 0
EnableOutput
Text Notes 700  900  0    60   ~ 0
DAC analog output part\nThis part connects the DAC of the Teensy to the instrument. It performs level shift and shrinking of the signal. A relay short to ground the signal path so that no unwanted signal is delivered to the instrument.
Text Notes 800  1250 0    60   Italic 0
To check\n1- Grounding configuration\n2- Position and value of the filter capacitor
$Comp
L INA128 U8
U 1 1 59DA8982
P 1950 7000
F 0 "U8" H 2100 7125 50  0000 L CNN
F 1 "INA128" H 2100 6875 50  0000 L CNN
F 2 "" H 2050 7000 50  0000 C CNN
F 3 "" H 2050 7000 50  0000 C CNN
	1    1950 7000
	1    0    0    -1  
$EndComp
NoConn ~ 1950 6700
NoConn ~ 1950 7300
NoConn ~ 2050 7300
NoConn ~ 1650 6900
NoConn ~ 1650 7100
$Comp
L R R3
U 1 1 59DA898E
P 1150 6800
F 0 "R3" V 1230 6800 50  0000 C CNN
F 1 "R" V 1150 6800 50  0000 C CNN
F 2 "" V 1080 6800 50  0000 C CNN
F 3 "" H 1150 6800 50  0000 C CNN
	1    1150 6800
	0    1    1    0   
$EndComp
$Comp
L R R4
U 1 1 59DA8995
P 1150 7200
F 0 "R4" V 1230 7200 50  0000 C CNN
F 1 "R" V 1150 7200 50  0000 C CNN
F 2 "" V 1080 7200 50  0000 C CNN
F 3 "" H 1150 7200 50  0000 C CNN
	1    1150 7200
	0    1    1    0   
$EndComp
$Comp
L C C2
U 1 1 59DA899C
P 1400 7000
F 0 "C2" H 1425 7100 50  0000 L CNN
F 1 "C" H 1425 6900 50  0000 L CNN
F 2 "" H 1438 6850 50  0000 C CNN
F 3 "" H 1400 7000 50  0000 C CNN
	1    1400 7000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 6800 1650 6800
Wire Wire Line
	1400 6850 1400 6800
Connection ~ 1400 6800
Wire Wire Line
	1300 7200 1650 7200
Wire Wire Line
	1400 7200 1400 7150
Connection ~ 1400 7200
NoConn ~ 1000 6800
NoConn ~ 1000 7200
Text Notes 950  6100 0    60   ~ 12
Filter implementation\n
Text Notes 950  6350 0    60   ~ 0
An INA can easily be implemented with a low pass filter.\nTime constant of the filter is 2*R*C.
Wire Notes Line
	900  5900 3700 5900
Wire Notes Line
	3700 5900 3700 7450
Wire Notes Line
	3700 7450 900  7450
Wire Notes Line
	900  7450 900  5900
$Comp
L C FilterCapacitor1
U 1 1 59DA89E5
P 5000 3450
F 0 "FilterCapacitor1" H 5025 3550 50  0000 L CNN
F 1 "10e-6" H 5025 3350 50  0000 L CNN
F 2 "" H 5038 3300 50  0000 C CNN
F 3 "" H 5000 3450 50  0000 C CNN
	1    5000 3450
	1    0    0    -1  
$EndComp
Connection ~ 5000 3300
Text HLabel 2800 2950 0    60   Input ~ 0
DAC_signal
Text HLabel 2800 3350 0    60   Input ~ 0
DAC_reference
$Comp
L R PullDown1
U 1 1 59DA1A96
P 7950 4250
F 0 "PullDown1" V 8030 4250 50  0000 C CNN
F 1 "47 k" V 7950 4250 50  0000 C CNN
F 2 "" V 7880 4250 50  0000 C CNN
F 3 "" H 7950 4250 50  0000 C CNN
	1    7950 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7950 4100 7950 4000
Connection ~ 7950 4000
$Comp
L GND #PWR5
U 1 1 59DA1B09
P 7950 4650
F 0 "#PWR5" H 7950 4400 50  0001 C CNN
F 1 "GND" H 7950 4500 50  0000 C CNN
F 2 "" H 7950 4650 50  0000 C CNN
F 3 "" H 7950 4650 50  0000 C CNN
	1    7950 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	7950 4650 7950 4400
$EndSCHEMATC
