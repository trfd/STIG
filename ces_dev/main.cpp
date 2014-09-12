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

#include <stdio.h>

//#define LUA_EXTRALIBS {"example",luaopen_example}
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
    
extern int luaopen_example(lua_State* L); // declare the wrapped module
}

int main(int argc,char* argv[])
{
    lua_State *L = luaL_newstate();
    if (argc<2)
    {
        printf("%s: <filename.lua>\n",argv[0]);
        return 0;
    }
    
    luaL_openlibs(L);
    luaopen_example(L);	// load the wrappered module
    if (luaL_loadfile(L,argv[1])==0) // load and run the file
        lua_pcall(L,0,0,0);
    else
        printf("unable to load %s\n",argv[1]);
    lua_close(L);
    return 0;
}