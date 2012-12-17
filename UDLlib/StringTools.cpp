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

#include "StringTools.h"
#include "wchar.h"
#include <limits>


StringTools::StringTools(){

}

StringTools::~StringTools(){


}

std::wstring StringTools::MbStrToWStr( const char* strIn )
{
   std::wstring str;
   MbStrToWStr( strIn, str );
   return str;
}

std::wstring& StringTools::MbStrToWStr( const char* strIn, std::wstring& strOut )
{
   size_t len = WStrLen(strIn);
   wchar_t* ws = new wchar_t[len+1];
   mbsrtowcs( ws, &strIn, len, 0 );
   ws[len]=L'\0';
   strOut = ws;
   delete[] ws;
   return strOut;
}

char* StringTools::WStrToMbStr( const std::wstring& strIn, char* strOut ){
   const wchar_t* ps = strIn.c_str();
   strOut[0] = '\0';
   size_t len = wcsrtombs( strOut, &ps, strIn.size(), 0);
   strOut[len] = '\0';
   return strOut;
}

std::string& StringTools::WStrToMbStr( const std::wstring& strIn, std::string& strOut ){
   // TODO: Didnt work
   char* psz = new char[ MbStrLen(strIn)+1 ];
   WStrToMbStr( strIn, psz );
   strOut = psz;
   delete[] psz;
}


size_t StringTools::MbStrLen( const std::wstring& strIn ){
   const wchar_t* ps = strIn.c_str();
   size_t len = wcsrtombs( 0, &ps, std::numeric_limits<std::size_t>::max(), 0 );  // It seems that -1 is needed to get it working on win32
   if( len < 0 )
      len = 0;

   return len;
}

size_t StringTools::WStrLen( const char* strIn ){

   size_t len = mbsrtowcs( 0, &strIn, std::numeric_limits<std::size_t>::max(), 0 ); // It seems that -1 is needed to get it working on win3
   if( len >0 )
      return len;

   return 0;
}

size_t StringTools::Split( const std::wstring& strIn,
                       const std::wstring& strSep,
                       std::vector<std::wstring>& strListOut )
{
   size_t pos = 0;
   size_t found = 0;
   strListOut.clear();

   do{
      found = strIn.find( strSep, pos );
      strListOut.push_back( strIn.substr( pos, found-pos ) );
      pos = found + strSep.size();
   } while( found != std::string::npos );

   return strListOut.size();
}

size_t StringTools::Split( const std::string& strIn,
                       const std::string& strSep,
                       std::vector<std::string>& strListOut )
{
   size_t pos = 0;
   size_t found = 0;
   strListOut.clear();

   do{
      found = strIn.find( strSep, pos );
      strListOut.push_back( strIn.substr( pos, found-pos ) );
      pos = found + strSep.size();
   } while( found != std::string::npos );

   return strListOut.size();
}

