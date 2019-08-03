/*
 * VF_nC3_RS485_Uno.ino
 *
 * Created: 1/29/2019 8:06:59 PM
 * Author: kanta
 */ 
// known issues : motor status doesn't work properly

#include <Arduino.h>
#include <Ethernet.h>
#include <SPI.h>
#include <OSCMessage.h>


byte mac[] = { 0x90, 0xA2, 0xDA, 0x10, 0x3D, 0x2F };
IPAddress myIp(10,110,20,42);
IPAddress destIp(10, 110, 20, 2);
IPAddress dns(192,168,233,120);
IPAddress gateway(10,110, 20, 254);
IPAddress subnet(255,255,255, 0);

unsigned int outPort = 20000;
unsigned int inPort = 20001;
EthernetUDP Udp;

#define TXDEN	3
#define ledPin	13
#define MODBUS_TIMEOUT	150
#define MODBUS_BAUDRATE    19200
uint8_t txBuf[64],txBufLength; // Modbus Tx Buffer
#define TARGET_ALL	0
uint8_t modbus_busy_target = 0;
#define RX_BUFFER_LENGTH	12
enum RxState {STAND_BY =0, SET_REGISTER, GET_DRIVER_STATE, GET_POS};
RxState rxState;
uint32_t lastModbusTxTime;
uint8_t modbusRxLength = 0;

#define DRIVER_CMD_BRAKE	7
#define DRIVER_CMD_DRIVE	10
#define DRIVER_CMD_FREE		11
#define DRIVER_CMD_EMERGENCY_STOP	12
#define DRIVER_CMD_RESET	13
#define DRIVER_CMD_FREQ_PRIORITY	14
#define DRIVER_CMD_CMD_PRIORITY	15
 

ISR	(USART_TX_vect) {
	digitalWrite(TXDEN, LOW);
}

// -------------------------------------------
// OSC
// -------------------------------------------

void sendOneData(char* address, int32_t data) {
	OSCMessage newMes(address);
	newMes.add(data);
	Udp.beginPacket(destIp, outPort);
	newMes.send(Udp);
	Udp.endPacket();
	newMes.empty();
}

void sendIdData(char* address, uint8_t id, int32_t data) {
	OSCMessage newMes(address);
	newMes.add(id);
	newMes.add(data);
	Udp.beginPacket(destIp, outPort);
	newMes.send(Udp);
	Udp.endPacket();
	newMes.empty();
}

void setDestIp(OSCMessage &msg ,int addrOffset) {
	destIp = Udp.remoteIP();
	OSCMessage newMes("/newDestIp");
	newMes.add(destIp[3]);
	Udp.beginPacket(destIp, outPort);
	newMes.send(Udp);
	Udp.endPacket();
	newMes.empty();
}

void sendStatus(uint16_t *data) {
	OSCMessage newMes("/status");
	for (uint8_t i=0; i<sizeof(data); i++)
	{
		newMes.add(data[i]);
	}
	Udp.beginPacket(destIp, outPort);
	newMes.send(Udp);
	Udp.endPacket();
	newMes.empty();
}

void motorRun(OSCMessage &msg ,int addrOffset) {
	if (modbus_busy_target > 0)
	{
		sendOneData("/modbusBusy", modbus_busy_target);
		return;
	}
	uint8_t target = msg.getInt(0);
	setCommand(target, DRIVER_CMD_DRIVE);
}

void motorStop(OSCMessage &msg ,int addrOffset) {
	if (modbus_busy_target > 0)
	{
		sendOneData("/modbusBusy", modbus_busy_target);
		return;
	}
	uint8_t target = msg.getInt(0);
	setCommand(target, 0);
}

void motorBrake(OSCMessage &msg ,int addrOffset) {
	if (modbus_busy_target > 0)
	{
		sendOneData("/modbusBusy", modbus_busy_target);
		return;
	}
	uint8_t target = msg.getInt(0);
	setCommand(target, DRIVER_CMD_BRAKE);
}

void motorFree(OSCMessage &msg ,int addrOffset) {
	if (modbus_busy_target > 0)
	{
		sendOneData("/modbusBusy", modbus_busy_target);
		return;
	}
	uint8_t target = msg.getInt(0);
	setCommand(target, DRIVER_CMD_FREE);
}

void driverReset(OSCMessage &msg ,int addrOffset) {
	if (modbus_busy_target > 0)
	{
		sendOneData("/modbusBusy", modbus_busy_target);
		return;
	}
	uint8_t target = msg.getInt(0);
	setCommand(target, DRIVER_CMD_RESET);
}


void getDriverState(OSCMessage &msg ,int addrOffset) {
	if (modbus_busy_target > 0)
	{
		sendOneData("/modbusBusy", modbus_busy_target);
		return;
	}
	uint8_t target = msg.getInt(0);
	if (target == TARGET_ALL)
	{
		sendOneData("/error_target_all", 0);
	} else {
		getDriverState(target);
	}
}

void setMotorSpeed(OSCMessage &msg ,int addrOffset) {
	if (modbus_busy_target > 0)
	{
		sendOneData("/modbusBusy", modbus_busy_target);
		return;
	}
	uint8_t target = msg.getInt(0);
	uint16_t speed = msg.getInt(1);
	setMotorSpeed(target, speed);
}

void getModbusBusy(OSCMessage &msg ,int addrOffset) {
	sendOneData("/modbusBusy", modbus_busy_target);
}

void getTxBuf(OSCMessage &msg ,int addrOffset) {

	OSCMessage newMes("/txBuf");
	for (uint8_t i=0; i<txBufLength; i++)
	{
		newMes.add(txBuf[i]);
	}
	Udp.beginPacket(destIp, outPort);
	newMes.send(Udp);
	Udp.endPacket();
	newMes.empty();
}
// -----------------------------------------------------------
// Modbus Tx
// -----------------------------------------------------------

void sendModbusCmd() {
	uint8_t i=0, j=0;
	uint16_t CRCresult = 0xFFFF;
	for(i=0; i<txBufLength; i++) {
		CRCresult ^= txBuf[i];
		for (j=0; j<8; j++) {
			if (CRCresult & 1) {
				CRCresult = (CRCresult >> 1) ^ 0xA001;
				} else {
				CRCresult = CRCresult >> 1;
			}
		}
	}
	txBuf[txBufLength++] = (CRCresult&0xFF);
	txBuf[txBufLength++] = (CRCresult>>8);

	//for (i=0; i<txBufLength; i++) {
		//Serial.print(txBuf[i]);
		//Serial.print(" ");
	//}
	//Serial.println(" <-- Modbus Tx");
	modbusFlush();
	digitalWrite(TXDEN, HIGH);
	Serial.write(txBuf, txBufLength);
	modbus_busy_target = txBuf[0];
	if ( txBuf[0] == TARGET_ALL )
	{
		rxState = STAND_BY;
	}
	lastModbusTxTime = millis();
}

void getDriverState(uint8_t target) { // read 5 words
	uint8_t data[6] = {1,0x03,0x18,0x75,0,5};
	data[0] = target;
	for (uint8_t i=0; i<6; i++)
	{
		txBuf[i] = data[i];
	}
	txBufLength = 6;
	rxState = GET_DRIVER_STATE;
	modbusRxLength = 15;
	sendModbusCmd();
}

void setMotorSpeed(uint8_t target, uint16_t speed) {
	uint8_t data[6] = {1,0x06,0xFA,0x01,0,0};
	data[0] = target;
	data[4] = speed>>8;
	data[5] = speed & 0xFF;
	for (uint8_t i=0; i<6; i++)
	{
		txBuf[i] = data[i];
	}
	txBufLength = 6;
	rxState = SET_REGISTER;
	modbusRxLength = 8;
	sendModbusCmd();
}

void setCommand(uint8_t target, uint8_t command) {
	const uint8_t header[] = {1,0x10,0xFA,0x00,0,1,2};
	for (uint8_t i=0; i<sizeof(header); i++)
	{
		txBuf[i] = header[i];
	}
	txBufLength = sizeof(header);
	txBuf[0] = target;
	uint16_t t = (1<<command) | (1<<DRIVER_CMD_FREQ_PRIORITY) | (1<<DRIVER_CMD_CMD_PRIORITY);
	txBuf[txBufLength++] = t>>8;
	txBuf[txBufLength++] = t & 0xFF;
	rxState = SET_REGISTER;
	modbusRxLength = 8;
	sendModbusCmd();
}

// -----------------------------------------------------------
// setup
// -----------------------------------------------------------
void setup()
{
	pinMode(TXDEN, OUTPUT);
	pinMode(ledPin, OUTPUT);
	// TOSVERT VF-nC3
	// 19200bps / 8bit / parity even /stop bit 1
	Serial.begin(MODBUS_BAUDRATE, SERIAL_8E1); // RS485
	UCSR0B |= (1<<TXCIE0);	// TX Complete Interrupt Enable 
	
	Ethernet.begin(mac, myIp, dns, gateway, subnet);
	Udp.begin(inPort);
}

void OSCMsgReceive() {
	OSCMessage msgIN;
	int size;
	if((size = Udp.parsePacket())>0){
		while(size--)
		msgIN.fill(Udp.read());
		if(!msgIN.hasError()){
			msgIN.route("/motorRun",motorRun);
			msgIN.route("/motorStop",motorStop);
			msgIN.route("/motorBrake",motorBrake);
			msgIN.route("/motorFree",motorFree);
			msgIN.route("/driverReset",driverReset);
			msgIN.route("/setMotorSpeed",setMotorSpeed);
			msgIN.route("/getDriverState",getDriverState);
			msgIN.route("/getModbusBusy",getModbusBusy);
			msgIN.route("/setDestIp",setDestIp);
			msgIN.route("/getTxBuf",getTxBuf);
		}
	}
}

void modbusFlush(){
	while(Serial.available() > 0) {
		char t = Serial.read();
	}
}

void modbusRx(uint8_t *rxData) {
	uint8_t rxTarget = rxData[0];
	uint16_t state[5];
	//int32_t posi;		
	
	switch(rxState) {
		case SET_REGISTER:
			modbus_busy_target = 0;
			break;
		case GET_DRIVER_STATE:
			for ( uint8_t i=0; i<5; i++)
			{
				state[i] = (rxData[3+i*2]<<8) | rxData[4+i*2];
			}
			sendStatus(*state);
			modbus_busy_target = 0;
			break;
		//case GET_POS:
			//posi = ((int32_t)rxData[3]<<24) | ((int32_t)rxData[4]<<16) | (rxData[5]<<8) | rxData[6];
			//sendIdData("/position", rxTarget, posi);
			//modbus_busy_target = 0;
			//break;
		default:
			break;
	}
}

// -----------------------------------------------------------
// loop
// -----------------------------------------------------------
void loop()
{
	OSCMsgReceive();
	byte t = Serial.available();
	if ( t >= modbusRxLength)
	{
		uint8_t buf[RX_BUFFER_LENGTH];
		for (uint8_t i=0; i<t; i++)
		{
			buf[i] = Serial.read();
		}
		modbusRx(buf);
	}

	if ( modbus_busy_target > 0)
	{
		if ( (uint32_t)(millis()-lastModbusTxTime)>=MODBUS_TIMEOUT || ( lastModbusTxTime > millis() ) )
		{
			sendOneData("/timeout", modbus_busy_target);
			modbus_busy_target = 0;
		}
	}

}
