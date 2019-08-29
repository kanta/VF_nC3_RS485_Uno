# VF_nC3_RS485_Uno
Modbus communication example for Arduino UNO and TOSHIBA's motor inveter VF_nC3 series.

## TOSVERT VF_nC3
https://www.toshiba.co.jp/sis/en/ind_sys/inverter/vf_nc3.htm
<br />
The driver's RS485 protocol have to be set as Modbus (The parameter F829 has to be set as 1).<br />
Build target : Arduino UNO + Ethernet Shield

## Connection
<img src="https://lh3.googleusercontent.com/uK-0-8KyZZ0C0CqGzKTKLAjxpnq1RI8i-rgjxKg7tR0urr4jdIjlX1Z-5BSuRDs82Hz30F936A3uhoyRhXrqnZwWMHTY0ZBrJcNaWKUzyEnNseKuVWHVWGyhY_usxwMEReHzOprALVzOcTeBkwpnM3Ei2O96JS9-8re0Qb0fDA8qbLjgoIQFoBf1V1DG4THfOc3IbwFGSZzL2Nb6nlPejfPL7KeEmsz5mWl0ycrzP731qc_WmdPgFEIQ16kMteGMIjTEqPAlKqVI5fjcS5_B48fh5G3LtxAuGsQOcGcxN5R5X_iFSqK904HuzAINZ8bQBLwdaL8IR1jTmknnHO5cY7Y8liTvPU2iRw0eImGOJJ80jclWNhjSLwMcdUVdCVT-tubQgo9l8C5755q4hfv20X1PJ_iX4mgULjf9t5gjMt_G7h_lr5ANC8JW5qmFbsmSGCngOzCj3T7T9gtshhU1C8bVvQTQHDZa3C1e8dl3KhCfne67l3zOiIAxUwP3c9Gaso_Mp5r29KzT7Muy8wGBt0jHsFkv8QLkj6ybn8RlL7UYdLlmoe7vS8-93-TvMQscrttx_XLOwhep0TU2Fsz01ZN8byg255hEw7KT7EneEwFJNuANCsJm9db7rkgyjyds3qC4aHI7lSZqZzq1KN8jziAZ5x2YwmyF=w2095-h850-no" />
![IMG_6119](https://user-images.githubusercontent.com/1097902/63916107-aa138800-ca72-11e9-9f43-02f7806e2ff7.jpg)
<br />
<p>
[Arduino - RS485] <br />
D0 - RXO<br />
D1 - TXI<br />
D3 - TX_ENABLE + RX_DISABLE<br />
</p>
