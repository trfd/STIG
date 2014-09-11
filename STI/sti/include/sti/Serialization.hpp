///
///  Serialization.hpp
///  is part of STI Library.
///
///  Static Type Info - A C++ Static Reflexion Library based on Clang.
///  Get more infos on STIG at <https://github.com/trfd/STI>
///
///  Created by Baptiste Dupy on 08/09/2014.
///  Contact:
///          Mail:       <baptiste.dupy@gmail.com>
///          GitHub:     trfd <https://github.com/trfd>
///          Website:    <http://www.trf-d.com>
///
///  Copyright (C) 2014 Baptiste (trfd) Dupy
///
///  This program is free software: you can redistribute it and/or modify
///  it under the terms of the GNU General Public License as published by
///  the Free Software Foundation, either version 3 of the License, or
///  (at your option) any later version.
///
///  This program is distributed in the hope that it will be useful,
///  but WITHOUT ANY WARRANTY; without even the implied warranty of
///  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
///  GNU General Public License for more details.
///
///  You should have received a copy of the GNU General Public License
///  along with this program.  If not, see <http://www.gnu.org/licenses/>.
///

#ifndef STIG_Serialization_hpp
#define STIG_Serialization_hpp

#include <string>
#include <vector>
#include <map>

namespace sti
{
    typedef std::string SerialKey;

    class Serializable;
    
    struct SerialValue
    {
        enum Type
        {
            SV_string,
            SV_array,
            SV_dict,
            SV_data,
            SV_unknown
        };
        
        SerialValue()
        : _type(SV_unknown)
        {}
        
        SerialValue(const Type& t_)
        : _type(t_)
        {}
        
        inline Type type(){ return _type; }
        
    protected:
        
        Type _type;
    };
    
    std::string serialType(const SerialValue::Type& t_)
    {
        switch(t_)
        {
            case SerialValue::SV_string : return "SV_string";
            case SerialValue::SV_array  : return "SV_array";
            case SerialValue::SV_dict   : return "SV_dict";
            case SerialValue::SV_data   : return "SV_data";
            case SerialValue::SV_unknown:
            default:         return "SV_unknown";
        }
    }
    
    struct StringSerialValue : public SerialValue
    {
        StringSerialValue()
        : SerialValue(SV_string),
        _stringValue("")
        {}
        
        StringSerialValue(const std::string& s_)
        : SerialValue(SV_string),
        _stringValue(s_)
        {}
    
        std::string _stringValue;
    };
    
    struct DataSerialValue : public SerialValue
    {
        DataSerialValue()
        : SerialValue(SV_data),
        _dataValue(nullptr),
        _dataLength(0)
        {}
        
        DataSerialValue(char* data_ , int length_)
        : SerialValue(SV_data),
        _dataValue(nullptr),
        _dataLength(0)
        {}
        
        char* _dataValue;
        unsigned _dataLength;
    };

    struct ArraySerialValue : public SerialValue
    {
        ArraySerialValue()
        : SerialValue(SV_array)
        {}
        
        std::vector<SerialValue*> _arrayValue;
    };
    
    struct DictSerialValue : public SerialValue
    {
        
        typedef std::map<SerialKey,SerialValue*> KeyValueMap;
        typedef KeyValueMap::iterator KeyValueMap_it;
        DictSerialValue()
        : SerialValue(SV_dict)
        {}
        
        std::map<SerialKey,SerialValue*> _dictValue;
    };
}

#endif
