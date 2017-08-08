//------------------------------------------------------------------------------------------------------------
//  cxslist.h
//
//  CxSList Class
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

#include <stdio.h>

//#define USE_EXCEPTION_PROCESSING TRUE

#include <Particle.h>

#ifdef USE_EXCEPTION_PROCESSING
#include <cxexception.h>
#endif

#ifndef _CxSList_h_
#define _CxSList_h_



template <class T>
class  CxListNode
{
public:
    T data;
    CxListNode<T> *next;
};


template <class T>
class  CxSListIterator
{
public:
    CxSListIterator(CxListNode<T>* n) : m_currentNode(n) { }

    CxSListIterator(const CxSListIterator<T>& i)
        : m_currentNode(i.m_currentNode) { }

    CxSListIterator<T>& operator=(const CxSListIterator<T>& i);

    T& operator*( ) const
    { return m_currentNode->data; }

    CxSListIterator<T>& operator++();   // pre-fix increment

    CxSListIterator<T> operator++(int); // post-fix increment

    int operator==(const CxSListIterator<T>& i)
    {
        return (i.m_currentNode == m_currentNode);
    }

    int operator!=(const CxSListIterator<T>& i)
    {
        return (i.m_currentNode != m_currentNode);
    }

    CxListNode<T>* getCurrentNode() const 
        { return m_currentNode; }


private:
    CxListNode<T>* m_currentNode;
};


//-------------------------------------------------------------------------
// class CxSList
//
//-------------------------------------------------------------------------
template <class T>
class  CxSList
{
public:

	CxSList( void );
	// constructor

	CxSList( const CxSList<T>& slist_ );
	// copy constructor

	~CxSList( void );
	// destructor

	CxSList<T>& operator=( const CxSList<T>& slist_ );
	// assignment operator

	void append( const T& item );
	// add item to the list

	void append( const CxSList<T>& slist_ );
	// add a list to the list

	void push( const T& item );
	// push an item onto the end of the list

	T pop( void );
	// remove the last item from the list

	T peek( void ) const;
	// get a copy of the last item on the list

	void replaceAt( int i, const T& item );
	// replace the item at index i

	void removeAt( int i );
	// remove the item at item i

	void clear( void );
	// clear the items from the list

	void clearAndDelete( void );
	// clear the items from the list and delete them

	size_t entries( void ) const;
	// return the number of items in the list
	
	T at( int i ) const;
	// return a copy of the item at index i

	T first( void );
	// return a copy of first item on the list

	T last( void );
	// return a copy of last item on the list

    // The following methods provide compatibility with STL.

    CxSListIterator<T> begin() { return _head; }
    CxSListIterator<T> end() { return _tail->next; }
    void push_back( const T& item ) { append(item); }
    CxSListIterator<T> erase(CxSListIterator<T> pos);
    size_t size() const { return _entries; }
    int empty() const { return _entries == 0; }


protected:

	size_t  _entries;
	CxListNode<T>   *_head;
	CxListNode<T>   *_tail;
	CxListNode<T>   *_work;
	CxListNode<T>   _pointerToHead;

	void deepCopy( const CxSList<T>& slist_ );
	// copy the items in the list

	void setNull( void );
	// set the list to no items
};



//------------------------------------------------------------------------------------------------------------
// CxSList<T>::
//
//------------------------------------------------------------------------------------------------------------
template <class T>
CxSList<T>::CxSList( void  )
{
	setNull();
}

//------------------------------------------------------------------------------------------------------------
// CxSList<T>::
//
//------------------------------------------------------------------------------------------------------------
template <class T>
CxSList<T>::CxSList( const CxSList<T>& slist_ )
{
	setNull();
	deepCopy( slist_ );
}

//------------------------------------------------------------------------------------------------------------
// CxSList<T>::operator=
//
//------------------------------------------------------------------------------------------------------------
template< class T >
CxSList<T>& 
CxSList<T>::operator=( const CxSList<T>& slist_ )
{
	if ( &slist_ != this ) {
		clear();
		setNull();
		deepCopy( slist_ );
	}
	return( *this );
}

//------------------------------------------------------------------------------------------------------------
// CxSList<T>::~CxSList<T>
//
//------------------------------------------------------------------------------------------------------------
template <class T>
CxSList<T>::~CxSList( void )
{
	clear();
}


//------------------------------------------------------------------------------------------------------------
// CxSList<T>::entries
//
//------------------------------------------------------------------------------------------------------------
template <class T>
size_t
CxSList<T>::entries(void) const
{
	return(_entries);
}


//------------------------------------------------------------------------------------------------------------
// CxSList<T>::setNull
//
//------------------------------------------------------------------------------------------------------------
template <class T>
void
CxSList<T>::setNull( void )
{
	_head = NULL;
	_tail = NULL;
	_work = NULL;
	_entries = 0;
}

//------------------------------------------------------------------------------------------------------------
// CxSList<T>::deepCopy
//
//------------------------------------------------------------------------------------------------------------
template <class T>
void
CxSList<T>::deepCopy( const CxSList<T>& slist_ )
{
	if (slist_.entries() == 0) return;
	CxListNode<T> *n = slist_._head;

	do {
		append( n->data );
		n = n->next;
	} while (n != NULL );

	_work = _head;
}

//------------------------------------------------------------------------------------------------------------
// CxSList<T>::replaceAt
//
//------------------------------------------------------------------------------------------------------------
template <class T>
void
CxSList<T>::replaceAt( int i, const T& item )
{
	int count = 0;

	CxListNode<T> *n = _head;

	while (n != NULL) {

		if (i == count) {
			n->data = item;
			return;
		}

		n = n->next;
		count++;
	}

#ifdef USE_EXCEPTION_PROCESSING
    throw CxException("CxSList::at(invalid index)");
#endif
}

//------------------------------------------------------------------------------------------------------------
// CxSList<T>::erase
//
//------------------------------------------------------------------------------------------------------------
template <class T>
CxSListIterator<T>
CxSList<T>::erase( CxSListIterator<T> i )
{
    CxListNode<T>* pNode = i.getCurrentNode();

    if (pNode == NULL) {
        return i;
    }

    CxListNode<T> *prev     = NULL;
    CxListNode<T> *n        = _head;
    CxListNode<T> *nextNode = NULL;

    while (n != NULL) {
        // If we have found the input node, fix up the
        // pointers and delete the node.

        nextNode = n->next;

        if (n == pNode) {
            if (pNode == _head) {
                _head = nextNode;
                _pointerToHead.next = _head;
                prev  = &_pointerToHead;
            }
            else {
                prev->next = nextNode;
            }

            // If the input node was the tail, make the
            // previous node the tail.

            if (pNode == _tail) {
                _tail = prev;
            }

            delete n;
            _entries--;

            break;
        }

        // Increment to the next node.
        prev = n;
        n = nextNode;
    }

    return prev;
}

//------------------------------------------------------------------------------------------------------------
// CxSList<T>::removeAt
//
//------------------------------------------------------------------------------------------------------------
template <class T>
void
CxSList<T>::removeAt( int i )
{
	int count = 0;

	CxListNode<T> *prev    = NULL;
	CxListNode<T> *n       = _head;

	while (n != NULL) {

		if (i == count) {

			// case where there is only one item
			if (( n == _head ) && ( n == _tail )) {
				_head = _tail = _work = NULL;
				delete n;
			} else

			// case where first item is deleted.
			if ( n == _head ) {
				_head = n->next;
				delete n;
			} else

			// case where last item is deleted.
			if ( n == _tail ) {
				prev->next = NULL;
				_tail = prev;
				delete n;
			} else

			// otherwise in the middle
			{
				prev->next = n->next;
				delete n;
			}

			_entries--;
			return;
		}
	
		prev = n;
		n    = n->next;
	
		count++;
	}

#ifdef USE_EXCEPTION_PROCESSING
    throw CxException("CxSList::at(invalid index)");
#endif

}

//------------------------------------------------------------------------------------------------------------
// CxSList<T>::push
//
//------------------------------------------------------------------------------------------------------------
template <class T>
void
CxSList<T>::push( const T& item )
{
	append( item );
}


//------------------------------------------------------------------------------------------------------------
// CxSList<T>::pop
//
//------------------------------------------------------------------------------------------------------------
template <class T>
T
CxSList<T>::pop( void )
{
	int count = entries();

	if (count==0) {
#ifdef USE_EXCEPTION_PROCESSING
        throw CxException("CxSList::at(invalid index)");
#endif
	}

	T data = at( count-1 );

	removeAt( count-1 );

	return( data );
};

//------------------------------------------------------------------------------------------------------------
// CxSList<T>::peek
//
//------------------------------------------------------------------------------------------------------------
template <class T>
T
CxSList<T>::peek( void ) const
{
	int count = entries();

	if (count==0) {
	    
#ifdef USE_EXCEPTION_PROCESSING
	    throw CxException("CxSList::at(invalid index)");
#endif

	}

	T data = at( count-1 );

	return( data );
}


//------------------------------------------------------------------------------------------------------------
// CxSList<T>::append
//
//------------------------------------------------------------------------------------------------------------
template <class T>
void
CxSList<T>::append( const T& item )
{
	CxListNode<T> *n = new CxListNode<T>;

	if (n == NULL) {
#ifdef USE_EXCEPTION_PROCESSING
	    throw CxException("CxSList::append(memory allocation error)");
#endif
	}

	n->data = item;
	n->next = NULL;

    // if no items in the list
	if (_tail == NULL) {

		_head = n;
		_tail = n;
		_work = n;

    // else just slide in at the end of the list
	} else {

		_tail->next = n;
		_tail = n;
	}

	_entries++;
}

//------------------------------------------------------------------------------------------------------------
// CxSList<T>::append
//
//------------------------------------------------------------------------------------------------------------
template <class T>
void
CxSList<T>::append( const CxSList<T>& slist_ )
{
	const CxListNode<T> *n = slist_._head;

	while (n != NULL) {
		append( n->data );
		n = n->next;
	}
}

//------------------------------------------------------------------------------------------------------------
// CxSList<T>::clear
//
//------------------------------------------------------------------------------------------------------------
template <class T>
void
CxSList<T>::clear( void )
{
	_work = _head;

	while (_work != NULL ) {

		_head = _work->next;
		delete _work;
		_work = _head;
	}

	setNull();
}

//------------------------------------------------------------------------------------------------------------
// CxSList<T>::clearAndDelete
//
//------------------------------------------------------------------------------------------------------------
template <class T>
void
CxSList<T>::clearAndDelete( void )
{
	_work = _head;

	while (_work != NULL ) {

		_head = _work->next;
		
		delete _work->data;
		delete _work;


		_work = _head;
	}

	setNull();
}

//------------------------------------------------------------------------------------------------------------
// CxSList<T>::at
//
//------------------------------------------------------------------------------------------------------------
template <class T>
T
CxSList<T>::at( int i ) const
{
	int count = 0;

	const CxListNode<T> *n = _head;

	while (n != NULL) {

		if (i == count) {
			return( n->data );
		}

		n = n->next;
		count++;
	}
#ifdef USE_EXCEPTION_PROCESSING
	throw CxException("CxSList::at(invalid index)");
#endif
	exit;
}	

//int fred;   (long) fred

//------------------------------------------------------------------------------------------------------------
// CxSList<T>::first
//
//------------------------------------------------------------------------------------------------------------
template <class T>
T
CxSList<T>::first( void )
{
	int count = entries();

	if (count==0) {
#ifdef USE_EXCEPTION_PROCESSING
	    throw CxException("CxSList::at(invalid index)");
#endif
	}

	T item = at( 0 );
	
	removeAt(0);

	return( item );
}

//------------------------------------------------------------------------------------------------------------
// CxSList<T>::last
//
//------------------------------------------------------------------------------------------------------------
template <class T>
T
CxSList<T>::last( void )
{
	int count = entries();

	if (count==0) {
#ifdef USE_EXCEPTION_PROCESSING
        throw CxException("CxSList::at(invalid index)");
#endif
	}

	T item = at( count-1 );

	removeAt(count-1);

	return( item );
}


template <class T>
CxSListIterator<T>&
CxSListIterator<T>::operator=(const CxSListIterator<T>& i)
{
    if (&i != this) {
        m_currentNode = i.m_currentNode;
    }
    return *this;
}

template <class T>
CxSListIterator<T>&
CxSListIterator<T>::operator++()
{
    if (m_currentNode != NULL) {
        m_currentNode = m_currentNode->next;
    }
    return *this;
}

template <class T>
CxSListIterator<T> 
CxSListIterator<T>::operator++(int)
{
    CxSListIterator<T> temp(this);

    if (m_currentNode != NULL) {
        m_currentNode = m_currentNode->next;

    }
    return temp;
}

#endif

