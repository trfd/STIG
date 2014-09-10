///
///  DeclNodeFactory.hpp
///  is part of STIG Library.
///
///  Static Type Info Generator - A C++ Static Reflexion Tool based on Clang.
///  Get more infos on STIG at <https://github.com/trfd/STI>
///
///  Created by Baptiste Dupy on 10/09/2014.
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

#ifndef STIG_DeclNodeFactory_hpp
#define STIG_DeclNodeFactory_hpp

#include "UniFactory.hpp"

#include <sti/DeclNode.hpp>

using namespace sti;

namespace stig
{
    typedef std::function<DeclNode*(Decl*)> DeclNodeCreator;
    
    typedef ck::UniFactory<DeclNode,DeclNode::Type,DeclNodeCreator> DeclNodeUniFactory;
    
    struct DeclNodeFactory : public DeclNodeUniFactory
    {
        static DeclNode* createField(clang::Decl* field)
        {
            if(field->getKind() != Decl::Field)
                throw std::exception();
            
            clang::FieldDecl* fieldDecl = static_cast<clang::FieldDecl*>(field);
            
            FieldDeclNode* node = new FieldDeclNode();
            
            node->_name = fieldDecl->getNameAsString();
            node->_type = fieldDecl->getType().getAsString();
            
            return node;
        }
        
        static DeclNode* createMethod(clang::Decl* method)
        {
            if(method->getKind() != Decl::CXXMethod      &&
               method->getKind() != Decl::CXXDestructor  &&
               method->getKind() != Decl::CXXConstructor &&
               method->getKind() != Decl::CXXConversion)
                throw std::exception();
            
            clang::CXXMethodDecl* methDecl = static_cast<clang::CXXMethodDecl*>(method);
            
            MethodDeclNode* node = new MethodDeclNode();
            
            node->_name = methDecl->getNameAsString();
            
            return node;
        }
        
        static DeclNode* createRecord(clang::Decl* record)
        {
            if(record->getKind() != Decl::CXXRecord)
                throw std::exception();
            
            clang::CXXRecordDecl* recordDecl = static_cast<clang::CXXRecordDecl*>(record);
            
            RecordDeclNode* node = new RecordDeclNode();
            
            node->_name = recordDecl->getNameAsString();
            
            node->_access = recordDecl->getAccess();
            
            /// Retrieve methods
            for(CXXRecordDecl::method_iterator it = recordDecl->method_begin() ;
                it != recordDecl->method_end() ; ++it)
            {
                node->_methods.push_back(static_cast<MethodDeclNode*>(createMethod(*it)));
            }
            
            /// Retrieve fields
            for(DeclContext::decl_iterator it = recordDecl->decls_begin() ;
                it != recordDecl->decls_end() ; ++it)
            {
                if(it->getKind() == Decl::Kind::Field)
                {
                    node->_fields.push_back(static_cast<FieldDeclNode*>(createField(static_cast<FieldDecl*>(*it))));
                }
            }
            
            extractContext(recordDecl,node);
            
            return node;
        }
        
        static void extractContext(CXXRecordDecl* astptr,RecordDeclNode* decl)
        {
            decl->_context = "";
            
            DeclContext* dclCtx = astptr->getDeclContext();
            
            do
            {
                if(dclCtx->isNamespace())
                {
                    decl->_context = static_cast<NamespaceDecl*>(dclCtx)->getNameAsString() + "::" + decl->_context;
                }
                else if(dclCtx->isRecord())
                {
                    decl->_context = static_cast<CXXRecordDecl*>(dclCtx)->getNameAsString() + "::" + decl->_context;
                }
                
                dclCtx = dclCtx->getParent();
                
            }while(dclCtx);
        }
        
        void registerAllCreator()
        {
            registerCreator(DeclNode::DN_field,&DeclNodeFactory::createField);
            registerCreator(DeclNode::DN_method,&DeclNodeFactory::createMethod);
            registerCreator(DeclNode::DN_record,&DeclNodeFactory::createRecord);
        }
    };

}


#endif
