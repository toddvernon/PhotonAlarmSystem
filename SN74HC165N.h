//------------------------------------------------------------------------------------------------------------
//  SN74HC165N.h
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

#include <Particle.h>

//------------------------------------------------------------------------------------------------------------
// SN74HC165N_shift_reg
//
// Program to shift in the bit values from a SN74HC165N 8-bit
// parallel-in/serial-out shift register.
//
// This sketch demonstrates reading in 16 digital states from a
// pair of daisy-chained SN74HC165N shift registers while using
// only 4 digital pins on the Arduino.
//
// You can daisy-chain these chips by connecting the serial-out
// (Q7 pin) on one shift register to the serial-in (Ds pin) of
// the other.
// 
// Of course you can daisy chain as many as you like while still
// using only 4 Arduino pins (though you would have to process
// them 4 at a time into separate unsigned long variables).
//
//------------------------------------------------------------------------------------------------------------
//  Based on code from
//  http://playground.arduino.cc/Code/ShiftRegSN74HC165N
//
//                       ----o---
// D0     |    SH/LD  1 |        | 16  VCC       | POWER
// D3     |      CLK  2 |        | 15  CLK INH   | D1
// SWITCH |        E  3 |        | 14  D         | SWITCH
// SWITCH |        F  4 |        | 13  C         | SWITCH
// SWITCH |        G  5 |        | 12  B         | SWITCH
// SWITCH |        H  6 |        | 11  A         | SWITCH
//        |   QH NOT  7 |        | 10  SER       | QH FROM PREVEOUS STAGE
// GROUND |      GND  8 |        | 9   QH        | D2
//                       --------
//
//------------------------------------------------------------------------------------------------------------

#include <cxstring.h>

#ifndef _SN74HC165_
#define _SN74HC165_

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


//------------------------------------------------------------------------------------------------------------
// class CxTime
//
//------------------------------------------------------------------------------------------------------------
class SN74HC165N
{
  public:
  
    SN74HC165N( void );
    // constructor

	SN74HC165N( int loadPin_, int clockEnablePin_, int clockPin_, int dataPin_ );
	// constructor with pins enabled

   ~SN74HC165N( void );
	// destructor

    void load_latch( void );
    // load a new value
    
    int readBit( void );
    // read the next bit
    
    void shift( void );
    // shift the bits
    
  private:

    int _loadPin;
    int _clockEnablePin;
    int _clockPin;
    int _dataPin; 
 
};


#endif

