//
//  CES.hpp
//  STIG
//
//  Created by Baptiste Dupy on 12/09/14.
//
//

#ifndef STIG_CES_hpp
#define STIG_CES_hpp

#include <iostream>
#include <string>

#include "Parser.hpp"

namespace stig
{
    /// CES : C++ Embedded Script is a script language
    /// derivated from lua andcreated with the special
    /// goal to be embedded in other languages (specialy C++).
    /// This scripting language is used to create template
    /// file for source code generation.
    /// Thus allowing to build flexible and extensible generators.
    /// This language is based on Lua and adds only a few
    /// rule to the Lua grammar:
    ///
    /// cpp_code         : Plain C++ code outputed to CES
    /// @[ lua_script ]@ : Embedded lua script
    /// @[ ... @var ..]@ : Outputs value of 'var' to CES
    
    namespace ces
    {
        extern void out(const char* str_);
    }
}

#endif
