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

/*! \file UdlDataBase.h
 *  \brief 
 * 
 *  Author: Marco / Created: 22.10.2009
 */

#ifndef UDLDATABASE_H
#define UDLDATABASE_H

#include "../UDLMeasDev/MeasDevTypes.h"

#include <string>
#include <vector>

class UdlDataBase {
public:
	UdlDataBase();
	virtual ~UdlDataBase();

	virtual bool CreateFile( std::string& FullFileName );

	virtual void SetMdNames( const std::vector<std::string>& vecMdNames );

	virtual void PushData( const std::vector<SMeasValue_t>& MeasVal, const int MeasTime );

	virtual void Flush( void );
private:

	std::string mFileName;

};

#endif /* UDLDATABASE_H */
