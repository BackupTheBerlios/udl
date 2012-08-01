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

#include "SerialPort.h"

/*
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * WinApi implementation
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
#ifdef WIN32

#include "windows.h"

SerialPort::SerialPort(){

};

SerialPort::~SerialPort(){

};

int SerialPort::Open( void ){

   m_hCom = CreateFile( m_strSerialPort.c_str(),
                        GENERIC_READ | GENERIC_WRITE,
                        0, // exclusive access
                        NULL, // no security
                        OPEN_EXISTING,
                        0, // no overlapped I/O
                        NULL); // null template

      if(m_hCom != INVALID_HANDLE_VALUE){
         ::DCB dcb;

         SetupComm(m_hCom, 128, 128); // set buffer sizes
         GetCommState(m_hCom, &dcb);

         dcb.BaudRate = 2400;
         dcb.ByteSize = 8;
         dcb.Parity = NOPARITY;
         dcb.StopBits = ONESTOPBIT;
         dcb.fAbortOnError = TRUE;
         dcb.fDtrControl = DTR_CONTROL_ENABLE;
         dcb.fRtsControl = RTS_CONTROL_DISABLE;
         dcb.fOutxCtsFlow = FALSE;
         dcb.fOutxDsrFlow = FALSE;
         dcb.fDsrSensitivity = FALSE;
         dcb.fOutX = FALSE;
         dcb.fInX = FALSE;

         if( SetCommState(m_hCom, &dcb) == TRUE ) eRet = EALLOK;


}

int SerialPort::Setup( void ){

}

int SerialPort::Close( void ){

}

int SerialPort::GetPortList( void ){

}

int SerialPort::Write( const void* pData, size_t DataSize ){

}

int SerialPort::Read(  void* pData, size_t DataSize ){

   //::ReadFile( m_hCom, rgchMsgBuffer, iAnz, &ulBytesRead, NULL);

}

#endif /* WIN32 */

/*
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Linux implementation
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
#ifndef WIN32

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

SerialPort::SerialPort()
   : m_Port(-1)
{

};

SerialPort::~SerialPort(){

};

int SerialPort::Open( void ){
   return 0;
}

int SerialPort::Setup( void ){
   return 0;
}

int SerialPort::Close( void ){
   return 0;
}

int SerialPort::GetPortList( void ){
   return 0;
}

int SerialPort::Write( const void* pData, size_t DataSize ){
   return 0;
}

int SerialPort::Read(  void* pData, size_t DataSize ){
   return 0;
}


#endif /* not WIN32 */
