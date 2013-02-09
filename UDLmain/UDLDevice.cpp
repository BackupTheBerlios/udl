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
#include "../lib/tinyxml2/tinyxml2.h"
using namespace tinyxml2;

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
      LoadFunction( (void**)&m_pfnGetDeviceInfo, "GetDeviceInfo" );

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

bool UDLDevice::GetDeviceInfo( const char* pszName, char* pszDeviceInfo, uint32_t* cDeviceInfo )
{
   bool fRet = false;

   if( m_pfnGetDeviceInfo ){
      fRet = ( m_pfnGetDeviceInfo( pszName, pszDeviceInfo, cDeviceInfo ) == MD_NO_ERROR );
   }
   return fRet;
}

bool UDLDevice::GetDeviceInfo( const std::string& strName, std::string& strDeviceInfo )
{
   bool fRet = false;
   uint32_t cByteDevInfoStr = 0;

   fRet = GetDeviceInfo( strName.c_str(), 0, &cByteDevInfoStr );
   if( fRet ){
      cByteDevInfoStr++;
      char szDevInfo[cByteDevInfoStr];
      fRet = GetDeviceInfo( strName.c_str(), szDevInfo, &cByteDevInfoStr );
      if( fRet ){
         strDeviceInfo = szDevInfo;
      }
   }
   return fRet;
}

bool UDLDevice::GetDeviceOptions( const std::string& strName, std::vector<DeviceOptions_t>& Opt )
{
   std::string strDevInfo;
   Opt.clear();
   bool fRet = GetDeviceInfo( strName, strDevInfo );

   if( fRet ){
      XMLDocument doc;
      if( doc.Parse( strDevInfo.c_str(), strDevInfo.size() ) != XML_NO_ERROR ){
         fRet = false;
         m_LastErrorMsg = doc.GetErrorStr1();
         m_LastErrorMsg += " ";
         m_LastErrorMsg += doc.GetErrorStr2();
      }

      if( fRet ){
         XMLNode* pDevInfoNode = doc.FirstChildElement( "DeviceInfo" );
         if( pDevInfoNode ){
            XMLNode* pPara = 0;
            pPara = pDevInfoNode->FirstChildElement( "SetupParameter" );

            while( pPara ) {
               DeviceOptions_t DevOpt;
               XMLText* pText = 0;

               pText = pPara->FirstChildElement( "Name" )->FirstChild()->ToText();
               if( pText ){
                  DevOpt.Name = pText->Value();
               }
               pText = pPara->FirstChildElement( "DefaultValue" )->FirstChild()->ToText();
               if( pText ){
                  DevOpt.DefaultValue = pText->Value();
               }
               pText = pPara->FirstChildElement( "Description" )->FirstChild()->ToText();
               if( pText ){
                  DevOpt.Comment = pText->Value();
               }
               Opt.push_back( DevOpt );
               pPara = pPara->NextSiblingElement( "SetupParameter" );
            }
         }
      }
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
      UdlOut::Error << "Can not load function!" << std::endl;
      UdlOut::Error << "\t Function: " << FunctionName << std::endl;
      UdlOut::Error << "\t Module:   " << s << std::endl;
      return false;
   }
   return true;
}
