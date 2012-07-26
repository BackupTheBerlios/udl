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

#ifndef STRINGTOOLS_H_
#define STRINGTOOLS_H_

#include <string>
#include <vector>

class StringTools {

public:
   StringTools();
   virtual ~StringTools();

   static std::wstring& MbStrToWStr( const char* strIn, std::wstring& strOut );

   static char* WStrToMbStr( const std::wstring& strIn, char* strOut );
   static std::string& WStrToMbStr( const std::wstring& strIn, std::string& strOut );

   static size_t MbStrLen( const std::wstring& strIn );

   static size_t WStrLen( const char* strIn );

   static size_t Split( const std::wstring& strIn,
                         const std::wstring& strSep,
                         std::vector<std::wstring>& strListOut );

   static size_t Split( const std::string& strIn,
                         const std::string& strSep,
                         std::vector<std::string>& strListOut );

protected:
private:


};


#endif /* STRINGTOOLS_H_ */
