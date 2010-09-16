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


#ifndef UDLDBCSV_H
#define UDLDBCSV_H

#include "../UDLMeasDev/MeasDevTypes.h"
#include "UDLDataBase.h"

#include <string>
#include <vector>

class UdlDbCsv : public UdlDataBase {
public:
	UdlDbCsv();
	virtual ~UdlDbCsv();

	virtual bool CreateDb( const std::string& FullFileName );

	virtual void SetMdNames( const std::vector<std::string>& vecMdNames );

	virtual void PushData( const std::vector<SMeasValue_t>& MeasVal, const int MeasTime );

	virtual void Flush( void );
private:

	std::string mFileName;
	std::vector<std::string> mMdNames;

};

#endif /* UDLDBCSV_H */
