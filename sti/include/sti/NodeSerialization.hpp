//
//  NodeSerialization.hpp
//  STIG
//
//  Created by Baptiste Dupy on 09/09/14.
//
//

#ifndef STIG_NodeSerialization_hpp
#define STIG_NodeSerialization_hpp

#include "Serialization.hpp"

namespace sti
{
    std::string tabs(int n_)
    {
        std::string result = "";
        for(int i=0 ; i<n_ ; i++)
            result += "\t";
        return result;
    }
    
    std::string access(const clang::AccessSpecifier& acc_)
    {
        switch(acc_)
        {
            case AS_public:
                return "public";
            case AS_protected:
                return "protected";
            case AS_private:
                return "private";
            case AS_none:
            default:
                return "none";
        }
    }
    
   
    DictSerialValue* serialize(FieldDeclNode* node)
    {
        DictSerialValue* dict = new DictSerialValue();

        dict->_dictValue["name"] = new StringSerialValue(node->_name);
        dict->_dictValue["type"] = new StringSerialValue(node->_type);
        
        return dict;
    }
    
    DictSerialValue* serialize(MethodDeclNode* node)
    {
        DictSerialValue* dict = new DictSerialValue();
        
        dict->_dictValue["name"] = new StringSerialValue(node->_name);
        //dict._dictValue["type"] = new StringSerialValue(node->_type);
        
        return dict;
    }
    
    DictSerialValue* serialize(RecordDeclNode* node)
    {
        DictSerialValue* dict = new DictSerialValue();
        
        dict->_dictValue["access"] = new StringSerialValue(access(node->_access));
        dict->_dictValue["name"]   = new StringSerialValue(node->_name);
        dict->_dictValue["context"]= new StringSerialValue(node->_context);
        
        ArraySerialValue* arrFields = new ArraySerialValue();
        for(FieldDeclNode* field : node->_fields)
            arrFields->_arrayValue.push_back(serialize(field));
        
        dict->_dictValue["fields"] = arrFields;
        
        ArraySerialValue* arrMethods = new ArraySerialValue();
        for(MethodDeclNode* method : node->_methods)
            arrMethods->_arrayValue.push_back(serialize(method));
        
        dict->_dictValue["methods"] = arrMethods;
        
        return dict;
    }

}

#endif
