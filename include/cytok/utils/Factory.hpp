///  
///  Factory.hpp
///  part of Cytok Library.
///
///  Cytok Library the core and tools library for the CytokEngine.
///  CytokEngine is a research game-engine project.
///  
///  Created by Baptiste Dupy on 10/07/2014.
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

#ifndef CYTOK_Factory_hpp
#define CYTOK_Factory_hpp

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
	class Factory 
	{
    public:
        
        typedef FactoryMap<_AbstractProduct, _ProductID, _ProductCreator, _ErrorPolicy> DelegatedMap;
        
        Factory(DelegatedMap* pMap)
        : _map(pMap)
        {}
        
        void setMap(DelegatedMap* pMap)
        {
            _map = pMap;
        }
        
        inline virtual void registerCreator(_ProductID id , _ProductCreator creator)
        {
            if(_map)
                _map->registerCreator(id,creator);
        }
        
        inline void unregisterCreator(_ProductID id)
        {
            if(_map)
                _map->unregisterCreator(id);
        }

        template<typename... _Args>
        _AbstractProduct* createProduct(_ProductID id,_Args&&... args_)
        {
            if(!_map)
                _ErrorPolicy<_ProductID>::OnNullMap();
            
            _ProductCreator* creator = _map->productCreator(id);
            
            if(!creator)
                return NULL; // Assume that map already call ErrorPolicy
            
            return (*creator)(std::forward<_Args>(args_)...);
        }
        
    protected:
        
        DelegatedMap* _map;
	};
}

#endif
