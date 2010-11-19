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

/*! \file UDLTask.h
 *  \brief
 *
 *  Author: Marco / Created: 22.10.2009
 */

#ifndef UDLTASK_H_
#define UDLTASK_H_

#include <vector>

#include "UDLDevice.h"
#include "UDLMeasDevice.h"
#include "UDLDataBase.h"

class UDLTask {

public:
	UDLTask();
	virtual ~UDLTask();

	int SetDevice( UDLDevice* dev );
	int SetDataBase( UdlDataBase* db );

	void SetSampleTime( size_t st ) { m_SampleTimeMs = st; };
	void SetSampleCount( size_t sc ) { m_SampleCount= sc; };

	int Start( void );
	int Stop( void );

	void Work( void );

protected:


private:

	std::vector<UDLDevice*>     m_Devices;
	std::vector<UdlDataBase*>   m_DataBases;

	size_t    m_SampleTimeMs;
	size_t    m_SampleCount;

};

#endif /* UDLTASK_H_ */
