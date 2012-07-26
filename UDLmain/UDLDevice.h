/*
 * UDL - Universal Data Logger
 * Copyright (C) 2012 Marco Hartung
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
#include "../UDLlib/DynamicLib.h"
#include <string>

class UDLDevice {

public:

	UDLDevice();

	virtual ~UDLDevice();

	virtual bool LoadDeviceLibrary( const std::wstring &strLibPath );


   virtual UDLMD_STATUS GetLibraryVer( uint32_t*  pu32APIVersion, uint32_t*  pu32DLLVersion );

   virtual UDLMD_STATUS GetDeviceNames( char *pszNames, uint32_t c );

	virtual UDLMD_STATUS GetDeviceVerStr( char *pszDeviceVer, uint32_t cBufferLength );

   virtual UDLMD_STATUS GetDeviceSetupInfo(  const std::string& strName, std::string& strSetupInfo );

   virtual DynamicLib GetLib( void ) {return m_Lib;};


protected:

	virtual bool LoadFunction( void** pfn, const std::string &FunctionName );

   typedef UDLMD_STATUS (*PFN_GETDEVICENAMES)( char *pszNames, uint32_t cBufferLength );
   typedef UDLMD_STATUS (*PFN_GETLIBRARYVER)( uint32_t*  pu32APIVerion, uint32_t*  pu32LibVerion );
   typedef UDLMD_STATUS (*PFN_GETDEVICEVERSTR)( char *pszDeviceVer, uint32_t cBufferLength );
   typedef UDLMD_STATUS (*PFN_GETDEVICESETUPINFO)( const char* pszName, char* pszSetupInfo, uint32_t cBufferLength );

   PFN_GETLIBRARYVER         m_pfnGetLibraryVer;
   PFN_GETDEVICEVERSTR       m_pfnGetDeviceVerStr;
   PFN_GETDEVICESETUPINFO    m_pfnGetDeviceSetupInfo;
   PFN_GETDEVICENAMES        m_pfnGetDevNames;

   DynamicLib                m_Lib;

private:



};

#endif /* UDLDEVICE_H_ */
