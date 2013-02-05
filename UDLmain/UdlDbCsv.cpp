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


#include "UdlDbCsv.h"
#include <fstream>


UdlDbCsv::UdlDbCsv()
{
}

UdlDbCsv::~UdlDbCsv() {
}

// Config the DB
void UdlDbCsv::SetMdNames( const std::vector<std::string>& vecMdNames ){

	mMdNames.clear();
	mMdNames.resize( vecMdNames.size() );
	for( size_t i = 0; i < vecMdNames.size(); i++ ){
		mMdNames[i] = vecMdNames[i];
	}
}

// Then create the DB
bool UdlDbCsv::CreateDb( const std::string& FullFileName )
{
	std::ofstream OutFile( FullFileName.c_str() );

	if( !OutFile ){
		return false;
	}

	for( size_t i = 0; i < mMdNames.size(); i++ ){
		if( i ) OutFile << ",";
		OutFile << mMdNames[i] << ".Value," << mMdNames[i] << ".Unit";

	}
	OutFile << "\n";

	mFileName = FullFileName;

	return true;
}



void UdlDbCsv::PushData( const std::vector<SMeasValue_t>& MeasVal, const int MeasTime  ){

	std::ofstream OutFile( mFileName.c_str(), std::ios_base::app );
	if( !OutFile ) return;

	for( size_t i = 0; i < MeasVal.size(); i++ ){
		if( i ) OutFile << ",";
		OutFile << MeasVal[i].dMeasValue << "," << MeasVal[i].szUnit;
	}
	OutFile << "\n";
}

void UdlDbCsv::Flush( void ){


}


