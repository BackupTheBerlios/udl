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

/*! \file MeasDevTypes.h
 *  \brief
 *
 */

#ifndef MEASDEVTYPES_H_
#define MEASDEVTYPES_H_

#include <stdint.h>

typedef unsigned long	UDLMD_HANDLE;
typedef unsigned long	UDLMD_STATUS;


#define MEASVALUE_UNIT_MAX_LENGTH 9
#define DLLVERSION_DLLINFO_MAX_LENGTH 99

enum MD_ERROR{
	MD_NO_ERROR = 0,
	MD_ERROR,
	MD_CANT_CREATE,
	MD_INVALIDE_HANDLE,
};

struct SMeasValue_t{
	double    dMeasValue;
//	double    dCoefficient;
//	uint32_t  u32TimeStamp;
	char      szUnit[MEASVALUE_UNIT_MAX_LENGTH+1];
};


#endif /* MEASDEVTYPES_H_ */
