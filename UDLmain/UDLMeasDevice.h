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

/*! \file MeasDevInterface.h
 *  \brief Interface to the UDL MeasDevice module
 *
 *  Author: Marco / Created: 16.08.2009
 */

#ifndef UDLMEASDEVICE_H_
#define UDLMEASDEVICE_H_

#include <string>
#include "windows.h"
#include "../UDLMeasDev/MeasDevTypes.h"


typedef  UDLMD_STATUS (*PFN_CREATE)( UDLMD_HANDLE* pMeasDevID );
typedef  UDLMD_STATUS (*PFN_DELETE)( UDLMD_HANDLE MeasDevID );
typedef  UDLMD_STATUS (*PFN_SETUP)( UDLMD_HANDLE MeasDevID, uint32_t cArgs, char *rgpszArg[] );
typedef  UDLMD_STATUS (*PFN_CONNECT)( UDLMD_HANDLE MeasDevID );
typedef  UDLMD_STATUS (*PFN_DISCONNECT)( UDLMD_HANDLE MeasDevID );
typedef  UDLMD_STATUS (*PFN_TRIGGER)( UDLMD_HANDLE MeasDevID, uint32_t iChannel );
typedef  UDLMD_STATUS (*PFN_GETMEASVALUE)( UDLMD_HANDLE MeasDevID, uint32_t iChannel, SMeasValue_t* pMeasVal  );
typedef  UDLMD_STATUS (*PFN_GETDLLVER)( uint32_t*  pu32APIVerion, uint32_t*  pu32DLLVerion, char* pszDLLInfo );
typedef  UDLMD_STATUS (*PFN_GETDEVICEVERSTR)( char *pszDeviceVer, uint32_t cBufferLength );

class UDLMeasDevice : public UDLDevice{

public:
	UDLMeasDevice();
	virtual ~UDLMeasDevice();
	virtual bool LoadDeviceLibrary( const std::string &strLibPath );

	virtual UDLMD_STATUS Create( UDLMD_HANDLE* pMeasDevID );
	// TODO: Delete instanz of MeasDev m_hMeasDev before destroying dll Handle

	virtual UDLMD_STATUS Setup(  std::vector<std::string> vecArgs );

	virtual UDLMD_STATUS Setup( uint32_t cArgs, char *rgpszArg[] );

	virtual UDLMD_STATUS Connect( void );

	virtual UDLMD_STATUS Disconnect( void );


	virtual UDLMD_STATUS Trigger( uint32_t iChannel );

	virtual UDLMD_STATUS GetMeasValue( uint32_t iChannel, SMeasValue_t* pMeasVal );

	virtual UDLMD_STATUS GetDeviceVerStr( char *pszDeviceVer, uint32_t cBufferLength );


	virtual UDLMD_STATUS GetDllVer( uint32_t*  pu32APIVerion, uint32_t*  pu32DLLVerion, char* pszDLLInfo );
protected:

	bool LoadFunction( void** pfn, const std::string &FunctionName );

private:

	UDLMeasDevice( const UDLMeasDevice& mdi );
	UDLMeasDevice& operator=( const UDLMeasDevice& mdi );

	UDLMD_HANDLE          m_hMeasDev;

	HINSTANCE             m_dllHandle;
	PFN_CREATE            m_pfnCreate;
	PFN_DELETE            m_pfnDelete;
	PFN_SETUP             m_pfnSetup;
	PFN_CONNECT           m_pfnConnect;
	PFN_DISCONNECT        m_pfnDisconnect;
	PFN_TRIGGER           m_pfnTrigger;
	PFN_GETMEASVALUE      m_pfnGetMeasValue;
	PFN_GETDLLVER         m_pfnGetDllVer;
	PFN_GETDEVICEVERSTR   m_pfnGetDeviceVerStr;

};

#endif /* UDLMEASDEVICE_H_ */
