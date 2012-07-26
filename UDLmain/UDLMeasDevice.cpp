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
{

}

UDLMeasDevice::UDLMeasDevice( UDLDevice* pUdlDev ){

//   m_pfnGetLibraryVer = pUdlDev->m_pfnGetLibraryVer;
//   m_pfnGetDeviceVerStr = pUdlDev->m_pfnGetDeviceVerStr;
//   m_pfnGetDeviceSetupInfo = pUdlDev->m_pfnGetDeviceSetupInfo;
//   m_pfnGetDevNames = pUdlDev->m_pfnGetDevNames;

   m_Lib = pUdlDev->GetLib();
  // m_Lib.DynamicLib( pUdlDev->GetLib().GetLibraryName() );
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
}

bool UDLMeasDevice::LoadDeviceLibrary( void ){

   LoadFunction( (void**)&m_pfnGetDevNames, "GetDeviceNames" );
   LoadFunction( (void**)&m_pfnCreate, "Create" );
   LoadFunction( (void**)&m_pfnDelete, "Delete" );
   LoadFunction( (void**)&m_pfnSetup, "Setup" );
   LoadFunction( (void**)&m_pfnConnect, "Connect" );
   LoadFunction( (void**)&m_pfnDisconnect, "Disconnect" );
   LoadFunction( (void**)&m_pfnTrigger, "Trigger" );
   LoadFunction( (void**)&m_pfnGetMeasValue, "GetMeasValue"  );
   LoadFunction( (void**)&m_pfnGetLibraryVer, "GetLibraryVer" );
   LoadFunction( (void**)&m_pfnGetDeviceVerStr, "GetDeviceVerStr" );

   return true;
}


bool UDLMeasDevice::LoadDeviceLibrary( const std::wstring &strLibPath ){

	if( m_Lib.LoadLibrary( strLibPath ) ){

	   LoadDeviceLibrary();

		Create( &m_hMeasDev, "test" );
	}
	else{
      UdlOut::Error << "Can not load library!" /* << strLibPath */<< std::endl;
      return false;
	}

	return true;
}


UDLMD_STATUS UDLMeasDevice::Create( UDLMD_HANDLE* pMeasDevID, const char* pszName )
{
	uint32_t Ret = 0;
	if(m_pfnCreate){
		Ret = m_pfnCreate(pMeasDevID, pszName );
	}
	return Ret;
}

UDLMD_STATUS UDLMeasDevice::Create( std::string strName )
{
   return Create( &m_hMeasDev, strName.c_str() );
}

UDLMD_STATUS UDLMeasDevice::Setup( const std::string& strArgs ){
	char rgszArg[strArgs.size()+1];

	std::strcpy( rgszArg, strArgs.c_str() );
	return Setup( rgszArg, strArgs.size() );
}

UDLMD_STATUS UDLMeasDevice::Setup( char *pszArg, uint32_t cArg  )
{
	uint32_t Ret = 0;
	if( m_pfnSetup ){
		Ret = m_pfnSetup( m_hMeasDev, pszArg, cArg );
	}
	return Ret;
}

UDLMD_STATUS UDLMeasDevice::Connect( void )
{
	uint32_t Ret = 0;
	if( m_pfnConnect ){
		Ret = m_pfnConnect( m_hMeasDev );
	}
	return Ret;
}


UDLMD_STATUS UDLMeasDevice::Disconnect( void )
{
	uint32_t Ret = 0;
	if( m_pfnDisconnect ){
		Ret = m_pfnDisconnect( m_hMeasDev );
	}
	return Ret;
}


UDLMD_STATUS UDLMeasDevice::Trigger( uint32_t iChannel )
{
	uint32_t Ret = 0;
	if( m_pfnTrigger ){
		Ret = m_pfnTrigger( m_hMeasDev, iChannel );
	}
	return Ret;
}

UDLMD_STATUS UDLMeasDevice::GetMeasValue( uint32_t iChannel, SMeasValue_t* pMeasVal )
{
	uint32_t Ret = 0;
	if( m_pfnGetMeasValue ){
		Ret = m_pfnGetMeasValue( m_hMeasDev, iChannel, pMeasVal );
	}
	return Ret;
}


