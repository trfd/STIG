///
///  CES.hpp
///  is part of CES Library.
///
///  C++ Embedded Script - CES is used to write template file for code generation.
///  More information about CES at <https://github.com/trfd/CES>
///
///  Created by Baptiste Dupy on 12/09/2014.
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

#ifndef CES_hpp
#define CES_hpp

#include <iostream>
#include <string>

#include "Parser.hpp"

/// CES : C++ Embedded Script is a script language
/// derivated from lua and created with the special
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
    extern void out(unsigned sessionID, const char* str_);
}

#endif
