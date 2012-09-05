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

#ifndef DYNAMICLIB_H_
#define DYNAMICLIB_H_

#include "../config.h"

#ifdef UDL_WIN32
   #include "windows.h"
   #define DYNLIB_HANDLE HINSTANCE
#endif
#ifdef UDL_LINUX
   #include <dlfcn.h>
   #define DYNLIB_HANDLE void*
#endif

#include <string>


class DynamicLib {

public:

   DynamicLib( );
   DynamicLib( const std::wstring& strLibPath );
   DynamicLib( const DynamicLib& DynLib );
   ~DynamicLib();

   bool LoadLibrary( const std::wstring& strLibPath );

   const std::wstring& GetLibraryName( void ) const { return m_strLibName;};
   const DYNLIB_HANDLE GetLibHandle( void ) const {return m_LibHandle; };

   void FreeLibrary( void );

   void* GetFunctionAddress( const std::string& strFooName );

protected:

private:

   std::wstring   m_strLibName;
   DYNLIB_HANDLE  m_LibHandle;
};

#endif /* DYNAMICLIB_H_ */
