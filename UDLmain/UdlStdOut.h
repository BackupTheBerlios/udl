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

class UdlStdOut {
public:

	enum VerbosityLevel {
		UDLOUT_VERB_ERROR = 0,
		UDLOUT_VERB_STATUS = 1,
		UDLOUT_VERB_INFO = 2,
	};

	UdlStdOut();
	virtual ~UdlStdOut();

	static void UdlPrint( std::string strMsg, int iLevel = 3 );

protected:

private:
	static VerbosityLevel mVerbLevel( UDLOUT_VERB_STATUS );
};

#endif /* UDLSTDOUT_H_ */
