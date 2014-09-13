///
///  AST.hpp
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

#ifndef CES_AST_hpp
#define CES_AST_hpp

namespace ces
{
    /// <summary>
    ///  Base class for AST's nodes.
    /// </summary>
    struct Node
    {
        /// <summary>
        /// The kind of AST node. Matches the derived type of ces::Node
        /// </summary>
        /// In CES there's only to kind of node: Blocks and Chunks
        /// "Blocks" represents basically a set of "Chunks".
        /// While "Chunks" are indivisible chunk of code.
        enum Kind
        {
            Block,
            Chunk,
            
            KindNone
        };
        
        /// <summary>
        /// Type of code represented by this node.
        /// </summary>
        /// CES allows to embed scripting language
        /// into other (embedding) languages.
        /// CESType represent either the embedding (Source)or
        /// the embedded (Embedded) language.
        /// Out chunk of scripting code which value are redirected
        /// to CES output.
        enum CESType
        {
            Source,
            Embedded,
            Out,
            
            TypeNone
        };
        
        /// <summary>
        /// CESType of AST's node
        /// </summary>
        CESType type = TypeNone;
        
        /// <summary>
        /// Kind of node
        /// </summary>
        Kind kind = KindNone;
        
        /// <summary>
        /// Parent node in the AST
        /// </summary>
        Node* parent = nullptr;
        
        Node()
        : type(TypeNone),
        kind(KindNone)
        {}
        
        Node(CESType t_, Kind k_)
        : type(t_),
        kind(k_)
        {}
        
        virtual ~Node(){}
        
        /// <summary>
        /// Dump content of node.
        /// </summary>
        virtual void dump(){}
    };

    /// <summary>
    /// Atomic chunk of code.
    /// </summary>
    /// Chunks are indivisible part of code. They
    /// can be either Source code (Embedding code),
    /// Script code (Embedded code) or Out code
    /// (Script code evaluadted and redirected to
    /// CES output).
    /// Chunks actually contain code. This code
    /// is either evaluated (Embedded), redirected
    /// to the CES output (Source), or evaluated
    /// and value redirected to CES output (Out).
    struct Chunk : public Node
    {
        /// <summary>
        /// Code content of chunk.
        /// </summary>
        std::string content;
        
        Chunk() :
        Node(Node::TypeNone,Node::Chunk)
        {}
        
        Chunk(Node::CESType t_) :
        Node(t_,Node::Chunk)
        {}
        
        /// <summary>
        /// Dump content of chunk.
        /// </summary>
        virtual void dump() override
        {
            std::cout<<"\tChunk["<<type<<"]: "<<content<<"\n";
        }
    };
    
    /// <summary>
    /// Set of chunks.
    /// </summary>
    /// Blocks are essentially list of chunks.
    /// They provide a context to chunks.
    struct Block : public Node
    {
        /// <summary>
        /// List of chunks in the blocks
        /// </summary>
        std::vector<Node*> nodes;
        
        Block() :
        Node(Node::TypeNone,Node::Block)
        {}
        
        Block(Node::CESType t_) :
        Node(t_,Node::Block)
        {}
        
        virtual ~Block()
        {
            for (auto it = nodes.begin();
                 it != nodes.end(); ++it)
            {
                delete *it;
            }
            nodes.clear();
        }
        
        /// <summary>
        /// Dump the chunks.
        /// </summary>
        virtual void dump() override
        {
            std::cout<<"Block["<<type<<"]{\n";
            for(Node* c : nodes)
                c->dump();
            std::cout<<"}\n";
        }
        
    };
    
    /// <summary>
    /// Abstract Syntax Tree for CES
    /// </summary>
    class AST
    {
    public:
        
        /// <summary>
        /// Sets the root of the AST.
        /// </summary>
        /// Previous root is destroyed.
        inline void setRoot(Node* root_)
        {
            if(m_root)
                delete m_root;
            
            m_root = root_;
        }
        
        /// <summary>
        /// Returns root of AST
        /// </summary>
        inline Node* root()
        {
            return m_root;
        }
        
    private:
        
        /// <summary>
        /// Root of AST.
        /// </summary>
        Node* m_root = nullptr;
    };
}


#endif
