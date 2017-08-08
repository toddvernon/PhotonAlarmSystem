//------------------------------------------------------------------------------------------------------------
//  cxzone.h
//
//  This class holds the state of the current security zones
//
//------------------------------------------------------------------------------------------------------------
// MIT License
// 
// Copyright (c) 2017 Todd Vernon
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//------------------------------------------------------------------------------------------------------------


#include <Particle.h>
#include <cxstring.h>


#ifndef _CxZONE_
#define _CxZONE_

#define ZONE_STATE_UNCHANGED 0
#define ZONE_STATE_CHANGED 1

#define CLOSED_STATE 1
#define OPEN_STATE   0

//------------------------------------------------------------------------------------------------------------
// CxPropEntry
//
//------------------------------------------------------------------------------------------------------------
class CxZone
{
  public:
 
    CxZone(void);
	// constructor

    CxZone( 
        int zoneNumber_,
        CxString roomName_,
        CxString description_,
        CxString compassLocation_,
        CxString sensorType_,
        CxString id_,
        int ledBitPosition_,
        int configured_,
        int activated_
        );
    // constructor with data

    CxZone( const CxZone& z );
	// copy constructor

	CxZone&
	operator=(const CxZone& z );
	// assignment operator

	int
    operator==(const CxZone& z) const;
	// comparison operator

    int setZoneActivated( int value );      // set the zone state, return true if its different
	CxString roomName( void ) const;        // Garage
	CxString description( void ) const;     // Garage Outside Door
	CxString compassLocation( void ) const; // Where in room sensor is
    CxString sensorType( void ) const;      // DOOR, WINDOW
    CxString id( void ) const;              // G_O_D = Garage Outside Door
    int      ledBitPosition( void ) const;  // The led output bit for the channel id
    int      configured( void ) const;      // used in the configuration or not
    int      activated( void ) const;       // zone is active (door, window, zone is active or open)
    int      zoneNumber( void ) const;      // zone number on unit
    int      changed( void ) const;
    
    CxString format_victorops_json(void) const;

    CxString _roomName;
    CxString _description;
    CxString _compassLocation;              // N, S, E, W, NL
    CxString _sensorType;
    CxString _id;
    int      _ledBitPosition;
    int      _configured;                   // TRUE == is this zone used in the system
    int      _activated;                    // TRUE == Window, Door is open, Motion is present
    int      _zoneNumber;                   // what zone number is this
    int      _changed;                      // did the zone change on the last set
};

#endif

