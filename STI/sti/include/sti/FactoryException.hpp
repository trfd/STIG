///  
///  FactoryException.hpp
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

#ifndef CYTOK_FactoryException_hpp
#define CYTOK_FactoryException_hpp

//#include "boost/lexical_cast.hpp"

namespace ck
{
    class FactoryException : std::exception
    {
    };
    
    class FactoryNullMapException : FactoryException
    {
    };
    
    template<typename _ProductID>
    class FactoryProductIDException : FactoryException
    {
        _ProductID m_id;
        
    public:
        
        FactoryProductIDException(const _ProductID& id)
        {
            m_id = id;
        }
        
        virtual const char* what() const throw()
        {
            return "Unknow ID passed to factory";
        }
        
        _ProductID id()
        {
            return m_id;
        }
		
    };
    
    template<typename _ProductID>
    class FactoryError
    {
    public:
        
        static void OnUnknownType(const _ProductID& id)
        {
            throw FactoryProductIDException<_ProductID>(id);
        }
        
        static void OnNullProduct(const _ProductID& id)
        {
            throw FactoryProductIDException<_ProductID>(id);
        }
        
        static void OnNullMap()
        {
            throw FactoryNullMapException();
        }
	};
}

#endif
