

//#include <iostream>
//#include "ActionHolder.hpp"

template<typename T_>
struct STRTemp
{
    T_ val;
};

class Object
{
    STRTemp<int> c;
};

namespace ck
{
    namespace Ohhh
    {
        class Object
        {
        
        };    
    }
    
    
	class Action : public Object
	{
    public:
        
        enum State
        {
            INVALID = 0,
            SUCCESS = 1,
            FAILURE = 2
        };

        struct AAA
        {

        };
        
        virtual State perform() = 0;
        
        virtual void testFunc(int t_);
        
        void test(float x, float y);
        
	};
}
