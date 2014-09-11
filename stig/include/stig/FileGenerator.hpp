///
///  FileGenerator.hpp
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

#ifndef STIG_FileGenerator_hpp
#define STIG_FileGenerator_hpp

#include <iterator>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
//#include <boost/spirit.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/lexical_cast.hpp>

#include <sti/Serialization.hpp>

namespace bfs = boost::filesystem;

using namespace sti;

namespace stig
{
    struct ErrorReport
    {
        std::string context;
        std::string file = "";
        int line = 0;
        
        void report(const std::string& logMessage)
        {
            std::cout<<"ERROR: (File: "<<file<<" : "<<line<<") ["
                     <<context<<"] "<<logMessage<<"\n";
        }
    };
    
    class FileGenerator
    {
    public:
        
        virtual void processTemplate(const std::string& templateFile_ ,
                                     const std::string& outputFile_   )
        {
            m_currentReport.context = "FileGenerator::processTemplate";
            m_currentReport.file = templateFile_;
            m_currentReport.line = 0;
            
            bfs::path templatePath(templateFile_);
            bfs::path outputPath(outputFile_);
            
            if(!bfs::exists(templatePath))
            {
                m_currentReport.report("File not found");
                return;
            }
            
            bfs::ofstream outStream(outputPath);
            bfs::ifstream inStream(templatePath);
            
            std::string line;
            
            if(inStream.is_open() && outStream.is_open())
            {
                while( std::getline(inStream,line) )
                {
                    m_currentReport.line++;
                    outStream<<processLine(line)<<"\n";
                }
                
                inStream.close();
                outStream.close();
            }
            else
            {
                m_currentReport.report("Cannot open template file");
            }
        }
        
    protected:
        
        virtual std::string processLine(const std::string& line)
        {
            m_currentReport.context = "FileGenerator::processLine";
            
            std::ostringstream out_line;
            
            std::string::const_iterator it;
            
            for(it = line.begin() ; it != line.end() ;)
            {
                // If open token
                if(*it == '@')
                {
                    std::string::const_iterator end_it = std::next(it);
                    
                    while(end_it != line.end() && *end_it != '@')
                        std::advance(end_it,1);
                    
                    // Doesn't match any end token at this line
                    if(end_it == line.end())
                    {
                        m_currentReport.report("Syntax error, token \'@\' at "+
                                               std::to_string(line.begin()-it)+
                                               " doesn't match any end token");
                        out_line<<*it;
                        ++it;
                        
                        continue;
                    }
                    
                    out_line<<processReplacement(it,end_it);
                    it = std::next(end_it);
                }
                else
                {
                    out_line<<*it;
                    ++it;
                }
            }
            
            return out_line.str();
        }
        
        virtual std::string processScript(const std::string::const_iterator& begin,
                                          const std::string::const_iterator& end)
        {
            m_currentReport.context = "FileGenerator::processReplacement";
            
            std::string script(begin+1, end);
            
            if(script == "")
                return script;
            
            return processScript(script);
        }
        
        virtual std::string processScript(const std::string& script)
        {
            typedef std::vector<std::string> KeyList;
            
            // Support succession of keys: "record:template:parameters:0"
            // each bit separated by ':' is a key
            KeyList keys;
            boost::split( keys, var, boost::is_any_of(":"), boost::token_compress_on );
            
            if(keys.size() == 0)
                return var;
            
            // Hold the current node of the serialisation tree
            SerialValue* currValue = m_rootDict;

            // Go through the list of key to find replacement value
            // This will look for keys into m_rootDict
            for(int idx = 0 ; idx < keys.size() ; idx++)
            {
                if(currValue->type() == SerialValue::SV_dict)
                {
                    // Current node of serialization tree is a dictionary
                    // Then next current key must be a key of the dict's key-value map
                    
                    DictSerialValue* dict = static_cast<DictSerialValue*>(currValue);
                    
                    DictSerialValue::KeyValueMap_it valIt = dict->_dictValue.find(keys[idx]);
                    
                    // Current key doesn't belongs to the current dict's key-value map
                    if(valIt == dict->_dictValue.end())
                    {
                        m_currentReport.report("Key \""+keys[idx]+"\" not in key-value map in variable \""+var+"\"");
                        return var;
                    }
                    
                    // New node in serialization tree
                    currValue = valIt->second;
                }
                else if(currValue->type() == SerialValue::SV_array)
                {
                    // Current node of serialization tree is an array
                    // Then next current key must be an index of the array
                    
                    ArraySerialValue* array = static_cast<ArraySerialValue*>(currValue);
                    
                    try
                    {
                        // Integer value of key
                        int arrayIdx = boost::lexical_cast<int>(keys[idx]);

                        if(arrayIdx >= array->_arrayValue.size())
                        {
                            m_currentReport.report("Index out of bound: "+std::to_string(arrayIdx)+
                                                   " in variable \""+var+"\"");
                            return var;
                        }
                        
                        // New node in serialization tree
                        currValue = array->_arrayValue[arrayIdx];
                    }
                    catch(const boost::bad_lexical_cast &)
                    {
                        m_currentReport.report("Invalid array index conversion of \""+
                                               keys[idx]+"\" in variable \""+var+"\"");
                        return var;
                    }
                }
                else
                {
                    // Key doesn't lead to a branch node (dict or array) in serialization tree
                    std::string expType = "SV_dict or SV_array";
                    
                    m_currentReport.report("Invalid type of key \""+keys[idx]+"\" in variable \""+var+
                                           "\". Type expected: "+expType+" Value's type: "+
                                           serialType(currValue->type()));

                    return var;
                }
            }
            
            // Only final key can be a leaf node.
            
            if(currValue->type() == SerialValue::SV_string)
            {
                return static_cast<StringSerialValue*>(currValue)->_stringValue;
            }
            else
            {
                // Key doesn't lead to a branch node (dict or array) in serialization tree
                std::string expType = "SV_string";
                
                m_currentReport.report("Invalid value type for last key in variable \""+var+
                                       "\". Type expected: "+expType+" Value's type: "+
                                       serialType(currValue->type()));
                
                return var;
            }
        }
        
        void setRootDictValue(DictSerialValue* dict_)
        {
            m_rootDict = dict_;
        }
        
        DictSerialValue* rootDictValue()
        {
            return m_rootDict;
        }
        
    private:
        
        ErrorReport m_currentReport;
    };
}

#endif
