///
///  LuaVM.hpp
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

#ifndef CES_LuaVM_hpp
#define CES_LuaVM_hpp

#include <iostream>

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
    
extern int luaopen_CES(lua_State* L); // declare the wrapped module
}

namespace ces
{
    /// <summary>
    /// Wrap Lua virtual machine call for simplified interface.
    /// </summary>
    class LuaVM
    {
    public:
        
        /// <summary>
        /// Opens Lua VM and CES
        /// </summary>
        inline void open()
        {
            m_luaState = luaL_newstate();
            luaL_openlibs(m_luaState);
            luaopen_CES(m_luaState);
        }
        
        /// <summary>
        /// Closes VM.
        /// </summary>
        inline void close()
        {
            lua_close(m_luaState);
        }
        
        /// <summary>
        /// Run a lua script.
        /// </summary>
        inline void runFile(const char* file_)
        {
            if(luaL_loadfile(m_luaState,file_)==0) // load and run the file
                lua_pcall(m_luaState,0,0,0);
            else
               std::cout<<"ERROR: Unable to load "<<file_<<"\n";
        }
        
        /// <summary>
        /// Run a chunk lua script.
        /// </summary>
        inline void runChunk(const std::string& snippet)
        {
            if(luaL_loadstring(m_luaState,snippet.c_str())==0) // load and run the file
                lua_pcall(m_luaState,0,0,0);
            else
                std::cout<<"ERROR: Unable to load "<<snippet<<"\n";
        }
        
    private:
        
        /// <summary>
        /// Current state of the Lua VM
        /// </summary>
        lua_State *m_luaState;
    };
}


#endif
