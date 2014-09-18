//
//  ScriptContext.hpp
//  STIG
//
//  Created by Baptiste Dupy on 11/09/14.
//
//

#ifndef STIG_ScriptContext_hpp
#define STIG_ScriptContext_hpp

namespace sti
{
    namespace ctx
    {
        class Var
        {
        protected:
            
            virtual void setValue(Var* v_) = 0;
        };
        
        class Array : public Var
        {
            virtual Var* value(int idx){ return }
            virtual std::string str() = 0;
            
        protected:
            
            virtual void setValue(Var* v_) = 0;
        };
        
        class Dictionary : public Var
        {
        public:
            
            Var* value(const std::string& key)
            {
                m_varDict->_dictValue[key];
            }
            
        private:
            
            // Dictionary of variables
            DictSerialValue* m_varDict = nullptr;
        };
    }
}

#endif
