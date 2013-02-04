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
#include "../UDLlib/StringTools.h"

UDLDevice::UDLDevice() {
   m_pfnGetDevNames = 0;
   m_pfnGetLibraryVer = 0;
   m_pfnGetDeviceInfo = 0;
}

UDLDevice::~UDLDevice(){

}


bool UDLDevice::LoadDeviceLibrary( const std::wstring &strLibPath ){

   if( m_Lib.LoadLibrary( strLibPath ) ){

      LoadFunction( (void**)&m_pfnGetDevNames, "GetDeviceNames" );
      LoadFunction( (void**)&m_pfnGetLibraryVer, "GetLibraryVer" );

   }
   else{
      UdlOut::Error << "Can not load library!" /* << strLibPath */<< std::endl;
      return false;
   }

   return true;
}


bool UDLDevice::GetLibraryVer(uint32_t*  pu32APIVersion, uint32_t*  pu32LibVersion )
{
   bool fRet = false;
   if( m_pfnGetLibraryVer ){
      fRet = ( m_pfnGetLibraryVer( pu32APIVersion, pu32LibVersion ) == MD_NO_ERROR );
   }
   return fRet;
}


bool UDLDevice::GetDeviceInfo(  const std::string& strName,
                                    std::string& strSetupInfo )
{
   bool fRet = false;
   strSetupInfo.clear();
   if( m_pfnGetDeviceInfo ){
      char s[10000];
      fRet = ( m_pfnGetDeviceInfo( strName.c_str(), s , 1000 ) == MD_NO_ERROR );
      strSetupInfo = s;
   }
   return fRet;
}


bool UDLDevice::GetDeviceNames( char *pszNames, uint32_t* c )
{
   bool fRet = false;
    if( m_pfnGetDevNames ){
       fRet = ( m_pfnGetDevNames( pszNames, c ) == MD_NO_ERROR );
    }
    return fRet;
}

bool UDLDevice::GetDeviceNames( std::vector<std::string>& Names )
{

   bool fRet = false;
   uint32_t cByteBuffer = 0;

   fRet = GetDeviceNames( 0, &cByteBuffer );
   if( fRet ){
      cByteBuffer++;
      char szNames[cByteBuffer];
      fRet = GetDeviceNames( szNames, &cByteBuffer );
      if( fRet ){
         StringTools::Split( szNames, ",", Names );
      }
   }


   return fRet;
}

bool UDLDevice::LoadFunction( void** pfn, const std::string &FunctionName )
{
   *pfn  =  m_Lib.GetFunctionAddress( FunctionName );
   if( !(*pfn) ){
      std::string s;
      StringTools::WStrToMbStr( m_Lib.GetLibraryName(), s );
      UdlOut::Error << "Can not load function! : " << s << " : " << FunctionName << std::endl;
      return false;
   }
   return true;
}
