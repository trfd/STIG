//
//  AST.hpp
//  STIG
//
//  Created by Baptiste Dupy on 13/09/14.
//
//

#ifndef STIG_AST_hpp
#define STIG_AST_hpp

namespace stig
{
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
    }
}

#endif
