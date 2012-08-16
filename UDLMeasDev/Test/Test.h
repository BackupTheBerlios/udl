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

#ifndef UDL_MD_TEST_H
#define UDL_MD_TEST_H

#include "../UDLMeasDevApi.h"

#include "../MeasDevTypes.h"

#define UDLMD_TEST_DLL_VER 0x00000001

class UdlMdTest {

public:

	enum EERRORNBR{
		EALLOK = 0,
	};

	UdlMdTest( int i );
	~UdlMdTest();

	virtual uint32_t Setup( const char *rgpszArg, uint32_t cBufferLength );

	virtual uint32_t Connect( void );

	virtual uint32_t Disconnect( void );


	virtual uint32_t Trigger( uint32_t iChannel );

	virtual uint32_t GetMeasValue( uint32_t iChannel, SMeasValue_t* pMeasVal );

	virtual uint32_t GetDeviceVerStr( char *pszDeviceVer, uint32_t cBufferLength );

protected:

private:

	int iInstance;

};

#endif /* UDL_MD_TEST_H */
