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

#include "UdlSettingsFile.h"
#include "UdlStdOut.h"
#include "../UDLlib/StringTools.h"

#include <fstream>
#include <algorithm>
#include <stdlib.h>


UdlSettingsFile::UdlSettingsFile()
 : m_chCommentSign('#')
 , m_chSectionStart('[')
 , m_chSectionEnd(']')
 , m_chSeparator('=')
{

}

UdlSettingsFile::~UdlSettingsFile() {
}

bool UdlSettingsFile::Parse( std::string strFilePath ){

	if( !strFilePath.empty() ){
		std::string strFileContend;

		std::ifstream file( strFilePath.c_str() );
		if(!file)
		{
//			UdlOut::Error << "Can�t open file: "<< strFilePath << UdlOut::EndLine;
			return false;
		}

		UdlSettingsSection* pSection = &m_Content;
		while( file.good() ){
			std::string strT;

			std::getline( file, strT );

			Trim( strT );

			if( *strT.begin() == m_chCommentSign )
				strT.clear();

			if( !strT.empty() ){

				if( *strT.begin() == m_chSectionStart ){
					pSection = &(m_Content.m_SubSections[strT.substr( 1, strT.find( m_chSectionEnd, 1 )-1 )]);
				}
				else{
					std::string key, val;
					Split( strT, key, val );
					pSection->m_Values[key] = val;
				}
			}
		}
	}

	return true;
}

bool UdlSettingsFile::ParseString( std::string strContend ){

	if( strContend.size() ){
		size_t iLastPos = 0;
		size_t i = 0;

		UdlSettingsSection* pSection = &(m_Content.m_SubSections[""]);
		while( i != std::string::npos ){
			std::string strT;

			strT.clear();

			i = strContend.find( '\n', iLastPos );
			strT = strContend.substr( iLastPos, i  );
			iLastPos = i+1;
			Trim( strT );

			if( *strT.begin() == m_chCommentSign )
				strT.clear();

			if( !strT.empty() ){

				if( *strT.begin() == m_chSectionStart ){
					pSection = &(m_Content.m_SubSections[strT.substr( 1, strT.find( m_chSectionEnd, 1 )-1 )]);
				}
				else{
					std::string key, val;
					Split( strT, key, val );
					pSection->m_Values[key] = val;
				}
			}
		}
	}

	return true;
}

bool UdlSettingsFile::GetValueAsString( const std::string& strSection,
		                                const std::string& strKey,
		                                std::string& str )
{
	std::map< std::string, UdlSettingsSection>::iterator itSec;
	std::map< std::string, std::string>::iterator itKey;
	bool fRet = false;

	itSec = m_Content.m_SubSections.find( strSection );
	if( itSec != m_Content.m_SubSections.end() ){

		itKey = itSec->second.m_Values.find( strKey );
 		if( itKey != itSec->second.m_Values.end() ){
 			str = itKey->second;
 			fRet = true;
		}
	}

	return fRet;
}

bool UdlSettingsFile::GetValueAsStringVec( const std::string& strSection,
                                                const std::string& strKey,
                                                std::vector<std::string>& str )
{
   std::string s;
   bool fRet = GetValueAsString( strSection, strKey, s );

   if( fRet ){
      StringTools::Split( s, ";", str );
   }
   return fRet;

}

bool UdlSettingsFile::GetValueAsStream( const std::string& strSection,
		                                const std::string& strKey,
		                                std::stringstream& stream )
{
	std::string strT;
	bool fRet =	GetValueAsString( strSection, strKey, strT );

	if( fRet ){
		stream << strT;
	}

	return fRet;
}

bool UdlSettingsFile::GetValueAsLong( const std::string& strSection,
		                              const std::string& strKey,
		                              long& l )
{
	std::stringstream sstr;
	bool fRet =	GetValueAsStream( strSection, strKey, sstr );

	if( fRet ){
		sstr >> l;
	}

	return fRet;
}

bool UdlSettingsFile::GetValueAsDouble( const std::string& strSection,
		               const std::string& strKey,
		               double& d )
{
	std::stringstream sstr;
	bool fRet =	GetValueAsStream( strSection, strKey, sstr );

	if( fRet ){
		sstr >> d;
	}

	return fRet;
}

const UdlSettingsSection* UdlSettingsFile::GetSection( const std::string& strSection )
{
	std::map< std::string, UdlSettingsSection>::iterator itSec;

	itSec = m_Content.m_SubSections.find( strSection );
	if( itSec != m_Content.m_SubSections.end() ){
		return &(itSec->second);
	}
	return 0;
}

bool UdlSettingsFile::GetSectionAsString( const UdlSettingsSection* pSection, std::string& str )
{
	bool fRet = false;

	if( pSection ){
		std::map< std::string, std::string>::const_iterator it;
		str.clear();

		for( it = pSection->m_Values.begin(); it != pSection->m_Values.end(); it++ ){
			str += it->first;
			str += m_chSeparator;
			str += it->second;
			str += '\n';
		}
		fRet = true;
	}

	return fRet;
}

void UdlSettingsFile::Trim( std::string& str ){

	str.erase( str.begin(),  std::find_if( str.begin(), str.end(), isgraph ) );
	str.erase( std::find_if( str.rbegin(), str.rend(), isgraph ).base(),  str.end() );
}

void UdlSettingsFile::Split( const std::string& str, std::string& key, std::string& val ){
	size_t iSep = str.find( m_chSeparator, 0 );

	key = str.substr( 0, iSep ) ;
	val = str.substr( iSep+1 );
	Trim( key );
	Trim( val );
}
