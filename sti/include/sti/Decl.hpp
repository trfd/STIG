///
///  Decl.hpp
///  is part of STI Library.
///
///  Static Type Info - A C++ Static Reflexion Library based on Clang.
///  Get more infos on STI at <https://github.com/trfd/STI>
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

#ifndef STIG_STIGLua_hpp
#define STIG_STIGLua_hpp

namespace sti
{
    class Type;
    
    class Field
    {
    public:
        
        inline std::string& name(){ return m_name; }
        inline void setName(const std::string& name){ m_name = name; }
        
        inline Type* type(){ return m_type; }
        inline void setType(Type* type){ m_type = type; }
        
    private:
        
        std::string m_name;
        Type* m_type;
    };
    
	class Method
    {
    public:
        
        inline std::string& name(){ return m_name; }
        inline void setName(const std::string& name){ m_name = name; }
        
    private:
        
        std::string m_name;
    };
    
    class Type
    {
    public:
        
        inline std::string& name(){ return m_name; }
        inline void setName(const std::string& name){ m_name = name; }
        
        inline Field** fields(){ return m_fields.data(); }
        inline void addField(Field* f){ m_fields.push_back(f); }
        unsigned fieldCount() { return m_fields.size(); }
        
        inline Method** methods(){ return m_methods.data(); }
        inline void addMethod(Method* m){ m_methods.push_back(m); }
        unsigned methodCount() { return m_methods.size(); }
        
    private:
        
        std::string m_name;
        std::vector<Field*> m_fields;
        std::vector<Method*> m_methods;
    };
    
}

#endif
