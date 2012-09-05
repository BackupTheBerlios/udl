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

#include "../UDLlib/system.h"

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

int UDLTask::SetDataBase( UdlDataBase* db ){

	m_DataBases.push_back( db );
	return 0;
}


int UDLTask::Start( void ){

	return 0;
}

void UDLTask::Work( void ){
	uint64_t StartTime;

	std::vector<SMeasValue_t> vecMeasVal( m_Devices.size() );
	UdlOut::Msg << "Start measuring..." <<  std::endl;
	UdlOut::Msg << "***************************************" << UdlOut::EndLine;
	UdlOut::Msg << "count\t";
	for( size_t i = 0; i < m_Devices.size() ; i++ ){
		UdlOut::Msg << i << "\t";
	}
	UdlOut::Msg << UdlOut::EndLine;

	size_t count(0);
	bool fExitAfterThis = false;

	while( fExitAfterThis == false ){

	   StartTime = Timer::GetTimeMs();

		if( count >= m_SampleCount-1 && m_SampleCount > 0 ){
			fExitAfterThis = true;
		}
		count++;

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

		UdlOut::Msg << count << "\t";
		for( size_t i = 0; i < vecMeasVal.size() ; i++ ){
			UdlOut::Msg << vecMeasVal[i].dMeasValue << " "<< vecMeasVal[i].szUnit <<  "|";
		}
		UdlOut::Msg <<  "\r";

		UdlOut::Msg.flush();

		uint64_t Duration = Timer::GetTimeMs() - StartTime;
		if( m_SampleTimeMs > Duration ){
		   Timer::SleepMs( m_SampleTimeMs - Duration );
		}
	}


}

int UDLTask::Stop( void ){

	return 0;
}

