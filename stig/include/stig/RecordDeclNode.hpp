///
///  RecordDeclNode.hpp
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

#ifndef STIG_RecordDeclNode_hpp
#define STIG_RecordDeclNode_hpp

#include <iostream>

#include <clang/AST/DeclCXX.h>
#include <clang/Basic/Specifiers.h>

using namespace clang;

namespace stig
{
    class RecordDeclNode;
    
    struct FieldDeclNode
    {
        FieldDeclNode(FieldDecl* astdecl)
        : astFieldDecl(astdecl),
        strName(static_cast<const NamedDecl*>(astdecl)->getNameAsString())
        {}
        
        FieldDeclNode(const FieldDeclNode& refnode)
        : astFieldDecl(refnode.astFieldDecl),
        strName(refnode.strName)
        {}
        
        void print() const
        {
            std::cout<<"Field: "<<strName<<"\n";
        }
        
        FieldDecl* astFieldDecl;
        std::string strName;
    };
    
    struct MethodDeclNode
    {        
        MethodDeclNode(CXXMethodDecl* astdecl)
        : astMethodDecl(astdecl),
        strName(static_cast<const NamedDecl*>(astdecl)->getNameAsString())
        {}
        
        MethodDeclNode(const MethodDeclNode& refnode)
        : astMethodDecl(refnode.astMethodDecl),
        strName(refnode.strName)
        {}
        
        void print() const
        {
            std::cout<<"Method: "<<strName;
            
            for(CXXMethodDecl::param_iterator it = astMethodDecl->param_begin() ;
                it != astMethodDecl->param_end() ; ++it)
            {
                if(!*it)
                    continue;
                std::cout<<(*it)->getNameAsString()<<",";
            }
            
            std::cout<<"\n";
        }
        
        CXXMethodDecl* astMethodDecl;
        std::string strName;
    };
    
    class RecordDeclNode
    {
    public:
        
        RecordDeclNode(CXXRecordDecl* astptr)
        : m_astRDecl(astptr)
        {
            extractFields();
        }
        
        CXXRecordDecl* ASTRecordDecl(){ return m_astRDecl; }
        
        void print()
        {
            std::cout<<"Class: "<<strName<<"\n";
            
            if(m_fields[AS_public].size() || m_methods[AS_public].size())
                std::cout<<"----|| Public:\n";
            
            for(const FieldDeclNode& field : m_fields[AccessSpecifier::AS_public])
                field.print();
            for(const MethodDeclNode& meth : m_methods[AS_public])
                meth.print();
            
            if(m_fields[AS_protected].size() || m_methods[AS_protected].size())
                std::cout<<"----|| Protected:\n";
            
            for(const FieldDeclNode& field : m_fields[AS_protected])
                field.print();
            for(const MethodDeclNode& meth : m_methods[AS_protected])
                meth.print();
            
            if(m_fields[AS_private].size() || m_methods[AS_private].size())
                std::cout<<"----|| Private:\n";
            
            for(const FieldDeclNode& field : m_fields[AS_private])
                field.print();
            for(const MethodDeclNode& meth : m_methods[AS_private])
                meth.print();
        }
        
    private:
        
        void extractFields()
        {
            strName =static_cast<const NamedDecl*>(m_astRDecl)->getNameAsString();
            
            for(CXXRecordDecl::method_iterator it = m_astRDecl->method_begin() ;
                it != m_astRDecl->method_end() ; ++it)
            {
                m_methods[it->getAccess()].emplace_back(*it);
            }
            
            for(DeclContext::decl_iterator it = static_cast<DeclContext*>(m_astRDecl)->decls_begin() ;
                it != static_cast<DeclContext*>(m_astRDecl)->decls_end() ; ++it)
            {
                if(it->getKind() == Decl::Kind::Field)
                {
                    m_fields[it->getAccess()].emplace_back(static_cast<FieldDecl*>(*it));
                }
            }
        }
        
        CXXRecordDecl* m_astRDecl;
        
        std::string strName;
        
        std::map<AccessSpecifier,std::vector<FieldDeclNode>> m_fields;
        
        std::map<AccessSpecifier,std::vector<MethodDeclNode>> m_methods;
        
    };
}

#endif
