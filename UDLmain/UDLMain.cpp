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

#include "../config.h"

#include <stdlib.h>
#include <iostream>
#include <string>
#include <set>
#include <sstream>
#include <exception>
#include <map>
#include <vector>

#include "UDLTask.h"
#include "UDLDevices.h"
#include "UdlDbCsv.h"
#include "UdlSettings.h"
#include "UdlStdOut.h"
#include "../UDLlib/system.h"
#include "../UDLlib/StringTools.h"

#include "../UDLlib/getopt_pp/getopt_pp.h"
using namespace GetOpt;


const wchar_t* Help( void );
const wchar_t* Version( void );
const wchar_t* Redist( void );
const wchar_t* Warranty( void );


int main( int argc, char *argv[] ){
   
   UdlSettings Settings;
   int Verbose(2);   // Set standard verbosity
   UdlOut::SetVerbosity( Verbose );

   std::string strConfigFile;

   GetOpt_pp ops(argc, argv);
   ops.exceptions(std::ios::failbit | std::ios::eofbit);

   try{
      if( ops >> OptionPresent('h', "help" ) ){
         std::wcout << Help() << std::endl;
         return EXIT_SUCCESS;
      }
      if(  ops >> OptionPresent('v', "version" ) ){
         std::wcout << Version() << std::endl;
         return EXIT_SUCCESS;
      }
      if(  ops >> OptionPresent( 'w', "warranty" ) ){
         std::wcout << Warranty() << std::endl;
         return EXIT_SUCCESS;
      }
      if(  ops >> OptionPresent( 'r', "redist" ) ){
         std::wcout << Redist() << std::endl;
         return EXIT_SUCCESS;
      }

      // Verbosite 0=Quiet - 3=Verbose
      ops >> Option( 'V', "Verbose", Verbose, 2 );
      if( UdlOut::SetVerbosity( Verbose ) != false ){
         UdlOut::Info << "Verbosity: " << Verbose << UdlOut::EndLine;
      }
      else{
         UdlOut::Error << "Wrong Verbosity: " << Verbose << UdlOut::EndLine;
      }

      ops >> Option('c', "config-file", strConfigFile );

   }
   catch(GetOpt::GetOptEx ex){
          std::wcerr << L"Error in arguments" << std::endl;
          return EXIT_FAILURE;
   }

   Settings.SetConfigFile( strConfigFile );
	Settings.ParseConfigFile();

   // scan for UDL supported devices
	// and load device modules
	UDLDevices Devices;
	Devices.LoadModules( L"./devices/" ); // TODO: perhaps this path should in an ini or so

	// Config Task
	UDLTask* pUDLTask = new UDLTask;

	// Load/Setup Devices
	UdlOut::Info << "Load/Setup Devices" << UdlOut::EndLine;
	for( size_t i = 0; i < Settings.MeasDev().size(); i++ ){
		UdlOut::Msg << "(" << Settings.MeasDev()[i].NiceName() << ")... ";

		UDLMD_STATUS result = -1;
		bool fMdCreated = false;
		std::wstring str;
		StringTools::MbStrToWStr( Settings.MeasDev()[i].MeasDev().c_str() , str );
		UDLMeasDevice* pUDLDev = Devices.GetDevice( str );
		if( pUDLDev ){
		   fMdCreated = true;
		//	fMdCreated = pUDLDev->LoadDeviceLibrary( Settings.MeasDev()[i].Library() );
		   //   fMdCreated = pUDLDev->LoadDeviceLibrary( Settings.MeasDev()[i].Library() );

		}else
			UdlOut::Error << "Failed to load: " << Settings.MeasDev()[i].MeasDev() << UdlOut::EndLine;

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


const wchar_t* Help( void ){
   return L"Help Text is missing!!";
}

const wchar_t* Version( void ){
   return L"udl 0.0.3\n"\
             "Copyright (C) 2012  Marco Hartung\n"\
             "This program comes with ABSOLUTELY NO WARRANTY; for details type 'udl --warranty'.\n"\
             "This is free software, and you are welcome to redistribute it\n"\
             "under certain conditions; type 'udl --redist' for details.";
}
const wchar_t* Redist( void ){
   return L"Redist Text is missing!!";
}
const wchar_t* Warranty( void ){
   return L"THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.\n"\
            "EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES\n"\
            "PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED,\n"\
            "INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.\n"\
            "THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.\n"\
            "SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,\n"\
            "REPAIR OR CORRECTION.";
}
