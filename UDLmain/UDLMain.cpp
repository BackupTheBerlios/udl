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

#include "windows.h"
#include <iostream>
#include <string>
#include <set>
#include <sstream>
#include <exception>
#include <map>

#include <boost/config.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>
#include "boost/program_options.hpp"
namespace po = boost::program_options;
namespace pod = boost::program_options::detail;

#include "UDLTask.h"
#include "UDLMeasDevice.h"
#include "UdlDbCsv.h"
#include "UdlSettings.h"
#include "UdlStdOut.h"


int main( int argc, char *argv[] ){

	UdlSettings Settings;
	UdlOut::SetVerbosity( 2 ); // Set standard Verbosity

	//Parse arguments
	try{

		po::options_description desc("Allowed options");
		desc.add_options()
			("help,h", "produce help message")
			("version,v", "udl 0.0.2")
			("config-file,f", po::value<std::string>(), "config file")
			("verbose", po::value<int>(), "Verbosite 0=Quiet - 3=Verbose")
//			("output-file,o", po::value<std::string>(), "output file")
			("warranty", "This program comes with ABSOLUTELY NO WARRANTY; for details type --warranty")
			("redist", "This is free software, and you are welcome to redistribute it under certain conditions; type --redist' for details.")
		;

		po::positional_options_description p;
		p.add("config-file", -1);


		po::variables_map vm;
		//po::store(po::parse_command_line(argc, argv, desc), vm);
		po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
		po::notify(vm);

		if( vm.empty() ){
			std::cout << desc << std::endl;
			return EXIT_FAILURE;
		}
		else if (vm.count("help")) {
			std::cout << desc << std::endl;
			return EXIT_SUCCESS;
		}
		else if (vm.count("version")) {

			std::cout << "udl 0.0.2" << std::endl;
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

		if (vm.count("verbose")) {
			if( UdlOut::SetVerbosity( vm["verbose"].as<int>() ) != false ){
				UdlOut::Info << "Verbosity: " << vm["verbose"].as<int>() << UdlOut::EndLine;
			}
			else{
				UdlOut::Error << "Wrong Verbosity: " << vm["verbose"].as<int>() << UdlOut::EndLine;
			}
		}

		if (vm.count("config-file")) {
			Settings.SetConfigFile( vm["config-file"].as<std::string>() );
		}

	}
	catch( std::exception& e ){
		std::cerr << e.what() << std::endl;
		return EXIT_SUCCESS;
	}

	Settings.ParseConfigFile();

	// Config Task
	UDLTask* pUDLTask = new UDLTask;

	// Load/Setup Devices
	UdlOut::Info << "Load/Setup Devices" << UdlOut::EndLine;
	for( size_t i = 0; i < Settings.MeasDev().size(); i++ ){
		UdlOut::Msg << "Config: " << Settings.MeasDev()[i].NiceName();
		UdlOut::Msg << "(" << Settings.MeasDev()[i].Library() << ")... ";

		UDLMD_STATUS result = -1;
		bool fMdCreated = false;
		UDLMeasDevice* pUDLDev = new UDLMeasDevice;
		if( pUDLDev )
			fMdCreated = pUDLDev->LoadDeviceLibrary( Settings.MeasDev()[i].Library() );
		else
			UdlOut::Error << "Failed to load: " << Settings.MeasDev()[i].Library() << UdlOut::EndLine;

		if( fMdCreated ){
			result = pUDLDev->Setup( Settings.MeasDev()[i].Args() );
			result = pUDLDev->Connect();
		}

		if( result == 0 ){
			UdlOut::Msg << "done" << UdlOut::EndLine;
		}
		else{
			UdlOut::Msg << "failed!" << UdlOut::EndLine;
			UdlOut::Error << "Failed to setup: " << Settings.MeasDev()[i].NiceName() << UdlOut::EndLine;
			return EXIT_FAILURE;
		}
		pUDLTask->SetDevice( pUDLDev );
	}

	pUDLTask->SetSampleTime( Settings.SampleTimeMs()  );
	pUDLTask->SetSampleCount( Settings.SampleCount() );

	// Config DataBase
	UdlOut::Info << "Config DataBase" << UdlOut::EndLine;
	UdlDbCsv* pUdlDb = new UdlDbCsv;
	std::vector<std::string> nn;
	Settings.MesDevNiceNames( nn );
	pUdlDb->SetMdNames( nn );
	pUdlDb->CreateDb( Settings.OutFile() );

	pUDLTask->SetDataBase( pUdlDb );

	pUDLTask->Start();

	// TODO: Start task as thread and do here some interaktive stuff (break, stop,)
	pUDLTask->Work();

	return EXIT_SUCCESS;
}

