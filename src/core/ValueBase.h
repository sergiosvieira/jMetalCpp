//  Variable.h
//
//  Author:
//       Sérgio S. Vieira <sergiosvieira@gmail.com>
//
//  Copyright (c) 2011 Antonio J. Nebro, Juan J. Durillo
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __VALUE_BASE__
#define __VALUE_BASE__

#include <iostream>
#include <memory>

enum class ValueType
{
    Integer,
    Double,
    Bool,
    Float,
    String,
    Object
};

class ValueBase
{
protected:
    ValueType m_type;
public:
    ValueBase(){}
    ~ValueBase(){}
    ValueType getType()
    {
        return m_type;
    }
    virtual void stream(std::ostream& os){};
    friend std::ostream& operator<<(std::ostream& os, ValueBase& instance)
    {
        instance.stream(os);
        return os;
    }
};

using ValuePtr = std::shared_ptr<ValueBase>;

#endif /** __VALUE_BASE__ **/