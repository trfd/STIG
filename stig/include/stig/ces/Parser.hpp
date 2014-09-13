//
//  Parser.hpp
//  STIG
//
//  Created by Baptiste Dupy on 12/09/14.
//
//

#ifndef STIG_Parser_hpp
#define STIG_Parser_hpp

#include <vector>
#include <string>

#include <boost/algorithm/string.hpp>

#include "AST.hpp"
#include "Diagnostic.hpp"

namespace stig
{
    namespace ces
    {
        /// <summary>
        /// CES parser.
        /// </summary>
        //TODO: Should track lines/files for better diagnostics
        class Parser
        {
        public:
            
            typedef std::string::iterator       String_it;
            typedef std::string::const_iterator String_cit;

            
            /// <summary>
            /// Defines the syntax elements of CES.
            /// </summary>
            struct Grammar
            {
                enum SyntaxElement
                {
                    EmbeddedStart       = 0,
                    EmbeddedEnd         = 1,
                    Out                 = 2,
                    Escape              = 3,
                    
                    SyntaxElementCount,
                    None = SyntaxElementCount
                };
                
                std::string elements[SyntaxElementCount] =
                {
                    "@[",
                    "]@",
                    "@" ,
                    "@@"
                };
            };
            
            /// <summary>
            /// Hold the state of Parser.
            /// </summary>
            struct ParsingState
            {
                /// <summary>
                /// Owner.
                /// </summary>
                Parser* parser;
                
                /// <summary>
                /// Root AST node.
                /// </summary>
                /// This must be a Block of source code.
                Block rootBlock;
                
                /// <summary>
                /// Node being parsed.
                /// </summary>
                /// The parser has encountered a openning
                /// syntax element and opend the current node.
                Node* currentNode;
                
                /// <summary>
                /// Position of the node's openning syntax element.
                /// </summary>
                String_cit nodeStartPosition;
                
                /// <summary>
                /// Current parsing position.
                /// </summary>
                String_cit currentPosition;
                
                /// <summary>
                /// Current valid syntax element.
                /// </summary>
                /// The value is relevant only when the parser
                /// needs to push/pop nodes to the AST.
                /// Otherwise the value is Grammar::None
                Grammar::SyntaxElement currentSyntaxElement = Grammar::None;
                
                ParsingState(Parser* p)
                :parser(p)
                {}
                
                /// <summary>
                /// Push a block to the AST.
                /// </summary>
                /// If current node is chunk, the chunk is poped
                /// before pushing the new block.
                /// The block pushed becomes the currentNode after
                /// that.
                /// Pushing block should be followed by pushing
                /// chunks of the same type, in order to
                /// gather the code in the chunk.
                void pushBlock(Node::CESType t_)
                {
                    if(currentNode->kind == Node::Chunk)
                        popChunk();
                    
                    Block* currBlock = static_cast<Block*>(currentNode);
                    
                    Block* newBlock = new Block(t_);
                    
                    currBlock->nodes.push_back(newBlock);
                    newBlock->parent = currBlock;
                    currentNode = newBlock;
                    
                    //advancePosition(currentSyntaxElement);
                    
                    nodeStartPosition = currentPosition;
                }
                
                /// <summary>
                /// Push a chunk to the AST.
                /// </summary>
                /// If current node is chunk, the chunk is poped
                /// before pushing the new chunk.
                /// The chunk pushed becomes the currentNode after
                /// that.
                void pushChunk(Node::CESType t_)
                {
                    if(currentNode->kind == Node::Chunk)
                        popChunk();
                    
                    Block* currBlock = static_cast<Block*>(currentNode);
                    
                    Chunk* newChunk = new Chunk(t_);
                    currBlock->nodes.push_back(newChunk);
                    newChunk->parent = currBlock;
                    currentNode = newChunk;
                    
                    advancePosition(currentSyntaxElement);
                    
                    nodeStartPosition = currentPosition;
                }
                
                /// <summary>
                /// Pop a block from the AST.
                /// </summary>
                /// Poping a block will close openned chunks
                /// and move currentNode to its parent.
                void popBlock()
                {
                    if(currentNode->kind == Node::Chunk)
                        popChunk();
                    
                    currentNode = currentNode->parent;

                }
                
                /// <summary>
                /// Pop a block from the AST.
                /// </summary>
                /// Poping a chunk will close and copy
                /// chunks content, then it
                /// moves currentNode to its parent.
                void popChunk()
                {
                    if(currentNode->kind == Node::Chunk)
                    {
                        Chunk* currChunk = static_cast<Chunk*>(currentNode);
                        
                        currChunk->content = std::string(nodeStartPosition,currentPosition);
                    }
                    
                    currentNode = currentNode->parent;
                }
                
                /// <summary>
                /// Advance current position by the size of syntax element.
                /// </summary>
                void advancePosition(const Grammar::SyntaxElement& sx)
                {
                    if(sx == Grammar::None)
                        return;
                    
                    unsigned size = parser->m_grammar.elements[(unsigned)sx].length();
                    
                    std::advance(currentPosition,size);
                }
                
                /// <summary>
                /// Dump AST.
                /// </summary>
                void dump()
                {
                    rootBlock.dump();
                }
            };
            
            Parser()
            :m_state(this)
            {}
            
            /// <summary>
            /// Dump parser AST.
            /// </summary>
            inline void dump()
            {
                m_state.dump();
            }
            
            /// <summary>
            /// Parse a chunk of CES code
            /// </summary>
            void parse(const std::string& cesChunk_)
            {
                parse(cesChunk_.begin() , cesChunk_.end());
            }
            
            /// <summary>
            /// Parse a chunk of CES code
            /// </summary>
            void parse(const String_cit& from_, const String_cit& to_)
            {
                m_state.rootBlock.type = Node::Source;
                m_state.currentNode = &m_state.rootBlock;
                m_state.pushChunk(Node::Source);
                
                m_state.nodeStartPosition = from_;
                m_state.currentPosition = from_;
                
                Grammar::SyntaxElement elmt;
                
                while(m_state.currentPosition != to_)
                {
                    if((elmt = matchSyntaxElement(m_state.currentPosition)) != Grammar::None)
                        applySyntaxElement(elmt);
                    else
                        ++m_state.currentPosition;
                }
                
                m_state.popBlock();
            }
            
            /// <summary>
            /// Applies the effect of a syntax element to the parser
            /// </summary>
            void applySyntaxElement(const Grammar::SyntaxElement& elmt_)
            {
                m_state.currentSyntaxElement = elmt_;
                
                switch(elmt_)
                {
                    case Grammar::EmbeddedStart:
                        applyEmbeddedStart();
                        break;
                    case Grammar::EmbeddedEnd:
                        applyEmbeddedStop();
                        break;
                    case Grammar::Out:
                        applyOut();
                        break;
                    case Grammar::Escape:
                        applyEmbeddedStart();
                        break;
                    default:;
                }
                
                m_state.currentSyntaxElement = Grammar::None;
            }
            
            /// <summary>
            /// Applies the effect of EmbeddedStart syntax element to the parser
            /// </summary>
            void applyEmbeddedStart()
            {
                m_state.pushBlock(Node::Embedded);
                m_state.pushChunk(Node::Embedded);
            }
            
            /// <summary>
            /// Applies the effect of EmbeddedStop syntax element to the parser
            /// </summary>
            void applyEmbeddedStop()
            {
                m_state.popBlock();
                m_state.pushChunk(m_state.currentNode->type);
            }
            
            /// <summary>
            /// Applies the effect of Out syntax element to the parser
            /// </summary>
            void applyOut()
            {
                if(m_state.currentNode->type == Node::Out)
                {
                    m_state.popChunk();

                    m_state.pushChunk(m_state.currentNode->type);
                }
                else
                {
                    m_state.pushChunk(Node::Out);
                }
            }
            
            /// <summary>
            /// Applies the effect of Escape syntax element to the parser
            /// </summary>
            void applyEscape()
            {
            }
            
            /// <summary>
            /// Checks if string starting at it_ matches a syntax element in the
            /// parser's grammar.
            /// </summary>
            Grammar::SyntaxElement matchSyntaxElement(const String_cit& it_)
            {
                for(int elmt=0 ; elmt< Grammar::SyntaxElementCount ; elmt++)
                {
                    if(match(it_,m_grammar.elements[elmt]))
                    {
                        if(!checkSyntaxElementValidity((Grammar::SyntaxElement)elmt))
                            continue;
                        return (Grammar::SyntaxElement)elmt;
                    }
                }
                
                return Grammar::None;
            }
            
            /// <summary>
            /// Checks if string starting at it_ matches string comp_
            /// </summary>
            bool match(const String_cit& it_,std::string& comp_)
            {
                String_cit cp_it = it_;
                for(String_cit comp_it = comp_.begin() ;
                    comp_it != comp_.end() ; ++comp_it , ++cp_it)
                {
                    if(*cp_it == '\0' || *cp_it != *comp_it)
                        return false;
                }
                
                return true;
            }
            
            /// <summary>
            /// Check if a syntax element is valid in the current context.
            /// </summary>
            ///
            //TODO: This is actually part of CES syntax rules and should belongs
            // to the grammar.
            bool checkSyntaxElementValidity(const Grammar::SyntaxElement& elmt_)
            {
                switch(elmt_)
                {
                    case Grammar::EmbeddedStart:
                        return(m_state.currentNode->type == Node::Source &&
                               (m_state.currentNode->kind == Node::Block ||
                                m_state.currentNode->kind == Node::Chunk ));
                    case Grammar::EmbeddedEnd:
                        return(m_state.currentNode->type == Node::Embedded &&
                               (m_state.currentNode->kind == Node::Block ||
                                m_state.currentNode->kind == Node::Chunk ));
                    case Grammar::Out:
                        return true;
                    case Grammar::Escape:
                        return false;
                }
                
                return false;
            }
          
        private:
            
            /// <summary>
            /// Parser's grammar.
            /// </summary>
            //TODO: Should have accessors
            Grammar m_grammar;
            
            /// <summary>
            /// Parser's state.
            /// </summary>
            //TODO: Should have accessors
            ParsingState m_state;
            
            /// <summary>
            /// Parser's diagnostics.
            /// </summary>
            /// Diagnostic dumps errors and warnings.
            //TODO: Should have accessors
            Diagnostic m_diagnostic;
        };
    }
}

#endif
