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

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "../config.h"

#include <vector>
#include <string>
#include <inttypes.h>

class System {
public:
   System() {};
   ~System() {};

   static bool GetDirContent( const std::wstring& Dir,
                                 std::vector<std::wstring>& Files,
                                 const std::wstring& FileEnding = L"" );

   static bool GetAppDir( std::wstring& AppDir );
};


class Timer {

   public:
      Timer();
      virtual ~Timer();

      static void SleepMs( uint64_t ms );

      static uint64_t GetTimeMs( void );

   private:

};

#endif /* SYSTEM_H_ */
