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

#include <iostream>

#include "ces/CES.hpp"
#include "ces/LuaVM.hpp"
#include "ces/Interpreter.hpp"

int main(int argc,char* argv[])
{
    
    ces::LuaVM vm;
    
    vm.open();
    
    ces::Parser parser;
    
    parser.parse("Hello@[for i=1,5 do]@ @i@,@[end]@ World");
    
    ces::Interpreter inter;
    
    inter.interpret(&vm, parser.ast(), std::cout);
    
    std::cout<<"\n";

    return 0;
}