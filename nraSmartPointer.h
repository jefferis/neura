/*
 *  nraSharedPointer.h
 *
 *  Created by Alexander Heusel on Fri Oct 24 2003.
 *  Copyright (c) 2003 __MyCompanyName__. All rights reserved.
 *
 */



#ifndef _NRASMARTPOINTER_H_
#define _NRASMARTPOINTER_H_

//#include "nraConfig.h"
#include "nraAssert.h"

#ifdef NRA_DEBUGOUTPUT_ON
	#include <iostream>
	using namespace std;
#endif


template <class T> 
class _nraSmartPointerCore
{
public:
    _nraSmartPointerCore(T* pPointer)
    {
#ifdef NRA_DEBUGOUTPUT_ON
	if(pPointer)
	{
	    cout.unsetf(ios::dec);
	    cout.setf(ios::hex);
	    cout << "Object generated. Adr.: " << (long)(pPointer) << endl;
	    cout.unsetf(ios::hex);
	    cout.setf(ios::dec);
	}
	else
	{
	    cout << "Null pointercore generated." << endl;
	}
#endif
	m_pPointer = pPointer;
	m_lCounterShared = 0;
	m_lCounterWeak = 0;
	NRA_CHECK_INVARIANT;
    };
        
    ~_nraSmartPointerCore()
    {
	NRA_ASSERT(CanBeDisposed(), "Deletion of pointercore with existing object!");
#ifdef NRA_DEBUGOUTPUT_ON
	cout << "Object disposed." << endl;
#endif
    };
    
    inline void AddShared() 
    {
	m_lCounterShared++;
	NRA_CHECK_INVARIANT;
    };

    inline void ReleaseShared()
    {
	m_lCounterShared--;
	if(m_lCounterShared == 0)
	{
	if(m_pPointer)
	    {
		delete m_pPointer;
		m_pPointer = 0L;
	    }
	}
	NRA_CHECK_INVARIANT;
    };
    
    inline void AddWeak()
    {
	m_lCounterWeak++;
	NRA_CHECK_INVARIANT;
    };
    
    inline void ReleaseWeak()
    {
	m_lCounterWeak--;
	NRA_CHECK_INVARIANT;
    };
    
    inline long GetSharedCount() const { return m_lCounterShared; };
    
    inline long GetWeakCount() const { return m_lCounterWeak; };
    
    inline T* GetPointer() const { return m_pPointer; };  
    
    inline bool IsValid() const { return m_lCounterShared > 0 && m_pPointer; };
    
    inline bool CanBeDisposed() const { return m_lCounterShared == 0 && m_lCounterWeak == 0 && !m_pPointer; };
    

private:
    T* m_pPointer;
    long m_lCounterShared;
    long m_lCounterWeak;
    NRA_INVARIANT
	NRA_ASSERT(m_lCounterShared > -1, "Pointercore corrupt (shared reference counter negative)!");
	NRA_ASSERT(m_lCounterWeak > -1, "Pointercore corrupt (weak reference counter negative)!");
    NRA_END_INVARIANT
    
};

template <class T>
class nraSharedPointer;

template <class T>
class nraWeakPointer
{
public:

    nraWeakPointer()
    {
	m_pCore = 0L;
	m_pCore = new _nraSmartPointerCore<T>(0L);
	NRA_ASSERT(m_pCore, "Unable to allocate pointercore!");
	m_pCore->AddWeak();
    };
    
    nraWeakPointer(const nraWeakPointer<T>& Orig)
    {
	m_pCore = 0L;
	m_pCore = Orig.m_pCore;
	NRA_REQUIRE(m_pCore, "nraWeakPointer in copy constructor does not contain pointercore!");
	m_pCore->AddWeak();
    };
    
    nraWeakPointer(const nraSharedPointer<T>& Orig)
    {
	m_pCore = 0L;
	m_pCore = Orig.m_pCore;
	NRA_REQUIRE(m_pCore, "nraWeakPointer in copy constructor does not contain pointercore!");
	m_pCore->AddWeak();
    };
    
    ~nraWeakPointer()
    {
	NRA_ASSERT(m_pCore, "nraWeakPointer corrupt (no pointercore)!");
	m_pCore->ReleaseWeak();
	if(m_pCore->CanBeDisposed())
	{
	    delete m_pCore;
	    m_pCore = 0L;
	}
    };
    
    nraWeakPointer<T>& operator= (const nraWeakPointer<T>& Rhs)
    {
	if(this != &Rhs)
	{
	    NRA_ASSERT(m_pCore, "nraWeakPointer corrupt (no pointercore)!");
	    m_pCore->ReleaseWeak();
	    if(m_pCore->CanBeDisposed())
	    {
		delete m_pCore;
	    }
	    m_pCore = 0L;
	    m_pCore = Rhs.m_pCore;
	    NRA_REQUIRE(m_pCore, "nraWeakPointer in assignment operator does not contain pointercore!");
	    m_pCore->AddWeak();
	}
	return *this;
    };
    
    nraWeakPointer<T>& operator= (const nraSharedPointer<T>& Rhs)
    {
	NRA_ASSERT(m_pCore, "nraWeakPointer corrupt (no pointercore)!");
	m_pCore->ReleaseWeak();
	if(m_pCore->CanBeDisposed())
	{
	    delete m_pCore;
	}
	m_pCore = 0L;
	m_pCore = Rhs.m_pCore;
	NRA_REQUIRE(m_pCore, "nraSharedPointer in assignment operator does not contain pointercore!");
	m_pCore->AddWeak();
	return *this;    
    };
    
    
    inline T& operator* () const
    {
	NRA_ASSERT(m_pCore, "nraWeakPointer corrupt (no pointercore)!");
	NRA_ASSERT(m_pCore->IsValid(), "Trying to dereference invalid pointer!");
	return *(m_pCore->GetPointer());
    };
    
    inline T* operator-> () const
    {
	NRA_ASSERT(m_pCore, "nraWeakPointer corrupt (no pointercore)!");
	NRA_ASSERT(m_pCore->IsValid(), "Trying to dereference invalid pointer!");
	return m_pCore->GetPointer();
    };
        
    inline T* GetPointer() const
    {
	NRA_ASSERT(m_pCore, "nraWeakPointer corrupt (no pointercore)!");
	NRA_ASSERT(m_pCore->IsValid(), "Trying to dereference invalid pointer!");
	return m_pCore->GetPointer();	
    };
    
    inline bool IsValid() const 
    {
	NRA_ASSERT(m_pCore, "nraWeakPointer corrupt (no pointercore)!");
	return m_pCore->IsValid();
    };
    
    void Release()
    {
	NRA_ASSERT(m_pCore, "nraWeakPointer corrupt (no pointercore)!");
	m_pCore->ReleaseWeak();
	if(m_pCore->CanBeDisposed())
	{
	    delete m_pCore;
	}
	m_pCore = 0L;
	m_pCore = new _nraSmartPointerCore<T>(0L);
	NRA_ASSERT(m_pCore, "Unable to allocate pointercore!");
	m_pCore->AddWeak();
    };

private:
    _nraSmartPointerCore<T>* m_pCore;
    
};

template <class T>
class nraSharedPointer
{
public:

    friend class nraWeakPointer<T>;
    
    nraSharedPointer()
    {
	m_pCore = 0L;
	m_pCore = new _nraSmartPointerCore<T>(0L);
	NRA_ASSERT(m_pCore, "Unable to allocate pointercore!");
	m_pCore->AddShared();
    };
    
    nraSharedPointer(T* pPointer)
    {
	m_pCore = 0L;
	m_pCore = new _nraSmartPointerCore<T>(pPointer);
	NRA_ASSERT(m_pCore, "Unable to allocate pointercore!");
	m_pCore->AddShared();
    };

    nraSharedPointer(const nraSharedPointer<T>& Orig)
    {
	m_pCore = 0L;
	m_pCore = Orig.m_pCore;
	NRA_REQUIRE(m_pCore, "No pointercore in copy-constructor object!");
	m_pCore->AddShared();
    };
    
    ~nraSharedPointer()
    {
	NRA_ASSERT(m_pCore, "nraSharedPointer corrupted (no pointercore)!");
	m_pCore->ReleaseShared();
	if(m_pCore->CanBeDisposed())
	{
	    delete m_pCore;
	    m_pCore = 0L;
	}
    };

    nraSharedPointer<T>& operator= (const nraSharedPointer<T>& Rhs)
    {
	if(this != &Rhs)
	{
	    NRA_ASSERT(m_pCore, "nraSharedPointer corrupted (no pointercore)!");
	    m_pCore->ReleaseShared();
	    if(m_pCore->CanBeDisposed())
	    {
		delete m_pCore;
	    }
	    m_pCore = 0L;
	    m_pCore = Rhs.m_pCore;
	    NRA_REQUIRE(m_pCore, "No pointercore in assignment-operator object!");
	    m_pCore->AddShared();
	}
	return *this;
    };

    inline T& operator* () const
    {
	NRA_ASSERT(m_pCore, "nraSharedPointer corrupt (no pointercore)!");
	NRA_ASSERT(m_pCore->IsValid(), "Trying to dereference invalid pointer!");
	return *(m_pCore->GetPointer());
    };
    
    inline T* operator-> () const
    {
	NRA_ASSERT(m_pCore, "nraSharedPointer corrupt (no pointercore)!");
	NRA_ASSERT(m_pCore->IsValid(), "Trying to dereference invalid pointer!");
	return (m_pCore->GetPointer());
    };
    
    inline operator nraWeakPointer<T>()
    {
	NRA_ASSERT(m_pCore, "nraSharedPointer corrupt (no pointercore)!");
	return nraWeakPointer<T>(*this);
    };

    
    inline T* GetPointer() const
    {
	NRA_ASSERT(m_pCore, "nraSharedPointer corrupt (no pointercore)!");
	NRA_ASSERT(m_pCore->IsValid(), "Trying to dereference invalid pointer!");
	return m_pCore->GetPointer();	
    };

    inline bool IsValid()
    {
	NRA_ASSERT(m_pCore, "nraSharedPointer corrupt (no pointercore)!");
	return m_pCore->IsValid();
    };
    
    void Release()
    {
	NRA_ASSERT(m_pCore, "nraSharedPointer corrupt (no pointercore)!");
	m_pCore->ReleaseShared();
	if(m_pCore->CanBeDisposed())
	{
	    delete m_pCore;
	}
	m_pCore = 0L;
	m_pCore = new _nraSmartPointerCore<T>(0L);
	NRA_ASSERT(m_pCore, "Unable to allocate pointercore!");
	m_pCore->AddShared();
    };

private:
    _nraSmartPointerCore<T>* m_pCore;
    
};

template <class T> 
class _nraSmartArrayCore
{
public:
    _nraSmartArrayCore(T* pPointer, long lSize = 0)
    {
#ifdef NRA_DEBUGOUTPUT_ON
	if(pPointer)
	{
	    cout.unsetf(ios::dec);
	    cout.setf(ios::hex);
	    cout << "Array generated. Adr.: " << (long)(pPointer);
	    cout.unsetf(ios::hex);
	    cout.setf(ios::dec);			
	    cout << " Size: " << lSize << endl;
	}
	else
	{
	    cout << "Null arraycore generated." << endl;
	}
#endif
	m_pPointer = pPointer;
	m_lCounterShared = 0;
	m_lCounterWeak = 0;
	m_lSize = lSize;
	NRA_CHECK_INVARIANT;
    };
        
    ~_nraSmartArrayCore()
    {
	NRA_ASSERT(CanBeDisposed(), "Deletion of arraycore with existing object!");
#ifdef NRA_DEBUGOUTPUT_ON
	cout << "Array disposed. Size: " << m_lSize << endl;
#endif
    };
    
    inline void AddShared()
    {
	m_lCounterShared++;
	NRA_CHECK_INVARIANT;
    };

    inline void ReleaseShared()
    {
	m_lCounterShared--;
	if(m_lCounterShared == 0)
	{
	    if(m_pPointer)
	    {
		delete [] m_pPointer;
		m_pPointer = 0L;
	    }
	}
	NRA_CHECK_INVARIANT;
    };
    
    inline void AddWeak()
    {
	m_lCounterWeak++;
	NRA_CHECK_INVARIANT;
    };
    
    inline void ReleaseWeak()
    {
	m_lCounterWeak--;
	NRA_CHECK_INVARIANT;
    };
    
    inline long GetSharedCount() const { return m_lCounterShared; };
    
    inline long GetWeakCount() const { return m_lCounterWeak; };
    
    inline long GetSize() const { return m_lSize; };
    
    inline T* GetPointer() const { return m_pPointer; };  
    
    inline bool IsValid() const { return m_lCounterShared > 0 && m_pPointer; };
    
    inline bool CanBeDisposed() const { return m_lCounterShared == 0 && m_lCounterWeak == 0 && !m_pPointer; };
    

private:
    T* m_pPointer;
    long m_lCounterShared;
    long m_lCounterWeak;
    long m_lSize;
    NRA_INVARIANT
	NRA_ASSERT(m_lCounterShared > -1, "Arraycore corrupt (shared reference counter negative)!");
	NRA_ASSERT(m_lCounterWeak > -1, "Arraycore corrupt (weak reference counter negative)!");
	NRA_ASSERT(m_lSize > -1, "Arraycore corrupt (size negative)!");
    NRA_END_INVARIANT
    
};

template <class T>
class nraSharedArray;

template <class T>
class nraWeakArray
{
public:

    nraWeakArray()
    {
	m_pCore = 0L;
	m_pCore = new _nraSmartArrayCore<T>(0L);
	NRA_ASSERT(m_pCore, "Unable to allocate arraycore!");
	m_pCore->AddWeak();
    };
    
    nraWeakArray(const nraWeakArray<T>& Orig)
    {
	m_pCore = 0L;
	m_pCore = Orig.m_pCore;
	NRA_REQUIRE(m_pCore, "nraWeakArray in copy constructor does not contain arraycore!");
	m_pCore->AddWeak();
    };
    
    nraWeakArray(const nraSharedArray<T>& Orig)
    {
	m_pCore = 0L;
	m_pCore = Orig.m_pCore;
	NRA_REQUIRE(m_pCore, "nraSharedArray in copy constructor does not contain arraycore!");
	m_pCore->AddWeak();
    };
    
    ~nraWeakArray()
    {
	NRA_ASSERT(m_pCore, "nraWeakArray corrupt (no arraycore)!");
	m_pCore->ReleaseWeak();
	if(m_pCore->CanBeDisposed())
	{
	    delete m_pCore;
	    m_pCore = 0L;
	}
    };
    
    nraWeakArray<T>& operator= (const nraWeakArray<T>& Rhs)
    {
	if(this != &Rhs)
	{
	    NRA_ASSERT(m_pCore, "nraWeakArray corrupt (no arraycore)!");
	    m_pCore->ReleaseWeak();
	    if(m_pCore->CanBeDisposed())
	    {
		delete m_pCore;
	    }
	    m_pCore = 0L;
	    m_pCore = Rhs.m_pCore;
	    NRA_REQUIRE(m_pCore, "nraWeakArray in assignment operator does not contain arraycore!");
	    m_pCore->AddWeak();
	}
	return *this;
    };
    
    nraWeakArray<T>& operator= (const nraSharedArray<T>& Rhs)
    {
	NRA_ASSERT(m_pCore, "nraWeakArray corrupt (no arraycore)!");
	m_pCore->ReleaseWeak();
	if(m_pCore->CanBeDisposed())
	{
	    delete m_pCore;
	}
	m_pCore = 0L;
	m_pCore = Rhs.m_pCore;
	NRA_REQUIRE(m_pCore, "nraSharedArray in assignment operator does not contain arraycore!");
	m_pCore->AddWeak();
	return *this;
    };
    
    
    inline T& operator* () const
    {
	NRA_ASSERT(m_pCore, "nraWeakArray corrupt (no arraycore)!");
	NRA_ASSERT(m_pCore->IsValid(), "Trying to access invalid pointer!");
	return *(m_pCore->GetPointer());
    };
    
    inline T* operator-> () const
    {
	NRA_ASSERT(m_pCore, "nraWeakArray corrupt (no arraycore)!");
	NRA_ASSERT(m_pCore->IsValid(), "Trying to access invalid pointer!");
	return m_pCore->GetPointer();
    };
    
    inline T& operator[] (long lIdx) const
    {
	NRA_ASSERT(m_pCore, "nraWeakArray corrupt (no arraycore)!");
	NRA_ASSERT(m_pCore->IsValid(), "Trying to access invalid pointer!");
	NRA_REQUIRE(lIdx > -1 && lIdx < m_pCore->GetSize(), "Subscript out of range!")
	return (m_pCore->GetPointer())[lIdx];
    };
    
    inline long GetSize() const
    {
	NRA_ASSERT(m_pCore, "nraWeakArray corrupt (no arraycore)!");
	NRA_ASSERT(m_pCore->IsValid(), "Trying to access invalid pointer!");
	return m_pCore->GetSize();
    };
    
    inline T* GetPointer()
    {
	NRA_ASSERT(m_pCore, "nraWeakArray corrupt (no arraycore)!");
	NRA_ASSERT(m_pCore->IsValid(), "Trying to access invalid pointer!");
	return (m_pCore->GetPointer());
    };
    
    inline bool IsValid() const
    {
	NRA_ASSERT(m_pCore, "nraWeakArray corrupt (no arraycore)!");
	return m_pCore->IsValid();
    };
	
    void Release()
    {
	NRA_ASSERT(m_pCore, "nraWeakArray corrupt (no arraycore)!");
	m_pCore->ReleaseWeak();
	if(m_pCore->CanBeDisposed())
	{
	    delete m_pCore;
	}
	m_pCore = 0L;
	m_pCore = new _nraSmartArrayCore<T>(0L);
	NRA_ASSERT(m_pCore, "Unable to allocate arraycore!");
	m_pCore->AddWeak();
    };
    
private:
    _nraSmartArrayCore<T>* m_pCore;
    
};


template <class T>
class nraSharedArray
{
public:

    friend class nraWeakArray<T>;
    
    nraSharedArray()
    {
	m_pCore = 0L;
	m_pCore = new _nraSmartArrayCore<T>(0L);
	NRA_ASSERT(m_pCore, "Unable to allocate arraycore!");
	m_pCore->AddShared();
    };
    
    nraSharedArray(T* pPointer, long lSize)
    {
	m_pCore = 0L;
	m_pCore = new _nraSmartArrayCore<T>(pPointer, lSize);
	NRA_ASSERT(m_pCore, "Unable to allocate arraycore!");
	m_pCore->AddShared();
    };

    nraSharedArray(const nraSharedArray<T>& Orig)
    {
	m_pCore = 0L;
	m_pCore = Orig.m_pCore;
	NRA_REQUIRE(m_pCore, "No arraycore in copy-constructor object!");
	m_pCore->AddShared();
    };
    
    ~nraSharedArray()
    {
	NRA_ASSERT(m_pCore, "nraSharedArray corrupt (no arraycore)!");
	m_pCore->ReleaseShared();
	if(m_pCore->CanBeDisposed())
	{
	    delete m_pCore;
	    m_pCore = 0L;
	}
    };

    nraSharedArray<T>& operator= (const nraSharedArray<T>& Rhs)
    {
	if(this != &Rhs)
	{
	    NRA_ASSERT(m_pCore, "nraSharedArray corrupt (no arraycore)!");
	    m_pCore->ReleaseShared();
	    if(m_pCore->CanBeDisposed())
	    {
		delete m_pCore;
	    }
	    m_pCore = 0L;
	    m_pCore = Rhs.m_pCore;
	    NRA_REQUIRE(m_pCore, "No arraycore in assignment-operator object!");
	    m_pCore->AddShared();
        }
	return *this;
    };

    inline T& operator* ()
    {
	NRA_ASSERT(m_pCore, "nraSharedArray corrupt (no arraycore)!");
	NRA_ASSERT(m_pCore->IsValid(), "Trying to access invalid pointer!");
	return *(m_pCore->GetPointer());
    };
    
    inline T* operator-> ()
    {
	NRA_ASSERT(m_pCore, "nraSharedArray corrupt (no arraycore)!");
	NRA_ASSERT(m_pCore->IsValid(), "Trying to access invalid pointer!");
	return (m_pCore->GetPointer());
    };
    
    inline T& operator[] (long lIdx) const
    {
	NRA_ASSERT(m_pCore, "nraSharedArray corrupt (no arraycore)!");
	NRA_ASSERT(m_pCore->IsValid(), "Trying to access invalid pointer!");
	NRA_REQUIRE(lIdx > -1 && lIdx < m_pCore->GetSize(), "Subscript out of range!");
	return (m_pCore->GetPointer())[lIdx];
    };
    
    inline operator nraWeakArray<T>()
    {
	NRA_ASSERT(m_pCore, "nraSharedArray corrupt (no arraycore)!");
	return nraWeakArray<T>(*this);
    };
    
    inline T* GetPointer()
    {
	NRA_ASSERT(m_pCore, "nraSharedArray corrupt (no arraycore)!");
	NRA_ASSERT(m_pCore->IsValid(), "Trying to access invalid pointer!");
	return (m_pCore->GetPointer());
    };
    
    inline long GetSize() const
    {
	NRA_ASSERT(m_pCore, "nraSharedArray corrupt (no arraycore)!");
	NRA_ASSERT(m_pCore->IsValid(), "Trying to access invalid pointer!");
	return m_pCore->GetSize();
    };
    
    inline bool IsValid() const
    {
	NRA_ASSERT(m_pCore, "nraSharedArray corrupt (no arraycore)!");
	return m_pCore->IsValid();
    };

    void Release()
    {
	NRA_ASSERT(m_pCore, "nraSharedArray corrupt (no arraycore)!");
	m_pCore->ReleaseShared();
	if(m_pCore->CanBeDisposed())
	{
	    delete m_pCore;
	}
	m_pCore = 0L;
	m_pCore = new _nraSmartArrayCore<T>(0L);
	NRA_ASSERT(m_pCore, "Unable to allocate arraycore!");
	m_pCore->AddShared();
    };

private:
    _nraSmartArrayCore<T>* m_pCore;
    
};

#endif


