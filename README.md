# VF_nC3_RS485_Uno
Modbus communication example for Arduino UNO and TOSHIBA's motor inveter VF_nC3 series.

## TOSVERT VF_nC3
https://www.toshiba.co.jp/sis/en/ind_sys/inverter/vf_nc3.htm
<br />
The driver's RS485 protocol have to be set as Modbus (The parameter F829 has to be set as 1).<br />
Build target : Arduino UNO + Ethernet Shield

## Connection
<img src="https://lh3.googleusercontent.com/2CqfQUv2V7SJgw6QMfisoGHrDVJKS-nRIndTvlMjwQZOZW9uLHSl_k1MqHWY59A4x61CP5aUtm4OBxSDqWm-64ZgpJtosBXyDMb0fnvgkazE4x82V-4PAJvBcQmvhDiw0HwBIDor_E7Udl2cQdQejn6dTYwURDuQTsmg6llbihvCOA44fna0t3_y8ovcz-dUUyUNqwCuFFlKANyXVQgW5d7uGKFNUu1kQ5_gdVVfrAqJKJTgDOhGHyngONJ-o5HsW3MEOmYCvC8p19s7NMRA8QC2DEkiWk9_iwrzkwbVS5NPogtUof9oH8aHswCO1ftnL15jbKBxXq0m4cueiMfcApDySUSY3bYkC52WNbGT-9_EToH_RN7eydC16wXBi7MBsIgL37I6Q97T4_9Wy69P3wEnhq1nx2ySjjV8r1PDqPGRxjrJOIKa7wz7mxEeGcCpvgByf7qSt4C59tBwcunRaflDyalZ14HKSEf8MPnX6_OmUFRgvfI2uuF3xNWOGyoKS4akI0SbJx3CsME80wE754kCCv5Q3ZZrBAn7Nh5gmRQqJ0M9EhE4kRchkkaU8PdKCuwgoGtHD8wQc7H_2MnBDXvKnvbklzkE061UxK0hEB5-RoiGqSCToYiEqNjiX2PI6xlFMAe7gCZkPbQo5WpW_FzhUmX18vlD=w2134-h1602-no" />
<br />
<p>
[Arduino - RS485] <br />
D0 - TX<br />
D1 - RX<br />
D3 - TX_ENABLE + RX_DISABLE<br />
</p>
