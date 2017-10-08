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
Sheet 2 4
Title "Power stage"
Date "2017-10-08"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L NMA0515SC U7
U 1 1 59DA6974
P 4900 2900
F 0 "U7" H 4500 3350 50  0000 L CNN
F 1 "NMA0515SC" H 4500 3250 50  0000 L CNN
F 2 "Converters_DCDC_ACDC:DCDC-Conv_muRata_NMAxxxxSC" H 5500 2500 50  0001 C CIN
F 3 "" H 4900 2900 50  0001 C CNN
	1    4900 2900
	1    0    0    -1  
$EndComp
$Comp
L USB_A P5
U 1 1 59DA697D
P 3300 2900
F 0 "P5" H 3500 2700 50  0000 C CNN
F 1 "USB_A" H 3250 3100 50  0000 C CNN
F 2 "" V 3250 2800 50  0000 C CNN
F 3 "" V 3250 2800 50  0000 C CNN
	1    3300 2900
	0    -1   1    0   
$EndComp
Wire Wire Line
	4400 2700 3600 2700
Wire Wire Line
	3600 3000 4150 3000
Wire Wire Line
	4150 3000 4150 3100
Wire Wire Line
	4150 3100 4400 3100
NoConn ~ 3600 2800
NoConn ~ 3600 2900
NoConn ~ 3200 3200
Wire Wire Line
	6500 3100 5400 3100
Wire Wire Line
	5400 2700 6500 2700
Wire Wire Line
	6500 2900 5400 2900
Text HLabel 6500 2700 2    60   Output ~ 0
PowerStage_VDD
Text HLabel 6500 3100 2    60   Output ~ 0
PowerStage_VSS
Text HLabel 6500 2900 2    60   Output ~ 0
PowerStage_GND
Text Notes 700  1000 0    60   ~ 0
DC-DC converter part\nThis part takes power from an USB cord (+5 V) and produce the bipolar voltage to power the INAs through a DC-DC converter
$EndSCHEMATC
