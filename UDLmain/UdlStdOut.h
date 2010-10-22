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


#ifndef UDLSTDOUT_H_
#define UDLSTDOUT_H_

#include <ostream>
#include <iostream>

class UdlOut {

public :

	static const char EndLine;

	static std::ostream Error;
	static std::ostream Info;
	static std::ostream Msg;

	/*
		UDLOUT_VERB_QUIET = 0,
		UDLOUT_VERB_ERROR = 1,
		UDLOUT_VERB_STATE = 2,
		UDLOUT_VERB_INFO = 3,
	*/
	static bool SetVerbosity( int vl );

};

#endif /* UDLSTDOUT_H_ */
