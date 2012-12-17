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

#include "../../config.h"

#ifdef UDL_WIN32
   #include "windows.h"
#endif /* UDL_WIN32 */

#ifdef UDL_LINUX
   #include <termios.h>
#endif /* UDL_LINUX */


#include <string>


#ifdef UDL_WIN32
   #define SERPORT_HANDLE HANDLE
#else
   #define SERPORT_HANDLE int
#endif



class SerialPort  {

public:

   SerialPort();
   virtual ~SerialPort();

   virtual int Open( const std::wstring& strPort,
                       long BaudRate = 9600,
                       long DataBits = 8,
                       long Parity = 0,
                       long StopBits = 1  );

   virtual int Open(  const std::string& strPort,
                        long BaudRate = 9600,
                        long DataBits = 8,
                        long Parity = 0,
                        long StopBits = 1  );

   virtual int Close( void );

   virtual int Setup(  long BaudRate = 9600,
                         long DataBits = 8,
                         long Parity = 0,
                         long StopBits = 1  );

   virtual int SetReadTimeout( long timeout );

   virtual int SetWriteTimeout( long timeout );

   virtual int SetDtr( bool State );

   virtual int SetRts( bool State );


   virtual int Write( const void* pData, size_t cData );

   virtual int Read(  void* pData, size_t cData );


   virtual std::wstring GetLastError( void ) const { return m_LastErrorMsg; };

protected:

   // Convert the options to system defined values
   virtual int GetSystemBaud(  int BaudRate );
   virtual int GetSystemDataBits(  int DataBits );
   virtual int GetSystemParity(  int Parity );
   virtual int GetSystemStopBits(  int Stopbits );

   virtual std::wstring BuildPortName( const std::wstring& port );

   SERPORT_HANDLE m_Port;

private:

   std::wstring m_LastErrorMsg;

#ifdef UDL_LINUX
   struct termios m_OldTio;
#endif /* UDL_WIN32 */

};


#endif /* SERIALPORT_H_ */
