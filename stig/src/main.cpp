///
///  main.cpp
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

#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS

#include <boost/algorithm/string/replace.hpp>

#include "clang/Tooling/CommonOptionsParser.h"

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"

#include "llvm/Support/CommandLine.h"

#include <sti/DeclNode.hpp>
#include <sti/JSONWriter.hpp>
#include <sti/NodeSerialization.hpp>

#include "stig/DeclNodeFactory.hpp"
#include "stig/Generators.hpp"

using namespace clang;
using namespace clang::tooling;
using namespace llvm;

using namespace sti;

namespace stig
{
    std::vector<RecordDeclNode*> recordDecls;
    
    stig::DeclNodeFactory nodeFactory;
    
    class StigVisitor
    : public RecursiveASTVisitor<StigVisitor> {
    public:
        explicit StigVisitor(ASTContext *Context)
        : Context(Context) {}
        
        bool VisitCXXRecordDecl(CXXRecordDecl *Declaration)
        {
            FullSourceLoc FullLocation = Context->getFullLoc(Declaration->getLocStart());
            llvm::outs() << "Declaration at "
            << FullLocation.getSpellingLineNumber() << ":"
            << FullLocation.getSpellingColumnNumber() << "\n";
            
            Declaration->dump();
            
            recordDecls.push_back(static_cast<RecordDeclNode*>(
                        nodeFactory.createProduct(DeclNode::DN_record,Declaration)));
            
            return true;
        }
        
    private:
        ASTContext *Context;
    };
    
    class StigConsumer : public clang::ASTConsumer {
    public:
        explicit StigConsumer(ASTContext *Context)
        : Visitor(Context) {}
        
        virtual void HandleTranslationUnit(clang::ASTContext &Context) {
            Visitor.TraverseDecl(Context.getTranslationUnitDecl());
        }
    private:
        StigVisitor Visitor;
    };
    
    
    class StigAction : public clang::SyntaxOnlyAction {
    public:
        virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer( clang::CompilerInstance &Compiler, llvm::StringRef InFile)
        {
            return std::unique_ptr<clang::ASTConsumer>(new StigConsumer(&Compiler.getASTContext()));
        }
    };

}

static llvm::cl::OptionCategory MyToolCategory("my-tool options");

int main(int argc,const char **argv)
{
    CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());
    
    
    stig::nodeFactory.registerAllCreator();
    
    int result = Tool.run(newFrontendActionFactory<stig::StigAction>().get());
    
    sti::JSONWriter writer;
    std::ostringstream strStream;

    stig::ClassInfoGenerator fgen;
    
    for(sti::RecordDeclNode* decl : stig::recordDecls)
    {
        writer.writeDict(strStream, sti::serialize(decl), 0);
        strStream<<"\n";
        
        fgen.setRecordDecl(decl);
        
        std::string fullname = decl->_context + decl->_name;
        
        boost::replace_all(fullname, "::" , "_");
        
        fgen.processTemplate("/Users/Baba/dev/trfd/STIG/devt/test_files/test_template.sting",
                             "/Users/Baba/dev/trfd/STIG/devt/test_files/sti_"+fullname+".cpp");
    }
    
    std::cout<<strStream.str()<<"\n";

    return result;
}