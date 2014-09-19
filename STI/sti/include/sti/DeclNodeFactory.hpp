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

#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>

#include <cytok/utils/UniFactory.hpp>

#include "DeclNode.hpp"
#include "TypeDatabase.hpp"

using namespace clang;
using namespace sti;

namespace stig
{
    typedef std::function<DeclNode*(Decl*)> DeclNodeCreator;
    
    typedef ck::UniFactory<DeclNode,DeclNode::Kind,DeclNodeCreator> DeclNodeUniFactory;
    
    struct DeclNodeFactory : public DeclNodeUniFactory
    {
        static DeclNode* createField(clang::Decl* field)
        {
            if(field->getKind() != Decl::Field)
                throw std::exception();
            
            clang::FieldDecl* fieldDecl = static_cast<clang::FieldDecl*>(field);
            
            FieldDeclNode* node = new FieldDeclNode();
            
            std::cout<<"-----------Type:"<<fieldDecl->getType().getAsString()<<"\n";
            std::cout<<"-----------C"<<fieldDecl->getType()->getTypeClassName()<<"\n";
            
            node->setName(fieldDecl->getNameAsString());
            node->setType(TypeDatabase::instance()->typeWith(fieldDecl->getType().getAsString()));
            
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
            
            node->setName(methDecl->getNameAsString());
            
            return node;
        }
        
        static DeclNode* createRecord(clang::Decl* record)
        {
            if(record->getKind() != Decl::CXXRecord)
                throw std::exception();
            
            clang::CXXRecordDecl* recordDecl = static_cast<clang::CXXRecordDecl*>(record);
            
            RecordDeclNode* node = new RecordDeclNode();
            
            node->setName(recordDecl->getNameAsString());
            
            node->setAccess((Access) recordDecl->getAccess());
            
            /// Retrieve methods
            for(CXXRecordDecl::method_iterator it = recordDecl->method_begin() ;
                it != recordDecl->method_end() ; ++it)
            {
                node->addMethod(static_cast<MethodDeclNode*>(createMethod(*it)));
            }
            
            /// Retrieve fields
            for(DeclContext::decl_iterator it = recordDecl->decls_begin() ;
                it != recordDecl->decls_end() ; ++it)
            {
                if(it->getKind() == Decl::Kind::Field)
                {
                    node->addField(static_cast<FieldDeclNode*>(createField(static_cast<FieldDecl*>(*it))));
                }
            }
            
            extractContext(recordDecl,node);
            
            return node;
        }
        
        static void extractContext(CXXRecordDecl* astptr,RecordDeclNode* decl)
        {
            std::string tmpCtx = "";
            
            DeclContext* dclCtx = astptr->getDeclContext();
            
            do
            {
                if(dclCtx->isNamespace())
                {
                    tmpCtx = static_cast<NamespaceDecl*>(dclCtx)->getNameAsString() + "::" + tmpCtx;
                }
                else if(dclCtx->isRecord())
                {
                    tmpCtx = static_cast<CXXRecordDecl*>(dclCtx)->getNameAsString() + "::" + tmpCtx;
                }
                
                dclCtx = dclCtx->getParent();
                
            }while(dclCtx);
        }
        
        void registerAllCreator()
        {
            registerCreator(DeclNode::FIELD,&DeclNodeFactory::createField);
            registerCreator(DeclNode::METHOD,&DeclNodeFactory::createMethod);
            registerCreator(DeclNode::RECORD,&DeclNodeFactory::createRecord);
        }
    };

}


#endif
