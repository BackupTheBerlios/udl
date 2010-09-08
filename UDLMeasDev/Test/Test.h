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

/*! \file Test.h
 *  \brief
 *
 *  Author: Marco / Created: 12.08.2009
 */

#ifndef Test_H_
#define Test_H_

#include "../MeasDevTypes.h"

#include <string> // for types uint32_t
#include <map>


#ifdef BUILD_DLL /* DLL export */
#define EXPORT __declspec(dllexport)
#else            /* EXE import */
#define EXPORT __declspec(dllimport)
#endif


extern "C" {

	EXPORT uint32_t Create( void );

	EXPORT uint32_t Delete( uint32_t MeasDevID );


	EXPORT uint32_t Setup( uint32_t MeasDevID, uint32_t cArgs, char *rgpszArg[] );

	EXPORT uint32_t Connect( uint32_t MeasDevID );

	EXPORT uint32_t Disconnect( uint32_t MeasDevID );


	EXPORT uint32_t Trigger( uint32_t MeasDevID, uint32_t iChannel );

	EXPORT uint32_t GetMeasValue( uint32_t MeasDevID, uint32_t iChannel, SMeasValue_t* pMeasVal );

	EXPORT uint32_t GetDeviceVerStr( uint32_t MeasDevID, char *pszDeviceVer, uint32_t cBufferLength );


	EXPORT uint32_t GetDllVer( SDLLVersion_t *pDllVer );

}



class Test {

public:

	enum EERRORNBR{
		EALLOK = 0,
		ECANTOPENPORT = 1,
		ENOTINIT = 2,
	};

	static Test* GetInstance( void );

	virtual ~Test();

	virtual uint32_t Create( void );

	virtual uint32_t Delete( uint32_t MeasDevID );


	virtual uint32_t Setup( uint32_t MeasDevID, uint32_t cArgs, char *rgpszArg[] );

	virtual uint32_t Connect( uint32_t MeasDevID );

	virtual uint32_t Disconnect( uint32_t MeasDevID );


	virtual uint32_t Trigger( uint32_t MeasDevID, uint32_t iChannel );

	virtual uint32_t GetMeasValue( uint32_t MeasDevID, uint32_t iChannel, SMeasValue_t* pMeasVal );

	virtual uint32_t GetDeviceVerStr( uint32_t MeasDevID, char *pszDeviceVer, uint32_t cBufferLength );

	virtual uint32_t GetDllVer( SDLLVersion_t *pDllVer );

protected:

	EERRORNBR GetLastError( void );
	EERRORNBR SetLastError( EERRORNBR ErrorNbr );

private:
	Test();
	SMeasValue_t    m_TrigMeasVall;

	std::map< uint32_t, Test*>    m_Devices;
	uint32_t                       m_cDevices;  //!< Number of erver created Devices

};


#endif /* Test_H_ */
