//
// SharedPtr.h
//
// $Id: //poco/1.2/Foundation/include/Poco/SharedPtr.h#6 $
//
// Library: Foundation
// Package: Core
// Module:  SharedPtr
//
// Definition of the SharedPtr template class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Foundation_SharedPtr_INCLUDED
#define Foundation_SharedPtr_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Exception.h"
#include "Poco/Mutex.h"
#include <algorithm>


namespace Poco {


class ReferenceCounter
		/// Simple ReferenceCounter object, does not delete itself when count reaches 0
{
public:
	ReferenceCounter(): _cnt(1)
	{
	}

	void duplicate()
	{
		FastMutex::ScopedLock lock(_mutex);
		++_cnt;
	}

	int release()
	{
		FastMutex::ScopedLock lock(_mutex);
		return --_cnt;
	}

private:
	FastMutex _mutex;
	int _cnt;
};


template <class C>
class SharedPtr
	/// SharedPtr is a "smart" pointer for classes implementing
	/// reference counting based garbage collection.
	/// SharedPtr is thus similar to AutoPtr. Unlike the
	/// AutoPtr template, which can only be used with
	/// classes that support reference counting, SharedPtr
	/// can be used with any class. For this to work, a
	/// SharedPtr manages a reference count for the object
	/// it manages.
	/// 
	/// SharedPtr works in the following way:
	/// If an SharedPtr is assigned an ordinary pointer to
	/// an object (via the constructor or the assignment operator),
	/// it takes ownership of the object and the object's reference 
	/// count is initialized to one.
	/// If the SharedPtr is assigned another SharedPtr, the
	/// object's reference count is incremented by one.
	/// The destructor of SharedPtr decrements the object's
	/// reference count by one and deletes the object if the
	/// reference count reaches zero.
	/// SharedPtr supports dereferencing with both the ->
	/// and the * operator. An attempt to dereference a null
	/// SharedPtr results in a NullPointerException being thrown.
	/// SharedPtr also implements all relational operators and
	/// a cast operator in case dynamic casting of the encapsulated data types
	/// is required.
{
public:
	SharedPtr(): _pCounter(new ReferenceCounter), _ptr(0)
	{
	}

	SharedPtr(C* ptr): _pCounter(new ReferenceCounter), _ptr(ptr)
	{
	}

	template <class Other> 
	SharedPtr(const SharedPtr<Other>& ptr): _pCounter(ptr._pCounter), _ptr(const_cast<Other*>(ptr.get()))
	{
		_pCounter->duplicate();
	}

	SharedPtr(const SharedPtr& ptr): _pCounter(ptr._pCounter), _ptr(ptr._ptr)
	{
		_pCounter->duplicate();
	}

	~SharedPtr()
	{
		release();
	}

	SharedPtr& assign(C* ptr)
	{
		if (get() != ptr)
		{
			ReferenceCounter* pTmp = new ReferenceCounter;
			release();
			_pCounter = pTmp;
			_ptr = ptr;
		}
		return *this;
	}
	
	SharedPtr& assign(const SharedPtr& ptr)
	{
		if (&ptr != this)
		{
			SharedPtr tmp(ptr);
			swap(tmp);
		}
		return *this;
	}
	
	template <class Other>
	SharedPtr& assign(const SharedPtr<Other>& ptr)
	{
		if (ptr.get() != _ptr)
		{
			SharedPtr tmp(ptr);
			swap(tmp);
		}
		return *this;
	}

	SharedPtr& operator = (C* ptr)
	{
		return assign(ptr);
	}

	SharedPtr& operator = (const SharedPtr& ptr)
	{
		return assign(ptr);
	}

	template <class Other>
	SharedPtr& operator = (const SharedPtr<Other>& ptr)
	{
		return assign<Other>(ptr);
	}

	void swap(SharedPtr& ptr)
	{
		std::swap(_ptr, ptr._ptr);
		std::swap(_pCounter, ptr._pCounter);
	}

	template <class Other> 
	SharedPtr<Other> cast() const
		/// Casts the SharedPtr via a dynamic cast to the given type.
		/// Returns an SharedPtr containing NULL if the cast fails.
		/// Example: (assume class Sub: public Super)
		///    SharedPtr<Super> super(new Sub());
		///    SharedPtr<Sub> sub = super.cast<Sub>();
		///    poco_assert (sub.get());
	{
		Other* pOther = dynamic_cast<Other*>(_ptr);
		if (pOther)
			return SharedPtr<Other>(_pCounter, pOther);
		return SharedPtr<Other>();
	}

	C* operator -> ()
	{
		return deref();
	}

	const C* operator -> () const
	{
		return deref();
	}

	C& operator * ()
	{
		return *deref();
	}

	const C& operator * () const
	{
		return *deref();
	}

	C* get()
	{
		return _ptr;
	}

	const C* get() const
	{
		return _ptr;
	}

	operator C* ()
	{
		return _ptr;
	}
	
	operator const C* () const
	{
		return _ptr;
	}

	bool operator ! () const
	{
		return _ptr == 0;
	}

	bool isNull() const
	{
		return _ptr == 0;
	}

	bool operator == (const SharedPtr& ptr) const
	{
		return get() == ptr.get();
	}

	bool operator == (const C* ptr) const
	{
		return get() == ptr;
	}

	bool operator == (C* ptr) const
	{
		return get() == ptr;
	}

	bool operator != (const SharedPtr& ptr) const
	{
		return get() != ptr.get();
	}

	bool operator != (const C* ptr) const
	{
		return get() != ptr;
	}

	bool operator != (C* ptr) const
	{
		return get() != ptr;
	}

	bool operator < (const SharedPtr& ptr) const
	{
		return get() < ptr.get();
	}

	bool operator < (const C* ptr) const
	{
		return get() < ptr;
	}

	bool operator < (C* ptr) const
	{
		return get() < ptr;
	}

	bool operator <= (const SharedPtr& ptr) const
	{
		return get() <= ptr.get();
	}

	bool operator <= (const C* ptr) const
	{
		return get() <= ptr;
	}

	bool operator <= (C* ptr) const
	{
		return get() <= ptr;
	}

	bool operator > (const SharedPtr& ptr) const
	{
		return get() > ptr.get();
	}

	bool operator > (const C* ptr) const
	{
		return get() > ptr;
	}

	bool operator > (C* ptr) const
	{
		return get() > ptr;
	}

	bool operator >= (const SharedPtr& ptr) const
	{
		return get() >= ptr.get();
	}

	bool operator >= (const C* ptr) const
	{
		return get() >= ptr;
	}

	bool operator >= (C* ptr) const
	{
		return get() >= ptr;
	}

private:
	C* deref() const
	{
		if (!_ptr)
			throw NullPointerException();

		return _ptr;
	}

	void release()
	{
		poco_assert_dbg (_pCounter);
		int i = _pCounter->release();
		if (i == 0)
		{
			if (_ptr)
				delete _ptr;
			_ptr = 0;

			delete _pCounter;
			_pCounter = 0;
		}
	}

	SharedPtr(ReferenceCounter* pCounter, C* ptr): _pCounter(pCounter), _ptr(ptr)
		/// for cast operation
	{
		poco_assert_dbg (_pCounter);
		_pCounter->duplicate();
	}

private:
	ReferenceCounter* _pCounter;
	C*                _ptr;

	template<class Other> friend class SharedPtr;
};


template <class C>
inline void swap(SharedPtr<C>& p1, SharedPtr<C>& p2)
{
	p1.swap(p2);
}


} // namespace Poco


#endif // Foundation_SharedPtr_INCLUDED
