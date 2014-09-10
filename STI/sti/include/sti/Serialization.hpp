//
//  Serialization.hpp
//  STIG
//
//  Created by Baptiste Dupy on 09/09/14.
//
//

#ifndef STIG_Serialization_hpp
#define STIG_Serialization_hpp

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
        DictSerialValue()
        : SerialValue(SV_dict)
        {}
        
        std::map<SerialKey,SerialValue*> _dictValue;
    };
}

#endif
