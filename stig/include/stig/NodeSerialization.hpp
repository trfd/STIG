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
    
    DictSerialValue serialize(RecordDeclNode* node)
    {
        DictSerialValue dict;
        
        dict._dictValue["access"] = new StringSerialValue(access(node->_access));
        
        return dict;
    }

}

#endif
