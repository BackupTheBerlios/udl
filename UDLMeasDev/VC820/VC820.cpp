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

//:TODO: Use Mutex or so to acces shared vars

#include "VC820.h"

#include "../../UDLmain/UdlSettingsFile.h"
#include "../../UDLlib/com/SerialPort.h"
#include "../../UDLlib/UdlSystem.h"

#include <string>
#include <cstring>
#include <map>
#include <algorithm>


static std::map< uint32_t, VC820*>    gDevices;
static uint32_t                       gcDevices;  //!< Number of ever created Devices

extern "C" {

   UDLMD_API UDLMD_STATUS GetDeviceNames( char* pszNames, uint32_t* cBufferLength )
   {
      const std::string strDevList = "VC820";

      if( pszNames == 0 ){
         *cBufferLength = strDevList.length();
      }
      else {
         strncpy( pszNames, strDevList.c_str(), *cBufferLength );
      }
      return MD_NO_ERROR;
   }

   UDLMD_API UDLMD_STATUS Create( UDLMD_HANDLE* phMeasDev, const char* pszName  )
   {
      if( strncmp( "VC820", pszName, sizeof("VC820") ) == 0  ){

         VC820* pVC820 = new VC820;
         if( pVC820 ){
            gcDevices++;
            *phMeasDev = gcDevices;
            gDevices[gcDevices] = pVC820;
            return MD_NO_ERROR;
         }

      }
      return MD_CANT_CREATE;
   }

   UDLMD_API UDLMD_STATUS Delete( UDLMD_HANDLE hMeasDev )
   {
      VC820* pDev = gDevices[hMeasDev];
      if( pDev ){
         delete pDev;
         gDevices.erase(gcDevices);
         return MD_NO_ERROR;
      }
      return MD_INVALIDE_HANDLE;
   }

   UDLMD_API UDLMD_STATUS Setup( UDLMD_HANDLE hMeasDev, const char* pszArg, uint32_t cBufferLength )
   {
      VC820* pDev = gDevices[hMeasDev];
      if( pDev ){
         return pDev->Setup( pszArg, cBufferLength );
      }
      return MD_INVALIDE_HANDLE;
   }

   UDLMD_API UDLMD_STATUS Connect( UDLMD_HANDLE hMeasDev )
   {
      VC820* pDev = gDevices[hMeasDev];
      if( pDev ){
         return pDev->Connect();
      }
      return MD_INVALIDE_HANDLE;
   }

   UDLMD_API UDLMD_STATUS Disconnect( UDLMD_HANDLE hMeasDev )
   {
      VC820* pDev = gDevices[hMeasDev];
      if( pDev ){
         return pDev->Disconnect();
      }
      return MD_INVALIDE_HANDLE;
   }


   UDLMD_API UDLMD_STATUS Trigger( UDLMD_HANDLE hMeasDev, uint32_t iChannel )
   {
      VC820* pDev = gDevices[hMeasDev];
      if( pDev ){
         return pDev->Trigger( iChannel );
      }
      return MD_INVALIDE_HANDLE;
   }

   UDLMD_API UDLMD_STATUS GetMeasValue( UDLMD_HANDLE hMeasDev, uint32_t iChannel, SMeasValue_t* pMeasVal )
   {
      VC820* pDev = gDevices[hMeasDev];
      if( pDev ){
         return pDev->GetMeasValue( iChannel, pMeasVal );
      }
      return MD_INVALIDE_HANDLE;
   }

   UDLMD_API UDLMD_STATUS GetDeviceVerStr( UDLMD_HANDLE hMeasDev, char *pszDeviceVer, uint32_t* cBufferLength )
   {
      VC820* pDev = gDevices[hMeasDev];
      if( pDev ){
         return pDev->GetDeviceVerStr( pszDeviceVer, cBufferLength );
      }
      return MD_INVALIDE_HANDLE;
   }

   UDLMD_API UDLMD_STATUS GetLibraryVer( uint32_t*  pu32ApiVer, uint32_t*  pu32LibVer )
   {
      *pu32ApiVer = UDLMD_API_VER;
      *pu32LibVer = UDLMD_VC820_DLL_VER;
      return MD_NO_ERROR;
   }

   UDLMD_API UDLMD_STATUS GetLastMeasDevError( UDLMD_HANDLE hMeasDev, char* pszErrorMsg, uint32_t* cBufferLength )
   {
      VC820* pDev = gDevices[hMeasDev];
      if( pDev ){

         if( pszErrorMsg == 0 ){
            *cBufferLength = pDev->GetLastErrorMsg().length();
         }
         else{
            strncpy( pszErrorMsg, pDev->GetLastErrorMsg().c_str(), *cBufferLength );
         }
         return MD_NO_ERROR;
      }
      return MD_INVALIDE_HANDLE;
   }

   UDLMD_API UDLMD_STATUS GetDeviceInfo( const char* pszName, char* pszDeviceInfo, uint32_t* cBufferLength )
   {
      //const std::string szDevInfo = "#Name of used Port e.g. COM1 or tty1\nPort=COM1";

   static const char szDevInfo[] = R"(
<?xml version="1.0" encoding="UTF-8"?>
<DeviceInfo>
  <DeviceName>NAME</DeviceName>
    <SetupParameter>
      <Port>
        <Description>Name of used Port e.g. COM1 or tty1</Description>
        <DefaultValue>COM1</DefaultValue>
      </Port>
  </SetupParameter>
</DeviceInfo>
)";

      if( pszDeviceInfo == 0 ){
         *cBufferLength = sizeof( szDevInfo );
      }
      else{
         strncpy( pszDeviceInfo, szDevInfo, *cBufferLength );
      }
      return MD_NO_ERROR;
   }

}

VC820::VC820()
   : m_strSerialPort("")
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

      if( sf.GetValueAsString( "", "Port", m_strSerialPort ) ){
         m_strLastError.clear();
         return MD_NO_ERROR;
      }
      else{
         m_strLastError = "Can't Setup device. No Port name.";
      }
   }
   else{
      m_strLastError = "No valid configuration.";
   }

   return MD_ERROR;
}


UDLMD_STATUS VC820::Connect( void ){
   int ret;

   Disconnect();

   if( m_Port.Open( m_strSerialPort, 2400 ) == 0 ){

      m_Port.SetDtr( false );
      m_Port.SetRts( true );

      try{
         m_fExitThread = false;
         m_MeasThread = std::thread( Measure, this );
      }
      catch(...){
         m_strLastError = "Can't start measurement Thread.";
         return MD_ERROR;
      }
      m_strLastError.clear();
      return MD_NO_ERROR;
   }
   else{
      m_strLastError = "Can't open Port: " + m_strSerialPort;
   }

   return MD_ERROR;
}

UDLMD_STATUS VC820::Disconnect( void )
{
   ExitThread( );
   m_Port.Close();
   return MD_NO_ERROR;
}


UDLMD_STATUS VC820::Trigger( uint32_t iChannel )
{
   std::memcpy( &m_TrigMeasVall, &m_ActMeasVall, sizeof(SMeasValue_t) );
   return MD_NO_ERROR;
}

UDLMD_STATUS VC820::GetMeasValue( uint32_t iChannel, SMeasValue_t* pMeasVal )
{
   std::memcpy( pMeasVal, &m_ActMeasVall, sizeof(SMeasValue_t) );

   if( m_MeasValValid ){
      return MD_NO_ERROR;
   }
   else{
      return MD_ERROR;
   }
}


UDLMD_STATUS VC820::GetDeviceVerStr( char *pszDeviceVer, uint32_t* cBufferLength )
{
   // Device submit no info String
   const std::string strInfo = "Voltcraft VC820/VC840 Digital Multimeter";
   if( pszDeviceVer == 0 ){
      *cBufferLength = strInfo.length();
   }
   else{
      strncpy( pszDeviceVer, strInfo.c_str(), *cBufferLength );
   }
   return MD_NO_ERROR;
}


void  VC820::Measure( void* pThis )
{
   VC820* pT = (VC820*)pThis;

   if( pT ){
      char rgchMsgBuffer[100];
      int iRead = 0;
      int iWrite = 0;
      uint64_t LastReqTimeMs = System::GetTimeMs();

      while( pT->m_fExitThread == false ){
         double dValue;
         std::string strUnit;

         if( LastReqTimeMs + 5000L < System::GetTimeMs() ){
            pT->m_strLastError = "Communication timeout.";
            pT->m_MeasValValid = false;
         }

         iWrite += pT->m_Port.Read( &rgchMsgBuffer[iWrite], 14 );
         while( ( (rgchMsgBuffer[iRead]&0xF0) != 0x10 ) &&  ( iRead < iWrite ) ){
            iRead++;
         }
         if( (iWrite - iRead) >= 14 ){

            if( DecodeMeasage( &rgchMsgBuffer[iRead], dValue, strUnit ) ){
               LastReqTimeMs = System::GetTimeMs();

               pT->m_MeasValValid = true;
               pT->m_strLastError.clear();
               pT->m_ActMeasVall.dMeasValue = dValue;
               std::strncpy( pT->m_ActMeasVall.szUnit, strUnit.c_str(), sizeof(pT->m_ActMeasVall.szUnit) );
            }
            else{
               pT->m_strLastError = "Can't decode data. Wrong checksum.";
            }

            // Move remaining data to the front
            if( (iWrite - iRead) > 14 ){
               std::memmove( &rgchMsgBuffer[0], &rgchMsgBuffer[iRead+14], iWrite-iRead-14 );
            }
            iWrite =  iWrite-iRead-14;
            iRead = 0;
         }

         if( iWrite > sizeof(rgchMsgBuffer)-20 ){
            iWrite = 0;
            iRead = 0;
         }
      }
   }
}


bool VC820::DecodeMeasage( char rgchData[], double& dValue, std::string& strUnit  ){

	dValue = 0.0;
	strUnit = "";

	if( ( (rgchData[0]  & 0xF0) != 0x10 ) ||
	    ( (rgchData[1]  & 0xF0) != 0x20 ) ||
	    ( (rgchData[2]  & 0xF0) != 0x30 ) ||
	    ( (rgchData[3]  & 0xF0) != 0x40 ) ||
	    ( (rgchData[4]  & 0xF0) != 0x50 ) ||
	    ( (rgchData[5]  & 0xF0) != 0x60 ) ||
	    ( (rgchData[6]  & 0xF0) != 0x70 ) ||
	    ( (rgchData[7]  & 0xF0) != 0x80 ) ||
	    ( (rgchData[8]  & 0xF0) != 0x90 ) ||
	    ( (rgchData[9]  & 0xF0) != 0xA0 ) ||
	    ( (rgchData[10] & 0xF0) != 0xB0 ) ||
	    ( (rgchData[11] & 0xF0) != 0xC0 ) ||
	    ( (rgchData[12] & 0xF0) != 0xD0 ) ||
	    ( (rgchData[13] & 0xF0) != 0xE0 )	)
	{
		return false; //Wrong data 'checksum'
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
