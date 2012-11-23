/*
 * UDL - Universal Data Logger
 * Copyright (C) 2012  Marco Hartung
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

//:TODO: Make Decode Measage fail saife
//:TODO: Implement Error handling
//:TODO: Use Mutex or so to acces shared vars

#include "VC820.h"

#include "../../UDLmain/UdlSettingsFile.h"
#include "../../UDLlib/com/SerialPort.h"

#include <string>
#include <cstring>
#include <map>
#include <algorithm>
//#include "windows.h"
//#include "process.h"

#include <iostream>

static std::map< uint32_t, VC820*>    gDevices;
static uint32_t                       gcDevices;  //!< Number of ever created Devices


UDLMD_API UDLMD_STATUS GetDeviceNames( char* pszNames, uint32_t cBufferLength ){
   pszNames[0] = '\0';
   strncpy( pszNames, "VC820", cBufferLength );
   return MD_NO_ERROR;
}

UDLMD_API UDLMD_STATUS Create( UDLMD_HANDLE* phMeasDev){

	VC820* pVC820 = new VC820;
	if( pVC820 ){
		gcDevices++;
		*phMeasDev = gcDevices;
		gDevices[gcDevices] = pVC820;
	}
	return 0;
}

UDLMD_API UDLMD_STATUS Delete( UDLMD_HANDLE hMeasDev ){

	VC820* pDev = gDevices[hMeasDev];
	if( pDev ){
		delete pDev;
		gDevices.erase(gcDevices);
		return MD_NO_ERROR;
	}
	return MD_INVALIDE_HANDLE;
}

UDLMD_API UDLMD_STATUS Setup( UDLMD_HANDLE hMeasDev,  char* pszArg, uint32_t cArg ){
	VC820* pDev = gDevices[hMeasDev];
	if( pDev ){
		return pDev->Setup( pszArg, cArg );
	}
	return MD_INVALIDE_HANDLE;
}

UDLMD_API UDLMD_STATUS Connect( UDLMD_HANDLE hMeasDev ){
	VC820* pDev = gDevices[hMeasDev];
	if( pDev ){
		return pDev->Connect();
	}
	return MD_INVALIDE_HANDLE;
}

UDLMD_API UDLMD_STATUS Disconnect( UDLMD_HANDLE hMeasDev ){
	VC820* pDev = gDevices[hMeasDev];
	if( pDev ){
		return pDev->Disconnect();
	}
	return MD_INVALIDE_HANDLE;
}


UDLMD_API UDLMD_STATUS Trigger( UDLMD_HANDLE hMeasDev, uint32_t iChannel ){
	VC820* pDev = gDevices[hMeasDev];
	if( pDev ){
		return pDev->Trigger( iChannel );
	}
	return MD_INVALIDE_HANDLE;
}

UDLMD_API UDLMD_STATUS GetMeasValue( UDLMD_HANDLE hMeasDev, uint32_t iChannel, SMeasValue_t* pMeasVal ){
	VC820* pDev = gDevices[hMeasDev];
	if( pDev ){
		return pDev->GetMeasValue( iChannel, pMeasVal );
	}
	return MD_INVALIDE_HANDLE;
}

UDLMD_API UDLMD_STATUS GetDeviceVerStr( UDLMD_HANDLE hMeasDev, char *pszDeviceVer, uint32_t cBufferLength ){
	VC820* pDev = gDevices[hMeasDev];
	if( pDev ){
		return pDev->GetDeviceVerStr( pszDeviceVer, cBufferLength );
	}
	return MD_INVALIDE_HANDLE;
}

UDLMD_API UDLMD_STATUS GetLibraryVer( uint32_t*  pu32ApiVer, uint32_t*  pu32LibVer ){

	*pu32ApiVer = UDLMD_API_VER;
	*pu32LibVer = UDLMD_VC820_DLL_VER;
//	strncpy( pszDLLInfo, "", 2 );
	return MD_NO_ERROR;
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

void ThreadProc( void* pParam ){
	VC820* pT = (VC820*)pParam;
	if( pT ){
		while( pT->m_fExitThread == false )
			pT->Measure();
	}
//	_endthread();
}


VC820::VC820()
   : m_LastError(ENOTINIT)
   , m_strSerialPort("")
   , m_pPort(0)
{
	m_TrigMeasVall.dMeasValue = 0.0;
	m_TrigMeasVall.szUnit[0] = '\0';
	m_ActMeasVall.dMeasValue = 0.0;
	m_ActMeasVall.szUnit[0] = '\0';
}

VC820::~VC820()
{
	Disconnect();
}

UDLMD_STATUS VC820::Setup( const char* pszArg, uint32_t cBufferLength )
{
	m_strSerialPort = "";

	if( pszArg && cBufferLength ){
		std::string strT( pszArg, std::min( (size_t)cBufferLength, (size_t)1000 ) );
		UdlSettingsFile sf;

		sf.ParseString( strT );

		if( sf.GetValueAsString( "", "Port", m_strSerialPort ) )
			return EALLOK;
	}
	return EWRONGARG;
}


UDLMD_STATUS VC820::Connect( void ){
	EERRORNBR eRet = ECANTOPENPORT;

	Disconnect();

//	m_pPort = new SerialPort( m_strSerialPort.c_str() );

	// TODO: check Port
	eRet = EALLOK;

	if( eRet == EALLOK ){
		m_fExitThread = false;
//		_beginthread( ThreadProc, 0, this );
	}

	return eRet;
}

UDLMD_STATUS VC820::Disconnect( void )
{
	ExitThread( );

	if( m_pPort ){
		delete( m_pPort );
		m_pPort = 0;
	}

	return 0;
}


UDLMD_STATUS VC820::Trigger( uint32_t iChannel )
{
	std::memcpy( &m_TrigMeasVall, &m_ActMeasVall, sizeof(SMeasValue_t) );
	return 0;
}

UDLMD_STATUS VC820::GetMeasValue( uint32_t iChannel, SMeasValue_t* pMeasVal )
{
	std::memcpy( pMeasVal, &m_ActMeasVall, sizeof(SMeasValue_t) );

	return 0;
}


UDLMD_STATUS VC820::GetDeviceVerStr( char *pszDeviceVer, uint32_t cBufferLength )
{
	// Device submit no info String
	strncpy( pszDeviceVer, "Voltcraft VC820/VC840 Digital Multimeter", cBufferLength );
	return 0;
}

UDLMD_STATUS VC820::GetSetupInfo( char* pszArg, uint32_t cArgs  ){

	const char szSetupInfo[] = "\
	#Name of used Port e.g. COM1 or tty1\n\
	Port = COM1\n\
	";

	strncpy( pszArg, szSetupInfo, cArgs );
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

	int iAnz = 14;
	//unsigned long ulBytesRead;
	double dValue;
	std::string strUnit;

	if( m_pPort ){
	   /*ulBytesRead = */ m_pPort->Read( rgchMsgBuffer, iAnz );
	}
	//::ReadFile( m_hCom, rgchMsgBuffer, iAnz, &ulBytesRead, NULL);
	rgchMsgBuffer[iAnz] = '\0';

	if( DecodeMeasage( rgchMsgBuffer, dValue, strUnit ) ){
		m_ActMeasVall.dMeasValue = dValue;
		std::strncpy( m_ActMeasVall.szUnit, strUnit.c_str(), sizeof(m_ActMeasVall.szUnit) );
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

	if(rgchData[9]&0x08) dValue = dValue/1000000;				/*�, m, k, M und n mit reinrechnen*/
	else if(rgchData[10]&0x08) dValue = dValue/1000;
	else if(rgchData[9]&0x02) dValue = dValue*1000;
	else if(rgchData[10]&0x02) dValue = dValue*1000000;
	else if(rgchData[9]&0x04) dValue = dValue/1000000000;

	if(rgchData[12] & 0x04)      strUnit = "V";
	else if(rgchData[12] & 0x08) strUnit = "A";
	else if(rgchData[11] & 0x04) strUnit = "Ohm";
	else if(rgchData[11] & 0x08) strUnit = "F";
	else if(rgchData[12] & 0x02) strUnit = "Hz";
	else if(rgchData[13] & 0x01) strUnit = "�C";

	if(rgchData[0] & 0x04) strUnit += " DC";
	else if(rgchData[0] & 0x08) strUnit += " AC";

	// if(rgchData[12] & 0x01) strUnit = "V"; // Batt
	// if(rgchData[11] & 0x01) strUnit = "V"; // Hold

	return true;	//Messwert korrekt empfangen und verarbeitet
}


bool VC820::ExitThread( void ){

	m_fExitThread = true;

	return true;
}
