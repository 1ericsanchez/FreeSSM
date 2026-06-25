/*
 * serialCOM.h - Serial port configuration and communication (macOS version)
 */

#ifndef SERIALCOM_H
#define SERIALCOM_H


#include <string>
#include <vector>
#include <QtSerialPort/QSerialPort>


class serialCOM
{

public:
	serialCOM();
	~serialCOM();
	static std::vector<std::string> GetAvailablePorts();
	bool IsOpen();
	std::string GetPortname();
	bool GetPortSettings(double *baudrate, unsigned short *databits = NULL, char *parity = NULL, float *stopbits = NULL);
	bool SetPortSettings(double baudrate, unsigned short databits, char parity, float stopbits);
	bool OpenPort(std::string portname);
	bool ClosePort();
	bool Write(std::vector<char> data);
	bool Write(char *data, unsigned int datalen);
	bool Read(unsigned int minbytes, unsigned int maxbytes, unsigned int timeout, std::vector<char> *data);
	bool Read(unsigned int minbytes, unsigned int maxbytes, unsigned int timeout, char *data, unsigned int *nrofbytesread);
	bool ClearSendBuffer();
	bool ClearReceiveBuffer();
	bool SendBreak(unsigned int duration_ms);
	bool SetBreak();
	bool ClearBreak();
	bool BreakIsSet();
	bool GetNrOfBytesAvailable(unsigned int *nbytes);
	bool SetControlLines(bool DTR, bool RTS);

private:
	QSerialPort port;
	bool breakset;
	std::string currentportname;

};


#endif
