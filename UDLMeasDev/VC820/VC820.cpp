/*
 * UDL - Universal Data Logger
 * Copyright (C) 2010  Marco Hartung
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * www.helektronik.de - udl@helektronik.de
 */

/*! \file VC820.cpp
 *  \brief Interface for Voltcraft VC820 Multimeters
 *
 */

#include "VC820.h"

#include <string>
#include "windows.h"
#include "boost/bind.hpp"

#include <iostream>

static HANDLE        m_hCom;
static SMeasValue_t  m_ActMeasVall;


static std::map< uint32_t, VC820*>    m_Devices;
static uint32_t                       m_cDevices;  //!< Number of ever created Devices

UDLMD_API UDLMD_STATUS Create( UDLMD_HANDLE* phMeasDev){

	return VC820::GetInstance()->Create( phMeasDev );
}

UDLMD_API UDLMD_STATUS Delete( UDLMD_HANDLE hMeasDev ){

	return VC820::GetInstance()->Delete( hMeasDev );
}

UDLMD_API UDLMD_STATUS Setup( UDLMD_HANDLE hMeasDev, uint32_t cArgs, char *rgpszArg[]){
	VC820* pDev = VC820::GetInstance();
	if( pDev ){
		return pDev->Setup( hMeasDev, cArgs, rgpszArg );
	}
	return MD_INVALIDE_HANDLE;
}

UDLMD_API UDLMD_STATUS Connect( UDLMD_HANDLE hMeasDev ){
	VC820* pDev = VC820::GetInstance();
	if( pDev ){
		return pDev->Connect( hMeasDev );
	}
	return MD_INVALIDE_HANDLE;
}

UDLMD_API UDLMD_STATUS Disconnect( UDLMD_HANDLE hMeasDev ){
	VC820* pDev = VC820::GetInstance();
	if( pDev ){
		return pDev->Disconnect( hMeasDev );
	}
	return MD_INVALIDE_HANDLE;
}


UDLMD_API UDLMD_STATUS Trigger( UDLMD_HANDLE hMeasDev, uint32_t iChannel ){
	VC820* pDev = VC820::GetInstance();
	if( pDev ){
		return pDev->Trigger( hMeasDev, iChannel );
	}
	return MD_INVALIDE_HANDLE;
}

UDLMD_API UDLMD_STATUS GetMeasValue( UDLMD_HANDLE hMeasDev, uint32_t iChannel, SMeasValue_t* pMeasVal ){
	VC820* pDev = VC820::GetInstance();
	if( pDev ){
		return pDev->GetMeasValue( hMeasDev, iChannel, pMeasVal );
	}
	return MD_INVALIDE_HANDLE;
}

UDLMD_API UDLMD_STATUS GetDeviceVerStr( UDLMD_HANDLE hMeasDev, char *pszDeviceVer, uint32_t cBufferLength ){
	VC820* pDev = VC820::GetInstance();
	if( pDev ){
		return pDev->GetDeviceVerStr( hMeasDev, pszDeviceVer, cBufferLength );
	}
	return MD_INVALIDE_HANDLE;
}


UDLMD_API UDLMD_STATUS GetDllVer( uint32_t*  pu32APIVerion, uint32_t*  pu32DLLVerion, char* pszDLLInfo ){
	/*
	VC820* pDev = VC820Container::getInstance()->GetDevice(hMeasDev);
	if( pDev ){
		return pDev->GetDllVer( pDllVer );
	}
	return MD_INVALIDE_HANDLE;
	*/
	return 0;
}

UDLMD_API UDLMD_STATUS GetLastMeasDevError( UDLMD_HANDLE hMeasDev, uint32_t*  pu32DevErrorNbr ){
	/*
	VC820* pDev = VC820Container::getInstance()->GetDevice(hMeasDev);
	if( pDev ){
		return pDev->GetDllVer( pDllVer );
	}
	return MD_INVALIDE_HANDLE;
	*/
	return 0;
}


VC820::VC820()
  : m_pThread(0)
{
}

VC820::~VC820()
{

}

VC820* VC820::GetInstance( void ){
  static VC820 instance;
  return reinterpret_cast<VC820*>(&instance);
};

UDLMD_STATUS VC820::Create( UDLMD_HANDLE* pMeasDevID ){

/*
    uint32_t hMeasDev = 0;
	VC820* pNewVC820 = new VC820;
	if( pNewVC820 && m_cDevices < 10){
		m_cDevices++;
		hMeasDev = m_cDevices;
		m_Devices[hMeasDev] = pNewVC820;
	}
	*/
	*pMeasDevID = 0;
	return MD_NO_ERROR;
}

UDLMD_STATUS VC820::Delete( UDLMD_HANDLE hMeasDev )
{
/*
	delete m_Devices[hMeasDev];
	m_Devices.erase( hMeasDev );

	if( m_Devices.empty() ){
		m_cDevices = 0;
	}
*/
	return MD_NO_ERROR;
}


UDLMD_STATUS VC820::Setup( UDLMD_HANDLE hMeasDev, uint32_t cArgs, char *rgpszArg[] )
{

	m_strSerialPort = "COM1";

	return EALLOK;
}

/*
uint32_t VC820::Connect( void )
{
	EERRORNBR eRet = ECANTOPENPORT;

	serial_port_base::baud_rate BAUD(2400);
	serial_port_base::character_size CSIZE( 8 );
	serial_port_base::flow_control FLOW( serial_port_base::flow_control::none );
	serial_port_base::parity PARITY( serial_port_base::parity::none );
	serial_port_base::stop_bits STOP( serial_port_base::stop_bits::one );

	Disconnect();

	io_service* pIo = new io_service;
	try{
		if( pIo )
			m_pSerialPort = new serial_port( *pIo, m_strSerialPort );
	}
	catch(...){

	}

	if( m_pSerialPort ){
		m_pSerialPort->set_option( BAUD );
		m_pSerialPort->set_option( CSIZE );
		m_pSerialPort->set_option( FLOW );
		m_pSerialPort->set_option( PARITY );
		m_pSerialPort->set_option( STOP );

		// TODO: Set RTS and so to power device
		DCB dcb;

		dcb.fDtrControl = DTR_CONTROL_ENABLE;
		dcb.fRtsControl = RTS_CONTROL_DISABLE;
		dcb.fOutxCtsFlow = FALSE;
		dcb.fOutxDsrFlow = FALSE;
		dcb.fDsrSensitivity = FALSE;
		dcb.fOutX = FALSE;
		dcb.fInX = FALSE;

		//SettableSerialPortOptiondcb> opt;

		//opt.dcb.fDtrControl = DTR_CONTROL_ENABLE;

		m_pSerialPort->set_option( )
*

		eRet = EALLOK;
	}


	m_pThread = new boost::thread( boost::bind( &VC820::Measure ) );

	return eRet;
}
*/

UDLMD_STATUS VC820::Connect( UDLMD_HANDLE hMeasDev )
{
	EERRORNBR eRet = ECANTOPENPORT;

	Disconnect( hMeasDev );

	m_hCom = CreateFile(	"COM1",
							GENERIC_READ | GENERIC_WRITE,
							0, // exclusive access
							NULL, // no security
							OPEN_EXISTING,
							0, // no overlapped I/O
							NULL); // null template

	if(m_hCom != INVALID_HANDLE_VALUE){
		::DCB dcb;

		SetupComm(m_hCom, 128, 128); // set buffer sizes
		GetCommState(m_hCom, &dcb);

		dcb.BaudRate = 2400;
		dcb.ByteSize = 8;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.fAbortOnError = TRUE;
		dcb.fDtrControl = DTR_CONTROL_ENABLE;
		dcb.fRtsControl = RTS_CONTROL_DISABLE;
		dcb.fOutxCtsFlow = FALSE;
		dcb.fOutxDsrFlow = FALSE;
		dcb.fDsrSensitivity = FALSE;
		dcb.fOutX = FALSE;
		dcb.fInX = FALSE;

		if( SetCommState(m_hCom, &dcb) == TRUE ) eRet = EALLOK;
	}

	if( eRet == EALLOK )
		m_pThread = new boost::thread( boost::bind( &VC820::Measure )  );

	return eRet;
}

UDLMD_STATUS VC820::Disconnect( UDLMD_HANDLE hMeasDev )
{
	ExitThread( m_pThread );

	if( m_hCom ){
		CloseHandle( m_hCom );
		m_hCom = 0;
	}
/*
	if( m_pSerialPort ){
		// TODO: must i delete the io??
		m_pSerialPort->close();
		delete m_pSerialPort;
		m_pSerialPort = 0;
	}
*/
	return 0;
}


UDLMD_STATUS VC820::Trigger( UDLMD_HANDLE hMeasDev, uint32_t iChannel )
{
	std::memcpy( &m_TrigMeasVall, &m_ActMeasVall, sizeof(SMeasValue_t) );
	return 0;
}

UDLMD_STATUS VC820::GetMeasValue( UDLMD_HANDLE hMeasDev, uint32_t iChannel, SMeasValue_t* pMeasVal )
{
	std::memcpy( pMeasVal, &m_ActMeasVall, sizeof(SMeasValue_t) );

	return 0;
}

UDLMD_STATUS VC820::GetDllVer( uint32_t*  pu32APIVerion, uint32_t*  pu32DLLVerion, char* pszDLLInfo )
{
	*pu32APIVerion = UDLMD_API_VER;
	*pu32DLLVerion = UDLMD_VC820_DLL_VER;
	strncpy( pszDLLInfo, "", 2 );
	return 0;
}


UDLMD_STATUS VC820::GetDeviceVerStr( UDLMD_HANDLE hMeasDev, char *pszDeviceVer, uint32_t cBufferLength )
{
	// Device submit no info String
	strncpy( pszDeviceVer, "Voltcraft VC820/VC840 Digital Multimeter", cBufferLength );
	return 0;
}


void  VC820::Measure( void ){
	char rgchMsgBuffer[100];

	// skip Buffer until first start sign
	/*
	do{
		int iAnz = 14;
		unsigned long ulBytesRead;
		double dValue;
		std::string strUnit;
	 ::ReadFile( m_hCom, rgchMsgBuffer, 1, &ulBytesRead, NULL);
	}
	*/

	while(1){

		int iAnz = 14;
		unsigned long ulBytesRead;
		double dValue;
		std::string strUnit;

		::ReadFile( m_hCom, rgchMsgBuffer, iAnz, &ulBytesRead, NULL);

		 rgchMsgBuffer[iAnz] = '\0';

		// std::cout << "VC820 -" << ulBytesRead ;

		if( DecodeMeasage( rgchMsgBuffer, dValue, strUnit ) ){

			//m_ActMeasVall.dCoefficient = 0.0;
			m_ActMeasVall.dMeasValue = dValue;
			std::strncpy( m_ActMeasVall.szUnit, strUnit.c_str(), sizeof(m_ActMeasVall.szUnit) );
			//m_ActMeasVall.u32TimeStamp = 0;
		}
	}
}


bool VC820::DecodeMeasage( char rgchData[], double& dValue, std::string& strUnit  ){

	dValue = 0.0;
	strUnit = "";

	if( (((rgchData[0]&0xF0)>>4) != 1) || (((rgchData[1]&0xF0)>>4) != 2)){
		return false; //fehlerhafte rgchData erhalten
	}

	int y(1);
	double faktor(1000.0);

	for( int n=0;n<5;n++){

		int tmp = (rgchData[y]&0x07)<<4 | (rgchData[y+1]&0x0F);
		y = y+2;

		switch(tmp){									/*Zahlenwert dekodieren*/
			case 0x05 : dValue = dValue + 1.0*faktor;break;
			case 0x5B : dValue = dValue + 2.0*faktor;break;
			case 0x1F : dValue = dValue + 3.0*faktor;break;
			case 0x27 : dValue = dValue + 4.0*faktor;break;
			case 0x3E : dValue = dValue + 5.0*faktor;break;
			case 0x7E : dValue = dValue + 6.0*faktor;break;
			case 0x15 : dValue = dValue + 7.0*faktor;break;
			case 0x7F : dValue = dValue + 8.0*faktor;break;
			case 0x3F : dValue = dValue + 9.0*faktor;break;
			case 0x7D : dValue = dValue + 0;break;
		}
		faktor = faktor/10.0;
	}

	if(rgchData[1]&0x08) dValue = dValue * -1.0;

	if(rgchData[3]&0x08) dValue = dValue/1000;					/*Dezimalpunkt mit reinrechnen*/
	else if(rgchData[5]&0x08) dValue = dValue/100;
	else if(rgchData[7]&0x08) dValue = dValue/10;

	if(rgchData[9]&0x08) dValue = dValue/1000000;				/*µ, m, k, M und n mit reinrechnen*/
	else if(rgchData[10]&0x08) dValue = dValue/1000;
	else if(rgchData[9]&0x02) dValue = dValue*1000;
	else if(rgchData[10]&0x02) dValue = dValue*1000000;
	else if(rgchData[9]&0x04) dValue = dValue/1000000000;

	if(rgchData[12] & 0x04)      strUnit = "V";
	else if(rgchData[12] & 0x08) strUnit = "A";
	else if(rgchData[11] & 0x04) strUnit = "Ohm";
	else if(rgchData[11] & 0x08) strUnit = "F";
	else if(rgchData[12] & 0x02) strUnit = "Hz";
	else if(rgchData[13] & 0x01) strUnit = "°C";

	if(rgchData[0] & 0x04) strUnit += " DC";
	else if(rgchData[0] & 0x08) strUnit += " AC";

	// if(rgchData[12] & 0x01) strUnit = "V"; // Batt
	// if(rgchData[11] & 0x01) strUnit = "V"; // Hold

	return true;	//Messwert korrekt empfangen und verarbeitet
}
