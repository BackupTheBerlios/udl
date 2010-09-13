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
#include <iostream>
#include <string>
#include <vector>

extern "C"

UDLMeasDevice::UDLMeasDevice()
 : m_dllHandle(0)
 , m_pfnCreate(0)
 , m_pfnDelete(0)
 , m_pfnSetup(0)
 , m_pfnConnect(0)
 , m_pfnDisconnect(0)
 , m_pfnTrigger(0)
 , m_pfnGetMeasValue(0)
 , m_pfnGetDllVer(0)
 , m_pfnGetDeviceVerStr(0)
{

}

UDLMeasDevice::~UDLMeasDevice()
{
	if( m_dllHandle ){
		FreeLibrary( m_dllHandle );
		m_dllHandle = 0;
	}
	m_pfnCreate = 0;
	m_pfnDelete = 0;
	m_pfnSetup = 0;
	m_pfnConnect = 0;
	m_pfnDisconnect = 0;
	m_pfnTrigger = 0;
	m_pfnGetMeasValue = 0;
	m_pfnGetDllVer = 0;
	m_pfnGetDeviceVerStr = 0;
}

bool UDLMeasDevice::LoadDeviceLibrary( const std::string &strLibPath ){

	if( !m_dllHandle ){

		m_dllHandle  =  ::LoadLibrary( strLibPath.c_str() );
		if( !m_dllHandle ){
			std::cout << "Cant load library!" << strLibPath;;
			return false;
		}
		LoadFunction( (void**)&m_pfnCreate, "Create" );
		LoadFunction( (void**)&m_pfnDelete, "Delete" );
		LoadFunction( (void**)&m_pfnSetup, "Setup" );
		LoadFunction( (void**)&m_pfnConnect, "Connect" );
		LoadFunction( (void**)&m_pfnDisconnect, "Disconnect" );
		LoadFunction( (void**)&m_pfnTrigger, "Trigger" );
		LoadFunction( (void**)&m_pfnGetMeasValue, "GetMeasValue"  );
		LoadFunction( (void**)&m_pfnGetDllVer, "GetDllVer" );
		LoadFunction( (void**)&m_pfnGetDeviceVerStr, "GetDeviceVerStr" );

		Create( &m_hMeasDev );
	}

	return true;
}

bool UDLMeasDevice::LoadFunction( void** pfn, const std::string &FunctionName ){

	if( m_dllHandle ){
		*pfn  =  (void*)GetProcAddress( m_dllHandle,  FunctionName.c_str() );
		if( !(*pfn) ){
			std::cout << "Cant load Funktion! : " << FunctionName << std::endl;
			return false;
		}
	}

	return true;
}

UDLMD_STATUS UDLMeasDevice::Create( UDLMD_HANDLE* pMeasDevID )
{
	uint32_t Ret = 0;
	if(m_pfnCreate){
		Ret = m_pfnCreate(pMeasDevID);
	}
	return Ret;
}

UDLMD_STATUS UDLMeasDevice::Setup( std::string strArg )
{
	std::vector<std::string> vecArgs;
	size_t cMaxArgLength(0);

	for( size_t i = 0; i < strArg.size(); ){
		size_t ii = strArg.find( "--", i );
		vecArgs.push_back( strArg.substr( i, ii-i) );
		i = ii;
		cMaxArgLength = vecArgs.rbegin()->size() > cMaxArgLength ? vecArgs.rbegin()->size() : cMaxArgLength;
	}

	uint32_t cArgs = (uint32_t)(vecArgs.size());
	char* rgszArg[cArgs];

	for( size_t i = 0; i < cArgs; i++){
		char* pszT = new char[vecArgs[i].size()+1];
		rgszArg[i] = pszT;
		strcpy( rgszArg[i], vecArgs[i].c_str() );
	}

	return Setup( cArgs, rgszArg );
}

UDLMD_STATUS UDLMeasDevice::Setup( uint32_t cArgs, char *rgpszArg[] )
{
	uint32_t Ret = 0;
	if( m_pfnSetup ){
		Ret = m_pfnSetup( m_hMeasDev, cArgs, rgpszArg );
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

UDLMD_STATUS UDLMeasDevice::GetDllVer(uint32_t*  pu32APIVerion, uint32_t*  pu32DLLVerion, char* pszDLLInfor )
{
	uint32_t Ret = 0;
	if( m_pfnGetDllVer ){
		Ret = m_pfnGetDllVer( pu32APIVerion, pu32DLLVerion, pszDLLInfor );
	}
	return Ret;
}


UDLMD_STATUS UDLMeasDevice::GetDeviceVerStr( char *pszDeviceVer, uint32_t cBufferLength )
{
	uint32_t Ret = 0;
	if( m_pfnGetDeviceVerStr ){
		Ret = m_pfnGetDeviceVerStr( pszDeviceVer, cBufferLength );
	}
	return Ret;
}

