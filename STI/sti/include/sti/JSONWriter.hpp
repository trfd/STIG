//
//  JSONWriter.hpp
//  STIG
//
//  Created by Baptiste Dupy on 09/09/14.
//
//

#ifndef STIG_JSONWriter_hpp
#define STIG_JSONWriter_hpp

#include <sstream>

namespace sti
{
        
    class JSONWriter
    {
    public:
        
        void writeRecordDecl(std::ostringstream& stream, RecordDeclNode* rdecl_, int indent)
        {
            stream<<tabs(indent)<<"{\n";
            
            stream<<tabs(indent+1)<<"\"qtype\":\""<<rdecl_->strName<<"\",\n";
            
            stream<<tabs(indent+1)<<"\"context\":\""<<rdecl_->context<<"\",\n";
            
            // Print class fields
            stream<<tabs(indent+1)<<"\"access\":\""<<access(rdecl_->access)<<"\",\n";
            
            // Print class fields
            stream<<tabs(indent+1)<<"\"fields\":[\n";
            
            for(FieldDeclNode& field : rdecl_->m_fields[AccessSpecifier::AS_public])
            {
                writeFieldDecl(stream,&field,indent+2);
                stream<<",";
            }
            for(FieldDeclNode& field : rdecl_->m_fields[AS_protected])
                writeFieldDecl(stream,&field,indent+2);
            for(FieldDeclNode& field : rdecl_->m_fields[AS_private])
                writeFieldDecl(stream,&field,indent+2);

            stream<<tabs(indent+1)<<"],\n";
            
            // Print class methods
            stream<<tabs(indent+1)<<"\"methods\":[\n";
            //...
            stream<<tabs(indent+1)<<"]\n";

            stream<<tabs(indent)<<"}\n";
        }
        
        void writeFieldDecl(std::ostringstream& stream, FieldDeclNode* fdecl_ , int indent)
        {
            stream<<tabs(indent)<<"{\n";
            stream<<tabs(indent+1)<<"\"name\":\""<<fdecl_->strName<<"\",\n";
            stream<<tabs(indent+1)<<"\"qtype\":\""<<fdecl_->strType<<"\"\n";
            stream<<tabs(indent)<<"}\n";
        }
        
        void writeMethodDecl(const std::ostringstream& stream, MethodDeclNode* mdecl_, int indent)
        {
            
        }
    };
}

#endif
