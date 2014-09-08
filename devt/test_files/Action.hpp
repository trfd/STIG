

#include <iostream>
#include "ActionHolder.hpp"

class Object
{
    int c;
};

namespace ck
{
    
    class Object
    {
        
    };
    
	class Action : public Object
	{
    public:
        
        enum State
        {
            INVALID = 0,
            SUCCESS = 1,
            FAILURE = 2
        };
        
        virtual State perform() = 0;
        
        virtual void testFunc();
        
        void test();
        
	};
}
