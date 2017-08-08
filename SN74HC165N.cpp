//------------------------------------------------------------------------------------------------------------
//  SN74HC165N.cpp
//
//  A simple wrapper class to operate one or more cascaded parallel input shift registers. 
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

#include <SN74HC165N.h>


SN74HC165N::SN74HC165N( void )
{
    _loadPin        = 100; 
    _clockEnablePin = 100;
    _clockPin       = 100;
    _dataPin        = 100;
}


//------------------------------------------------------------------------------------------------------------
// SN74HC165N::SN74HC165N
//
//------------------------------------------------------------------------------------------------------------
SN74HC165N::SN74HC165N( int loadPin_, int clockEnablePin_, int clockPin_, int dataPin_ )
{
    _loadPin = loadPin_;
    _clockEnablePin = clockEnablePin_;
    _clockPin = clockPin_;
    _dataPin  = dataPin_;
    
    // Initialize our digital pins...
    pinMode(_loadPin, OUTPUT);
    pinMode(_clockEnablePin, OUTPUT);
    pinMode(_clockPin, OUTPUT);
    pinMode(_dataPin, INPUT);

    digitalWrite(_clockPin, LOW);
    digitalWrite(_loadPin, HIGH);

}


//------------------------------------------------------------------------------------------------------------
// SN74HC165N::SN74HC165N
//
//------------------------------------------------------------------------------------------------------------
SN74HC165N::~SN74HC165N( void )
{

}

//------------------------------------------------------------------------------------------------------------
// SN74HC165N::load_latch
//
// Load the inputs to the shift register latching them so we can then start shifting them in
//
//------------------------------------------------------------------------------------------------------------

void SN74HC165N::load_latch( void )
{
    // SN74HC165N load and latch sequence
    digitalWrite(_clockEnablePin, HIGH);
    digitalWrite(_loadPin, LOW);
    
    // delay 5 usec
    delayMicroseconds(5);
    
    digitalWrite(_loadPin, HIGH);
    digitalWrite(_clockEnablePin, LOW);
}

//------------------------------------------------------------------------------------------------------------
// SN74HC165N::readBit
// 
// Read the next bit avaiable
//
//------------------------------------------------------------------------------------------------------------

int SN74HC165N::readBit( )
{
    int bitVal = digitalRead(_dataPin);
    return(bitVal);
}

//------------------------------------------------------------------------------------------------------------
// SN74HC165N::shift
//
// Shift the next bit so we can read it.
//
//------------------------------------------------------------------------------------------------------------

void SN74HC165N::shift( )
{
    digitalWrite(_clockPin, HIGH);

    // delay 5 usec
    delayMicroseconds(5);
    
    digitalWrite(_clockPin, LOW);
}


