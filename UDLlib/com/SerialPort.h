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

#ifndef SERIALPORT_H_
#define SERIALPORT_H_

#ifdef WIN32
   #include "windows.h"
#else

#endif

#include <string>


#ifdef WIN32
   #define SERPORT_HANDLE HANDLE
#else
   #define SERPORT_HANDLE int
#endif



class SerialPort  {

public:

   SerialPort();
   virtual ~SerialPort();

   virtual int Open( void );

   virtual int Setup( void );

   virtual int Close( void );

   virtual int GetPortList( void );

   virtual int Write( const void* pData, size_t DataSize );

   virtual int Read(  void* pData, size_t DataSize );


protected:

   bool BuildPortName( const char* pPortName, char* pRealPortName );

   // Get the OS Defines
//   int OsDefBaudRate( EBaudrate BaudRate );
//   int OsDefParity( EParity Parity );
//   int OsDefStop( EStopBits Stop );

   SERPORT_HANDLE m_Port;

private:


};


#endif /* SERIALPORT_H_ */
