//------------------------------------------------------------------------------------------------------------
//  SN74HC595.cpp
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

#include <SN74HC595.h>

//------------------------------------------------------------------------------------------------------------
// SN74HC595::SN74HC595
//
//------------------------------------------------------------------------------------------------------------
SN74HC595::SN74HC595( void )
{
    _DS         = 100; 
    _SHCP       = 100;
    _STCP       = 100;
}


//------------------------------------------------------------------------------------------------------------
// SN74HC595::SN74HC595
//
//------------------------------------------------------------------------------------------------------------
SN74HC595::SN74HC595( int SHCP_, int STCP_, int DS_ )
{
    _SHCP = SHCP_;
    _STCP = STCP_;
    _DS   = DS_;
    
    pinMode( _DS, OUTPUT);
    pinMode( _STCP, OUTPUT);
    pinMode( _SHCP, OUTPUT);

}

//------------------------------------------------------------------------------------------------------------
// SN74HC595::~SN74HC595
//
//------------------------------------------------------------------------------------------------------------
SN74HC595::~SN74HC595( void )
{
    
}

//------------------------------------------------------------------------------------------------------------
// SN74HC595::latch_output
//
// latch the bits that were shifted into the shift registers making their state appear on
// outputs
//
//------------------------------------------------------------------------------------------------------------

void SN74HC595::latch_output( void )
{
    digitalWrite( _STCP, LOW );
    delayMicroseconds(5);
    digitalWrite( _STCP, HIGH );
}

//------------------------------------------------------------------------------------------------------------
// SN74HC595::writeBit
//
// Shift in a 1 or 0 in the shift register.  This bit will not appear at output of shift registers
// until latch_output is called.
//
//------------------------------------------------------------------------------------------------------------
int SN74HC595::writeBit( int bit )
{
    digitalWrite( _SHCP, LOW);

    if (bit) {
        digitalWrite( _DS, HIGH);
    } else {
        digitalWrite( _DS, LOW);
    }
    
    digitalWrite( _SHCP, HIGH);
}
    


