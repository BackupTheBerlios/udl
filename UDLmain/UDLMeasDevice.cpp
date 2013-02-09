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

/*! \file MeasDevInterface.cpp
 *  \brief
 *
 *  Author: Marco / Created: 16.08.2009
 */

#include "UDLDevice.h"
#include "UDLMeasDevice.h"
#include "UdlStdOut.h"
#include <string>
#include <vector>
#include <cstring>


UDLMeasDevice::UDLMeasDevice()
 : m_pfnCreate(0)
 , m_pfnDelete(0)
 , m_pfnSetup(0)
 , m_pfnConnect(0)
 , m_pfnDisconnect(0)
 , m_pfnTrigger(0)
 , m_pfnGetMeasValue(0)
 , m_pfnGetDeviceVerStr(0)
 , m_pfnGetLastMeasDevError(0)
{

}

UDLMeasDevice::UDLMeasDevice( UDLDevice* pUdlDev )
{
   m_Lib = pUdlDev->GetLib();
   LoadDeviceLibrary( );
}

UDLMeasDevice::~UDLMeasDevice()
{
	m_pfnCreate = 0;
	m_pfnDelete = 0;
	m_pfnSetup = 0;
	m_pfnConnect = 0;
	m_pfnDisconnect = 0;
	m_pfnTrigger = 0;
	m_pfnGetMeasValue = 0;
	m_pfnGetLastMeasDevError = 0;
}

UDLMeasDevice* UDLMeasDevice::NewMeasDev( UDLDevice* pUdlDev, const std::string& strName )
{
   UDLMeasDevice* pMd = new UDLMeasDevice( pUdlDev );
   if( pMd ){
      if( pMd->Create( strName ) == false ){
         delete pMd;
         pMd = 0;
      }
   }
   return pMd;
}

bool UDLMeasDevice::LoadDeviceLibrary( void ){

   LoadFunction( (void**)&m_pfnGetDevNames, "GetDeviceNames" );
   LoadFunction( (void**)&m_pfnGetLibraryVer, "GetLibraryVer" );
   LoadFunction( (void**)&m_pfnGetDeviceInfo, "GetDeviceInfo" );

   LoadFunction( (void**)&m_pfnCreate, "Create" );
   LoadFunction( (void**)&m_pfnDelete, "Delete" );
   LoadFunction( (void**)&m_pfnSetup, "Setup" );
   LoadFunction( (void**)&m_pfnConnect, "Connect" );
   LoadFunction( (void**)&m_pfnDisconnect, "Disconnect" );
   LoadFunction( (void**)&m_pfnTrigger, "Trigger" );
   LoadFunction( (void**)&m_pfnGetMeasValue, "GetMeasValue"  );
   LoadFunction( (void**)&m_pfnGetDeviceVerStr, "GetDeviceVerStr" );
   LoadFunction( (void**)&m_pfnGetLastMeasDevError, "GetLastMeasDevError" );

   return true;
}


bool UDLMeasDevice::LoadDeviceLibrary( const std::wstring &strLibPath )
{
   bool fRet;

   fRet = UDLDevice::LoadDeviceLibrary( strLibPath );
   if( fRet ){
      fRet = LoadDeviceLibrary();
   }

	return fRet;
}


bool UDLMeasDevice::Create( UDLMD_HANDLE* pMeasDevID, const char* pszName )
{
	bool fRet = false;
	if(m_pfnCreate){
		fRet = ( m_pfnCreate(pMeasDevID, pszName ) == MD_NO_ERROR );
	}
	return fRet;
}

bool UDLMeasDevice::Create( std::string strName )
{
   return Create( &m_hMeasDev, strName.c_str() );
}

bool UDLMeasDevice::Setup( const std::string& strArgs ){
	char rgszArg[strArgs.size()+1];

	std::strcpy( rgszArg, strArgs.c_str() );
	return Setup( rgszArg, strArgs.size() );
}

bool UDLMeasDevice::Setup( char *pszArg, uint32_t cArg  )
{
	bool fRet = false;
	if( m_pfnSetup ){
		fRet = ( m_pfnSetup( m_hMeasDev, pszArg, cArg ) == MD_NO_ERROR );
	}
	return fRet;
}

bool UDLMeasDevice::Connect( void )
{
	bool fRet = false;
	if( m_pfnConnect ){
		fRet = ( m_pfnConnect( m_hMeasDev ) == MD_NO_ERROR );
	}
	return fRet;
}


bool UDLMeasDevice::Disconnect( void )
{
	bool fRet = false;
	if( m_pfnDisconnect ){
		fRet = ( m_pfnDisconnect( m_hMeasDev ) == MD_NO_ERROR );
	}
	return fRet;
}


bool UDLMeasDevice::Trigger( uint32_t iChannel )
{
	bool fRet = false;
	if( m_pfnTrigger ){
		fRet = ( m_pfnTrigger( m_hMeasDev, iChannel ) == MD_NO_ERROR );
	}
	return fRet;
}

bool UDLMeasDevice::GetMeasValue( uint32_t iChannel, SMeasValue_t* pMeasVal )
{
	bool fRet = 0;
	if( m_pfnGetMeasValue ){
		fRet = ( m_pfnGetMeasValue( m_hMeasDev, iChannel, pMeasVal ) == MD_NO_ERROR );
	}
	return fRet;
}

bool UDLMeasDevice::GetDeviceVerStr( char *pszDeviceVer, uint32_t cBufferLength )
{
   bool fRet = 0;
   if( m_pfnGetDeviceVerStr ){
      fRet = ( m_pfnGetDeviceVerStr( m_hMeasDev, pszDeviceVer, cBufferLength ) == MD_NO_ERROR );
   }
   return fRet;
}

bool UDLMeasDevice::GetLastErrorMessage( std::string& strLastError )
{
   bool fRet = false;
   if( m_pfnGetLastMeasDevError ){
      char* pMsg = 0;
      uint32_t cByteMsg;
      if( m_pfnGetLastMeasDevError( m_hMeasDev, pMsg, &cByteMsg ) == MD_NO_ERROR ) {
         pMsg = new char[cByteMsg+1];
         fRet = ( m_pfnGetLastMeasDevError( m_hMeasDev, pMsg, &cByteMsg ) == MD_NO_ERROR );
         strLastError = std::string( pMsg, cByteMsg );
         delete[] pMsg;
      }
   }
   return fRet;
}
