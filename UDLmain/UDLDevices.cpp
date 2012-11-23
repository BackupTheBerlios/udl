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

#include "UDLDevices.h"
#include "../UDLlib/system.h"
#include "../UDLlib/StringTools.h"
#include "UdlStdOut.h"

#include <vector>
#include <string>

UDLDevices::UDLDevices(){

}

UDLDevices::~UDLDevices(){

}

void UDLDevices::LoadModules( std::wstring strPath ){
   std::vector<std::wstring> Files;

   System::GetDirContent( strPath, Files, L".md" );
   for( size_t i = 0; i < Files.size(); i++ ) {
      std::wstring f = strPath /*+ L"/"*/ + Files[i]; // TODO: AddPath function deside if '/' is needed

      UDLDevice* pUDLDev = new UDLDevice;
      bool fMdCreated = false;
      char names[1024];
      if( pUDLDev ){
          fMdCreated = pUDLDev->LoadDeviceLibrary( f );
      }

      if( fMdCreated ){
         pUDLDev->GetDeviceNames( names, sizeof(names) );

         std::wstring  strNames;
         std::vector<std::wstring> NamesList;
         StringTools::MbStrToWStr( names, strNames );
         StringTools::Split( strNames, L",", NamesList  );

         for (int n = 0; n < NamesList.size(); n++ ){

            m_strDevMap[NamesList[n]] = pUDLDev;

            //std::string s;
            //StringTools::WStrToMbStr( NamesList[n], names );
            //UdlOut::Msg << names << UdlOut::EndLine;
         }

      }
   }
}

void UDLDevices::GetDeviceNames( std::vector<std::wstring>& DevNames )
{
   std::map<std::wstring, UDLDevice*>::const_iterator it;
   DevNames.clear();

   for( it = m_strDevMap.begin(); it != m_strDevMap.end(); it++ ){
      DevNames.push_back( it->first );
   }
}


UDLMeasDevice* UDLDevices::GetDevice( const std::wstring& strName )
{
   std::map<std::wstring, UDLDevice*>::const_iterator it = m_strDevMap.find( strName );
   UDLMeasDevice* pMd = 0;

   if( it != m_strDevMap.end() ){

      pMd = new UDLMeasDevice( it->second );
     // UDLMeasDevice* pMd = new UDLMeasDevice();
      if( pMd ){
       //  pMd->LoadDeviceLibrary( it->second->GetLib().GetLibraryName() );
         std::string s;
         StringTools::WStrToMbStr( strName, s );
         pMd->Create( s );
      }
   }
   return pMd;
}
