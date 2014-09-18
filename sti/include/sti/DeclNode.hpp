///
///  DeclNode.hpp
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

#ifndef STIG_DeclNode_hpp
#define STIG_DeclNode_hpp

#include <iostream>

#include "Serialization.hpp"
#include "Decl.hpp"

namespace sti
{
    struct DeclNode
    {
        enum Kind
        {
            UNKNOWN,
            FIELD,
            METHOD,
            RECORD
        };
        
        enum Access
        {
            NONE,
            
            PUBLIC,
            PROTECTED,
            PRIVATE
        };
        
        DeclNode()
        : m_kind(UNKNOWN)
        {}
        
        DeclNode(const Kind& t_)
        : m_kind(t_)
        {}
    
        inline Kind kind(){ return m_kind; }
        
    private:
        
        Kind m_kind;
    };
    
    class RecordDeclNode;
    
    struct FieldDeclNode : public DeclNode, public Field
    {
        FieldDeclNode()
        : DeclNode(FIELD)
        {}
    };
    
    struct MethodDeclNode : public DeclNode, public Method
    {        
        MethodDeclNode()
        : DeclNode(METHOD)
        {}
    };
    
    class RecordDeclNode : public DeclNode, public Type
    {
    public:
        
        RecordDeclNode()
        : DeclNode(RECORD)
        {}
    };
}

#endif
