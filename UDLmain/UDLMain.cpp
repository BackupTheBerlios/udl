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
#include "../UDLlib/UdlSystem.h"
#include "../UDLlib/StringTools.h"

#include "../lib/getopt_pp/getopt_pp.h"
using namespace GetOpt;


const char* Help( void );
const char* Version( void );
const char* Redist( void );
const char* Warranty( void );

void ListDevices( void );
void ListDeviceOptions( const std::string& DevName );

int main( int argc, char *argv[] ){
   
   UdlSettings Settings;
   int Verbose(2);   // Set standard verbosity
   UdlOut::SetVerbosity( Verbose );

   std::string strConfigFile;

   GetOpt_pp ops(argc, argv);
   ops.exceptions(std::ios::failbit | std::ios::eofbit);

   try{
      if( ops >> OptionPresent('h', "help" ) ){
         UdlOut::Msg << Help() << std::endl;
         return EXIT_SUCCESS;
      }
      if(  ops >> OptionPresent('v', "version" ) ){
         UdlOut::Msg << Version() << std::endl;
         return EXIT_SUCCESS;
      }
      if(  ops >> OptionPresent( 'w', "warranty" ) ){
         UdlOut::Msg << Warranty() << std::endl;
         return EXIT_SUCCESS;
      }
      if(  ops >> OptionPresent( 'r', "redist" ) ){
         UdlOut::Msg << Redist() << std::endl;
         return EXIT_SUCCESS;
      }
      if( ops >> OptionPresent('d', "list-devices" ) ){
         ListDevices();
         return EXIT_SUCCESS;
      }
      if( ops >> OptionPresent('o', "list-options" ) ){
         std::string strDevName;
         ops >> Option('o', "list-options", strDevName );
         ListDeviceOptions( strDevName );
         return EXIT_SUCCESS;
      }

      // Verbosite 0=Quiet - 3=Verbose--
      ops >> Option( 'V', "Verbose", Verbose, 2 );
      if( UdlOut::SetVerbosity( Verbose ) != false ){
         UdlOut::Info << "Verbosity: " << Verbose << UdlOut::EndLine;
      }
      else{
         UdlOut::Error << "Wrong Verbosity: " << Verbose << UdlOut::EndLine;
      }

      std::vector<std::string> args;
      ops >> GlobalOption(args);
      if( args.size() >= 1 ){
         strConfigFile = args[0];
      }

   }
   catch(GetOpt::GetOptEx ex){
      UdlOut::Error << "Error in arguments!" << UdlOut::EndLine;
      UdlOut::Msg << Help() << UdlOut::EndLine;
      return EXIT_FAILURE;
   }


   Settings.SetConfigFile( strConfigFile );
   if( Settings.ParseConfigFile() == false ){
      UdlOut::Error << "Failed to read file: \"" << strConfigFile << "\"" << UdlOut::EndLine;
      return EXIT_FAILURE;
   }
   // Print verbose configuration info
   UdlOut::Info << "File: " << strConfigFile << UdlOut::EndLine;
   // Section Measurement
   UdlOut::Info << "SampleTime: " << Settings.SampleTimeMs() << UdlOut::EndLine;
   UdlOut::Info << "SampleCount: " << Settings.SampleCount() << UdlOut::EndLine;
   // Section Out
   UdlOut::Info << "OutFileName: " << Settings.OutFile() << UdlOut::EndLine;
   // MeasDevices
   UdlOut::Info << Settings.MeasDev().size() << " MeasDevices:" << UdlOut::EndLine;
   std::vector<UdlMdSettings>::const_iterator it;
   for( it = Settings.MeasDev().begin(); it != Settings.MeasDev().end(); it++ ){
      UdlOut::Info << it->NiceName() << UdlOut::EndLine;
      UdlOut::Info << "\t" << it->DevName() << UdlOut::EndLine;
      // TODO: print args nice
      UdlOut::Info << "\t" << it->Args() << UdlOut::EndLine;
   }

   // scan for UDL supported devices
	// and load device modules
	UDLDevices Devices;
	std::wstring strModulePath;
	System::GetDevicesDir( strModulePath );
	Devices.LoadModules( strModulePath );

	// Config Task
	UDLTask* pUDLTask = new UDLTask;

	// Load/Setup Devices
	UdlOut::Msg << "Load Devices:" << UdlOut::EndLine;
	for( size_t i = 0; i < Settings.MeasDev().size(); i++ ){
		UdlOut::Msg << "(" << Settings.MeasDev()[i].NiceName() << ")... "; UdlOut::Msg.flush();

		bool result = false;
		std::string ErrMsg;
		UDLMeasDevice* pUDLDev = Devices.GetMeasDevice( Settings.MeasDev()[i].DevName() );
		if( pUDLDev ){
		   result = pUDLDev->Setup( Settings.MeasDev()[i].Args() );
         if( result ){
            result = pUDLDev->Connect();
         }

         if( result ){
            pUDLTask->SetDevice( pUDLDev );
         }
         else{
            pUDLDev->GetLastErrorMessage( ErrMsg );
         }
		}
		else{
		   ErrMsg = "Failed to load: ";
		   ErrMsg += Settings.MeasDev()[i].DevName();
		}

		if( result != false ){
		   UdlOut::Msg << "done" << UdlOut::EndLine;
		}
		else{
		   UdlOut::Error << "failed!" << UdlOut::EndLine;
		   UdlOut::Error << ErrMsg << UdlOut::EndLine;
		   return EXIT_FAILURE;
		}
	}

	pUDLTask->SetSampleTime( Settings.SampleTimeMs()  );
	pUDLTask->SetSampleCount( Settings.SampleCount() );

	// Config DataBase
	UdlDbCsv* pUdlDb = new UdlDbCsv;
	std::vector<std::string> nn;
	Settings.MesDevNiceNames( nn );
	pUdlDb->SetMdNames( nn );
	if( pUdlDb->CreateDb( Settings.OutFile() ) != false ){
	   UdlOut::Msg << "Write data to: " << Settings.OutFile() << UdlOut::EndLine;
	}
	else{
	   UdlOut::Error << "Failed to open: " << Settings.OutFile()  << UdlOut::EndLine;
	   return EXIT_FAILURE;
	}

	pUDLTask->SetDataBase( pUdlDb );
	pUDLTask->Start();

	// TODO: Start task as thread and do here some interactive stuff (break, stop,)
	pUDLTask->Work();

	return EXIT_SUCCESS;
}


void ListDevices( void ){

   // scan for UDL supported devices
   // and load device modules
   UDLDevices Devices;
   std::wstring strModulePath;
   System::GetDevicesDir( strModulePath );
   Devices.LoadModules( strModulePath );

   std::vector<std::string> DevNames;
   Devices.GetDeviceNames( DevNames );

   UdlOut::Msg << "List of all available devices:" << std::endl;
   for( size_t i = 0; i < DevNames.size(); i++ ){
      UdlOut::Msg << DevNames[i] << std::endl;
   }

}

void ListDeviceOptions( const std::string& DevName ){

   UDLDevices Devices;
   std::wstring strModulePath;
   System::GetDevicesDir( strModulePath );
   Devices.LoadModules( strModulePath );

   UDLDevice* pDev = Devices.GetDevice( DevName );

   if( pDev ){
      std::vector<UDLDevice::DeviceOptions_t> opt;
      pDev->GetDeviceOptions( DevName, opt );

      UdlOut::Msg << "Options of device \"" << DevName << "\":" << UdlOut::EndLine;
      std::vector<UDLDevice::DeviceOptions_t>::const_iterator it;
      for( it = opt.begin(); it !=  opt.end(); it++ ){
         UdlOut::Msg << "Option name    :" << it->Name << UdlOut::EndLine;
         UdlOut::Msg << "  Default value:" << it->DefaultValue << UdlOut::EndLine;
         UdlOut::Msg << "  Comment      :" << it->Comment << UdlOut::EndLine;
         UdlOut::Msg << UdlOut::EndLine;
       }
   }
   else{
      UdlOut::Error << "Device \"" << DevName << "\" not found." << UdlOut::EndLine;
   }
}

const char* Help( void ){
   return "usage: udl [MeasurementFile] [Arguments]\n"\
            "\n"\
            "Arguments:\n"\
            "   -h, --help                     Print this help message\n"\
            "   -v, --version                  Display the version number and copyrights\n"\
            "   -d, --list-devices             Print a list of all supported devices\n"\
            "   -o, --list-options <DevName>   Print a list of all available options of a device\n"\
            "   -V, --Verbose                  Set the verbosity\n"
            "                                     0 = Print nothing\n"
            "                                     1 = Print only errors\n"
            "                                     2 = Normal\n"
            "                                     3 = Verbose\n";
}

const char* Version( void ){
   return "udl 0.0.3\n"\
             "Copyright (C) 2013  Marco Hartung\n"\
             "This program comes with ABSOLUTELY NO WARRANTY; for details type 'udl --warranty'.\n"\
             "This is free software, and you are welcome to redistribute it\n"\
             "under certain conditions; type 'udl --redist' for details.";
}

const char* Redist( void ){
   return "Redist Text is missing!!";
}

const char* Warranty( void ){
   return "THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.\n"\
            "EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES\n"\
            "PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED,\n"\
            "INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.\n"\
            "THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.\n"\
            "SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,\n"\
            "REPAIR OR CORRECTION.";
}
