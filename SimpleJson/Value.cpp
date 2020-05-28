#include "Value.h"

//
// MIT License
// 
// Copyright (c) 2020 Pharap (@Pharap)
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

#include <type_traits>

#include "InvalidTypeAccess.h"

namespace SimpleJson
{
	template<typename Type, std::size_t size>
	Type & lvalueBuffer(unsigned char(&buffer)[size])
	{
		return *reinterpret_cast<std::remove_reference_t<Type> *>(buffer);
	}

	template<typename Type, std::size_t size>
	const Type & lvalueBuffer(const unsigned char(&buffer)[size])
	{
		return *reinterpret_cast<const std::remove_reference_t<Type> *>(buffer);
	}

	template<typename Type, std::size_t size>
	Type && rvalueBuffer(unsigned char(&buffer)[size])
	{
		return std::move(*reinterpret_cast<std::remove_reference_t<Type> *>(buffer));
	}

	template<typename Type, std::size_t size>
	void destroyBuffer(unsigned char(&buffer)[size])
	{
		reinterpret_cast<std::remove_reference_t<Type> *>(buffer)->~Type();
	}

	void Value::initialiseFrom(const Value & other)
	{
		switch(other.type)
		{
		case ValueType::Null:
			new (this->data) Null(lvalueBuffer<Null>(other.data));
			break;

		case ValueType::Boolean:
			new (this->data) Boolean(lvalueBuffer<Boolean>(other.data));
			break;

		case ValueType::Number:
			new (this->data) Number(lvalueBuffer<Number>(other.data));
			break;

		case ValueType::String:
			new (this->data) String(lvalueBuffer<String>(other.data));
			break;

		case ValueType::Object:
			new (this->data) Object(lvalueBuffer<Object>(other.data));
			break;

		case ValueType::Array:
			new (this->data) Array(lvalueBuffer<Array>(other.data));
			break;
		}
	}

	void Value::initialiseFrom(Value && other)
	{
		switch(other.type)
		{
		case ValueType::Null:
			new (this->data) Null(rvalueBuffer<Null>(other.data));
			break;

		case ValueType::Boolean:
			new (this->data) Boolean(rvalueBuffer<Boolean>(other.data));
			break;

		case ValueType::Number:
			new (this->data) Number(rvalueBuffer<Number>(other.data));
			break;

		case ValueType::String:
			new (this->data) String(rvalueBuffer<String>(other.data));
			break;

		case ValueType::Object:
			new (this->data) Object(rvalueBuffer<Object>(other.data));
			break;

		case ValueType::Array:
			new (this->data) Array(rvalueBuffer<Array>(other.data));
			break;
		}
	}

	void Value::assignFromSameType(const Value & other)
	{
		switch(other.type)
		{
		case ValueType::Null:
			lvalueBuffer<Null>(this->data) = lvalueBuffer<Null>(other.data);
			break;

		case ValueType::Boolean:
			lvalueBuffer<Boolean>(this->data) = lvalueBuffer<Boolean>(other.data);
			break;

		case ValueType::Number:
			lvalueBuffer<Number>(this->data) = lvalueBuffer<Number>(other.data);
			break;

		case ValueType::String:
			lvalueBuffer<String>(this->data) = lvalueBuffer<String>(other.data);
			break;

		case ValueType::Object:
			lvalueBuffer<Object>(this->data) = lvalueBuffer<Object>(other.data);
			break;

		case ValueType::Array:
			lvalueBuffer<Array>(this->data) = lvalueBuffer<Array>(other.data);
			break;
		}
	}

	void Value::assignFromSameType(Value && other)
	{
		switch(other.type)
		{
		case ValueType::Null:
			lvalueBuffer<Null>(this->data) = rvalueBuffer<Null>(other.data);
			break;

		case ValueType::Boolean:
			lvalueBuffer<Boolean>(this->data) = rvalueBuffer<Boolean>(other.data);
			break;

		case ValueType::Number:
			lvalueBuffer<Number>(this->data) = rvalueBuffer<Number>(other.data);
			break;

		case ValueType::String:
			lvalueBuffer<String>(this->data) = rvalueBuffer<String>(other.data);
			break;

		case ValueType::Object:
			lvalueBuffer<Object>(this->data) = rvalueBuffer<Object>(other.data);
			break;

		case ValueType::Array:
			lvalueBuffer<Array>(this->data) = rvalueBuffer<Array>(other.data);
			break;
		}
	}

	void Value::destroy()
	{
		switch(this->type)
		{
		case ValueType::Null:
			destroyBuffer<Null>(this->data);
			break;

		case ValueType::Boolean:
			destroyBuffer<Boolean>(this->data);
			break;

		case ValueType::Number:
			destroyBuffer<Number>(this->data);
			break;

		case ValueType::String:
			destroyBuffer<String>(this->data);
			break;

		case ValueType::Object:
			destroyBuffer<Object>(this->data);
			break;

		case ValueType::Array:
			destroyBuffer<Array>(this->data);
			break;
		}
	}

	Value & Value::operator =(const Value & other)
	{
		if(this->type != other.type)
		{
			this->destroy();
			this->initialiseFrom(other);
		}
		else
		{
			this->assignFromSameType(other);
		}

		this->type = other.type;

		return *this;
	}

	Value & Value::operator =(Value && other)
	{
		if(this->type != other.type)
		{
			this->destroy();
			this->initialiseFrom(other);
		}
		else
		{
			this->assignFromSameType(other);
		}

		this->type = other.type;

		return *this;
	}


	const Null & Value::asNull() const
	{
		if(this->type != ValueType::Null)
			throw InvalidTypeAccess();

		return lvalueBuffer<Null>(this->data);
	}

	const Boolean & Value::asBoolean() const
	{
		if(this->type != ValueType::Boolean)
			throw InvalidTypeAccess();

		return lvalueBuffer<Boolean>(this->data);
	}

	const Number & Value::asNumber() const
	{
		if(this->type != ValueType::Number)
			throw InvalidTypeAccess();

		return lvalueBuffer<Number>(this->data);
	}

	const String & Value::asString() const
	{
		if(this->type != ValueType::String)
			throw InvalidTypeAccess();

		return lvalueBuffer<String>(this->data);
	}

	const Object & Value::asObject() const
	{
		if(this->type != ValueType::Object)
			throw InvalidTypeAccess();

		return lvalueBuffer<Object>(this->data);
	}

	const Array & Value::asArray() const
	{
		if(this->type != ValueType::Array)
			throw InvalidTypeAccess();

		return lvalueBuffer<Array>(this->data);
	}
}