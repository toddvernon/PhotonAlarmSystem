//------------------------------------------------------------------------------------------------------------
//  cxzone.cpp
//
//  
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

#include <cxstring.h>
#include <cxzone.h>
    
    
//------------------------------------------------------------------------------------------------------------
// CxZone::CxZone
//
//------------------------------------------------------------------------------------------------------------
CxZone::CxZone()
{
    
}


CxZone::CxZone( int zoneNumber_,
                CxString roomName_,
                CxString description_,
                CxString compassLocation_,
                CxString sensorType_,
                CxString id_,
                int ledBitPosition_,
                int configured_,
                int activated_
                ):
                _zoneNumber(zoneNumber_),
                _roomName(roomName_),
		        _description(description_),
		        _compassLocation( compassLocation_ ),
		        _sensorType( sensorType_ ),
		        _id( id_ ),
		        _ledBitPosition( ledBitPosition_),
		        _configured( configured_ ),
		        _activated( activated_ ),
		        _changed( ZONE_STATE_UNCHANGED )
{
}

//------------------------------------------------------------------------------------------------------------
// CxZone::CxZone
//
//------------------------------------------------------------------------------------------------------------
CxZone::CxZone(const CxZone& z)
{
	if (this != &z) {
		_roomName = z.roomName();
		_description = z.description();
		_compassLocation = z.compassLocation();
		_sensorType  = z.sensorType();
		_id          = z.id();
		_ledBitPosition = z.ledBitPosition();
		_configured  = z.configured();
		_activated   = z.activated();
		_zoneNumber  = z.zoneNumber();
		_changed     = z.changed();
	}
}

//------------------------------------------------------------------------------------------------------------
// CxZone::operator=
//
//------------------------------------------------------------------------------------------------------------
CxZone&
CxZone::operator=(const CxZone& z)
{
	if (this != &z) {
        _roomName        = z.roomName();
		_description     = z.description();
		_compassLocation = z.compassLocation();
		_sensorType      = z.sensorType();
		_id              = z.id();
		_ledBitPosition  = z.ledBitPosition();
		_configured      = z.configured();
		_activated       = z.activated();
		_zoneNumber      = z.zoneNumber();
		_changed         = z.changed();
	}
	return(*this);
}

//------------------------------------------------------------------------------------------------------------
// CxZone::setZoneState
//
//------------------------------------------------------------------------------------------------------------
int 
CxZone::setZoneActivated( int activated )
{
    
    if ( _activated == activated) {
        
        _changed = FALSE;
        return( _changed );
        
    }
    
    _activated = activated;
    _changed   = TRUE;
    
    return( _changed );
}


//------------------------------------------------------------------------------------------------------------
// CxZone::operator==
//
//------------------------------------------------------------------------------------------------------------
int
CxZone::operator==(const CxZone& z) const
{
    if (_id == z.id()) return (1);
    return (0);
}

//------------------------------------------------------------------------------------------------------------
// CxZone::roomName
//
//------------------------------------------------------------------------------------------------------------
CxString
CxZone::roomName(void) const
{
	return(_roomName);
}

//------------------------------------------------------------------------------------------------------------
// CxZone::description
//
//------------------------------------------------------------------------------------------------------------
CxString
CxZone::description(void) const
{
	return(_description);
}

//------------------------------------------------------------------------------------------------------------
// CxZone::compassLocation
//
//------------------------------------------------------------------------------------------------------------
CxString
CxZone::compassLocation(void) const
{
	return(_compassLocation);
}

//------------------------------------------------------------------------------------------------------------
// CxZone::sensorType
//
//------------------------------------------------------------------------------------------------------------
CxString
CxZone::sensorType(void) const
{
	return(_sensorType);
}

//------------------------------------------------------------------------------------------------------------
// CxZone::id
//
//------------------------------------------------------------------------------------------------------------
CxString
CxZone::id(void) const
{
	return(_id);
}

//------------------------------------------------------------------------------------------------------------
// CxZone::ledBitPosition
//
//------------------------------------------------------------------------------------------------------------
int
CxZone::ledBitPosition(void) const
{
    return( _ledBitPosition );
}


//------------------------------------------------------------------------------------------------------------
// CxZone::configured
//
//------------------------------------------------------------------------------------------------------------
int
CxZone::configured(void) const
{
	return(_configured);
}

//------------------------------------------------------------------------------------------------------------
// CxZone::activated
//
//------------------------------------------------------------------------------------------------------------
int
CxZone::activated(void) const
{
	return(_activated);
}

//------------------------------------------------------------------------------------------------------------
// CxZone::activated
//
//------------------------------------------------------------------------------------------------------------
int
CxZone::zoneNumber(void) const
{
	return(_zoneNumber);
}

//------------------------------------------------------------------------------------------------------------
// CxZone::changed
// 
// returns true if the last call to setZoneActivated actually changed the value of activated.  Used 
// to do processing based on state changes in the activated flag.
//
//------------------------------------------------------------------------------------------------------------
int
CxZone::changed(void) const
{
    return( _changed );
}

//------------------------------------------------------------------------------------------------------------
// CxZone::format_victorops_json
//
// Formats a json payload to notify particle cloud that a zone (window or door) has changed state
//
//------------------------------------------------------------------------------------------------------------
CxString
CxZone::format_victorops_json(void) const
{
    CxString openBracket  = "{";
    CxString closeBracket = "}";
    CxString quote        = "\"";
    CxString colon        = ":";
    CxString comma        = ",";
    
    char buffer[100];
    
    sprintf(buffer, "%lu", Time.now() );
    CxString startTimeString = buffer;

    uint32_t freemem = System.freeMemory();
    sprintf(buffer, "%lu", freemem );
    CxString freeMemString = buffer;
    
    CxString id            = _id;
    
    sprintf(buffer, "%d", _zoneNumber );
    CxString zoneNumberString = buffer;
    
    CxString messageString = _description;
    CxString severityString;
    
    if (_activated ) {
        messageString  += " is OPEN";
        severityString  = "CRITICAL";
    } else {
        messageString  += " is CLOSED";
        severityString  = "RECOVERY";
    }
    
    CxString data;
    data += openBracket;
    data += quote + "channel_number" + quote + colon + quote + zoneNumberString + quote;
    data += comma;
    data += quote + "message_type" + quote + colon + quote + severityString + quote;
    data += comma;
    data += quote + "entity_id" + quote + colon + quote + id + quote;
    data += comma;
    data += quote + "entity_display_name" + quote + colon + quote + messageString + quote;
    data += comma;
    data += quote + "state_message" + quote + colon + quote + "Some more data" + quote;
    data += comma;
    data += quote + "state_start_time" + quote + colon + startTimeString.data();
    data += comma;
    data += quote + "free_memory" + quote + colon + freeMemString.data();
    data += closeBracket;
    
    return(data);
}



