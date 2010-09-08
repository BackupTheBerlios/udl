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

/*! \file main.cpp
 *  \brief
 *
 *  Author: Marco / Created: 24.06.2009
 */

#include "windows.h"
#include <iostream>
#include <string>
#include <set>
#include <sstream>
#include <exception>
#include <fstream>
#include <map>

#include <boost/config.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>
#include "boost/program_options.hpp"
namespace po = boost::program_options;
namespace pod = boost::program_options::detail;

#include "UDLTask.h"
#include "UDLMeasDevice.h"



// TODO port to new file
template <typename T>
std::basic_string<T> UDLitoa(long n,unsigned w=0){
	std::basic_stringstream<T> stream;
	if (w){
		stream.fill('0');
		stream.width(w);
	}
	stream << n;
	return stream.str();
}



int main( int argc, char *argv[] )
{

	//Parse arguments
	int uSampleTime;
	std::string strConfigFile;
	try{

		po::options_description desc("Allowed options");
		desc.add_options()
			("help,h", "produce help message")
			("version,v", "udl 0.0.1")
			("warranty", "This program comes with ABSOLUTELY NO WARRANTY; for details type --warranty")
			("redist", "This is free software, and you are welcome to redistribute it under certain conditions; type --redist' for details.")
//			("devices,d", po::value<std::string>()->multitoken(), "devices")
//			("sample-time,t", po::value<int>(&uSampleTime)->default_value(1000), "sample time [ms]")
			("config-file,f", po::value<std::string>(), "config file")
//			("output-file,o", po::value<std::string>(), "output file")
		;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);


		if (vm.count("help")) {
			std::cout << desc << "\n";
			return EXIT_SUCCESS;
		}
		else if (vm.count("version")) {

			std::cout << "udl 0.0.1" << std::endl;
			std::cout << "Copyright (C) 2010  Marco Hartung" << std::endl;
			std::cout << "This program comes with ABSOLUTELY NO WARRANTY; for details type 'udl --warranty'." << std::endl;
			std::cout << "This is free software, and you are welcome to redistribute it" << std::endl;
			std::cout << "under certain conditions; type 'udl --redist' for details." << std::endl;

			return EXIT_SUCCESS;
		}
		else if( vm.count("warranty") ){
			std::cout << "THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW. EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU. SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION." << std::endl;
			return EXIT_SUCCESS;
		}
		else if( vm.count("redist") ){
			std::cout << "" << std::endl;
			return EXIT_SUCCESS;
		}
		else if (vm.count("config-file")) {
			strConfigFile = vm["config-file"].as<std::string>();
			std::cout << "Using Config-File :" << strConfigFile << std::endl;
		}

	}
	catch( std::exception& e ){
		std::cerr << e.what() << std::endl;
	}


	// read ini
	std::map<std::string, std::string> parameters;
	if( !strConfigFile.empty() ){
		std::cout << "Reading Config-File: " << strConfigFile << std::endl;

		std::ifstream config( strConfigFile.c_str() );
		if(!config)
		{
			std::cerr<< "Can´t open file: "<< strConfigFile << std::endl;
			return 1;
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
			std::cout << parameters["Out.DataBase"] << std::endl;
		}
		catch( std::exception& e )
		{
			std::cerr << "Exception: " << e.what() << std::endl;
		}

	}

	// Config Task
	UDLTask* pUDLTask = new UDLTask;

	// Load/Setup Devices
	size_t i(1);
	while(  i >= 1 ){
		std::string strMeasDevName;

		strMeasDevName = "Devices.MeasDev";
	//	strMeasDevName += "01";//UDLitoa( i, 2 );

		switch(i){
			case 1: strMeasDevName += "01"; break;
			case 2: strMeasDevName += "02"; break;
			case 3: strMeasDevName += "03"; break;
			case 4: strMeasDevName += "04"; break;
		}

		std::cout << "Config: " << strMeasDevName << std::endl;

		// Scan config data for devices
		if( parameters.find(strMeasDevName) != parameters.end() ){
			std::string strMeasDevLibrary;
			std::string strMeasDevArgs;
			uint32_t cMeasDevArgs;

			strMeasDevLibrary = parameters[strMeasDevName];
			strMeasDevArgs = parameters[strMeasDevName+".Args"];

			UDLMeasDevice* pUDLDev = new UDLMeasDevice;
			pUDLDev->LoadDeviceLibrary( strMeasDevLibrary );
			pUDLDev->Setup( strMeasDevArgs );
			pUDLDev->Connect();

			pUDLTask->SetDevice( pUDLDev );

			i++;
		}
		else{
			i = 0;
		}
	}

//	return EXIT_SUCCESS;


/*
	char* rgpszArg[] = {"COM1"};

	UDLMeasDevice* pUDLDev = new UDLMeasDevice;
 //   pUDLDev->LoadDeviceLibrary( "VC820.dll" );

	pUDLDev->LoadDeviceLibrary( "Test.dll" );
	pUDLDev->Setup( 1, rgpszArg );

	pUDLTask->SetDevice( pUDLDev );
*/

	// Config Action
	std::cout << "Config Action" << std::endl;
	UDLAction* pUDLAction = new UDLAction;
	pUDLAction->SetSampleTime( 1000 );
	pUDLTask->SetAction( pUDLAction );

	// Config DataBase
	std::cout << "Config DataBase" << std::endl;
	UDLDataBase* pUDLDB = new UDLDataBase;
	pUDLTask->SetDataBase( pUDLDB );

	std::cout << "Start Taks" << std::endl;
	pUDLTask->Start();

	std::cout << "Task -> work" << std::endl;
	pUDLTask->Work();


	return EXIT_SUCCESS;
}

