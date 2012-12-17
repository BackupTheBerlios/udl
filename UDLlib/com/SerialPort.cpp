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
#include "../StringTools.h"
#include <cerrno>

/*
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * WinApi implementation
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
#ifdef UDL_WIN32

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

#endif /* UDL_WIN32 */

/*
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Linux implementation
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
#ifdef UDL_LINUX

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <cstring>

SerialPort::SerialPort()
   : m_Port(-1)
{

};

SerialPort::~SerialPort()
{
   Close();
};

int SerialPort::Open( const std::wstring& strPort,
                         long BaudRate  /* = 9600 */,
                         long DataBits  /* = 8    */,
                         long Parity    /* = 0    */,
                         long StopBits  /* = 1    */  )
{
   std::string port;
   StringTools::WStrToMbStr( BuildPortName( strPort ), port );

   if( m_Port > 0 ){
      Close();
   }

   m_Port = open( port.c_str(), O_RDWR | O_NOCTTY );
   if( m_Port < 0 ){
      m_LastErrorMsg = L"Can not open: ";
      m_LastErrorMsg += strPort;
      m_LastErrorMsg += L"!\n";
      m_LastErrorMsg += L"!\n";
      m_LastErrorMsg += StringTools::MbStrToWStr( strerror(errno) );
      return -1;
   }

   tcgetattr( m_Port, &m_OldTio ); /* save current port settings */

   return Setup( BaudRate, DataBits, Parity, StopBits );
}

int SerialPort::Open( const std::string& strPort,
                         long BaudRate  /* = 9600 */,
                         long DataBits  /* = 8    */,
                         long Parity    /* = 0    */,
                         long StopBits  /* = 1    */  )
{
   std::wstring wstrPort;
   StringTools::MbStrToWStr( strPort.c_str(), wstrPort );
   return Open( wstrPort, BaudRate, DataBits, Parity, StopBits );
}

int SerialPort::Close( void )
{
   if( m_Port > 0 ){
      tcsetattr( m_Port, TCSANOW, &m_OldTio );
      close( m_Port );
   }

   m_Port  = -1;

   return 0;
}

int SerialPort::Setup( long BaudRate  /* = 9600 */,
                          long DataBits  /* = 8    */,
                          long Parity    /* = 0    */,
                          long StopBits  /* = 1    */  )
{
   struct termios newtio;

   if( m_Port < 0 ){
      m_LastErrorMsg = L"Port not ready.";
      return -1;
   }

   std::memset( &newtio, 0, sizeof(newtio) );
   newtio.c_cflag  = GetSystemBaud( BaudRate );
   newtio.c_cflag |= GetSystemDataBits( DataBits );
   newtio.c_cflag |= GetSystemParity( Parity );
   newtio.c_cflag |= GetSystemStopBits( StopBits );
   newtio.c_cflag |= CLOCAL | CREAD;
   newtio.c_iflag = IGNPAR;
   newtio.c_oflag = 0;

   /* set input mode (non-canonical, no echo,...) */
   newtio.c_lflag = 0;

   newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
   newtio.c_cc[VMIN]     = 0;   /* non blocking read */

   tcflush( m_Port, TCIFLUSH );
   tcsetattr( m_Port, TCSANOW, &newtio );

   return 0;
}

int SerialPort::SetReadTimeout( long timeout )
{
   return 0;
}

int SerialPort::SetWriteTimeout( long timeout )
{
   return 0;
}

int SerialPort::SetDtr( bool State )
{
   int r;
   int c = TIOCM_DTR;

   if( m_Port < 0 ){
      return -1;
   }

   if( State ){
     r = ioctl( m_Port, TIOCMBIC, &c );
   }
   else{
     r = ioctl( m_Port, TIOCMBIS, &c );
   }

   if( r != 0 ){
      m_LastErrorMsg = L"Failed to set the DTR line";
      r = -1;
   }

   return r;
}

int SerialPort::SetRts( bool State )
{
   int r;
   int c = TIOCM_RTS;

   if( m_Port < 0 ){
      return -1;
   }

   if( State ){
     r = ioctl( m_Port, TIOCMBIC, &c );
   }
   else{
     r = ioctl( m_Port, TIOCMBIS, &c );
   }

   if( r != 0 ){
      m_LastErrorMsg = L"Failed to set the RTS line";
      r = -1;
   }

   return r;
}


int SerialPort::Write( const void* pData, size_t DataSize )
{
   if( m_Port < 0 ){
      m_LastErrorMsg = L"Port not ready.";
      return -1;
   }
   int cByteWritten;

   cByteWritten = write( m_Port, pData, DataSize );
   if( cByteWritten < 0 ){
      m_LastErrorMsg = L"Write error occurred.";
      return 0;
   }

   return 0;
}

int SerialPort::Read(  void* pData, size_t DataSize )
{
   if( m_Port < 0 ){
      m_LastErrorMsg = L"Port not ready.";
      return -1;
   }
   int cByteRead;
   cByteRead = read( m_Port, pData, DataSize );
   return cByteRead;
}

std::wstring SerialPort::BuildPortName( const std::wstring& port ){

   return port;
}

int SerialPort::GetSystemBaud(  int BaudRate )
{
   switch( BaudRate ){
      case  2400   : return B2400;
      case  9600   : return B9600;
      case  19200  : return B19200;
      case  115200 : return B115200;
      default      : return B0;
   }
}

int SerialPort::GetSystemDataBits(  int DataBits )
{
   switch( DataBits ){
      case  5 : return CS5;
      case  6 : return CS6;
      case  7 : return CS7;
      case  8 : return CS8;
      default : return CS8;
   }
}

int SerialPort::GetSystemParity(  int Parity )
{
   switch( Parity ){
      case  0   : return 0;               // No Parity
      case  1   : return PARENB;          // Even Parity
      case  2   : return PARENB | PARODD; // Odd Parity
      default   : return 0;
   }
}

int SerialPort::GetSystemStopBits(  int Stopbits )
{
   switch( Stopbits ){
      case  1   : return 0;
      case  2   : return CSTOPB;
      default  : return 0;
   }
}


#endif /* UDL_LINUX */
