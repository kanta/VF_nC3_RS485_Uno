# VF_nC3_RS485_Uno
Modbus communication example for Arduino UNO and TOSHIBA's motor inveter VF_nC3 series.

## TOSVERT VF_nC3
https://www.toshiba.co.jp/sis/en/ind_sys/inverter/vf_nc3.htm
<br />
The driver's RS485 protocol have to be set as Modbus (The parameter F829 has to be set as 1).<br />
Build target : Arduino UNO + Ethernet Shield

## Connection
<img src="https://horiokanta.com/control-panel/wp-content/uploads/2021/03/tosvert_rs485_schmetic.png" />

![IMG_6119](https://user-images.githubusercontent.com/1097902/63916107-aa138800-ca72-11e9-9f43-02f7806e2ff7.jpg)

<br />
<p>
[Arduino - RS485] <br />
D0 - RXO<br />
D1 - TXI<br />
D3 - TX_ENABLE + RX_DISABLE<br />
</p>
