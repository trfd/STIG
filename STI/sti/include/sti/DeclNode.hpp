///
///  DeclNode.hpp
///  is part of STIG Project.
///
///  Static Type Info Generator - A C++ Static Reflexion Tool based on Clang.
///  Get more infos on STIG at <https://github.com/trfd/STIG>
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

#include <clang/AST/DeclCXX.h>
#include <clang/Basic/Specifiers.h>

#include "Serialization.hpp"


using namespace clang;

namespace sti
{
    struct DeclNode
    {
        enum Type
        {
            DN_unknown,
            DN_field,
            DN_method,
            DN_record
        };
        
        DeclNode()
        : m_type(DN_unknown)
        {}
        
        DeclNode(const Type& t_)
        : m_type(t_)
        {}
    
        inline Type type(){ return m_type; }
        
    private:
        
        Type m_type;
    };
    
    class RecordDeclNode;
    
    struct FieldDeclNode : public DeclNode
    {
        FieldDeclNode()
        : DeclNode(DN_field)
        {}
        
        std::string _name;
        std::string _type;
    };
    
    struct MethodDeclNode : public DeclNode
    {        
        MethodDeclNode(CXXMethodDecl* astdecl)
        : DeclNode(DN_method)
        {}

        std::string _name;
        std::vector<std::string> _params;
    };
    
    class RecordDeclNode : public DeclNode
    {
    public:
        
        RecordDeclNode(CXXRecordDecl* astptr)
        : DeclNode(DN_record)
        {}
        
        void extractFields(CXXRecordDecl* astptr)
        {
            _name =static_cast<const NamedDecl*>(astptr)->getNameAsString();
            
            _access = astptr->getAccess();
            
            for(CXXRecordDecl::method_iterator it = astptr->method_begin() ;
                it != astptr->method_end() ; ++it)
            {
                _methods.emplace_back(*it);
            }
            
            for(DeclContext::decl_iterator it = static_cast<DeclContext*>(astptr)->decls_begin() ;
                it != static_cast<DeclContext*>(astptr)->decls_end() ; ++it)
            {
                if(it->getKind() == Decl::Kind::Field)
                {
                    _fields.emplace_back(static_cast<FieldDecl*>(*it));
                }
            }
            
            extractContext(astptr);
        }
        
        void extractContext(CXXRecordDecl* astptr)
        {
            _context = "";
            
            DeclContext* dclCtx = astptr->getDeclContext();
            
            do
            {
                
                if(dclCtx->isNamespace())
                {
                    _context = static_cast<NamespaceDecl*>(dclCtx)->getNameAsString() + "::" + _context;
                }
                else if(dclCtx->isRecord())
                {
                   _context = static_cast<CXXRecordDecl*>(dclCtx)->getNameAsString() + "::" + _context;
                }
                
                dclCtx = dclCtx->getParent();

            }while(dclCtx);
        }
        
        AccessSpecifier _access;
        
        std::string _name;
        
        std::string _context;
        
        std::vector<FieldDeclNode> _fields;
        
        std::vector<MethodDeclNode> _methods;
    };
}

#include "NodeSerialization.hpp"

#endif
