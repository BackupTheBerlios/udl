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


#ifndef UDLSETTINGS_H
#define UDLSETTINGS_H

#include <string>
#include <vector>


class UdlMdSettings {

public:

	UdlMdSettings( const std::string& NiceName,
			       const std::string& Library,
			       const std::string& Args )
	{
		mNiceName = NiceName;
		mArgs = Args;
		mLibrary = Library;
	};
	~UdlMdSettings(){};

	const std::string& NiceName( void ) const { return mNiceName; };
	const std::string& Args( void ) const { return mArgs; };
	const std::string& Library( void ) const { return mLibrary; };

protected:

private:

	std::string mArgs;
	std::string mNiceName;
	std::string mLibrary;
};

class UdlSettings {
public:

	UdlSettings();
	virtual ~UdlSettings();

	void SetConfigFile( const std::string& s ) { mConfigFile = s; };
	void SetMd( const UdlMdSettings& md ) { mMd.push_back(md); };
	void SetOutFile( const std::string& f ) { mOutFile = f; };
	void SetSampleTime( int t ) { mSampleTimeMs = t; };
	void SetSampleCount( int c ) { mSampleCount = c; };

	bool ParseConfigFile( void );

	const std::string& ConfigFile( void ) const { return mConfigFile; };
	const std::string& OutFile( void ) const { return mOutFile; };
	const std::vector<UdlMdSettings>& MeasDev( void ) const { return mMd; };
	int SampleTimeMs( void )const { return mSampleTimeMs; };
	int SampleCount( void )const { return mSampleCount; };

	void MesDevNiceNames( std::vector<std::string>& nn ) const
	{
		nn.clear();
		for( size_t i = 0; i < mMd.size(); i++ ) nn.push_back( mMd[i].NiceName() );
	}

protected:

private:

	// General
	std::string mConfigFile;

	// Measurement
	std::string mOutFile;
	int mSampleTimeMs;
	int mSampleCount;

	std::vector<UdlMdSettings> mMd;

	// DataBase

};

#endif /* UDLSETTINGS_H */
