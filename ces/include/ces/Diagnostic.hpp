///
///  Diagnostic.hpp
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

#ifndef CES_Diagnostic_hpp
#define CES_Diagnostic_hpp

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


#endif
