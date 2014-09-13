//
//  Diagnostic.hpp
//  STIG
//
//  Created by Baptiste Dupy on 12/09/14.
//
//

#ifndef STIG_Diagnostic_hpp
#define STIG_Diagnostic_hpp

namespace stig
{
    namespace ces
    {
        /// <summary>
        /// Manage error and warning of parsing CES.
        /// </summary>
        //TODO: Add proper diagnostics of errors
        class Diagnostic
        {
        public:
            
            bool report(bool cond_, const std::string& diag)
            {
                if(!cond_)
                    report(diag);
                return cond_;
            }
            
            void report(const std::string& diag)
            {
                std::cout<<diag<<"\n";
            }
            
        private:
            
            int m_line = 0;
        };
    }
}

#endif
