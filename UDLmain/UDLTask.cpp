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

#include "boost/asio.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

#include "UDLTask.h"
#include "UdlStdOut.h"

UDLTask::UDLTask()
{
}

UDLTask::~UDLTask()
{
}

int UDLTask::SetDevice( UDLDevice* dev ){

	if( dev ){
		m_Devices.push_back( dev );
		return 0;
	}
	return -1;
}

int UDLTask::SetAction( UDLAction* action ){

	return 0;
}

int UDLTask::SetDataBase( UdlDataBase* db ){

	m_DataBases.push_back( db );
	return 0;
}


int UDLTask::Start( void ){

	return 0;
}

void UDLTask::Work( void ){
	boost::asio::io_service io_service;
	boost::asio::deadline_timer timer(io_service);


	std::vector<SMeasValue_t> vecMeasVal( m_Devices.size() );
	UdlOut::Msg << "Start measureing..." <<  std::endl;
	UdlOut::Msg << "***************************************" << std::endl;
	for( size_t i = 0; i < m_Devices.size() ; i++ ){
		UdlOut::Msg << i;
	}

	while(1){

		timer.expires_from_now( boost::posix_time::seconds(1) );

		for( size_t i = 0; i < m_Devices.size() ; i++ ){
			UDLMeasDevice* pMeasDev;

			pMeasDev = dynamic_cast<UDLMeasDevice*>( m_Devices.at(i) );
			if( pMeasDev ){
				pMeasDev->Trigger( 0 );
				pMeasDev->GetMeasValue( 0, &vecMeasVal[i] );
			}
		}

		m_DataBases.at(0)->PushData( vecMeasVal, 0  );
		m_DataBases.at(0)->Flush();

		for( size_t i = 0; i < vecMeasVal.size() ; i++ ){
			UdlOut::Msg << vecMeasVal[i].dMeasValue << "\t"<< vecMeasVal[i].szUnit <<  "|";
		}
		UdlOut::Msg <<  "\r";

		timer.wait();
	}


}

int UDLTask::Stop( void ){

	return 0;
}

