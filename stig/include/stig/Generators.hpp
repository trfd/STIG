//
//  Generators.hpp
//  STIG
//
//  Created by Baptiste Dupy on 11/09/14.
//
//

#ifndef STIG_Generators_hpp
#define STIG_Generators_hpp

#include "FileGenerator.hpp"

using namespace sti;

namespace stig
{
    class ClassInfoGenerator : public FileGenerator
    {
    public:
        
        void setRecordDecl(RecordDeclNode* node)
        {
            setRootDictValue(serialize(node));
        }

    };
}

#endif
