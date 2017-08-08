//------------------------------------------------------------------------------------------------------------
//  SN74HC595.h
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


//------------------------------------------------------------------------------------------------------------
// standard includes
//
//------------------------------------------------------------------------------------------------------------
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <cxstring.h>
//#include <iostream.h>
//#include <time.h>

#include <Particle.h>


//------------------------------------------------------------------------------------------------------------
// local includes
//
//------------------------------------------------------------------------------------------------------------
#include <cxstring.h>


#ifndef _SN74HC595_
#define _SN74HC595_


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
class SN74HC595
{
  public:
  
    SN74HC595( void );
    // constructor
    
    SN74HC595( int shcp_, int stcp_, int ds_ );
    // constructor with pins defined

   ~SN74HC595( void );
	// destructor

    void latch_output( void );
    // load a new value
    
    int writeBit( int bit );
    // read the next bit
    
    void shift( void );
    // shift the bits
    
  private:

    int _SHCP;
    int _STCP;
    int _DS;

};


#endif

