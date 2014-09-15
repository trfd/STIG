///
///  InterpreterSession.hpp
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

#ifndef CES_InterpreterSession_hpp
#define CES_InterpreterSession_hpp

namespace ces
{
    /// <summary>
    /// Holds a single session of Interpreter.
    /// </summary>
    /// InterpreterSession receives the outputs
    /// from lua vm
    struct InterpreterSession
    {
        unsigned id;
        
        virtual void out(const char* str_) = 0;
        
        virtual ~InterpreterSession(){}
    };
    
    /// <summary>
    /// Holds a single session of Interpreter with a stream
    /// </summary>
    /// InterpreterSession receives the outputs
    /// from lua vm.
    /// StreamedInterpreterSession stream this output
    /// to the stream held
    template<typename _OutputStream>
    struct StreamedInterpreterSession : public InterpreterSession
    {
        /// <summary>
        /// Stream that received result from lua method "CES.out".
        /// </summary>
        _OutputStream& stream;
        
        StreamedInterpreterSession(_OutputStream& stream_)
        : stream(stream_)
        {}
        
        inline virtual void out(const char* str_) override
        {
            if(str_) stream<<str_;
        }
    };
}

#endif
