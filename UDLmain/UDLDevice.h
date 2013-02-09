/*
 * UDL - Universal Data Logger
 * Copyright (C) 2013 Marco Hartung
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
#include <vector>

class UDLDevice {

public:

   struct DeviceOptions_t {
      std::string Name;
      std::string Comment;
      std::string DefaultValue;
      int Type; // 0 = string; 1 = integer;
   };


	UDLDevice();

	virtual ~UDLDevice();

	virtual bool LoadDeviceLibrary( const std::wstring &strLibPath );


   virtual bool GetLibraryVer( uint32_t*  pu32APIVersion, uint32_t*  pu32DLLVersion );

   virtual bool GetDeviceNames( std::vector<std::string>& Names );

   virtual bool GetDeviceInfo(  const std::string& strName, std::string& strDeviceInfo );

   virtual bool GetDeviceOptions( const std::string& strName, std::vector<DeviceOptions_t>& Opt );

   virtual DynamicLib GetLib( void ) { return m_Lib; };


protected:

   virtual bool GetDeviceNames( char* pszNames, uint32_t* c );
   virtual bool GetDeviceInfo( const char* pszName, char* pszDeviceInfo, uint32_t* cDeviceInfo );

	virtual bool LoadFunction( void** pfn, const std::string &FunctionName );

	typedef UDLMD_STATUS (*PFN_GETLIBRARYVER)( uint32_t*  pu32APIVerion, uint32_t*  pu32LibVerion );
   typedef UDLMD_STATUS (*PFN_GETDEVICENAMES)( char *pszNames, uint32_t* cBufferLength );
   typedef UDLMD_STATUS (*PFN_GETDEVICENFO)( const char* pszName, char* pszDeviceInfo, uint32_t* cBufferLength );

   PFN_GETLIBRARYVER         m_pfnGetLibraryVer;
   PFN_GETDEVICENAMES        m_pfnGetDevNames;
   PFN_GETDEVICENFO          m_pfnGetDeviceInfo;

   DynamicLib                m_Lib;

private:

   std::string               m_LastErrorMsg;

};

#endif /* UDLDEVICE_H_ */
