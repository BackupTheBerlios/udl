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

#ifndef VC820_H_
#define VC820_H_


#include "../UDLMeasDevApi.h"
#include "../MeasDevTypes.h"
#include "../../UDLlib/com/SerialPort.h"

#include <string>
#include <thread>


#define UDLMD_VC820_DLL_VER 0x00000001


class VC820 {

public:

	VC820();

	~VC820();

	virtual UDLMD_STATUS Setup( const char *pszArg, uint32_t cBufferLength );

	virtual UDLMD_STATUS Connect( void );

	virtual UDLMD_STATUS Disconnect( void );


	virtual UDLMD_STATUS Trigger( uint32_t iChannel );

	virtual UDLMD_STATUS GetMeasValue( uint32_t iChannel, SMeasValue_t* pMeasVal );

	virtual UDLMD_STATUS GetDeviceVerStr( char *pszDeviceVer, uint32_t* cBufferLength );

	virtual std::string GetLastErrorMsg( void ) const { return m_strLastError; };

	bool                           m_fExitThread;

protected:

	bool ExitThread( void );

private:

	static void Measure( void* pThis );

	static bool DecodeMeasage( char rgchData[], double& dValue, std::string& strUnit );

	SMeasValue_t   m_TrigMeasVall;
	SMeasValue_t   m_ActMeasVall;
	bool          m_MeasValValid;

	std::string    m_strLastError;

	std::string    m_strSerialPort;
	SerialPort     m_Port;

	std::thread    m_MeasThread;

};

#endif /* VC820_H_ */
