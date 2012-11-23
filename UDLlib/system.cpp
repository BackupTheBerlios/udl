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

#include "system.h"

#ifdef UDL_WIN32
	#include "windows.h"
#endif /* UDL_WIN32 */

#include "StringTools.h"
#include "dirent.h"
#include <wchar.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <ctime>
//#include <cerrno>
//#include <cstring>

/* Class System */

bool System::GetDirContent( const std::wstring& Dir,
                               std::vector<std::wstring>& Files,
                               const std::wstring& FileEnding /* = "" */ )
{
   DIR *dir;
   struct dirent *ent;
   Files.clear();

   char* ps = new char[StringTools::MbStrLen(Dir)+1];
   StringTools::WStrToMbStr( Dir, ps );
   dir = opendir( ps );
   delete[] ps;
   if( dir != NULL ){

     /* print all the files and directories within directory */
     while( (ent = readdir(dir)) != NULL ){
        std::wstring s;
        StringTools::MbStrToWStr( ent->d_name, s );
        //  s.compare(s.size()-FileEnding.size(), FileEnding.size(), FileEnding )
        if(  s.size() >= FileEnding.size() &&
             s.substr( s.size()-FileEnding.size(), FileEnding.size() ) == FileEnding  )
        {
           Files.push_back( s );
        }

     }
     closedir (dir);
   }else{
      //std::string strErr = std::strerror( errno );
      return false;
   }
   return true;
}


bool System::GetAppDir( std::wstring& AppDir ){

#ifdef UDL_WIN32
    WCHAR AppFileName[ MAX_PATH ] = L"";
	DWORD res = ::GetModuleFileNameW( NULL, AppFileName, MAX_PATH );

    if( res != 0 ){
    	AppDir = std::wstring( AppFileName );
    	size_t i = AppDir.rfind( L'\\' );
    	if( i != std::wstring::npos )
    		AppDir.erase( i );
    	return true;
    }
#endif /* UDL_WIN32 */

#ifdef UDL_LINUX
   char buff[1024];
   AppDir.clear();
   ssize_t len = ::readlink("/proc/self/exe", buff, sizeof(buff)-1);
   if (len != -1) {
      buff[len] = '\0';
      StringTools::MbStrToWStr( buff, AppDir );
      // try to remove the executable name
      size_t i = AppDir.rfind( L'/' );
      if( i != std::string::npos ){
         AppDir.erase( i );
      }
      return true;
   }
#endif /* UDL_LINUX */

   return false;
}

bool System::GetDevicesDir( std::wstring& DevDir ){
   bool ret = false;

   if( GetAppDir( DevDir ) ){

#ifdef UDL_WIN32
      DevDir += L"\\devices\\";
#endif /* UDL_WIN32 */
#ifdef UDL_LINUX
      DevDir += L"/devices/";
#endif /* UDL_LINUX */

      ret = true;
   }

   return ret;
}


/* Class Timer */


Timer::Timer(){

}


Timer::~Timer(){

}


void Timer::SleepMs( uint64_t ms ){

#ifdef UDL_WIN32
	::Sleep(ms);
#endif

#ifdef UDL_LINUX
	timespec StartTime, rem;
	clock_gettime( CLOCK_REALTIME, &StartTime );

	StartTime.tv_sec += ms/1000;
	StartTime.tv_nsec += (ms%1000) * 1000000;
	if (StartTime.tv_nsec >= 1000000000L) {
		StartTime.tv_sec++ ;  StartTime.tv_nsec = StartTime.tv_nsec - 1000000000L ;
	}

	while( clock_nanosleep( CLOCK_REALTIME, TIMER_ABSTIME, &StartTime  ,&rem ) != 0 ){
	  // TODO : Errorhandling
	}
#endif

}

uint64_t Timer::GetTimeMs( void ){
	uint64_t time;

#ifdef UDL_WIN32
	// GetTickCount64(); work for vista or newer
	time = ::GetTickCount();
#endif

#ifdef UDL_LINUX
	timespec StartTime;
	clock_gettime( CLOCK_REALTIME, &StartTime );

	time = StartTime.tv_sec * 1000;
	time += StartTime.tv_nsec / 1000000;
#endif

	return time;
}


