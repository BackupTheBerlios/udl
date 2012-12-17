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

#include "DynamicLib.h"
#include "StringTools.h"

DynamicLib::DynamicLib( const DynamicLib& DynLib )
{
   m_LibHandle = const_cast<DYNLIB_HANDLE>(DynLib.GetLibHandle());
   m_strLibName = const_cast<std::wstring&>(DynLib.GetLibraryName());
}

/*
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * WinApi implementation
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

#ifdef UDL_WIN32

DynamicLib::DynamicLib( )
 : m_LibHandle(0)
{
}

DynamicLib::DynamicLib( const std::wstring& strLibPath ){
   ::LoadLibraryW( strLibPath.c_str() );
}


DynamicLib::~DynamicLib(){
   FreeLibrary();
}

bool DynamicLib::LoadLibrary( const std::wstring& strLibPath ){

   FreeLibrary();
   m_LibHandle  =  ::LoadLibraryW( strLibPath.c_str() );
   return m_LibHandle != 0;
}

void DynamicLib::FreeLibrary( void ){

   if( m_LibHandle ){
      ::FreeLibrary( m_LibHandle );
      m_LibHandle = 0;
   }
}


void* DynamicLib::GetFunctionAddress( const std::string& strFooName ){
   void* pf = 0;

   if( m_LibHandle ){
      pf = (void*)::GetProcAddress( m_LibHandle,  strFooName.c_str() );
    }
   return pf;
}

#endif /* UDL_WIN32 */

/*
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Linux implementation
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
#ifdef UDL_LINUX

DynamicLib::DynamicLib( )
 : m_LibHandle(0)
{
}

DynamicLib::DynamicLib( const std::wstring& strLibPath )
{
   LoadLibrary( strLibPath );
}


DynamicLib::~DynamicLib()
{
   FreeLibrary();
}

bool DynamicLib::LoadLibrary( const std::wstring& strLibPath )
{
   bool fRet;
   FreeLibrary();
   m_strLibName = strLibPath;
   char* s = new char[StringTools::MbStrLen(strLibPath)+1];
   m_LibHandle = dlopen( StringTools::WStrToMbStr(strLibPath, s), RTLD_LAZY);
   delete[] s;
   fRet = m_LibHandle != 0;
   if( fRet == false ){
      StringTools::MbStrToWStr( dlerror(), m_strLastError );
   }
   return fRet;
}

void DynamicLib::FreeLibrary( void )
{
   if( m_LibHandle ){
  //    dlclose(m_LibHandle);
   }
}

void* DynamicLib::GetFunctionAddress( const std::string& strFooName )
{
   void* pf = 0;

   if( m_LibHandle ){
      pf = dlsym( m_LibHandle, strFooName.c_str() );
   }
   return pf;
}

#endif /* UDL_LINUX */
