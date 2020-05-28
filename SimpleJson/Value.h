#pragma once

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

#include "Types.h"
#include "ValueType.h"

namespace SimpleJson
{
	class Value
	{
	private:
		union Union
		{
			Null null;
			Boolean boolean;
			Number number;
			String string;
			Object object;
			Array array;

			Union() = delete;
			~Union() = delete;
		};

	private:
		ValueType type;
		unsigned char data[sizeof(Union)];

	private:
		void initialiseFrom(const Value & other);
		void initialiseFrom(Value && other);
		void assignFromSameType(const Value & other);
		void assignFromSameType(Value && other);
		void destroy();

	public:
		~Value()
		{
			this->destroy();
			this->type = ValueType::Null;
		}

		Value() :
			type { ValueType::Null }, data {}
		{
		}

		Value(Null && null) :
			type { ValueType::Null }, data {}
		{
			new (this->data) Null(null);
		}

		Value(Boolean && boolean) :
			type { ValueType::Boolean }, data {}
		{
			new (this->data) Boolean(boolean);
		}

		Value(Number && number) :
			type { ValueType::Number }, data {}
		{
			new (this->data) Number(number);
		}

		Value(String && string) :
			type { ValueType::String }, data {}
		{
			new (this->data) String(string);
		}

		Value(const Object & object) :
			type { ValueType::Object }, data {}
		{
			new (this->data) Object(object);
		}

		Value(Object && object) :
			type { ValueType::Object }, data {}
		{
			new (this->data) Object(object);
		}

		Value(const Array & array) :
			type { ValueType::Array }, data {}
		{
			new (this->data) Array(array);
		}

		Value(Array && array) :
			type { ValueType::Array }, data {}
		{
			new (this->data) Array(array);
		}

		Value(const Value & other) :
			type { other.type }, data {}
		{
			this->initialiseFrom(other);
		}

		Value(Value && other) :
			type { other.type }, data {}
		{
			this->initialiseFrom(other);
		}

		ValueType getType() const
		{
			return this->type;
		}

		const Null & asNull() const;
		const Boolean & asBoolean() const;
		const Number & asNumber() const;
		const String & asString() const;
		const Object & asObject() const;
		const Array & asArray() const;

		Value & operator =(const Value & other);
		Value & operator =(Value && other);
	};
}