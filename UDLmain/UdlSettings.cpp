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

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <map>

#include <boost/config.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>
#include "boost/program_options.hpp"
namespace pod = boost::program_options::detail;

UdlSettings::UdlSettings()
{
}

UdlSettings::~UdlSettings()
{
}

bool UdlSettings::ParseConfigFile( void ){

	std::map<std::string, std::string> parameters;
	if( !mConfigFile.empty() ){
		std::cout << "Reading Config-File: " << mConfigFile << std::endl;

		std::ifstream config( mConfigFile.c_str() );
		if(!config)
		{
			std::cerr<< "Can�t open file: "<< mConfigFile << std::endl;
			return false;
		}

		//parameters
		std::set<std::string> options;
		options.insert("*");

		try{
			for (pod::config_file_iterator i(config, options), e ; i != e; ++i)
			{
				std::cout << i->string_key <<" "<<i->value[0] << std::endl;
				parameters[i->string_key] = i->value[0];
			}
		}
		catch( std::exception& e )
		{
			std::cerr << "Exception: " << e.what() << std::endl;
		}

	}

	mSampleTimeMs = atoi( parameters["Measurement.SampleTime"].c_str() );

	mSampleCount = atoi( parameters["Measurement.SampleCount"].c_str() );

	mOutFile = parameters["Out.OutFileName"];

	// Md
	mMd.clear();

	size_t i(1);
	while(  i >= 1 ){
		std::string strMeasDevName;

		char szBuff[1000];
		strMeasDevName = "MeasDev_";
		strMeasDevName += std::string( itoa(i, szBuff, 10 ) ); //TODO This is not realy up to date

		// Scan config data for devices
		if( parameters.find(strMeasDevName+".Library") != parameters.end() ){
			std::cout << "Config: " << strMeasDevName << std::endl;

			std::string Library = parameters[strMeasDevName+".Library"];
			std::string NiceName = parameters[strMeasDevName+".NiceName"];
			//:TODO: Should work with all args Arg.xx
			std::string Args = parameters[strMeasDevName+".Arg.Port"];

			std::cout << "Library: " << Library << std::endl;
			std::cout << "Args: " << Args << std::endl;
			std::cout << "NiceName: " << NiceName << std::endl;

			mMd.push_back( UdlMdSettings( NiceName, Library, Args ) );

			i++;
		}
		else{
			i = 0;
		}
	}

}
