///
///  TypeDatabase.hpp
///  is part of STI Library.
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

#ifndef STI_TypeDatabase_hpp
#define STI_TypeDatabase_hpp

#include <map>
#include <string>

#include "cytok/utils/Singleton.hpp"

#include "Decl.hpp"

namespace sti
{
    class TypeDatabase : public ck::utils::Singleton<TypeDatabase>
    {
    public:
        
        typedef std::shared_ptr<Type>           Type_ptr;
        typedef std::map<std::string, Type_ptr> TypeMap;
        typedef TypeMap::iterator               TypeMap_it;
        
        inline static TypeDatabase* instance(){return ck::utils::Singleton<TypeDatabase>::instance();}
        
        inline void addType(const Type_ptr& t){ m_typeMap[t->fullname()] = t; }
        
        inline Type_ptr typeWith(const std::string& fullname)
        {
            TypeMap_it it = m_typeMap.find(fullname);
            
            if(it != m_typeMap.end())
                return it->second;
            return Type_ptr(nullptr);
        }
        
    private:
        
        TypeMap m_typeMap;
        
    };
}

#endif
