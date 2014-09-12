//
//  LuaVM.hpp
//  STIG
//
//  Created by Baptiste Dupy on 12/09/14.
//
//

#ifndef STIG_LuaVM_hpp
#define STIG_LuaVM_hpp

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
    }
}

#endif
