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

#include "UDLDevice.h"
#include "UdlStdOut.h"

UDLDevice::UDLDevice() {
   m_pfnGetDevNames = 0;
   m_pfnGetLibraryVer = 0;
   m_pfnGetDeviceVerStr = 0;
   m_pfnGetDeviceSetupInfo = 0;
}

UDLDevice::~UDLDevice(){

}


bool UDLDevice::LoadDeviceLibrary( const std::wstring &strLibPath ){

   if( m_Lib.LoadLibrary( strLibPath ) ){

      LoadFunction( (void**)&m_pfnGetDevNames, "GetDeviceNames" );
      LoadFunction( (void**)&m_pfnGetLibraryVer, "GetLibraryVer" );
      LoadFunction( (void**)&m_pfnGetDeviceVerStr, "GetDeviceVerStr" );
   }
   else{
      UdlOut::Error << "Can not load library!" /* << strLibPath */<< std::endl;
      return false;
   }

   return true;
}


UDLMD_STATUS UDLDevice::GetLibraryVer(uint32_t*  pu32APIVersion, uint32_t*  pu32LibVersion )
{
   uint32_t Ret = 0;
   if( m_pfnGetLibraryVer ){
      Ret = m_pfnGetLibraryVer( pu32APIVersion, pu32LibVersion );
   }
   return Ret;
}


UDLMD_STATUS UDLDevice::GetDeviceVerStr( char *pszDeviceVer, uint32_t cBufferLength )
{
   uint32_t Ret = 0;
   if( m_pfnGetDeviceVerStr ){
      Ret = m_pfnGetDeviceVerStr( pszDeviceVer, cBufferLength );
   }
   return Ret;
}

UDLMD_STATUS UDLDevice::GetDeviceSetupInfo(  const std::string& strName,
                                                     std::string& strSetupInfo )
{
   uint32_t Ret = 0;
   strSetupInfo.clear();
   if( m_pfnGetDeviceSetupInfo ){
      char s[10000];
      Ret = m_pfnGetDeviceSetupInfo( strName.c_str(), s , 1000 );
      strSetupInfo = s;
   }
   return Ret;
}


UDLMD_STATUS UDLDevice::GetDeviceNames( char *pszNames, uint32_t c ){
   uint32_t Ret = 0;
    if(m_pfnGetDevNames){
       Ret = m_pfnGetDevNames(pszNames, c);
    }
    return Ret;
}


bool UDLDevice::LoadFunction( void** pfn, const std::string &FunctionName ){


   *pfn  =  m_Lib.GetFunctionAddress( FunctionName );
   if( !(*pfn) ){
      UdlOut::Error << "Can not load function! : " << FunctionName << std::endl;
      return false;
   }

   return true;
}


