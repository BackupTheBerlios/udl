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

#include "UdlStdOut.h"

const char UdlOut::EndLine( '\n' );

std::ostream UdlOut::Error( std::cerr.rdbuf() );
std::ostream UdlOut::Info( std::cout.rdbuf() );
std::ostream UdlOut::Msg( std::cout.rdbuf() );


bool UdlOut::SetVerbosity( int vl ){
	bool fRet = true;

	switch( vl ){
	case 0 :
		Error.rdbuf( 0 );
		Msg.rdbuf( 0 );
		Info.rdbuf( 0 );
		break;
	case 1 :
		Error.rdbuf( std::cerr.rdbuf() );
		Msg.rdbuf( 0 );
		Info.rdbuf( 0 );
		break;
	case 2 :
		Error.rdbuf( std::cerr.rdbuf() );
		Msg.rdbuf( std::cout.rdbuf() );
		Info.rdbuf( 0 );
		break;
	case 3 :
		Error.rdbuf( std::cerr.rdbuf() );
		Msg.rdbuf( std::cout.rdbuf() );
		Info.rdbuf( std::cout.rdbuf() );
		break;
	default : fRet = false;
	}

	return fRet;
}
