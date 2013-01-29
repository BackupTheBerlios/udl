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

/** \file UDLMeasDev.h
 *  \brief UDLMeasDev API
 *
 *  UDLMeasDev API Version 1.1
 *
 */

#ifndef UDLMEASDEVAPI_H_
#define UDLMEASDEVAPI_H_

#include "../config.h"
#include "MeasDevTypes.h"

#ifdef UDL_WIN32
//   #ifdef BUILD_DLL /* DLL export */
   #define UDLMD_API __declspec(dllexport)
//   #else            /* DLL import */
//   #define UDLMD_API __declspec(dllimport)
//   #endif
#endif
#ifdef UDL_LINUX
   #define UDLMD_API
#endif

#define UDLMD_API_VER 0x00010002

extern "C" {

   /** Get supported Devices.
    *
    * Return a list of supported devices.
    *
    * @param pszNames [in/out] List with names e.g. "DeviceA,DeviceB".
    *                          If 0 is passed cBufferLenfth will set to the need size in Byte
    * @param cBufferLength [in/out] The length of pszNames in bytes.
    * @return UDLMD_STATUS
    */
   UDLMD_API UDLMD_STATUS GetDeviceNames( char* pszNames, uint32_t* cBufferLength );

	/** Create an UDLMeasDev instance.
	 *
	 * @param phMeasDev [out] Handle to the created instance.
	 * @param pszName [in] Name of the desired device.
	 * @return UDLMD_STATUS
	 */
	UDLMD_API UDLMD_STATUS Create( UDLMD_HANDLE* phMeasDev, const char* pszName );

	/** Delete an UDLMeasDev instance.
	 *
	 * @param hMeasDev [in] Handle to the UDLMeasDev instance.
	 * @return UDLMD_STATUS
	 */
	UDLMD_API UDLMD_STATUS Delete( UDLMD_HANDLE hMeasDev );

	/** Configure the UDLMeasDev.
	 *
	 * Config the Measurement device with an ini-File syle string
	 * e.g.: "Port=COM1\n"
	 * See the associated MeasDev documentation for details.
	 *
	 * @param hMeasDev [in] UDLMeasDevHandle.
	 * @param pszArg [in] Arguments as StringNumber of arguments.
	 * @param cBufferLength [in] Lenght of the pszArg in Byte.
	 * @return UDLMD_STATUS
	 */
	UDLMD_API UDLMD_STATUS Setup( UDLMD_HANDLE hMeasDev, const char* pszArg, uint32_t cBufferLength );

	/** Connect with the UDLMeasDev.
	 *
	 * Call this function to open e.g. ports.
	 *
	 * @param hMeasDev [in] Handle to the UDLMeasDev instance.
	 * @return UDLMD_STATUS
	 */
	UDLMD_API UDLMD_STATUS Connect( UDLMD_HANDLE hMeasDev );

	/** Disconnect the UDLMeasDev.
	 *
	 *
	 * @param hMeasDev [in] Handle to the UDLMeasDev instance.
	 * @return UDLMD_STATUS
	 */
	UDLMD_API UDLMD_STATUS Disconnect( UDLMD_HANDLE hMeasDev );

	/** Trigger the Measurement.
	 *
	 * Triger the measurement means save the actual measurement and leave
	 * this function quick.
	 *
	 * @param hMeasDev [in] Handle to the UDLMeasDev instance.
	 * @param iChannel [in] Index of the desired channel.
	 * @return UDLMD_STATUS
	 */
	UDLMD_API UDLMD_STATUS Trigger( UDLMD_HANDLE hMeasDev, uint32_t iChannel );

	/** Request the Measurement.
	 *
	 * Get the measurement of the last Trigger event.
	 *
	 * @param hMeasDev [in] Handle to the UDLMeasDev instance.
	 * @param iChannel [in] Index of the desired channel.
	 * @param pMeasVal [out] Measurement Value.
	 * @return UDLMD_STATUS
	 */
	UDLMD_API UDLMD_STATUS GetMeasValue( UDLMD_HANDLE hMeasDev, uint32_t iChannel, SMeasValue_t* pMeasVal );

	/** Get the Device Version String.
	 *
	 * Get the Device Version (If supported)
	 *
	 * @param hMeasDev [in] Handle to the UDLMeasDev instance.
	 * @param pszDeviceVer [in/out] Buffer for the version String.
	 *                              If 0 is passed cBufferLenfth will set to the need size in byte.
	 * @param cBufferLength [in/out] Size of the Buffer in byte.
	 * @return UDLMD_STATUS
	 */
	UDLMD_API UDLMD_STATUS GetDeviceVerStr( UDLMD_HANDLE hMeasDev, char *pszDeviceVer, uint32_t* cBufferLength );

	/** Get the Library Version.
	 *
	 *
	 * @param pu32APIVerion [out] The Version of the used UDLAPI.
	 * @param pu32DLLVerion [out] The Dll Version.
	 * @param pszDLLInfo [out] Dll info String.
	 * @return UDLMD_STATUS
	 */
	UDLMD_API UDLMD_STATUS GetLibraryVer( uint32_t*  pu32ApiVer, uint32_t*  pu32LibVer );

	/** Get last Error.
	 *
	 * Get the last error message of the device.
	 *
	 * @param hMeasDev [in] Handle to the UDLMeasDev instance.
	 * @param pszErrorMsg [in/out] Pointer to the buffer for the error message.
	 *                             If 0 is passed cBufferLenfth will set to the need size in byte.
	 * @param cBufferLength [in/out] Maximum size of the buffer in byte.
	 * @return UDLMD_STATUS
	 */
	UDLMD_API UDLMD_STATUS GetLastMeasDevError( UDLMD_HANDLE hMeasDev, char* pszErrorMsg, uint32_t* cBufferLength );

	/** Get Setup Information.
	 *
	 * This function write all available setup parameter to the given string pointer.
	 * Comment for the all points are required.
	 * The parameter are in the ini-file-style format e.g.:
	 * 		"#Name of used Port\n"
	 * 		"Port=COM1\n"
	 *
	 * Comment Sign: 	'#'
	 * Seperator Sign: 	'='
	 *
	 * @param pszName [in] Name of the device to be queried.
	 * @param pszSetupInfo [in/out] String buffer for the information.
	 *                              If 0 is passed cBufferLenfth will set to the need size in byte
	 * @param cBufferLength [in/out] Maximum length of the pszSetupInfo buffer.
	 * @return UDLMD_STATUS
	 */
	UDLMD_API UDLMD_STATUS GetDeviceInfo( const char* pszName, char* pszDeviceInfo, uint32_t* cBufferLength );

}

#endif /* UDLMEASDEV_H_ */
