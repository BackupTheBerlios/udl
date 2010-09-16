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

/*! \file UDLDataBase.cpp
 *  \brief 
 * 
 *  Author: Marco / Created: 22.10.2009
 */

#include "UDLDataBase.h"
#include <fstream>


UdlDataBase::UdlDataBase()
{
}

UdlDataBase::~UdlDataBase() {
}

bool UdlDataBase::CreateFile( std::string& FullFileName ){


	std::ofstream OutFile( FullFileName.c_str() );

	if( !OutFile ){
		return false;
	}

	mFileName = FullFileName;
	OutFile << "Test\n";

}

void UdlDataBase::SetMdNames( const std::vector<std::string>& vecMdNames ){

}

void UdlDataBase::PushData( const std::vector<SMeasValue_t>& MeasVal, const int MeasTime  ){

	std::ofstream OutFile( mFileName.c_str(), std::ios_base::app );
	if( !OutFile ) return;

	for( size_t i = 0; i < MeasVal.size(); i++ ){
		OutFile << MeasVal[i].dMeasValue << ";" << MeasVal[i].szUnit << ";" ;
	}
	OutFile << "\n";
}

void UdlDataBase::Flush( void ){


}
