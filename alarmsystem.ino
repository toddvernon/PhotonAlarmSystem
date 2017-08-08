//------------------------------------------------------------------------------------------------------------
// AlarmSystem
//
// This is an implementation of a home alarm system build using a photon microcontroller and supporting
// cast of TTL logic chips.  It is a 48 channel (or zone) system where each channel (or zone) represents
// a discrete window or door that is instrumented with a magnetic reed type switch and a home run wire pair that
// terminate at the enclosure for this system.  The system has a set of 48 green led's on the front panel that
// show the current zone state.  The system sends transition state messages to the particle cloud and an 
// outgoing webhook in the particle cloud calls out to VictorOps to alert people to an open state of a zone.  
// An open window creates a CRITICAL event that creates an "incident" in VictorOps that begins a paging 
// policy.  When the zone is closed an "RECOVERY" event is sent to VictorOps and the incident is 
// automatically closed and the paging event is canceled.
//
//------------------------------------------------------------------------------------------------------------
//
// Notes:
// To RESET Photon push both buttons, release top button (assuming USB plug on left)
// Photon is now connected to particle but not running application code reflash to change software
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


// supporting classes to operate the connected shift register banks
#include "SN74HC595.h"
#include "SN74HC165N.h"

// supporting classes or strings, lists, and zones
#include "cxprop.h"
#include "cxstring.h"
#include "cxslist.h"
#include "cxzone.h"

// a few constants in the system
#define TOTAL_CHANNELS 48
#define NUM_PROPERTIES 6

//------------------------------------------------------------------------------------------------------------
// The LED's connected to the ledOutputShiftRegisters map to the channels through this table.
// This is largely because of the pattern of how they are physically mounted on the front panel
// to keep the wire lengths manageable.  If you want to light the first led on the panel, that is
// actually output shift position 7.  ( ie LightUp( LED_MAPPING[0] ) = 1  )
//
//------------------------------------------------------------------------------------------------------------
int LED_MAPPING [48] = {
    7, 5, 2, 0, 14, 12, 10, 8, 22, 20,          
    6, 4,  3, 1, 15, 13, 11, 9, 23, 21,        
    18, 16, 30, 28, 26, 24, 38, 36, 34, 32,     
    19, 17, 31, 29, 27, 25, 39, 37, 35, 33,     
    47, 46, 45, 44, 43, 42, 41, 40              
};

// this array is used to represent the desired state of the LED's
int zoneLEDBits[ TOTAL_CHANNELS ];


//------------------------------------------------------------------------------------------------------------
// This is the channel definition map. It indicates where the channel is located, what its 
// human readable name is (Basement North Right Window), compass location in the room, 
// what kind of zone it is (DOOR,WINDOW), its short symbolic name (B_NR_W), and if the zone
// is active or not (meaning in use).
//
// The channel index (TOTAL_CHANNELS) are numbered according to the input header block on
// the device.  In other words, zone 0 is the Garage Outside Door in this system.
//
//------------------------------------------------------------------------------------------------------------
char *chanDef[ TOTAL_CHANNELS ][ NUM_PROPERTIES ] = {
  {"Garage",         "Garage Outside Door",              "O",  "DOOR",   "G_O_D"   , "ACTIVE"   }, // 1
  {"Garage",         "Garage East Window",               "E",  "WINDOW", "G_E_W"   , "ACTIVE"   }, // 2
  {"Garage Entry",   "Door from Garage",                 "S",  "DOOR",   "GE_S_D"  , "ACTIVE"   }, // 3
  
  {"Main Entry",     "Front Door",                       "S",  "DOOR",   "ME_S_D"  , "ACTIVE"   }, // 4
  
  {"Laundry Room",   "Laundry Room Window",              "E",  "WINDOW", "L_E_W"   , "ACTIVE"   }, // 5
  
  {"Basement",       "Basement North Left Window",       "NL", "WINDOW", "B_NL_W"  , "ACTIVE"   }, // 6
  {"Basement",       "Basement North Right Window",      "NR", "WINDOW", "B_NR_W"  , "ACTIVE"   }, // 7
  {"Basement",       "Basement North Center Window",     "NC", "WINDOW", "B_NC_W"  , "ACTIVE"   }, // 8

  {"Kitchen",        "Kitchen North Center Window",      "NC", "WINDOW", "K_NC_W"  , "ACTIVE"   }, // 9
  {"Kitchen",        "Kitchen North Left Window",        "NL", "WINDOW", "K_NL_W"  , "ACTIVE"   }, // 10
  {"Kitchen",        "Kitchen North Right Window",       "NR", "WINDOW", "K_NR_W"  , "ACTIVE"   }, // 11
  {"Kitchen",        "Kitchen West Window",              "W",  "WINDOW", "K_W_W"   , "ACTIVE"   }, // 12
  
  {"Family Room",    "Family Room East Window",          "E",  "WINDOW", "FR_E_W"  , "ACTIVE"   }, // 13
  {"Family Room",    "Family Room North Right Window",   "NR", "WINDOW", "FR_NR_W" , "ACTIVE"   }, // 14
  {"Family Room",    "Family Room North Left Window",    "NL", "WINDOW", "FR_NL_W" , "ACTIVE"   }, // 15
  {"Family Room",    "Family Room Door",                 "E",  "DOOR",   "FR_E_D"  , "ACTIVE"   }, // 16
  {"Family Room",    "Family Room North Center Window",  "NC", "WINDOW", "FR_NC_W" , "ACTIVE"   }, // 17

  {"Sitting Area",   "Sitting Area North Window",        "N",  "WINDOW", "SA_N_W"  , "ACTIVE"   }, // 18
  {"Sitting Area",   "Sitting Area West Window",         "W",  "WINDOW", "SA_W_W"  , "ACTIVE"   }, // 19
  {"Sitting Area",   "Sitting Area East Door",           "E",  "DOOR",   "SA_E_D"  , "ACTIVE"   }, // 20

  {"Vinyl Room",     "South Right Window",               "SR", "WINDOW", "VR_SR_W" , "ACTIVE"   }, // 21
  {"Vinyl Room",     "South Left Window",                "SL", "WINDOW", "VR_SL_W" , "ACTIVE"   }, // 22
  {"Vinyl Room",     "West Window",                      "W",  "WINDOW", "VR_W_W"  , "ACTIVE"   }, // 23
  
  {"Master Bedroom", "Door",                             "W",  "DOOR",   "MB_W_D"  , "ACTIVE"   }, // 24
  {"Master Bedroom", "Master Bedroom North Right Window","NR", "WINDOW", "MB_NR_W" , "ACTIVE"   }, // 25
  {"Master Bedroom", "Master Bedroom North Left Window", "NL", "WINDOW", "MB_NL_W" , "ACTIVE"   }, // 26 
  
  {"Bedroom 2",      "East Window",                      "E",  "WINDOW", "B2_E_W"  , "INACTIVE" }, // 27
  
  {"Main Floor",     "Main Floor Motion Detector",       "1F", "MOTION", "1F_M"    , "INACTIVE" }, // 28
  {"Second Floor",   "Second Floor Motion Detector",     "2F", "MOTION", "2F_M"    , "INACTIVE" }, // 29
  {"Basement",       "Basement Motion Detector",         "B",  "MOTION", "B_M"     , "INACTIVE" }, // 30 

  {"Unused",         "Unused Channel",                   "X",  "X",          "X"   , "INACTIVE" }, // 31
  {"Unused",         "Unused Channel",                   "X",  "X",          "X"   , "INACTIVE" }, // 32
  {"Unused",         "Unused Channel",                   "X",  "X",          "X"   , "INACTIVE" }, // 33
  {"Unused",         "Unused Channel",                   "X",  "X",          "X"   , "INACTIVE" }, // 34
  {"Unused",         "Unused Channel",                   "X",  "X",          "X"   , "INACTIVE" }, // 35
  {"Unused",         "Unused Channel",                   "X",  "X",          "X"   , "INACTIVE" }, // 36
  {"Unused",         "Unused Channel",                   "X",  "X",          "X"   , "INACTIVE" }, // 37
  {"Unused",         "Unused Channel",                   "X",  "X",          "X"   , "INACTIVE" }, // 38
  {"Unused",         "Unused Channel",                   "X",  "X",          "X"   , "INACTIVE" }, // 39
  {"Unused",         "Unused Channel",                   "X",  "X",          "X"   , "INACTIVE" }, // 40
  {"Unused",         "Unused Channel",                   "X",  "X",          "X"   , "INACTIVE" }, // 41
  {"Unused",         "Unused Channel",                   "X",  "X",          "X"   , "INACTIVE" }, // 42
  {"Unused",         "Unused Channel",                   "X",  "X",          "X"   , "INACTIVE" }, // 43
  {"Unused",         "Unused Channel",                   "X",  "X",          "X"   , "INACTIVE" }, // 44
  {"Unused",         "Unused Channel",                   "X",  "X",          "X"   , "INACTIVE" }, // 45
  {"Unused",         "Unused Channel",                   "X",  "X",          "X"   , "INACTIVE" }, // 46
  {"Unused",         "Unused Channel",                   "X",  "X",          "X"   , "INACTIVE" }, // 47
  {"Unused",         "Unused Channel",                   "X",  "X",          "X"   , "INACTIVE" }  // 48
};

//------------------------------------------------------------------------------------------------------------
// some basic globals needed in the main file
//
//------------------------------------------------------------------------------------------------------------

int blinkState = 0;
int counter    = 0;

CxString openBracket  = "{";
CxString closeBracket = "}";
CxString quote        = "\"";
CxString colon        = ":";
CxString comma        = ",";

// interface class to the bank of shift registers that connect to zone reed switches
SN74HC165N zoneInputShiftRegister;

// interface class to the bank of output shift registers that connect to the zone led's
SN74HC595  LEDOutputShiftRegister;

// the link list that holds all the zone data
CxSList< CxZone *> zoneList;


//------------------------------------------------------------------------------------------------------------
// format_restart_json( void )
// 
// Creates a json blob that communicates that the photon has restarted.  This is used to create an
// unrecoverable incident in VictorOps that needs to be manually resolved.  This is used to 
// determine if the Alarm System has reset for any reason.
//
// EXAMPLE
// {
//    "channel_number":"SYSTEM",
//    "message_type":"CRITICAL",                    <== Used by VictorOps to create an incident and page
//    "entity_id":"SYSTEM_RESTARTED",               <== Used by VictorOps to keep track of this
//    "entity_display_name":"SYSTEM restarted",     <== What you read in the timeline
//    "state_message":"System has restarted",
//    "state_start_time":<seconds from epoch>,      <== What time did it happen
//    "free_memeory":<amount of free memory in photon>
// }
//
//------------------------------------------------------------------------------------------------------------

CxString format_restart_json( void )
{
    char buffer[100];
    
    sprintf(buffer, "%lu", Time.now() );
    CxString startTimeString = buffer;

    uint32_t freemem = System.freeMemory();
    sprintf(buffer, "%lu", freemem );
    CxString freeMemString = buffer;

    CxString id       = "SYSTEM_RESTART";
    CxString message  = "SYSTEM restarted";
    CxString severity = "CRITICAL";
    
    CxString data;
    data += openBracket;
    data += quote + "channel_number" + quote + colon + quote + "SYSTEM" + quote;
    data += comma;
    data += quote + "message_type" + quote + colon + quote + severity + quote;
    data += comma;
    data += quote + "entity_id" + quote + colon + quote + id + quote;
    data += comma;
    data += quote + "entity_display_name" + quote + colon + quote + message + quote;
    data += comma;
    data += quote + "state_message" + quote + colon + quote + "System has restarted" + quote;
    data += comma;
    data += quote + "state_start_time" + quote + colon + startTimeString.data();
    data += comma;
    data += quote + "free_memory" + quote + colon + freeMemString.data();
    data += closeBracket;

    return( data );
    
}

//------------------------------------------------------------------------------------------------------------
// format_heartbeat_json( void )
// 
// Creates a json blob that communicates a heartbeat message to the particle cloud and on to VictorOps
//  
// EXAMPLE
// {
//    "channel_number":"SYSTEM",
//    "message_type":"INFO",
//    "entity_id":"SYSTEM_HEARTBEAT",
//    "entity_display_name":"System heartbeat",
//    "state_message":"System has restarted",
//    "state_start_time":<seconds from epoch>,
//    "free_memeory":<amount of free memory in photon>
// }
//
//------------------------------------------------------------------------------------------------------------

CxString format_heartbeat_json( void )
{
    char buffer[100];
    
    sprintf(buffer, "%lu", Time.now() );
    CxString startTimeString = buffer;

    uint32_t freemem = System.freeMemory();
    sprintf(buffer, "%lu", freemem );
    CxString freeMemString = buffer;

    CxString id       = "SYSTEM_HEARTBEAT";
    CxString message  = "SYSTEM heartbeat";
    CxString severity = "INFO";
    
    CxString data;
    data += openBracket;
    data += quote + "channel_number" + quote + colon + quote + "SYSTEM" + quote;
    data += comma;
    data += quote + "message_type" + quote + colon + quote + severity + quote;
    data += comma;
    data += quote + "entity_id" + quote + colon + quote + id + quote;
    data += comma;
    data += quote + "entity_display_name" + quote + colon + quote + message + quote;
    data += comma;
    data += quote + "state_message" + quote + colon + quote + "System has restarted" + quote;
    data += comma;
    data += quote + "state_start_time" + quote + colon + startTimeString.data();
    data += comma;
    data += quote + "free_memory" + quote + colon + freeMemString.data();
    data += closeBracket;

    return( data );
}


//------------------------------------------------------------------------------------------------------------
// channel_load_list 
//
// loads the static config data for the system into a c++ zone array structure
//
//------------------------------------------------------------------------------------------------------------
    
void channel_load_list( void )
{
    char buffer[100];
    
    for (int channel=0; channel<TOTAL_CHANNELS; channel++) {
        
        int configured = FALSE;
        int activated  = FALSE;
        CxString configuredString = chanDef[channel][5];
        
        if (configuredString == "ACTIVE") {
            configured = TRUE;
        }

        CxZone *zone = new CxZone(
            channel+1,              // channel number
            chanDef[channel][0],    // room name
            chanDef[channel][1],    // description
            chanDef[channel][2],    // compass location
            chanDef[channel][3],    // sensor type
            chanDef[channel][4],    // unique id
            LED_MAPPING[ channel ], // where in output shift registers is that led
            configured,
            activated );
            
        zoneList.append( zone );
    }
}


//------------------------------------------------------------------------------------------------------------
// setLEDs 
//
// uses that array of int's that represent each zone in the system and shifts a 1 or 0 out to light 
// or unlight the led that represents the zone.
//
//------------------------------------------------------------------------------------------------------------

void setLEDs( void )
{
    for (int c=0; c<TOTAL_CHANNELS; c++) {
        LEDOutputShiftRegister.writeBit( zoneLEDBits[ c ] );
    }
    
    LEDOutputShiftRegister.latch_output();
}


//------------------------------------------------------------------------------------------------------------
// setup
//
// The Photon runtime executive calls this function once after device reset.  We use it to setup a few of the
// classes that communicate with the input and output shift registers used to check the channel zones and
// light the LED's
//
//------------------------------------------------------------------------------------------------------------

void setup()
{
    pinMode(A0, OUTPUT);
    digitalWrite(A0, HIGH);
    
    zoneInputShiftRegister = SN74HC165N( 
        D2,    // Connects to Parallel load pin the 165
        D1,    // Connects to Clock Enable pin the 165
        D0,    // Connects to the Q7 pin the 165
        D3 );  // Connects to the data pin
        
        
    LEDOutputShiftRegister = SN74HC595(
        D6,    // Connects to SHCP pin
        D5,    // Connects to STCP pin
        D4 );  // Connects to DS pin
    
    // load the channel map        
    channel_load_list( );
    
    CxString json = format_restart_json();
    Particle.publish( "access_changed" , json.data());
}


//------------------------------------------------------------------------------------------------------------
// loop
//
// The Photon executive calls this function repeatedly for the duration of the device execution. 
//
// The loop mainly consists of 4 tasks.  The first is to read the input shift registers that contain
// a bit for each zone (window, door, etc) in the system and update the zone list in memory.  The second tasks
// looks at the updated zone list and sends a message to particle if that state of any zone has changed.  The 
// third looks at the updated zones and adjusts the front panel LED's.  The fourth, looks at the updated
// zones and opens or closes the circuit on a passthrough to the existing alarm system basically OR'ing
// all the individual zones into one zone for the entire system.  Finally the last thing is a quarter second
// delay before returning control to the executive.
//
//------------------------------------------------------------------------------------------------------------

void loop()
{
    
    //--------------------------------------------------------------------------------------------------------
    // initialize all the LED bits to off, we will turn on the ones that have closed zone below 
    // which are usually most or all of them.
    //
    //--------------------------------------------------------------------------------------------------------
    for (int c=0; c<TOTAL_CHANNELS; c++) {
        zoneLEDBits[c] = 0;
    }
    
    //--------------------------------------------------------------------------------------------------------
    // send a heardbeat message once an hour'ish.  In mu system with my processing we can run the loop about
    // 14248 times at 4hz.  Could do this on actual clock time if we wanted it exact.
    //
    //--------------------------------------------------------------------------------------------------------
    if (++counter == 14248) {
    
        CxString json = format_heartbeat_json();
        Particle.publish( "access_changed" , json.data());
        counter = 0;
    }
    
    
    //========================================================================================================
    //========================================================================================================
    // loop through each of the 48 zones in the system, and update the zone list
    //
    //========================================================================================================
    //========================================================================================================

    // load the 48 bits into the cascade input shift register. The 48 bits represent each window or door
    // in the house that have reed switches on them and home runned to the location of the security system
    
    zoneInputShiftRegister.load_latch();

    // loop through reading each bit (zone) and updationg or zone list of objects to their new state
    // the zone will set a flag in itself to signifiy it has changed state so we can use that in later
    // processing to send messages and light led's
    
    for (int c=0; c<zoneList.entries(); c++) {

        // read the current output bit representing the window or door 
        // (window open == 5 volts, window closed == 0 volts)

        int pinValue = zoneInputShiftRegister.readBit( );
        
        // get the next zone
        
        CxZone *zone = zoneList.at( c );
        
        if ( zone->configured()) {
        
            // the zone is configured ( used by the system), so set the new state in the object
            
            if (pinValue == 1) {
                zone->setZoneActivated( TRUE );
            } else {
                zone->setZoneActivated( FALSE );
            }
            
        } else {
            
            // the zone is not configured (not used in the current system) so just set it to 
            // closed state (even though its actually electically open).  These keeps you
            // from having to jumper unused zones on the input block.
        
            zone->setZoneActivated( FALSE );
        }
        
        // shift in the next bit representing the next zone

        zoneInputShiftRegister.shift();
        
    }
    
    //========================================================================================================
    //========================================================================================================
    // loop through the updated zone list now that we have read in new data.  Each time we see a zone
    // that is both configured (used in the ssystem) and changed meaning its state has changed from open
    // to closed, or closed to open, we send a change event to the partice cloud with the new state
    //
    //========================================================================================================
    //========================================================================================================
    for (int c=0; c<zoneList.entries(); c++) {

        // get the next zone 

        CxZone *zone = zoneList.at( c );
        
        if ( zone->configured() ) {
        
            if (zone->changed() ) {

                // send zone state change to particle cloud
        
                CxString json = zone->format_victorops_json();
                Particle.publish( "access_changed" , json.data());
            }
        }
    }

    //========================================================================================================
    // once again loop through the zone list and update an integer array that represents each
    // LED on the front panel of the unit.  Each zone was loaded at startup with its bit offset into
    // the output shift registers (they aren't 1:1 with zone mapping due to panel config and install errors
    // in my system)
    //
    //========================================================================================================
    for (int c=0; c<zoneList.entries(); c++) {

        // get the next zone

        CxZone *zone = zoneList.at( c );
        
        if (zone->configured()) {
        
            if (zone->activated()) {
            
                // if the zone is activated (window open) then the green led blinks at 4hz

                if (blinkState) {
                    zoneLEDBits[ zone->ledBitPosition() ] = 0;
                    blinkState=0;
                } else {
                    zoneLEDBits[ zone->ledBitPosition() ] = 1;
                    blinkState=1;
                }
            
            } else {
            
                // if the zone is not activated (window closed) the green led is on

                zoneLEDBits[ zone->ledBitPosition() ] = 1;
                
            }
        }
    }
    
    // call the function above that writes the bits out to the LED shift registers
    
    setLEDs( );
    
    //========================================================================================================
    // once again loop through each of the 48 zones in the system, if any are open then we want to open the
    // the solid state relay by sending zero volts to the A0 pin opening the zone circuit for
    // the existing security system simulating a window open.
    //
    //========================================================================================================
    
    int anyActivated = FALSE;
    
    for (int c=0; c<zoneList.entries(); c++) {

        // get the next zone
        CxZone *zone = zoneList.at( c );
        
        if (zone->configured()) {
        
            if (zone->activated()) {
                
                anyActivated = TRUE;
            }
        }
    }
    
    // if any zone in our system was found to be open, then pass that onto the existing system
    
    if (anyActivated) {
        digitalWrite(A0, LOW);
    } else {
        digitalWrite(A0, HIGH);
    }
    
    
    //========================================================================================================
    // delay a quarter second
    //
    //========================================================================================================

    delay(250);
}

