///  
///  FactoryMap.hpp
///  part of Cytok Library.
///
///  Cytok Library the core and tools library for the CytokEngine.
///  CytokEngine is a research game-engine project.
///  
///  Created by Baptiste Dupy on 11/07/2014.
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

#ifndef CYTOK_FactoryMap_hpp
#define CYTOK_FactoryMap_hpp

#include <map>

#include "FactoryMap.hpp"
#include "FactoryException.hpp"

namespace ck
{
	template
    <
        typename _AbstractProduct,
        typename _ProductID,
        typename _ProductCreator,
        template<typename>class _ErrorPolicy = FactoryError
    >
	class FactoryMap
	{
    public:
        
        typedef std::map<_ProductID,_ProductCreator> CreatorMap;
        typedef std::pair<_ProductID,_ProductCreator> CreatorPair;
        typedef typename std::map<_ProductID,_ProductCreator>::iterator CreatorMap_it;
        
        virtual void registerCreator(_ProductID id , _ProductCreator creator)
        {
            m_associations.insert(CreatorPair(id , creator));
        }
        
        virtual void unregisterCreator(_ProductID id)
        {
            m_associations.erase(id);
        }
        
        virtual _ProductCreator* productCreator(_ProductID id)
        {
            CreatorMap_it it = m_associations.find(id);
            
            if(it == m_associations.end())
            {
                _ErrorPolicy<_ProductID>::OnUnknownType(id);
                return nullptr;
            }
            
            return &(it->second);
        }
        
    private:
        
        CreatorMap m_associations;
        
	};
}

#endif
