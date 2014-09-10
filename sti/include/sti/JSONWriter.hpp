//
//  JSONWriter.hpp
//  STIG
//
//  Created by Baptiste Dupy on 09/09/14.
//
//

#ifndef STIG_JSONWriter_hpp
#define STIG_JSONWriter_hpp

#include <sstream>

#include "Serialization.hpp"

namespace sti
{
    class JSONWriter
    {
    public:
        
        void writeSerializable(std::ostringstream& stream, SerialValue* val_, int indent)
        {
            switch (val_->type())
            {
                case SerialValue::SV_string:
                    writeString(stream,static_cast<StringSerialValue*>(val_),indent);
                    break;
                case SerialValue::SV_array:
                    writeArray(stream, static_cast<ArraySerialValue*>(val_),indent);
                    break;
                case SerialValue::SV_dict:
                    writeDict(stream,  static_cast<DictSerialValue*>(val_),indent);
                    break;
                case SerialValue::SV_data:
                    writeData(stream,  static_cast<DataSerialValue*>(val_),indent);
                    break;
                default:
                    throw std::exception();
                    break;
            }
        }
        
        void writeDict(std::ostringstream& stream, DictSerialValue* val_, int indent)
        {
            typedef std::map<SerialKey,SerialValue*>::iterator Map_it;
            
            stream<<"{";
            
            if(val_->_dictValue.size() > 0)
                stream<<"\n";
            
            for(Map_it it = val_->_dictValue.begin();
                it != val_->_dictValue.end() ; ++it)
            {
                stream<<tabs(indent+1)<<"{\""<<it->first<<"\":";
                
                writeSerializable(stream,it->second,indent+1);
                
                stream<<"}";
                
                if(std::next(it) != val_->_dictValue.end())
                    stream<<",";
                
                stream<<"\n";
            }
            
            stream<<tabs(indent)<<"}";
        }
        
        void writeArray(std::ostringstream& stream, ArraySerialValue* val_, int indent)
        {
            stream<<"[";
            
            if(val_->_arrayValue.size() > 0)
                stream<<"\n";
            
            for(std::vector<SerialValue*>::iterator it = val_->_arrayValue.begin();
                it != val_->_arrayValue.end() ; ++it)
            {
                stream<<tabs(indent+1);
                writeSerializable(stream,*it,indent+1);
                
                if(std::next(it) != val_->_arrayValue.end())
                    stream<<",";
                
                stream<<"\n";
            }
            if(val_->_arrayValue.size() > 0)
                stream<<tabs(indent);
            stream<<"]";
        }
        
        void writeString(std::ostringstream& stream, StringSerialValue* val_ , int indent)
        {
            stream<<"\""<<val_->_stringValue<<"\"";
        }
        
        void writeData(std::ostringstream& stream, DataSerialValue* val_, int indent)
        {
            stream<<std::hex<<std::bitset<8>(val_->_dataValue,val_->_dataLength);
        }
    };
}

#endif
