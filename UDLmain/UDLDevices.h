/*
 * UDL - Universal Data Logger
 * Copyright (C) 2012  Marco Hartung
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

#ifndef UDLDEVICES_H_
#define UDLDEVICES_H_

#include "UDLDevice.h"
#include "UDLMeasDevice.h"
#include <string>
#include <map>

class UDLDevices {

public:

   UDLDevices();
   virtual ~UDLDevices();

   void LoadModules( std::wstring strPath );

   //const std::vector<std::wstring>& DeviceNames( void ){ return m_strDevNames; };

   UDLMeasDevice* GetDevice( const std::wstring& strName );

protected:

private:

   std::map<std::wstring,UDLDevice*> m_strDevMap;
};


#endif /* UDLDEVICES_H_ */
