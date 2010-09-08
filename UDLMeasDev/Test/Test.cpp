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

/*! \file Test.cpp
 *  \brief
 *
 *  Author: Marco / Created: 12.08.2009
 */

#include "Test.h"

static SMeasValue_t  m_ActMeasVall = {0.0,0.0,0,""};


EXPORT uint32_t Create( UDLMD_HANDLE* MeasDevID ){

	return Test::GetInstance()->Create();
}

EXPORT uint32_t Delete( uint32_t TestID ){

	return Test::GetInstance()->Delete( TestID );
}

EXPORT uint32_t Setup( uint32_t TestID, uint32_t cArgs, char *rgpszArg[]){
	Test* pDev = Test::GetInstance();
	if( pDev ){
		return pDev->Setup( TestID, cArgs, rgpszArg );
	}
	return MD_INVALIDE_HANDLE;
}

EXPORT uint32_t Connect( uint32_t id ){
	Test* pDev = Test::GetInstance();
	if( pDev ){
		return pDev->Connect( id );
	}
	return MD_INVALIDE_HANDLE;
}

EXPORT uint32_t Disconnect( uint32_t id ){
	Test* pDev = Test::GetInstance();
	if( pDev ){
		return pDev->Disconnect( id );
	}
	return MD_INVALIDE_HANDLE;
}


EXPORT uint32_t Trigger( uint32_t id, uint32_t iChannel ){
	Test* pDev = Test::GetInstance();
	if( pDev ){
		return pDev->Trigger( id, iChannel );
	}
	return MD_INVALIDE_HANDLE;
}

EXPORT uint32_t GetMeasValue( uint32_t id, uint32_t iChannel, SMeasValue_t* pMeasVal ){
	Test* pDev = Test::GetInstance();
	if( pDev ){
		return pDev->GetMeasValue( id, iChannel, pMeasVal );
	}
	return MD_INVALIDE_HANDLE;
}

EXPORT uint32_t GetDeviceVerStr( uint32_t id, char *pszDeviceVer, uint32_t cBufferLength ){
	Test* pDev = Test::GetInstance();
	if( pDev ){
		return pDev->GetDeviceVerStr( id, pszDeviceVer, cBufferLength );
	}
	return MD_INVALIDE_HANDLE;
}


EXPORT uint32_t GetDllVer( SDLLVersion_t *pDllVer ){
	/*
	Test* pDev = TestContainer::getInstance()->GetDevice(TestID);
	if( pDev ){
		return pDev->GetDllVer( pDllVer );
	}
	return MD_INVALIDE_HANDLE;
	*/
	return 0;
}




Test::Test()
{
}

Test::~Test()
{
}

Test* Test::GetInstance( void ){
  static Test instance;
  return reinterpret_cast<Test*>(&instance);
};

uint32_t Test::Create( void ){
	uint32_t TestID = 0;

	return TestID;
}

uint32_t Test::Delete( uint32_t TestID )
{

	return MD_NO_ERROR;
}


uint32_t Test::Setup( uint32_t TestID, uint32_t cArgs, char *rgpszArg[] )
{
	return EALLOK;
}

uint32_t Test::Connect( uint32_t TestID )
{
	return EALLOK;
}

uint32_t Test::Disconnect( uint32_t TestID )
{
	return EALLOK;
}


uint32_t Test::Trigger( uint32_t TestID, uint32_t iChannel )
{
	return EALLOK;
}

uint32_t Test::GetMeasValue( uint32_t TestID, uint32_t iChannel, SMeasValue_t* pMeasVal )
{
	pMeasVal->dMeasValue =  100.1;
	strcpy( pMeasVal->szUnit, "Volt" );

	return 0;
}

uint32_t Test::GetDllVer( SDLLVersion_t *pDllVer )
{
	pDllVer->u32MinorVerion = 0;
	pDllVer->u32MajorVerion = 0;
	strncpy( pDllVer->szDLLInfo, "", sizeof(pDllVer->szDLLInfo) );
	return 0;
}


uint32_t Test::GetDeviceVerStr( uint32_t TestID, char *pszDeviceVer, uint32_t cBufferLength )
{
	// Device submit no info String
	strncpy( pszDeviceVer, "Virtual test Multimeter", cBufferLength );
	return 0;
}


