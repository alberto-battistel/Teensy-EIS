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
Sheet 3 4
Title "ADC input analog frontend"
Date "2017-10-08"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L INA128 U1
U 1 1 59DA7D65
P 4250 2550
F 0 "U1" H 4400 2675 50  0000 L CNN
F 1 "INA128" H 4400 2425 50  0000 L CNN
F 2 "" H 4350 2550 50  0000 C CNN
F 3 "" H 4350 2550 50  0000 C CNN
	1    4250 2550
	1    0    0    -1  
$EndComp
$Comp
L BNC P1
U 1 1 59DA7D6C
P 3000 2350
F 0 "P1" H 3010 2470 50  0000 C CNN
F 1 "BNC" V 3110 2290 50  0000 C CNN
F 2 "" H 3000 2350 50  0000 C CNN
F 3 "" H 3000 2350 50  0000 C CNN
	1    3000 2350
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3150 2350 3950 2350
Wire Wire Line
	3000 2550 3000 2750
Wire Wire Line
	3000 2750 3950 2750
NoConn ~ 3950 2450
NoConn ~ 3950 2650
Wire Wire Line
	3950 2050 4250 2050
Wire Wire Line
	4250 2050 4250 2250
Wire Wire Line
	3950 3100 4250 3100
Wire Wire Line
	4250 3100 4250 2850
$Comp
L INA128 U4
U 1 1 59DA7D7F
P 4250 3950
F 0 "U4" H 4400 4075 50  0000 L CNN
F 1 "INA128" H 4400 3825 50  0000 L CNN
F 2 "" H 4350 3950 50  0000 C CNN
F 3 "" H 4350 3950 50  0000 C CNN
	1    4250 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 4500 4250 4500
Wire Wire Line
	4250 4500 4250 4250
Wire Wire Line
	4250 3650 4250 3400
Wire Wire Line
	4250 3400 3950 3400
$Comp
L BNC P4
U 1 1 59DA7D8B
P 3000 3750
F 0 "P4" H 3010 3870 50  0000 C CNN
F 1 "BNC" V 3110 3690 50  0000 C CNN
F 2 "" H 3000 3750 50  0000 C CNN
F 3 "" H 3000 3750 50  0000 C CNN
	1    3000 3750
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3150 3750 3950 3750
NoConn ~ 3950 3850
NoConn ~ 3950 4050
Wire Wire Line
	3950 4150 3000 4150
Wire Wire Line
	3000 4150 3000 3950
Wire Wire Line
	4900 3000 4350 3000
Wire Wire Line
	4350 3000 4350 2850
Wire Wire Line
	4900 4400 4350 4400
Wire Wire Line
	4350 4400 4350 4250
Wire Wire Line
	4900 2550 4650 2550
Wire Wire Line
	4900 3950 4650 3950
Text HLabel 3950 2050 0    60   Input ~ 0
PowerStage_VDD
Text HLabel 3950 3400 0    60   Input ~ 0
PowerStage_VDD
Text HLabel 3950 3100 0    60   Input ~ 0
PowerStage_VSS
Text HLabel 3950 4500 0    60   Input ~ 0
PowerStage_VSS
Text HLabel 4900 2550 2    60   Output ~ 0
ADC_input1
Text HLabel 4900 3950 2    60   Output ~ 0
ADC_input2
Text HLabel 4900 3000 2    60   Input ~ 0
DAC_reference
Text HLabel 4900 4400 2    60   Input ~ 0
DAC_reference
Text Notes 750  900  0    60   ~ 0
ADC part\nThere are two INAs to connect the instrument and the ADC of the Teensy. A reference from the Teensy is used for level shifting.
$Comp
L INA128 U6
U 1 1 59DA81F2
P 8450 2800
F 0 "U6" H 8600 2925 50  0000 L CNN
F 1 "INA128" H 8600 2675 50  0000 L CNN
F 2 "" H 8550 2800 50  0000 C CNN
F 3 "" H 8550 2800 50  0000 C CNN
	1    8450 2800
	1    0    0    -1  
$EndComp
NoConn ~ 8450 2500
NoConn ~ 8450 3100
NoConn ~ 8550 3100
NoConn ~ 8150 2700
NoConn ~ 8150 2900
$Comp
L R R1
U 1 1 59DA82A5
P 7650 2600
F 0 "R1" V 7730 2600 50  0000 C CNN
F 1 "R" V 7650 2600 50  0000 C CNN
F 2 "" V 7580 2600 50  0000 C CNN
F 3 "" H 7650 2600 50  0000 C CNN
	1    7650 2600
	0    1    1    0   
$EndComp
$Comp
L R R2
U 1 1 59DA82E4
P 7650 3000
F 0 "R2" V 7730 3000 50  0000 C CNN
F 1 "R" V 7650 3000 50  0000 C CNN
F 2 "" V 7580 3000 50  0000 C CNN
F 3 "" H 7650 3000 50  0000 C CNN
	1    7650 3000
	0    1    1    0   
$EndComp
$Comp
L C C1
U 1 1 59DA8310
P 7900 2800
F 0 "C1" H 7925 2900 50  0000 L CNN
F 1 "C" H 7925 2700 50  0000 L CNN
F 2 "" H 7938 2650 50  0000 C CNN
F 3 "" H 7900 2800 50  0000 C CNN
	1    7900 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 2600 8150 2600
Wire Wire Line
	7900 2650 7900 2600
Connection ~ 7900 2600
Wire Wire Line
	7800 3000 8150 3000
Wire Wire Line
	7900 3000 7900 2950
Connection ~ 7900 3000
NoConn ~ 7500 2600
NoConn ~ 7500 3000
Text Notes 7450 1900 0    60   ~ 12
Filter implementation\n
Text Notes 7450 2150 0    60   ~ 0
An INA can easily be implemented with a low pass filter.\nTime constant of the filter is 2*R*C.
Wire Notes Line
	7400 1700 10200 1700
Wire Notes Line
	10200 1700 10200 3250
Wire Notes Line
	10200 3250 7400 3250
Wire Notes Line
	7400 3250 7400 1700
$EndSCHEMATC
