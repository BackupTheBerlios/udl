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

/* Class System */

bool System::GetDirContent( const std::wstring& Dir,
                               std::vector<std::wstring>& Files,
                               const std::wstring& FileEnding /* = "" */ )
{
   DIR *dir;
   struct dirent *ent;
   Files.clear();

   char* ps = new char[StringTools::MbStrLen(Dir)+1];
   dir = opendir( StringTools::WStrToMbStr( Dir, ps ) );
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
     return false;
   }
   return true;
}


bool System::GetAppDir( std::wstring& AppDir ){

#ifdef UDL_WIN32
    WCHAR result[ MAX_PATH ];
	DWORD res = ::GetModuleFileNameW( NULL, result, MAX_PATH );

    if( res == ERROR_SUCCESS ){
    	AppDir = std::wstring( result );
    	return true;
    }
    // return std::string( result, GetModuleFileName( NULL, result, MAX_PATH ) );
#endif /* UDL_WIN32 */

#ifdef UDL_LINUX
   char buff[1024];
   AppDir.clear();
   ssize_t len = ::readlink("/proc/self/exe", buff, sizeof(buff)-1);
   if (len != -1) {
      buff[len] = '\0';
      StringTools::MbStrToWStr( buff, AppDir );
      return true;
   }
#endif /* UDL_LINUX */

   return false;
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

static uint64_t Timer::GetTimeMs( void ){
	uint64_t time;

#ifdef UDL_WIN32
	::Sleep(ms);
#endif

#ifdef UDL_LINUX
	timespec StartTime, rem;
	clock_gettime( CLOCK_REALTIME, &StartTime );

	time = StartTime.tv_sec += ms/1000;
	StartTime.tv_nsec += (ms%1000) * 1000000;
	if (StartTime.tv_nsec >= 1000000000L) {
		StartTime.tv_sec++ ;  StartTime.tv_nsec = StartTime.tv_nsec - 1000000000L ;
	}

#endif

	return time;
}


void Timer::WaitMsAndRestart( void ){
   timespec rem;

   if( m_StartTime.tv_sec == 0){
      clock_gettime( CLOCK_REALTIME, &m_StartTime );
   }

   m_StartTime.tv_sec += m_DurationMs/1000;
   m_StartTime.tv_nsec += (m_DurationMs%1000) * 1000000;
   if (m_StartTime.tv_nsec >= 1000000000L) {
      m_StartTime.tv_sec++ ;  m_StartTime.tv_nsec = m_StartTime.tv_nsec - 1000000000L ;
   }

   while( clock_nanosleep( CLOCK_REALTIME, TIMER_ABSTIME, &m_StartTime  ,&rem ) != 0 ){
      // TODO : Errorhandling
   }
}



