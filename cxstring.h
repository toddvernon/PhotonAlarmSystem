//------------------------------------------------------------------------------------------------------------
//  cxstring.h
//
//  CxString Class
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

#include <ctype.h>

#ifndef _CxString_h_
#define _CxString_h_

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


//------------------------------------------------------------------------------------------------------------
// class CxString
//
//------------------------------------------------------------------------------------------------------------
class CxString
{
  public:

	CxString( void );
	// constructor

	CxString( const CxString& sr_ );
	// copy constructor

	CxString( const char * cptr_, int len=-1 );
	// construct from a const char string

	CxString( const CxString * sr_ );
	// copy from a pointer

    CxString( const char cc_ );
	// construct from a single char

	~CxString( void );
	// destructor

	CxString& operator=( const CxString& sr_ );
	// assignment from CxString

	CxString operator+( const CxString& sr_ );
	// append contents of a CxString

	CxString& operator+=( const CxString& sr_ );
	// append CxString to self

    int operator==(const CxString& sr_ ) const;
	// compare CxString to self

    int operator!=(const CxString& sr_ ) const;
	// compare CxString to self

	void append( const CxString& sr_ );
	// append CxString to self

	void insert( const CxString& sr_, int pos );
	// append CxString to self
	
	int length( void ) const;
	// return length of self

	int firstChar( const char ) const;
	// return index of first occurance of char, or -1

	int firstChar( const char * ) const;
	// return index of first occurance of char, or -1

    int index( CxString s, int startpos_=0 ) const;
	// return index of first occurance of s starting at pos, or -1

	int lastChar( const char ) const;
	// return index of last occurance of char, or -1

	CxString& stripLeading( const char* charSet_ );
	// strip all leading characters contained in charSet

	CxString& stripTrailing( const char* charSet_ );
	// strip all the trailing characters contained in charSet

	CxString  nextToken( const char* charSet_ ); 
	// return the next token as delimited by charSet, removing from self

	CxString  subString( int start, int len );
	// return a copy of the substring between start and start+len

	CxString& remove( int start, int len );
	// remove a sub part of self and return

    static CxString netNormalize( CxString text );
	// remove any trailing CR and/or LF from text and return

    static CxString toUpper( CxString s );
	// convert all characters in s to upper case and return

    static CxString toLower( CxString s );
	// convert all characters in s to lower case and return

	char *data( void ) const;
	// return a pointer to a raw c string

	int isNull( void ) const;
	// if self contains nothing return true

	unsigned int hashValue( void ) const;
	// return a integer hash value of self

	static CxString urlDecode( CxString s_ );
	// return a decoded string	


  private:

    //friend ostream& operator<<(ostream& str, const CxString& cxstring_ );
    // outputs a CxString to an ostream
 
	static int charInSet( char ch, const char* charSet_ );
	// return true if ch is in charSet

	void reAssign( const char *, int len=-1 );
	// internal assignment of self

	char *_data;
	// internal pointer to data
	
};


#endif

