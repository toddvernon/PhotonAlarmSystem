//------------------------------------------------------------------------------------------------------------
//  cxstring.cpp
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

#include <string.h>
#include <cxstring.h>

//------------------------------------------------------------------------------------------------------------
// CxString::CxString
//
//------------------------------------------------------------------------------------------------------------
CxString::CxString( void )
: _data( NULL )
{
	reAssign( (char *) NULL );
}


//------------------------------------------------------------------------------------------------------------
// CxString::CxString
//
//------------------------------------------------------------------------------------------------------------
CxString::CxString( const char* cptr_, int len )
: _data(NULL)
{
	reAssign( cptr_, len );
}


//------------------------------------------------------------------------------------------------------------
// CxString::
//
//------------------------------------------------------------------------------------------------------------
CxString::CxString( const CxString& sr_ )
{
	if ( &sr_ != this ) {
		_data = (char*) NULL;
		reAssign( sr_.data() );
	}
}


//------------------------------------------------------------------------------------------------------------
// CxString::
//
//------------------------------------------------------------------------------------------------------------
CxString::CxString( const char c_ )
:_data(NULL)
{
    char d[2];
    d[0] = c_;
    d[1] = (char) NULL;
    reAssign( d );
}


//------------------------------------------------------------------------------------------------------------
// CxString::~CxString
//
//------------------------------------------------------------------------------------------------------------
CxString::~CxString( void )
{
	if (_data) delete[] _data;	
}


//------------------------------------------------------------------------------------------------------------
// CxString::operator=
//
//------------------------------------------------------------------------------------------------------------
CxString&
CxString::operator=( const CxString& sr_ )
{
	if ( &sr_ != this ) {
		reAssign( sr_.data() );
	}
	return( *this );
}


//------------------------------------------------------------------------------------------------------------
// CxString::operator+
//
//------------------------------------------------------------------------------------------------------------
CxString
CxString::operator+( const CxString& sr_ )
{
	int newLen = sr_.length() + strlen( _data ) + 1;
	char *cptr = new char[ sr_.length() + strlen( _data ) + 1 ];
	memset( cptr, 0, newLen );

	int len = strlen( _data );

	memcpy( cptr, &(_data[0]), len );
	memcpy( &(cptr[len]), sr_.data(), sr_.length() );

	CxString newString = cptr;
	delete [] cptr;

	return( newString ); 
}



//------------------------------------------------------------------------------------------------------------
// CxString::insert
//
//------------------------------------------------------------------------------------------------------------
void
CxString::insert( const CxString& sr_, int n )
{

	if ( sr_.length()  == 0 ) return;
	
	if ( strlen(_data) == 0 ) {
		reAssign( sr_.data() );
		return;
	}
	
//	if ( n > strlen(_data) -1 ) n = strlen(_data)-1;
	if ( n < 0 ) n = 0;

	int newLen = sr_.length() + strlen(_data) + 1;
	char *cptr = new char[ sr_.length() + strlen( _data ) + 1 ];
	memset( cptr, 0, newLen );

	// copy first n
	memcpy( cptr, &(_data[0]), n );
	memcpy( cptr + n, sr_.data(), sr_.length() );
	memcpy( cptr + n + sr_.length(), &(_data[n]), newLen - sr_.length() );
	
	reAssign( cptr );

	delete [] cptr;
}



//------------------------------------------------------------------------------------------------------------
// CxString::operator+=
//
//------------------------------------------------------------------------------------------------------------
CxString&
CxString::operator+=( const CxString& sr_ )
{
	append( sr_ );	
	return( *this );
}


//------------------------------------------------------------------------------------------------------------
// CxString::operator==
//
//------------------------------------------------------------------------------------------------------------
int
CxString::operator==( const CxString& sr_ ) const
{
    if ( strcmp(sr_.data(), _data) == 0 )  return( TRUE );
    return( FALSE );
}

//------------------------------------------------------------------------------------------------------------
// CxString::operator!=
//
//------------------------------------------------------------------------------------------------------------
int
CxString::operator!=( const CxString& sr_ ) const
{
    if ( strcmp(sr_.data(), _data) != 0 )  return( TRUE );
    return( FALSE );
}


//------------------------------------------------------------------------------------------------------------
// CxString::append
//
//------------------------------------------------------------------------------------------------------------
void
CxString::append( const CxString& sr_ )
{
	int newLen = sr_.length() + strlen( _data ) + 1;
	char *cptr = new char[ sr_.length() + strlen( _data ) + 1 ];
	memset( cptr, 0, newLen );

	int len = strlen( _data );

	memcpy( cptr, &(_data[0]), len );
	memcpy( &(cptr[len]), sr_.data(), sr_.length() );

	reAssign( cptr );

	delete [] cptr;	
}


//------------------------------------------------------------------------------------------------------------
// CxString::reAssign
//
//------------------------------------------------------------------------------------------------------------
void
CxString::reAssign( const char *cptr, int len )
{
	if (_data) delete[] _data;	

	if (cptr != NULL) {

		if (len == -1) {
			_data = new char[ strlen( cptr ) + 1 ];
			if (_data) {
				strcpy( _data, cptr );
			}
		} else {
			_data = new char[ len + 1 ];
			if (_data) {
				strncpy( _data, cptr, len );
				_data[len]=(char)NULL;
			}
		}
	
	} else {
		_data = new char[1];
		_data[0]=(char) NULL;
	}
}


//------------------------------------------------------------------------------------------------------------
// CxString::data
//
//------------------------------------------------------------------------------------------------------------
char *
CxString::data( void ) const
{
	return( _data );
}


//------------------------------------------------------------------------------------------------------------
// CxString::firstChar
//
//------------------------------------------------------------------------------------------------------------
int
CxString::firstChar( const char ch ) const
{
	if ( isNull() )  return( -1 );
		
	int c;
	int i = strlen( _data );

	for (c=0; c<i; c++ ) {
		if ( _data[c] == ch ) return( c );
	}

	return( -1 );
}

//------------------------------------------------------------------------------------------------------------
// CxString::lastChar
//
//------------------------------------------------------------------------------------------------------------
int
CxString::lastChar( const char ch ) const
{
	if ( isNull() )  return( -1 );
		
	int c;
	int i = strlen( _data );

	if (i==0) return -1;

	for (c=i-1; c>=0; c-- ) {
		if ( _data[c] == ch ) return( c );
	}

	return( -1 );
}


//------------------------------------------------------------------------------------------------------------
// CxString::firstChar
//
//------------------------------------------------------------------------------------------------------------
int
CxString::firstChar( const char *delim_ ) const
{
	if ( isNull() )  return( -1 );
		
	int c;
    int j;
    int delimLength = strlen( delim_ );
	int i = strlen( _data );

	for (c=0; c<i; c++ ) {
        for (j=0; j<delimLength; j++) {
		    if ( _data[c] == delim_[j] ) return( c );
	    }
    }

	return( -1 );
}


//------------------------------------------------------------------------------------------------------------
// CxString::index
//
//------------------------------------------------------------------------------------------------------------
int
CxString::index( CxString s_, int startpos_ ) const
{
	if ( isNull() )  return( -1 );

	int c;
	int i = strlen( _data );

	if ( startpos_  > i-1) return( -1 );

	for (c=startpos_; c<i; c++ ) {
        if (!strncmp( (const char *) &_data[c], (const char *) s_.data(), s_.length())) {
            return(c);
        }
    }
	return( -1 );
}	


//------------------------------------------------------------------------------------------------------------
// CxString::stripLeading
//
//------------------------------------------------------------------------------------------------------------
CxString&
CxString::stripLeading( const char *charSet_ )
{
	if ( isNull() ) return( *this );

	while ( 1 ) {

		if (_data[0] == (char) NULL) {
			reAssign( (char*) NULL );	
			return( *this );
		}

		if (CxString::charInSet(_data[0], charSet_)) {
			memmove( &(_data[0]), &(_data[1]), strlen( _data ) );
		} else {
			return( *this );
		}
	}
}


//------------------------------------------------------------------------------------------------------------
// CxString::stripTrailing
//
//------------------------------------------------------------------------------------------------------------
CxString&
CxString::stripTrailing( const char *charSet_ )
{
	if ( isNull() ) return( *this );

	int cursor = strlen( _data ) -1;

	while ( 1 ) {

		if (_data[cursor] != (char) NULL) {
			if (CxString::charInSet(_data[ cursor ], charSet_)) {
				_data[ cursor ] = (char) NULL;
			} else {
				return( *this );
			}
		}

		if (cursor == 0) {
			return( *this );
		}

		cursor--;
	}
}


//------------------------------------------------------------------------------------------------------------
// CxString::nextToken
//
//------------------------------------------------------------------------------------------------------------
CxString
CxString::nextToken( const char *charSet_ )
{
	stripLeading( charSet_ );

	int pos = firstChar( charSet_ );

	if (pos == -1) {
		pos = length();
	}

	CxString s = subString( 0, pos );

	remove( 0, pos );

	return( s );
}


//------------------------------------------------------------------------------------------------------------
// CxString::substring
//
//------------------------------------------------------------------------------------------------------------
CxString
CxString::subString( int start, int len )
{
	CxString s;

	if (start < 0) return( *this );
    if (len  < 0) return( *this );

	if (start > (int) strlen( _data )) return( s );

	while ( start+len > (int) strlen(_data) ) len--;

	char *cptr = new char[ len+1 ];
	memset( cptr, 0, len+1);
	memcpy( cptr, &(_data[start]), len );
 
	s = cptr;

	delete[] cptr;

	return( s );
}


//------------------------------------------------------------------------------------------------------------
// CxString::remove
//
//------------------------------------------------------------------------------------------------------------
CxString&
CxString::remove( int start, int len )
{
	if (start < 0) return( *this );
	if (start > (int) strlen( _data )-1) return( *this );
	if (len < 0) return( *this );

	while (start+len > (int) strlen(_data)) len--;

	int sLen = strlen( _data );
	memmove( &(_data[start]), &(_data[start+len]), sLen-(start+len)+1);
	return(*this);	
}


//------------------------------------------------------------------------------------------------------------
// CxString::length
//
//------------------------------------------------------------------------------------------------------------
int
CxString::length( void ) const
{
	return( strlen( _data ) );
}



//------------------------------------------------------------------------------------------------------------
// CxString::charInSet
//
//------------------------------------------------------------------------------------------------------------
int 
CxString::charInSet( char ch, const char *charSet_ )
{
	int c;
	int len = strlen( charSet_ );

	for (c=0; c<len; c++) {
		if (ch==charSet_[c]) return(1);
	}
	return(0);
}


//------------------------------------------------------------------------------------------------------------
// CxString::isNull
//
//------------------------------------------------------------------------------------------------------------
int
CxString::isNull(void) const
{
	if (!_data) return( TRUE );
	if (_data[0] == (char) NULL) return( TRUE );
	return( FALSE );
}

 
//------------------------------------------------------------------------------------------------------------
// CxString::netNormalize
//
//------------------------------------------------------------------------------------------------------------
CxString
CxString::netNormalize( CxString line )
{
    int nlIndex;
    int crIndex;

    do {
        nlIndex = line.firstChar('\012');
        if (nlIndex != -1) line = line.remove(nlIndex,1);
    } while (nlIndex != -1);
    
    do {
        crIndex = line.firstChar('\015');
        if (crIndex != -1) line = line.remove(crIndex,1);
    } while (crIndex != -1);
    
    return(line);
}


//------------------------------------------------------------------------------------------------------------
// CxString::toUpper
//
//------------------------------------------------------------------------------------------------------------
/* static */
CxString
CxString::toUpper( CxString s )
{
    char *cptr = s.data();
    for (int c=0; c<s.length(); c++) {
        *cptr = toupper( *cptr );
        cptr++;
    }
    return( s );
}

    
//------------------------------------------------------------------------------------------------------------
// CxString::toLower
//
//------------------------------------------------------------------------------------------------------------
/* static */
CxString
CxString::toLower( CxString s )
{
    char *cptr = s.data();
    for (int c=0; c<s.length(); c++) {
        *cptr = tolower( *cptr );
        cptr++;
    }
    return( s );
}


//------------------------------------------------------------------------------------------------------------
// CxString::hashValue
//
//------------------------------------------------------------------------------------------------------------
unsigned int
CxString::hashValue( void ) const
{
	unsigned int i = 0;

	char *ptr = &_data[0];

	while (*ptr != (char) NULL) {

		i += (unsigned int) *ptr;

		ptr++;
	}

	return( i );
}


//------------------------------------------------------------------------------------------------------------
// CxString::urlDecode
//
//------------------------------------------------------------------------------------------------------------
CxString
CxString::urlDecode( CxString s_ )
{
	return( s_ );
}

