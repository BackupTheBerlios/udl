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

#include "UdlSettings.h"
#include "UdlSettingsFile.h"
#include "UdlStdOut.h"

#include <sstream>
#include <map>

UdlSettings::UdlSettings()
{
}

UdlSettings::~UdlSettings()
{
}

bool UdlSettings::ParseConfigFile( void ){

	UdlSettingsFile sf;
	UdlOut::Msg << "Reading Config-File: " << mConfigFile << UdlOut::EndLine;
	sf.Parse( mConfigFile );

	sf.GetValueAsLong( "Measurement" , "SampleTime", m_SampleTimeMs );
	sf.GetValueAsLong( "Measurement" , "SampleCount", m_SampleCount );
	sf.GetValueAsString( "Out" , "OutFileName", mOutFile );

	UdlOut::Info << "SampleTime: " << m_SampleTimeMs << UdlOut::EndLine;
	UdlOut::Info << "SampleCount: " << m_SampleCount << UdlOut::EndLine;
	UdlOut::Info << "OutFileName: " << mOutFile << UdlOut::EndLine;


	// Read MeasDevIdentifier
	sf.GetValueAsStringVec( "MeasDevices", "Devices", m_DevIdentifier );

	// Read Md-config for each dev
	mMd.clear();

	for( size_t i = 0; i < m_DevIdentifier.size(); i++ ){
	   const UdlSettingsSection* pSection;

		// Scan config data for devices
		UdlOut::Info << "Scan for config of: " << m_DevIdentifier[i] << UdlOut::EndLine;
		pSection = sf.GetSection( m_DevIdentifier[i] );
		if( pSection ){
			std::string strSection;
			std::string strMeasDev;
			std::string strNiceName;

			sf.GetSectionAsString( pSection, strSection );
			//UdlOut::Info << "Config: " << strMeasDev << UdlOut::EndLine;
			//UdlOut::Info << strSection << UdlOut::EndLine;

			sf.GetValueAsString( m_DevIdentifier[i], "MeasDev", strMeasDev );
			sf.GetValueAsString( m_DevIdentifier[i], "NiceName", strNiceName );

			mMd.push_back( UdlMdSettings( strNiceName, strMeasDev, strSection ) );
		}
	}

}

