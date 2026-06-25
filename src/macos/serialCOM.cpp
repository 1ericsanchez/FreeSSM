/*
 * serialCOM.cpp - Serial port configuration and communication (macOS version)
 */

#include "serialCOM.h"

#include <algorithm>
#include <cstring>
#include <unistd.h>
#include <QtSerialPort/QSerialPortInfo>


serialCOM::serialCOM()
{
	breakset = false;
}


serialCOM::~serialCOM()
{
	if (IsOpen())
		ClosePort();
}


std::vector<std::string> serialCOM::GetAvailablePorts()
{
	std::vector<std::string> portlist;
	const QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
	for (const QSerialPortInfo& info : ports)
	{
		const QString location = info.systemLocation();
		if (!location.isEmpty())
			portlist.push_back(location.toStdString());
	}
	std::sort(portlist.begin(), portlist.end());
	return portlist;
}


bool serialCOM::IsOpen()
{
	return port.isOpen();
}


std::string serialCOM::GetPortname()
{
	return currentportname;
}


bool serialCOM::GetPortSettings(double *baudrate, unsigned short *databits, char *parity, float *stopbits)
{
	if (!IsOpen())
		return false;

	if (baudrate)
		*baudrate = port.baudRate(QSerialPort::AllDirections);

	if (databits)
	{
		switch (port.dataBits())
		{
		case QSerialPort::Data5: *databits = 5; break;
		case QSerialPort::Data6: *databits = 6; break;
		case QSerialPort::Data7: *databits = 7; break;
		case QSerialPort::Data8: *databits = 8; break;
		default: return false;
		}
	}

	if (parity)
	{
		switch (port.parity())
		{
		case QSerialPort::NoParity: *parity = 'N'; break;
		case QSerialPort::EvenParity: *parity = 'E'; break;
		case QSerialPort::OddParity: *parity = 'O'; break;
		case QSerialPort::SpaceParity: *parity = 'S'; break;
		case QSerialPort::MarkParity: *parity = 'M'; break;
		default: return false;
		}
	}

	if (stopbits)
	{
		switch (port.stopBits())
		{
		case QSerialPort::OneStop: *stopbits = 1; break;
		case QSerialPort::OneAndHalfStop: *stopbits = 1.5; break;
		case QSerialPort::TwoStop: *stopbits = 2; break;
		default: return false;
		}
	}

	return true;
}


bool serialCOM::SetPortSettings(double baudrate, unsigned short databits, char parity, float stopbits)
{
	if (!IsOpen() || baudrate <= 0)
		return false;

	QSerialPort::DataBits qdatabits;
	switch (databits)
	{
	case 5: qdatabits = QSerialPort::Data5; break;
	case 6: qdatabits = QSerialPort::Data6; break;
	case 7: qdatabits = QSerialPort::Data7; break;
	case 8: qdatabits = QSerialPort::Data8; break;
	default: return false;
	}

	QSerialPort::Parity qparity;
	switch (parity)
	{
	case 'N': qparity = QSerialPort::NoParity; break;
	case 'E': qparity = QSerialPort::EvenParity; break;
	case 'O': qparity = QSerialPort::OddParity; break;
	case 'S': qparity = QSerialPort::SpaceParity; break;
	case 'M': qparity = QSerialPort::MarkParity; break;
	default: return false;
	}

	QSerialPort::StopBits qstopbits;
	if (stopbits == 1)
		qstopbits = QSerialPort::OneStop;
	else if (stopbits == 1.5)
		qstopbits = QSerialPort::OneAndHalfStop;
	else if (stopbits == 2)
		qstopbits = QSerialPort::TwoStop;
	else
		return false;

	return port.setBaudRate(static_cast<qint32>(baudrate), QSerialPort::AllDirections)
		&& port.setDataBits(qdatabits)
		&& port.setParity(qparity)
		&& port.setStopBits(qstopbits)
		&& port.setFlowControl(QSerialPort::NoFlowControl);
}


bool serialCOM::OpenPort(std::string portname)
{
	if (IsOpen())
		return false;

	port.setPortName(QString::fromStdString(portname));
	if (!port.open(QIODevice::ReadWrite))
		return false;

	currentportname = portname;
	breakset = false;
	return true;
}


bool serialCOM::ClosePort()
{
	if (!IsOpen())
		return false;

	port.close();
	currentportname.clear();
	breakset = false;
	return true;
}


bool serialCOM::Write(std::vector<char> data)
{
	if (data.empty())
		return true;
	return Write(data.data(), static_cast<unsigned int>(data.size()));
}


bool serialCOM::Write(char *data, unsigned int datalen)
{
	if (!IsOpen())
		return false;

	qint64 written = port.write(data, datalen);
	if (written != static_cast<qint64>(datalen))
		return false;

	return port.waitForBytesWritten(-1);
}


bool serialCOM::Read(unsigned int minbytes, unsigned int maxbytes, unsigned int timeout, std::vector<char> *data)
{
	if (!data)
		return false;

	data->clear();
	if (!maxbytes)
		return true;

	std::vector<char> buffer(maxbytes);
	unsigned int nread = 0;
	if (!Read(minbytes, maxbytes, timeout, buffer.data(), &nread))
		return false;

	data->assign(buffer.begin(), buffer.begin() + nread);
	return true;
}


bool serialCOM::Read(unsigned int minbytes, unsigned int maxbytes, unsigned int timeout, char *data, unsigned int *nrofbytesread)
{
	if (!IsOpen() || !data || !nrofbytesread || minbytes > maxbytes)
		return false;

	*nrofbytesread = 0;
	while (*nrofbytesread < maxbytes)
	{
		const qint64 readcount = port.read(data + *nrofbytesread, maxbytes - *nrofbytesread);
		if (readcount > 0)
		{
			*nrofbytesread += static_cast<unsigned int>(readcount);
			if (*nrofbytesread >= minbytes)
			{
				if (!timeout || !port.waitForReadyRead(0))
					break;
			}
			continue;
		}

		if (*nrofbytesread >= minbytes)
			break;

		if (!port.waitForReadyRead(static_cast<int>(timeout)))
			break;
	}

	return *nrofbytesread >= minbytes;
}


bool serialCOM::ClearSendBuffer()
{
	return IsOpen() && port.clear(QSerialPort::Output);
}


bool serialCOM::ClearReceiveBuffer()
{
	return IsOpen() && port.clear(QSerialPort::Input);
}


bool serialCOM::SendBreak(unsigned int duration_ms)
{
	if (!SetBreak())
		return false;
	usleep(1000 * duration_ms);
	return ClearBreak();
}


bool serialCOM::SetBreak()
{
	if (!IsOpen())
		return false;
	breakset = port.setBreakEnabled(true);
	return breakset;
}


bool serialCOM::ClearBreak()
{
	if (!IsOpen())
		return false;
	if (!port.setBreakEnabled(false))
		return false;
	breakset = false;
	return true;
}


bool serialCOM::BreakIsSet()
{
	return breakset;
}


bool serialCOM::GetNrOfBytesAvailable(unsigned int *nbytes)
{
	if (!IsOpen() || !nbytes)
		return false;
	*nbytes = static_cast<unsigned int>(port.bytesAvailable());
	return true;
}


bool serialCOM::SetControlLines(bool DTR, bool RTS)
{
	return IsOpen() && port.setDataTerminalReady(DTR) && port.setRequestToSend(RTS);
}
