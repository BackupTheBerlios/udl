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

/*! \file UDLAction.h
 *  \brief
 *
 *  Author: Marco / Created: 22.10.2009
 */

#ifndef UDLACTION_H_
#define UDLACTION_H_

#include <vector>

class UDLAction {

public:
	UDLAction();
	virtual ~UDLAction();

	inline void SetSampleTime( uint32_t st ) { m_SampleTimeMs = st; };
	inline uint32_t GetSampleTime( void ) const { return m_SampleTimeMs; };

protected:

private:

	uint32_t    m_SampleTimeMs;

};

#endif /* UDLACTION_H_ */
