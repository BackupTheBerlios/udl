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

/** \file UdlDataBase.h
 *  \brief UdlDataBase API
 *
 *  UdlDataBase API Version 0.1
 *
 */

#ifndef UDLDATABASE_H_
#define UDLDATABASE_H_

#ifdef BUILD_DLL /* DLL export */
#define UDLDB_API __declspec(dllexport)
#else            /* DLL import */
#define UDLDB_API __declspec(dllimport)
#endif

#define UDLDB_API_VER 0x00000001

extern "C" {

	/** Create an UDLMeasDev instance.
	 *
	 * @param phMeasDev [out] Handle to the created instance.
	 * @return UDLMD_STATUS
	 */
	UDLDB_API UDLDB_STATUS Create( void );

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
	 * @param cArgs [in] Lenght of the String in Byte.
	 * @return UDLMD_STATUS
	 */
	UDLMD_API UDLMD_STATUS Setup( UDLMD_HANDLE hMeasDev, char* pszArg, uint32_t cArgs );

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
	 * @param pszDeviceVer [out] Buffer for the version String.
	 * @param cBufferLength [in] Size of the Buffer in characters.
	 * @return UDLMD_STATUS
	 */
	UDLMD_API UDLMD_STATUS GetDeviceVerStr( UDLMD_HANDLE hMeasDev, char *pszDeviceVer, uint32_t cBufferLength );

	/** Get the DLL Version.
	 *
	 *
	 * @param pu32APIVerion [out] The Version of the used UDLAPI.
	 * @param pu32DLLVerion [out] The Dll Version.
	 * @param pszDLLInfo [out] Dll info String.
	 * @return UDLMD_STATUS
	 */
	UDLMD_API UDLMD_STATUS GetDllVer( uint32_t*  pu32APIVerion, uint32_t*  pu32DLLVerion, char* pszDLLInfo );

	/** Get last Error.
	 *
	 * Get the last error number of the device.
	 * See the dll documentation for the error number definition.
	 *
	 * @param hMeasDev [in] Handle to the UDLMeasDev instance.
	 * @param pu32DevErrorNbr [out] The Error Nbr.
	 * @return UDLMD_STATUS
	 */
	UDLMD_API UDLMD_STATUS GetLastMeasDevError( UDLMD_HANDLE hMeasDev, uint32_t*  pu32DevErrorNbr );

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
	 * @param pszArg [out] String buffer for the information.
	 * @param cArgs [in/out] The lenth of pszArg and on exit number of written character.
	 * @return UDLMD_STATUS
	 */
	UDLMD_API UDLMD_STATUS GetSetupInfo( char* pszArg, uint32_t cArgs );


}

#endif /* UDLDATABASE_H_ */
