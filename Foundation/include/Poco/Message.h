//
// Message.h
//
// $Id: //poco/1.2/Foundation/include/Poco/Message.h#2 $
//
// Library: Foundation
// Package: Logging
// Module:  Message
//
// Definition of the Message class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
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


#ifndef Foundation_Message_INCLUDED
#define Foundation_Message_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Timestamp.h"
#include <map>


namespace Poco {


class Foundation_API Message
	/// This class represents a log message that is sent through a
	/// chain of log channels.
	///
	/// A Message contains a priority denoting the severity of the
	/// message, a source describing its origin, a text describing
	/// its meaning, the time of its creation, and an identifier of
	/// the process and thread that created the message.
	///
	/// A Message can also contain any number of named parameters
	/// that contain additional information about the event that
	/// caused the message.
{
public:
	enum Priority
	{
		PRIO_FATAL = 1,   /// A fatal error. The application will most likely terminate. This is the highest priority.
		PRIO_CRITICAL,    /// A critical error. The application might not be able to continue running successfully.
		PRIO_ERROR,       /// An error. An operation did not complete successfully, but the application as a whole is not affected.
		PRIO_WARNING,     /// A warning. An operation completed with an unexpected result.
		PRIO_NOTICE,      /// A notice, which is an information with just a higher priority.
		PRIO_INFORMATION, /// An informational message, usually denoting the successful completion of an operation.
		PRIO_DEBUG,       /// A debugging message.
		PRIO_TRACE        /// A tracing message. This is the lowest priority.
	};
	
	Message();
		/// Creates an empty Message.
		/// The thread and process ids are set.
		
	Message(const std::string& source, const std::string& text, Priority prio);
		/// Creates a Message with the given source, text and priority.
		/// The thread and process ids are set.
		
	Message(const Message& msg);
		/// Creates a Message by copying another one.
		
	Message(const Message& msg, const std::string& text);
		/// Creates a Message by copying all but the text from another message.
		
	~Message();
		/// Destroys the Message.
	
	Message& operator = (const Message& msg);
		/// Assignment operator.
		
	void swap(Message& msg);
		/// Swaps the message with another one.	
		
	void setSource(const std::string& src);
		/// Sets the source of the message.
		
	const std::string& getSource() const;
		/// Returns the source of the message.
		
	void setText(const std::string& text);
		/// Sets the text of the message.
		
	const std::string& getText() const;
		/// Returns the text of the message.
		
	void setPriority(Priority prio);
		/// Sets the priority of the message.
		
	Priority getPriority() const;
		/// Returns the priority of the message.
	
	void setTime(const Timestamp& time);
		/// Sets the time of the message.
		
	const Timestamp& getTime() const;
		/// Returns the time of the message.
		
	void setThread(const std::string& thread);
		/// Sets the thread identifier for the message.
		
	const std::string& getThread() const;
		/// Returns the thread identifier for the message.

	void setTid(long pid);
		/// Sets the numeric thread identifier for the message.
		
	long getTid() const;
		/// Returns the numeric thread identifier for the message.
	
	void setPid(long pid);
		/// Sets the process identifier for the message.
		
	long getPid() const;
		/// Returns the process identifier for the message.
		
	const std::string& operator [] (const std::string& param) const;
		/// Returns a const reference to the value of the parameter
		/// with the given name. Throws a NotFoundException if the
		/// parameter does not exist.
		
	std::string& operator [] (const std::string& param);
		/// Returns a reference to the value of the parameter with the
		/// given name. This can be used to set the parameter's value.
		/// If the parameter does not exist, it is created with an
		/// empty string value.

protected:
	void init();
	typedef std::map<std::string, std::string> StringMap;

private:	
	std::string _source;
	std::string _text;
	Priority    _prio;
	Timestamp   _time;
	int         _tid;
	std::string _thread;
	long        _pid;
	StringMap*  _pMap;
};


//
// inlines
//
inline const std::string& Message::getSource() const
{
	return _source;
}


inline const std::string& Message::getText() const
{
	return _text;
}


inline Message::Priority Message::getPriority() const
{
	return _prio;
}


inline const std::string& Message::getThread() const
{
	return _thread;
}


inline long Message::getTid() const
{
	return _tid;
}


inline long Message::getPid() const
{
	return _pid;
}


inline void swap(Message& m1, Message& m2)
{
	m1.swap(m2);
}


} // namespace Poco


#endif // Foundation_Message_INCLUDED
