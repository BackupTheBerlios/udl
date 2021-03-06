/*
 * UDL - Universal Data Logger
 * Copyright (C) 2010  Marco Hartung
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

/*! \file UdlMdTest.cpp
 *  \brief Simple module to UdlMdTest the UDLMD_API
 *
 */

#include "Test.h"
#include <map>
#include <cstring>
#include <string>


static std::map< uint32_t, UdlMdTest*>  gDevices;
static uint32_t                         gcDevices;  //!< Number of ever created Devices

extern "C" {

   UDLMD_API UDLMD_STATUS GetDeviceNames( char* pszNames, uint32_t* cBufferLength ){
      const std::string strDevList = "UDL-Test-MD,UDL-Test-MD2";

      if( pszNames == 0 ){
         *cBufferLength = strDevList.length();
      }
      else{
         strncpy( pszNames, strDevList.c_str(), *cBufferLength );
      }
      return MD_NO_ERROR;
   }

   UDLMD_API UDLMD_STATUS Create( UDLMD_HANDLE* phMeasDev, const char* pszName ){
      static int cNbrOfCration;

      UdlMdTest* pUdlMdTest = 0;

      if( strcmp( pszName, "UDL-Test-MD" ) == 0 ){
         cNbrOfCration++;
         pUdlMdTest = new UdlMdTest(cNbrOfCration);
      }

      if( pUdlMdTest ){
         gcDevices++;
         *phMeasDev = gcDevices;
         gDevices[gcDevices] = pUdlMdTest;
         return MD_NO_ERROR;
      }
      return MD_CANT_CREATE;
   }

   UDLMD_API UDLMD_STATUS Delete( UDLMD_HANDLE hMeasDev ){

      UdlMdTest* pDev = gDevices[hMeasDev];
      if( pDev ){
         delete pDev;
         gDevices.erase(hMeasDev);
         return MD_NO_ERROR;
      }
      return MD_INVALIDE_HANDLE;
   }

   UDLMD_API UDLMD_STATUS Setup( UDLMD_HANDLE hMeasDev, const char *pszArg, uint32_t cBufferLength){
      UdlMdTest* pDev = gDevices[hMeasDev];
      if( pDev ){
         return pDev->Setup( pszArg, cBufferLength );
      }
      return MD_INVALIDE_HANDLE;
   }

   UDLMD_API UDLMD_STATUS Connect( UDLMD_HANDLE hMeasDev ){
      UdlMdTest* pDev = gDevices[hMeasDev];
      if( pDev ){
         return pDev->Connect();
      }
      return MD_INVALIDE_HANDLE;
   }

   UDLMD_API UDLMD_STATUS Disconnect( UDLMD_HANDLE hMeasDev ){
      UdlMdTest* pDev = gDevices[hMeasDev];
      if( pDev ){
         return pDev->Disconnect();
      }
      return MD_INVALIDE_HANDLE;
   }


   UDLMD_API UDLMD_STATUS Trigger( UDLMD_HANDLE hMeasDev, uint32_t iChannel ){
      UdlMdTest* pDev = gDevices[hMeasDev];
      if( pDev ){
         return pDev->Trigger( iChannel );
      }
      return MD_INVALIDE_HANDLE;
   }

   UDLMD_API UDLMD_STATUS GetMeasValue( UDLMD_HANDLE hMeasDev, uint32_t iChannel, SMeasValue_t* pMeasVal ){
      UdlMdTest* pDev = gDevices[hMeasDev];
      if( pDev ){
         return pDev->GetMeasValue( iChannel, pMeasVal );
      }
      return MD_INVALIDE_HANDLE;
   }

   UDLMD_API UDLMD_STATUS GetDeviceVerStr( UDLMD_HANDLE hMeasDev, char *pszDeviceVer, uint32_t* cBufferLength ){
      UdlMdTest* pDev = gDevices[hMeasDev];
      if( pDev ){
         return pDev->GetDeviceVerStr( pszDeviceVer, *cBufferLength );
      }
      return MD_INVALIDE_HANDLE;
   }


   UDLMD_API UDLMD_STATUS GetLibraryVer( uint32_t*  pu32ApiVer, uint32_t*  pu32LibVer ){

      *pu32ApiVer = UDLMD_API_VER;
      *pu32LibVer = UDLMD_TEST_DLL_VER;
      return MD_NO_ERROR;
   }

   UDLMD_API UDLMD_STATUS GetLastMeasDevError( UDLMD_HANDLE hMeasDev, char* pszErrorMsg, uint32_t* cBufferLength ){

      return MD_NO_ERROR;
   }

   UDLMD_API UDLMD_STATUS GetDeviceInfo( const char* pszName, char* pszDeviceInfo, uint32_t* cBufferLength )
   {

   static const char szDevInfo[] = R"( <?xml version="1.0" encoding="UTF-8"?><DeviceInfo></DeviceInfo> )";

      if( pszDeviceInfo == 0 ){
         *cBufferLength = sizeof( szDevInfo );
      }
      else{
         strncpy( pszDeviceInfo, szDevInfo, *cBufferLength );
      }
      return MD_NO_ERROR;
   }
}

UdlMdTest::UdlMdTest( int i )
{
	iInstance = i;
}

UdlMdTest::~UdlMdTest()
{
}

uint32_t UdlMdTest::Setup( const char *rgpszArg, uint32_t cBufferLength )
{
	return EALLOK;
}

uint32_t UdlMdTest::Connect( void )
{
	return EALLOK;
}

uint32_t UdlMdTest::Disconnect( void )
{
	return EALLOK;
}


uint32_t UdlMdTest::Trigger( uint32_t iChannel )
{
	return EALLOK;
}

uint32_t UdlMdTest::GetMeasValue( uint32_t iChannel, SMeasValue_t* pMeasVal )
{
	pMeasVal->dMeasValue =  100.1 * iInstance;
	strcpy( pMeasVal->szUnit, "Volt" );

	return EALLOK;
}


uint32_t UdlMdTest::GetDeviceVerStr( char *pszDeviceVer, uint32_t cBufferLength )
{
	strncpy( pszDeviceVer, "UniversalDataLogger MeasDev Test-Module", cBufferLength );
	return EALLOK;
}


