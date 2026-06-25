/*
 * J2534_API.h - API for accessing SAE-J2534 compliant interfaces (macOS version)
 */

#ifndef J2534_API_H
#define J2534_API_H


//#define __J2534_API_DEBUG__

#include <string>
#include <vector>
#include "../J2534.h"
#include <cstring>
#include <iostream>
extern "C"
{
	#include <errno.h>
	#include <dlfcn.h>
}
#include "J2534misc.h"


class J2534_API
{

public:
	J2534_API();
	~J2534_API();
	static std::vector<J2534Library> getAvailableJ2534Libs();
	bool selectLibrary(std::string libPath);
	std::string library();
	J2534_API_version libraryAPIversion();

	long PassThruOpen(void* pName, unsigned long *pDeviceID);
	long PassThruClose(unsigned long DeviceID);
	long PassThruConnect(unsigned long ProtocolID, unsigned long Flags, unsigned long *pChannelID);
	long PassThruConnect(unsigned long DeviceID, unsigned long ProtocolID, unsigned long Flags, unsigned long BaudRate, unsigned long *pChannelID);
	long PassThruDisconnect(unsigned long ChannelID);
	long PassThruReadVersion(char *pFirmwareVersion, char *pDllVersion, char *pApiVersion);
	long PassThruReadVersion(unsigned long DeviceID, char *pFirmwareVersion, char * pDllVersion, char *pApiVersion);
	long PassThruGetLastError(char *pErrorDescription);
	long PassThruReadMsgs(unsigned long ChannelID, PASSTHRU_MSG *pMsg, unsigned long *pNumMsgs, unsigned long Timeout);
	long PassThruStartMsgFilter(unsigned long ChannelID, unsigned long FilterType, PASSTHRU_MSG *pMaskMsg, PASSTHRU_MSG *pPatternMsg, PASSTHRU_MSG *pFlowControlMsg, unsigned long *pMsgID);
	long PassThruStopMsgFilter(unsigned long ChannelID, unsigned long MsgID);
	long PassThruWriteMsgs(unsigned long ChannelID, PASSTHRU_MSG *pMsg, unsigned long *pNumMsgs, unsigned long Timeout);
	long PassThruStartPeriodicMsg(unsigned long ChannelID, PASSTHRU_MSG *pMsg, unsigned long *pMsgID, unsigned long TimeInterval);
	long PassThruStopPeriodicMsg(unsigned long ChannelID, unsigned long MsgID);
	long PassThruIoctl(unsigned long ChannelID, unsigned long IoctlID, void *pInput, void *pOutput);
	long PassThruSetProgrammingVoltage(unsigned long PinNumber, unsigned long Voltage);
	long PassThruSetProgrammingVoltage(unsigned long DeviceID, unsigned long PinNumber, unsigned long Voltage);

private:
	void *_J2534LIB;
	std::string _lib_path;
	J2534_API_version _api_version;

	J2534_PassThruOpen _PassThruOpen;
	J2534_PassThruClose _PassThruClose;
	J2534_PassThruConnect_0202 _PassThruConnect_0202;
	J2534_PassThruConnect_0404 _PassThruConnect_0404;
	J2534_PassThruDisconnect _PassThruDisconnect;
	J2534_PassThruReadVersion_0202 _PassThruReadVersion_0202;
	J2534_PassThruReadVersion_0404 _PassThruReadVersion_0404;
	J2534_PassThruGetLastError _PassThruGetLastError;
	J2534_PassThruReadMsgs _PassThruReadMsgs;
	J2534_PassThruStartMsgFilter _PassThruStartMsgFilter;
	J2534_PassThruStopMsgFilter _PassThruStopMsgFilter;
	J2534_PassThruWriteMsgs _PassThruWriteMsgs;
	J2534_PassThruStartPeriodicMsg _PassThruStartPeriodicMsg;
	J2534_PassThruStopPeriodicMsg _PassThruStopPeriodicMsg;
	J2534_PassThruIoctl _PassThruIoctl;
	J2534_PassThruSetProgrammingVoltage_0202 _PassThruSetProgrammingVoltage_0202;
	J2534_PassThruSetProgrammingVoltage_0404 _PassThruSetProgrammingVoltage_0404;

	void assignJ2534fcns();
	static std::vector<J2534Library> parseJ2534LibsXml(const std::string& libsDefXmlFile);

};


#endif
