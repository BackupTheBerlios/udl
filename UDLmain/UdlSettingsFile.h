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

#ifndef UDLSETTINGSFILE_H_
#define UDLSETTINGSFILE_H_

#include <string>
#include <sstream>
#include <map>

struct UdlSettingsSection{
	std::map< std::string, std::string> m_Values;
	std::map< std::string, UdlSettingsSection > m_SubSections;
};


class UdlSettingsFile {

public:
	UdlSettingsFile();
	virtual ~UdlSettingsFile();

	bool Parse( std::string strFilePath );

	bool ParseString( std::string strContend );

	bool GetValueAsString( const std::string& strSection,
			               const std::string& strKey,
			               std::string& str );

	bool GetValueAsStream( const std::string& strSection,
			               const std::string& strKey,
			               std::stringstream& stream );

	bool GetValueAsLong( const std::string& strSection,
			             const std::string& strKey,
			             long& l );

	bool GetValueAsDouble( const std::string& strSection,
			               const std::string& strKey,
			               double& d );

	const UdlSettingsSection* GetSection( const std::string& strSection );

	bool GetSectionAsString( const UdlSettingsSection* pSection, std::string& str );

protected:

private:

	void Trim( std::string& str );
	void Split( const std::string& str, std::string& key, std::string& val );

	UdlSettingsSection m_Content;

	char m_chCommentSign;
	char m_chSectionStart;
	char m_chSectionEnd;
	char m_chSeparator;

};

#endif /* UDLSETTINGSFILE_H_ */
