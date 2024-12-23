EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	2100 2450 2150 2450
$Comp
L Device:R R5
U 1 1 5F117059
P 3900 1750
F 0 "R5" H 3970 1796 50  0000 L CNN
F 1 "390k" H 3970 1705 50  0000 L CNN
F 2 "" V 3830 1750 50  0001 C CNN
F 3 "~" H 3900 1750 50  0001 C CNN
	1    3900 1750
	1    0    0    -1  
$EndComp
$Comp
L Device:R_POT RV2
U 1 1 5F13608A
P 3900 2150
F 0 "RV2" H 3831 2196 50  0000 R CNN
F 1 "500k" H 3831 2105 50  0000 R CNN
F 2 "" H 3900 2150 50  0001 C CNN
F 3 "~" H 3900 2150 50  0001 C CNN
	1    3900 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 2450 2150 3200
Wire Wire Line
	2100 2150 2300 2150
Wire Wire Line
	2300 2150 2300 2350
Wire Wire Line
	2850 2350 2850 2450
Wire Wire Line
	2300 2350 2850 2350
$Comp
L Device:R R3
U 1 1 5F113502
P 2500 1450
F 0 "R3" V 2293 1450 50  0000 C CNN
F 1 "120k" V 2384 1450 50  0000 C CNN
F 2 "" V 2430 1450 50  0001 C CNN
F 3 "~" H 2500 1450 50  0001 C CNN
	1    2500 1450
	0    1    1    0   
$EndComp
$Comp
L Device:R_POT RV1
U 1 1 5F0C667E
P 3450 1650
F 0 "RV1" H 3380 1696 50  0000 R CNN
F 1 "10k" H 3380 1605 50  0000 R CNN
F 2 "" H 3450 1650 50  0001 C CNN
F 3 "~" H 3450 1650 50  0001 C CNN
	1    3450 1650
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3900 1900 3900 1950
$Comp
L Device:CP1 C3
U 1 1 5F17A0CF
P 4350 2150
F 0 "C3" H 4465 2196 50  0000 L CNN
F 1 "10uF" H 4465 2105 50  0000 L CNN
F 2 "" H 4350 2150 50  0001 C CNN
F 3 "~" H 4350 2150 50  0001 C CNN
	1    4350 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 2000 4350 1950
Wire Wire Line
	4350 1950 3900 1950
Connection ~ 3900 1950
Wire Wire Line
	3900 1950 3900 2000
Wire Wire Line
	4050 2150 4050 2400
Wire Wire Line
	4050 2400 4350 2400
Wire Wire Line
	4350 2400 4350 2300
$Comp
L power:+5V #PWR?
U 1 1 5F186285
P 3900 1550
F 0 "#PWR?" H 3900 1400 50  0001 C CNN
F 1 "+5V" H 3915 1723 50  0000 C CNN
F 2 "" H 3900 1550 50  0001 C CNN
F 3 "" H 3900 1550 50  0001 C CNN
	1    3900 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 1550 3900 1600
$Comp
L Device:R R4
U 1 1 5F116C08
P 3050 1450
F 0 "R4" V 3257 1450 50  0000 C CNN
F 1 "10k" V 3166 1450 50  0000 C CNN
F 2 "" V 2980 1450 50  0001 C CNN
F 3 "~" H 3050 1450 50  0001 C CNN
	1    3050 1450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3200 1450 3450 1450
Wire Wire Line
	3450 1450 3450 1500
Wire Wire Line
	2900 1450 2800 1450
$Comp
L power:GND #PWR?
U 1 1 5F172208
P 2850 5250
F 0 "#PWR?" H 2850 5000 50  0001 C CNN
F 1 "GND" H 2855 5077 50  0000 C CNN
F 2 "" H 2850 5250 50  0001 C CNN
F 3 "" H 2850 5250 50  0001 C CNN
	1    2850 5250
	1    0    0    -1  
$EndComp
NoConn ~ 3900 2300
Wire Wire Line
	4350 2500 4350 2400
Connection ~ 4350 2400
Wire Wire Line
	2350 1450 2200 1450
Wire Wire Line
	2200 1450 2200 1650
Wire Wire Line
	2200 1650 2100 1650
Wire Wire Line
	3300 1650 3200 1650
Wire Wire Line
	3200 1650 3200 1950
Connection ~ 3200 1950
Wire Wire Line
	3200 1950 3900 1950
NoConn ~ 3450 1800
Text GLabel 2950 950  2    50   Input ~ 0
VoltageSensor
Text GLabel 2000 1200 0    50   Input ~ 0
1VAC
Text GLabel 2000 800  0    50   Input ~ 0
1VAC
$Comp
L Device:R R1
U 1 1 5F0C6087
P 5600 2950
F 0 "R1" H 5670 2996 50  0000 L CNN
F 1 "330R" H 5670 2905 50  0000 L CNN
F 2 "" V 5530 2950 50  0001 C CNN
F 3 "~" H 5600 2950 50  0001 C CNN
	1    5600 2950
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:LM317_TO-220 U1
U 1 1 5F0C7D70
P 5150 2750
F 0 "U1" H 5150 2992 50  0000 C CNN
F 1 "LM317_TO-220" H 5150 2901 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 5150 3000 50  0001 C CIN
F 3 "http://www.ti.com/lit/ds/symlink/lm317.pdf" H 5150 2750 50  0001 C CNN
	1    5150 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 3200 2850 3200
Wire Wire Line
	2850 3200 2850 3050
$Comp
L Device:R R2
U 1 1 5F0F6174
P 5150 3300
F 0 "R2" H 5220 3346 50  0000 L CNN
F 1 "1k" H 5220 3255 50  0000 L CNN
F 2 "" V 5080 3300 50  0001 C CNN
F 3 "~" H 5150 3300 50  0001 C CNN
	1    5150 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 3050 5150 3100
Wire Wire Line
	5150 3450 5150 3500
Wire Wire Line
	5600 3100 5150 3100
Connection ~ 5150 3100
Wire Wire Line
	5150 3100 5150 3150
Wire Wire Line
	3600 3500 2500 3500
$Comp
L Device:C C2
U 1 1 5F107921
P 4200 3100
F 0 "C2" H 4315 3146 50  0000 L CNN
F 1 "100nF" H 4315 3055 50  0000 L CNN
F 2 "" H 4238 2950 50  0001 C CNN
F 3 "~" H 4200 3100 50  0001 C CNN
	1    4200 3100
	1    0    0    -1  
$EndComp
Connection ~ 3600 3500
Wire Wire Line
	4200 3250 4200 3500
Wire Wire Line
	3600 3700 3600 3500
Wire Wire Line
	3600 3150 3600 3500
Wire Wire Line
	3600 3500 4200 3500
$Comp
L Device:CP1 C1
U 1 1 5F0C7078
P 3600 3000
F 0 "C1" H 3715 3046 50  0000 L CNN
F 1 "220uF" H 3715 2955 50  0000 L CNN
F 2 "" H 3600 3000 50  0001 C CNN
F 3 "~" H 3600 3000 50  0001 C CNN
	1    3600 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 2750 3600 2850
Wire Wire Line
	4200 2950 4200 2750
Connection ~ 4200 2750
$Comp
L power:GND #PWR?
U 1 1 5F157D21
P 4350 2500
F 0 "#PWR?" H 4350 2250 50  0001 C CNN
F 1 "GND" H 4355 2327 50  0000 C CNN
F 2 "" H 4350 2500 50  0001 C CNN
F 3 "" H 4350 2500 50  0001 C CNN
	1    4350 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 2750 4200 2750
Connection ~ 5600 2750
Wire Wire Line
	6000 2750 5600 2750
Wire Wire Line
	6000 2700 6000 2750
$Comp
L power:+5V #PWR?
U 1 1 5F188028
P 6000 2700
F 0 "#PWR?" H 6000 2550 50  0001 C CNN
F 1 "+5V" H 6015 2873 50  0000 C CNN
F 2 "" H 6000 2700 50  0001 C CNN
F 3 "" H 6000 2700 50  0001 C CNN
	1    6000 2700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F153502
P 3600 3700
F 0 "#PWR?" H 3600 3450 50  0001 C CNN
F 1 "GND" H 3605 3527 50  0000 C CNN
F 2 "" H 3600 3700 50  0001 C CNN
F 3 "" H 3600 3700 50  0001 C CNN
	1    3600 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 2750 5450 2750
Wire Wire Line
	5600 2800 5600 2750
Wire Wire Line
	2500 2750 2500 3500
Wire Wire Line
	2500 2750 2550 2750
Wire Wire Line
	4200 3500 5150 3500
Connection ~ 4200 3500
Wire Wire Line
	4200 2750 4850 2750
Text GLabel 4650 1750 2    50   Input ~ 0
2.5VDC
Wire Wire Line
	4700 1950 4350 1950
Connection ~ 4350 1950
Wire Wire Line
	2950 950  2800 950 
Wire Wire Line
	2800 950  2800 1450
Connection ~ 2800 1450
Wire Wire Line
	2800 1450 2650 1450
Connection ~ 2800 950 
$Comp
L Device:R R7
U 1 1 5F27636F
P 2400 4500
F 0 "R7" H 2470 4546 50  0000 L CNN
F 1 "390k" H 2470 4455 50  0000 L CNN
F 2 "" V 2330 4500 50  0001 C CNN
F 3 "~" H 2400 4500 50  0001 C CNN
	1    2400 4500
	1    0    0    -1  
$EndComp
$Comp
L Device:R_POT RV3
U 1 1 5F276375
P 2400 4900
F 0 "RV3" H 2331 4946 50  0000 R CNN
F 1 "500k" H 2331 4855 50  0000 R CNN
F 2 "" H 2400 4900 50  0001 C CNN
F 3 "~" H 2400 4900 50  0001 C CNN
	1    2400 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 4650 2400 4700
$Comp
L Device:CP1 C4
U 1 1 5F27637C
P 2850 4900
F 0 "C4" H 2965 4946 50  0000 L CNN
F 1 "10uF" H 2965 4855 50  0000 L CNN
F 2 "" H 2850 4900 50  0001 C CNN
F 3 "~" H 2850 4900 50  0001 C CNN
	1    2850 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 4750 2850 4700
Wire Wire Line
	2850 4700 2400 4700
Connection ~ 2400 4700
Wire Wire Line
	2400 4700 2400 4750
Wire Wire Line
	2550 4900 2550 5150
Wire Wire Line
	2550 5150 2850 5150
Wire Wire Line
	2850 5150 2850 5050
$Comp
L power:+5V #PWR?
U 1 1 5F276389
P 2400 4300
F 0 "#PWR?" H 2400 4150 50  0001 C CNN
F 1 "+5V" H 2415 4473 50  0000 C CNN
F 2 "" H 2400 4300 50  0001 C CNN
F 3 "" H 2400 4300 50  0001 C CNN
	1    2400 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 4300 2400 4350
NoConn ~ 2400 5050
Wire Wire Line
	2850 5250 2850 5150
Connection ~ 2850 5150
Text GLabel 3150 4450 2    50   Input ~ 0
2.5VDC
Wire Wire Line
	3200 4700 2850 4700
Connection ~ 2850 4700
$Comp
L Connector:AudioJack3 J4
U 1 1 5F27DB22
P 1250 4600
F 0 "J4" H 1232 4925 50  0000 C CNN
F 1 "Current Sensor 30A/1V" H 1232 4834 50  0000 C CNN
F 2 "" H 1250 4600 50  0001 C CNN
F 3 "~" H 1250 4600 50  0001 C CNN
	1    1250 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 4700 2400 4700
Wire Wire Line
	1700 4500 1450 4500
Connection ~ 3600 2750
Wire Wire Line
	3150 2750 3600 2750
$Comp
L Device:D_Bridge_+-AA D1
U 1 1 5F0C8A8A
P 2850 2750
F 0 "D1" H 2500 2950 50  0000 L CNN
F 1 "DB107" H 2350 2850 50  0000 L CNN
F 2 "" H 2850 2750 50  0001 C CNN
F 3 "~" H 2850 2750 50  0001 C CNN
	1    2850 2750
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J2
U 1 1 5F0CBD7F
P 1800 950
F 0 "J2" H 2000 950 50  0000 C CNN
F 1 "1VAC_Test" H 2150 850 50  0000 C CNN
F 2 "" H 1800 950 50  0001 C CNN
F 3 "~" H 1800 950 50  0001 C CNN
	1    1800 950 
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J5
U 1 1 5F0CC589
P 3400 4700
F 0 "J5" H 3480 4742 50  0000 L CNN
F 1 "2.5VDC_Test" H 3480 4651 50  0000 L CNN
F 2 "" H 3400 4700 50  0001 C CNN
F 3 "~" H 3400 4700 50  0001 C CNN
	1    3400 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 1950 2300 1950
Wire Wire Line
	2000 950  2100 950 
Wire Wire Line
	2000 1050 2100 1050
Wire Wire Line
	2300 1050 2300 1950
Connection ~ 2300 1950
Wire Wire Line
	2300 1950 3200 1950
Wire Wire Line
	2000 800  2100 800 
Wire Wire Line
	2100 800  2100 950 
Connection ~ 2100 950 
Wire Wire Line
	2100 950  2800 950 
Wire Wire Line
	2100 1050 2100 1200
Wire Wire Line
	2100 1200 2000 1200
Connection ~ 2100 1050
Wire Wire Line
	2100 1050 2300 1050
$Comp
L Connector_Generic:Conn_01x01 J3
U 1 1 5F0F806C
P 4900 1950
F 0 "J3" H 4980 1992 50  0000 L CNN
F 1 "2.5VDC_Test" H 4980 1901 50  0000 L CNN
F 2 "" H 4900 1950 50  0001 C CNN
F 3 "~" H 4900 1950 50  0001 C CNN
	1    4900 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 1750 4350 1750
Wire Wire Line
	4350 1750 4350 1950
Wire Wire Line
	1850 3900 1700 3900
Wire Wire Line
	1700 3900 1700 4500
Wire Wire Line
	3150 4450 2850 4450
Wire Wire Line
	2850 4450 2850 4700
Text GLabel 1850 3900 2    50   Input ~ 0
CurrentSensor
Text GLabel 1150 1850 0    50   Input ~ 0
F
Text GLabel 1150 2250 0    50   Input ~ 0
N
Wire Wire Line
	1150 1850 1300 1850
Wire Wire Line
	1150 2250 1300 2250
Wire Wire Line
	5550 4550 5550 4850
Wire Wire Line
	5400 4550 5550 4550
Wire Wire Line
	4800 4250 5500 4250
Connection ~ 5500 4250
Wire Wire Line
	5700 4250 5500 4250
Wire Wire Line
	5500 4450 5400 4450
Wire Wire Line
	5500 4250 5500 4450
Wire Wire Line
	4800 4450 4800 4250
Wire Wire Line
	4900 4450 4800 4450
Wire Wire Line
	4800 4850 4700 4850
Connection ~ 4800 4850
Wire Wire Line
	4800 4650 4800 4850
Wire Wire Line
	4900 4650 4800 4650
Wire Wire Line
	5450 4850 4800 4850
Wire Wire Line
	5450 4650 5450 4850
Wire Wire Line
	5400 4650 5450 4650
Text GLabel 5700 4250 2    50   Input ~ 0
VoltageSensor
Text GLabel 4700 4850 0    50   Input ~ 0
CurrentSensor
Wire Wire Line
	4450 4550 4450 4350
Wire Wire Line
	4900 4550 4450 4550
$Comp
L power:+5V #PWR?
U 1 1 5F1433D7
P 4450 4350
F 0 "#PWR?" H 4450 4200 50  0001 C CNN
F 1 "+5V" H 4465 4523 50  0000 C CNN
F 2 "" H 4450 4350 50  0001 C CNN
F 3 "" H 4450 4350 50  0001 C CNN
	1    4450 4350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F1407C7
P 5550 4850
F 0 "#PWR?" H 5550 4600 50  0001 C CNN
F 1 "GND" H 5555 4677 50  0000 C CNN
F 2 "" H 5550 4850 50  0001 C CNN
F 3 "" H 5550 4850 50  0001 C CNN
	1    5550 4850
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x03_Odd_Even J1
U 1 1 5F0C9C23
P 5100 4550
F 0 "J1" H 5700 4600 50  0000 C CNN
F 1 "Connector_Arduino" H 6000 4500 50  0000 C CNN
F 2 "" H 5100 4550 50  0001 C CNN
F 3 "~" H 5100 4550 50  0001 C CNN
	1    5100 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	8150 1500 8150 1800
Wire Wire Line
	8000 1500 8150 1500
Wire Wire Line
	7400 1200 8100 1200
Connection ~ 8100 1200
Wire Wire Line
	8300 1200 8100 1200
Wire Wire Line
	8100 1400 8000 1400
Wire Wire Line
	8100 1200 8100 1400
Wire Wire Line
	7400 1400 7400 1200
Wire Wire Line
	7500 1400 7400 1400
Wire Wire Line
	7400 1800 7300 1800
Connection ~ 7400 1800
Wire Wire Line
	7400 1600 7400 1800
Wire Wire Line
	7500 1600 7400 1600
Wire Wire Line
	8050 1800 7400 1800
Wire Wire Line
	8050 1600 8050 1800
Wire Wire Line
	8000 1600 8050 1600
Text GLabel 8300 1200 2    50   Input ~ 0
A0
Text GLabel 7300 1800 0    50   Input ~ 0
A1
Wire Wire Line
	7050 1500 7050 1300
Wire Wire Line
	7500 1500 7050 1500
$Comp
L power:+5V #PWR?
U 1 1 5F127BC8
P 7050 1300
F 0 "#PWR?" H 7050 1150 50  0001 C CNN
F 1 "+5V" H 7065 1473 50  0000 C CNN
F 2 "" H 7050 1300 50  0001 C CNN
F 3 "" H 7050 1300 50  0001 C CNN
	1    7050 1300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F127BCE
P 8150 1800
F 0 "#PWR?" H 8150 1550 50  0001 C CNN
F 1 "GND" H 8155 1627 50  0000 C CNN
F 2 "" H 8150 1800 50  0001 C CNN
F 3 "" H 8150 1800 50  0001 C CNN
	1    8150 1800
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x03_Odd_Even J1
U 1 1 5F127BD4
P 7700 1500
F 0 "J1" H 7450 1950 50  0000 C CNN
F 1 "Connector_Sensors" H 7750 1850 50  0000 C CNN
F 2 "" H 7700 1500 50  0001 C CNN
F 3 "~" H 7700 1500 50  0001 C CNN
	1    7700 1500
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x04_Odd_Even J2
U 1 1 5F12F58B
P 9100 1500
F 0 "J2" H 8850 1950 50  0000 C CNN
F 1 "Connector_Relay" H 9100 1850 50  0000 C CNN
F 2 "" H 9100 1500 50  0001 C CNN
F 3 "~" H 9100 1500 50  0001 C CNN
	1    9100 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8900 1400 8800 1400
Wire Wire Line
	8800 1400 8800 1200
Wire Wire Line
	8800 1200 9500 1200
Wire Wire Line
	9500 1200 9500 1400
Wire Wire Line
	9500 1400 9400 1400
Wire Wire Line
	9400 1700 9500 1700
Wire Wire Line
	9500 1700 9500 1850
Wire Wire Line
	9500 1850 8800 1850
Wire Wire Line
	8800 1850 8800 1700
Wire Wire Line
	8800 1700 8900 1700
Wire Wire Line
	9400 1500 9450 1500
Wire Wire Line
	9450 1500 9450 1600
Wire Wire Line
	9450 1600 9400 1600
Wire Wire Line
	8900 1500 8850 1500
Wire Wire Line
	8850 1500 8850 1600
Wire Wire Line
	8850 1600 8900 1600
$Comp
L power:GND #PWR?
U 1 1 5F140D31
P 9800 1250
F 0 "#PWR?" H 9800 1000 50  0001 C CNN
F 1 "GND" H 9805 1077 50  0000 C CNN
F 2 "" H 9800 1250 50  0001 C CNN
F 3 "" H 9800 1250 50  0001 C CNN
	1    9800 1250
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5F1459DF
P 8600 1800
F 0 "#PWR?" H 8600 1650 50  0001 C CNN
F 1 "+5V" H 8615 1973 50  0000 C CNN
F 2 "" H 8600 1800 50  0001 C CNN
F 3 "" H 8600 1800 50  0001 C CNN
	1    8600 1800
	1    0    0    -1  
$EndComp
Text GLabel 9600 1600 2    50   Input ~ 0
PIN_12
Text GLabel 8700 1500 0    50   Input ~ 0
PIN_11
Wire Wire Line
	9500 1200 9800 1200
Wire Wire Line
	9800 1200 9800 1250
Connection ~ 9500 1200
Wire Wire Line
	8600 1800 8600 1850
Wire Wire Line
	8600 1850 8800 1850
Connection ~ 8800 1850
Wire Wire Line
	8700 1500 8850 1500
Connection ~ 8850 1500
Wire Wire Line
	9450 1600 9600 1600
Connection ~ 9450 1600
$Comp
L Device:Transformer_1P_2S T1
U 1 1 5F0C3B06
P 1700 2050
F 0 "T1" H 1700 2631 50  0000 C CNN
F 1 "230v-6v" H 1700 2540 50  0000 C CNN
F 2 "" H 1700 2050 50  0001 C CNN
F 3 "~" H 1700 2050 50  0001 C CNN
	1    1700 2050
	1    0    0    -1  
$EndComp
Text GLabel 1000 6500 0    50   Input ~ 0
F
Text GLabel 1000 6900 0    50   Input ~ 0
N
Wire Wire Line
	1000 6500 1150 6500
Wire Wire Line
	1000 6900 1150 6900
$Comp
L Device:Transformer_1P_2S T1
U 1 1 5F6B4579
P 1550 6700
F 0 "T1" H 1550 7281 50  0000 C CNN
F 1 "230v-12v" H 1550 7190 50  0000 C CNN
F 2 "" H 1550 6700 50  0001 C CNN
F 3 "~" H 1550 6700 50  0001 C CNN
	1    1550 6700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 7100 2750 6950
Wire Wire Line
	3500 7400 2400 7400
$Comp
L Device:C C2
U 1 1 5F6C07C6
P 4100 7000
F 0 "C2" H 4215 7046 50  0000 L CNN
F 1 "100nF" H 4215 6955 50  0000 L CNN
F 2 "" H 4138 6850 50  0001 C CNN
F 3 "~" H 4100 7000 50  0001 C CNN
	1    4100 7000
	1    0    0    -1  
$EndComp
Connection ~ 3500 7400
Wire Wire Line
	4100 7150 4100 7400
Wire Wire Line
	3500 7050 3500 7400
Wire Wire Line
	3500 7400 3800 7400
$Comp
L Device:CP1 C1
U 1 1 5F6C07D1
P 3500 6900
F 0 "C1" H 3615 6946 50  0000 L CNN
F 1 "220uF" H 3615 6855 50  0000 L CNN
F 2 "" H 3500 6900 50  0001 C CNN
F 3 "~" H 3500 6900 50  0001 C CNN
	1    3500 6900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 6650 3500 6750
Wire Wire Line
	4100 6850 4100 6650
Wire Wire Line
	2400 6650 2400 7400
Wire Wire Line
	2400 6650 2450 6650
Connection ~ 3500 6650
Wire Wire Line
	3050 6650 3500 6650
$Comp
L Device:D_Bridge_+-AA D1
U 1 1 5F6C07ED
P 2750 6650
F 0 "D1" H 2400 6850 50  0000 L CNN
F 1 "DB107" H 2250 6750 50  0000 L CNN
F 2 "" H 2750 6650 50  0001 C CNN
F 3 "~" H 2750 6650 50  0001 C CNN
	1    2750 6650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 7100 2050 7100
Wire Wire Line
	1950 6300 2250 6300
Wire Wire Line
	2750 6300 2750 6350
Wire Wire Line
	1950 6800 2250 6800
Wire Wire Line
	2250 6800 2250 6300
Connection ~ 2250 6300
Wire Wire Line
	2250 6300 2750 6300
Wire Wire Line
	1950 6600 2050 6600
Wire Wire Line
	2050 6600 2050 7100
Connection ~ 2050 7100
Wire Wire Line
	2050 7100 2750 7100
$Comp
L power:GND #PWR?
U 1 1 5F74273A
P 3800 7500
F 0 "#PWR?" H 3800 7250 50  0001 C CNN
F 1 "GND" H 3805 7327 50  0000 C CNN
F 2 "" H 3800 7500 50  0001 C CNN
F 3 "" H 3800 7500 50  0001 C CNN
	1    3800 7500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 7500 3800 7400
Connection ~ 3800 7400
Wire Wire Line
	3800 7400 4100 7400
Wire Wire Line
	3500 6650 4100 6650
$Comp
L Device:R R2
U 1 1 5F6C07AC
P 5200 6850
F 0 "R2" H 5270 6896 50  0000 L CNN
F 1 "330R" H 5270 6805 50  0000 L CNN
F 2 "" V 5130 6850 50  0001 C CNN
F 3 "~" H 5200 6850 50  0001 C CNN
	1    5200 6850
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:LM317_TO-220 U1
U 1 1 5F6C07B2
P 4750 6650
F 0 "U1" H 4750 6892 50  0000 C CNN
F 1 "LM317_TO-220" H 4750 6801 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 4750 6900 50  0001 C CIN
F 3 "http://www.ti.com/lit/ds/symlink/lm317.pdf" H 4750 6650 50  0001 C CNN
	1    4750 6650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 7000 4750 7000
Connection ~ 5200 6650
Wire Wire Line
	5600 6650 5200 6650
Wire Wire Line
	5600 6600 5600 6650
$Comp
L power:+5V #PWR?
U 1 1 5F6C07DE
P 5600 6600
F 0 "#PWR?" H 5600 6450 50  0001 C CNN
F 1 "+5V" H 5615 6773 50  0000 C CNN
F 2 "" H 5600 6600 50  0001 C CNN
F 3 "" H 5600 6600 50  0001 C CNN
	1    5600 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 6650 5050 6650
Wire Wire Line
	5200 6700 5200 6650
Wire Wire Line
	4750 6950 4750 7000
Wire Wire Line
	4750 7000 4750 7050
Connection ~ 4750 7000
Wire Wire Line
	4750 7350 4750 7400
$Comp
L Device:R R1
U 1 1 5F6C07BA
P 4750 7200
F 0 "R1" H 4820 7246 50  0000 L CNN
F 1 "1k" H 4820 7155 50  0000 L CNN
F 2 "" V 4680 7200 50  0001 C CNN
F 3 "~" H 4750 7200 50  0001 C CNN
	1    4750 7200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 6650 4100 6650
Connection ~ 4100 6650
Wire Wire Line
	4750 7400 4100 7400
Connection ~ 4100 7400
$EndSCHEMATC
