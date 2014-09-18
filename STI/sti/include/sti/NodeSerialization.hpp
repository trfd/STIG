///
///  NodeSerialization.hpp
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

#ifndef STIG_NodeSerialization_hpp
#define STIG_NodeSerialization_hpp

#include "Serialization.hpp"

namespace sti
{
    
       
   
    DictSerialValue* serialize(FieldDeclNode* node)
    {
        DictSerialValue* dict = new DictSerialValue();

        dict->_dictValue["name"] = new StringSerialValue(node->name());
        dict->_dictValue["type"] = new StringSerialValue(node->type()->name());
        
        return dict;
    }
    
    DictSerialValue* serialize(MethodDeclNode* node)
    {
        DictSerialValue* dict = new DictSerialValue();
        
        dict->_dictValue["name"] = new StringSerialValue(node->name());
        //dict._dictValue["type"] = new StringSerialValue(node->_type);
        
        return dict;
    }
    
    DictSerialValue* serialize(RecordDeclNode* node)
    {
        DictSerialValue* dict = new DictSerialValue();
        
        dict->_dictValue["access"] = new StringSerialValue(node->accessAsString());
        dict->_dictValue["name"]   = new StringSerialValue(node->name());
        dict->_dictValue["context"]= new StringSerialValue(node->context());
        
        ArraySerialValue* arrFields = new ArraySerialValue();
        for(unsigned idx = 0 ; idx < node->fieldCount() ; ++idx)
        {
            FieldDeclNode* field = (FieldDeclNode*) node->fields()[idx];
            arrFields->_arrayValue.push_back(serialize(field));
        }
        
        dict->_dictValue["fields"] = arrFields;
        
        ArraySerialValue* arrMethods = new ArraySerialValue();
        for(unsigned idx = 0 ; idx < node->methodCount() ; ++idx)
        {
            MethodDeclNode* method = (MethodDeclNode*) node->methods()[idx];
            arrMethods->_arrayValue.push_back(serialize(method));
        }
        
        dict->_dictValue["methods"] = arrMethods;
        
        return dict;
    }

}

#endif
