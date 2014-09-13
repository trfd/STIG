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

#include "stig/ces/CES.hpp"
#include "stig/ces/LuaVM.hpp"

int main(int argc,char* argv[])
{
    if (argc<2)
    {
        printf("%s: <filename.lua>\n",argv[0]);
        return 0;
    }

    
    stig::ces::Parser parser;
    
    parser.parse("source1@[lua1]@source2@[lua2@out1@lua3]@source3@out2@source4");
    
    parser.dump();

    return 0;
}