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

/*! \file UDLDevice.h
 *  \brief
 *
 *  Author: Marco / Created: 22.10.2009
 */

#ifndef UDLDEVICE_H_
#define UDLDEVICE_H_

#include "../UDLMeasDev/MeasDevTypes.h"

class UDLDevice {
public:

	UDLDevice();

	virtual ~UDLDevice();

	virtual bool LoadDeviceLibrary( const std::string &strLibPath ) = 0;

	//virtual UDLMD_STATUS Create( void ) = 0;

	//virtual UDLMD_STATUS Delete( uint32_t MeasDevID ) = 0;


	virtual UDLMD_STATUS Setup( uint32_t cArgs, char *rgpszArg[] ) = 0;

	virtual UDLMD_STATUS Connect( void ) = 0;

	virtual UDLMD_STATUS Disconnect( void ) = 0;


	virtual UDLMD_STATUS Trigger( uint32_t iChannel ) = 0;

	virtual UDLMD_STATUS GetMeasValue( uint32_t iChannel, SMeasValue_t* pMeasVal ) = 0;

	virtual UDLMD_STATUS GetDeviceVerStr( char *pszDeviceVer, uint32_t cBufferLength ) = 0;


	virtual UDLMD_STATUS GetDllVer( SDLLVersion_t *pDllVer ) = 0;

};

#endif /* UDLDEVICE_H_ */
