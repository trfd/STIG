//
//  LuaVM.hpp
//  STIG
//
//  Created by Baptiste Dupy on 12/09/14.
//
//

#ifndef STIG_LuaVM_hpp
#define STIG_LuaVM_hpp

#include <iostream>

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
    
extern int luaopen_CES(lua_State* L); // declare the wrapped module
}

namespace stig
{
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
}

#endif
