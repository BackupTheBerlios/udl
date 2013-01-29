/*
 * UDL - Universal Data Logger
 * Copyright (C) 2013  Marco Hartung
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
#include <vector>

#include "../UDLMeasDev/MeasDevTypes.h"
#include "UDLDevice.h"


class UDLMeasDevice : public UDLDevice{

public:

   static UDLMeasDevice* NewMeasDev( UDLDevice* pUdlDev, const std::string& strName );

   UDLMeasDevice();
   UDLMeasDevice( UDLDevice* pUdlDev );

	virtual ~UDLMeasDevice();


	virtual bool Setup(  const std::string& strArgs );

	virtual bool Setup( char *pszArg, uint32_t cArgs );

	virtual bool Connect( void );

	virtual bool Disconnect( void );


	virtual bool Trigger( uint32_t iChannel );

	virtual bool GetMeasValue( uint32_t iChannel, SMeasValue_t* pMeasVal );

	virtual bool GetDeviceVerStr( char *pszDeviceVer, uint32_t cBufferLength );

	virtual bool GetLastErrorMessage( std::string& strLastError );

protected:

	virtual bool LoadDeviceLibrary( void );
	virtual bool LoadDeviceLibrary( const std::wstring &strLibPath );

	virtual bool Create( UDLMD_HANDLE* pMeasDevID, const char* pszName );
	virtual bool Create( std::string strName );
	// TODO: Delete instans of MeasDev m_hMeasDev before destroying dll Handle

private:

	UDLMeasDevice( const UDLMeasDevice& mdi );
	UDLMeasDevice& operator=( const UDLMeasDevice& mdi );

	UDLMD_HANDLE          m_hMeasDev;

	typedef UDLMD_STATUS (*PFN_CREATE)( UDLMD_HANDLE* pMeasDevID, const char* pszName );
	typedef UDLMD_STATUS (*PFN_DELETE)( UDLMD_HANDLE MeasDevID );
	typedef UDLMD_STATUS (*PFN_SETUP)( UDLMD_HANDLE MeasDevID, const char* pszArg, uint32_t cArg );
	typedef UDLMD_STATUS (*PFN_CONNECT)( UDLMD_HANDLE MeasDevID );
	typedef UDLMD_STATUS (*PFN_DISCONNECT)( UDLMD_HANDLE MeasDevID );
	typedef UDLMD_STATUS (*PFN_TRIGGER)( UDLMD_HANDLE MeasDevID, uint32_t iChannel );
	typedef UDLMD_STATUS (*PFN_GETMEASVALUE)( UDLMD_HANDLE MeasDevID, uint32_t iChannel, SMeasValue_t* pMeasVal  );
	typedef UDLMD_STATUS (*PFN_GETDEVICEVERSTR)( UDLMD_HANDLE hMeasDev, char *pszDeviceVer, uint32_t cBufferLength );
	typedef UDLMD_STATUS (*PFN_GETLASTMEASDEVERROR)( UDLMD_HANDLE hMeasDev, char* pszErrorMsg, uint32_t* cBufferLength );

	PFN_CREATE                m_pfnCreate;
	PFN_DELETE                m_pfnDelete;
	PFN_SETUP                 m_pfnSetup;
	PFN_CONNECT               m_pfnConnect;
	PFN_DISCONNECT            m_pfnDisconnect;
	PFN_TRIGGER               m_pfnTrigger;
	PFN_GETMEASVALUE          m_pfnGetMeasValue;
	PFN_GETDEVICEVERSTR       m_pfnGetDeviceVerStr;
	PFN_GETLASTMEASDEVERROR   m_pfnGetLastMeasDevError;

};

#endif /* UDLMEASDEVICE_H_ */
