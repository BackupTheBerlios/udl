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

#ifndef VC820_H_
#define VC820_H_

#include "windows.h"

#include <string>
#include "boost/thread/thread.hpp"
#include "boost/thread/mutex.hpp"

#include "../UDLMeasDev.h"

#define UDLMD_VC820_DLL_VER 0x00000001

class VC820 {

public:

	enum EERRORNBR{
		EALLOK = 0,
		ECANTOPENPORT = 1,
		ENOTINIT = 2,
		EWRONGARG = 3
	};

	VC820();

	~VC820();

	virtual UDLMD_STATUS Setup( uint32_t cArgs, char *rgpszArg[] );

	virtual UDLMD_STATUS Connect( void );

	virtual UDLMD_STATUS Disconnect( void );


	virtual UDLMD_STATUS Trigger( uint32_t iChannel );

	virtual UDLMD_STATUS GetMeasValue( uint32_t iChannel, SMeasValue_t* pMeasVal );

	virtual UDLMD_STATUS GetDeviceVerStr( char *pszDeviceVer, uint32_t cBufferLength );


protected:

	EERRORNBR GetLastError( void );
	EERRORNBR SetLastError( EERRORNBR ErrorNbr );

	bool ExitThread( void );

private:

	static void ThreadProc( VC820* pThis );

	void Measure( void );
	bool DecodeMeasage( char rgchData[], double& dValue, std::string& strUnit );


	SMeasValue_t                   m_TrigMeasVall;
	SMeasValue_t                   m_ActMeasVall;

	HANDLE                         m_hCom;
	boost::thread*                 m_pThread;
	bool                           m_fExitThread;
	boost::mutex                   m_mutex;

	std::string                    m_strSerialPort;
	EERRORNBR                      m_LastError;

};

#endif /* VC820_H_ */
