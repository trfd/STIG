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

#include "Diagnostic.hpp"

namespace stig
{
    namespace ces
    {
        class Parser
        {
        public:
            
            typedef std::string::iterator       String_it;
            typedef std::string::const_iterator String_cit;
            
            struct Node
            {
                enum Kind
                {
                    Block,
                    Chunk,
                    
                    KindNone
                };
                
                enum Type
                {
                    Source,
                    Embedded,
                    Out,
                    
                    TypeNone
                };
                
                Type type = TypeNone;
                Kind kind = KindNone;
                Node* parent = nullptr;
                
                Node()
                : type(TypeNone),
                kind(KindNone)
                {}
                
                Node(Type t_, Kind k_)
                : type(t_),
                kind(k_)
                {}
                
                virtual void dump(){}
            };
            
            struct Block;
            
            struct Chunk : public Node
            {
                std::string content;
                
                Chunk() :
                Node(Node::TypeNone,Node::Chunk)
                {}
                
                Chunk(Node::Type t_) :
                Node(t_,Node::Chunk)
                {}
                
                virtual void dump() override
                {
                    std::cout<<"\tChunk["<<type<<"]: "<<content<<"\n";
                }
            };
            
            /// CESBlock represent a block of CEScript
            /// This allow distinction between the
            /// embedding and embedded languages
            /// The Embedding language is the one
            /// in which the CES is embedded.
            /// Embedding language blocks are redirected
            /// to the CES output without any
            /// further parsing.
            /// The Embedded language is the
            /// scripting language (eg. Lua).
            /// Embedded block are send to the
            /// scripting langage for parsing.
            /// The "Out" block is an evaluation
            /// block of the scripting language.
            /// The "Out" is used to "print" the
            /// values of the scripting language
            /// in the CES output.
            
            struct Block : public Node
            {
                std::vector<Node*> nodes;
                
                Block() :
                Node(Node::TypeNone,Node::Block)
                {}
                
                Block(Node::Type t_) :
                Node(t_,Node::Block)
                {}
                
                
                virtual void dump() override
                {
                    std::cout<<"Block["<<type<<"]{\n";
                    for(Node* c : nodes)
                        c->dump();
                    std::cout<<"}\n";
                }
            };

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
            
            struct ParsingState
            {
                Parser* parser;
                
                Block rootBlock;
                
                Node* currentNode;
                
                String_cit nodeStartPosition;
                String_cit currentPosition;
                Grammar::SyntaxElement currentSyntaxElement = Grammar::None;
                
                ParsingState(Parser* p)
                :parser(p)
                {}
                
                void pushBlock(Node::Type t_)
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
                
                void pushChunk(Node::Type t_)
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
                
                void popBlock()
                {
                    if(currentNode->kind == Node::Chunk)
                        popChunk();
                    
                    currentNode = currentNode->parent;

                }
                
                void popChunk()
                {
                    if(currentNode->kind == Node::Chunk)
                    {
                        Chunk* currChunk = static_cast<Chunk*>(currentNode);
                        
                        currChunk->content = std::string(nodeStartPosition,currentPosition);
                    }
                    
                    currentNode = currentNode->parent;
                }
                
                void advancePosition(const Grammar::SyntaxElement& sx)
                {
                    if(sx == Grammar::None)
                        return;
                    
                    unsigned size = parser->m_grammar.elements[(unsigned)sx].length();
                    
                    std::advance(currentPosition,size);
                }
                
                void dump()
                {
                    rootBlock.dump();
                }
            };
            
            Parser()
            :m_state(this)
            {}
            
            inline void dump()
            {
                m_state.dump();
            }
            
            void parse(const std::string& cesChunk_)
            {
                parse(cesChunk_.begin() , cesChunk_.end());
            }
            
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
            
            void applyEmbeddedStart()
            {
                m_state.pushBlock(Node::Embedded);
                m_state.pushChunk(Node::Embedded);
            }
            
            void applyEmbeddedStop()
            {
                m_state.popBlock();
                m_state.pushChunk(m_state.currentNode->type);
            }
            
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
            
            void applyEscape()
            {
                
            }
            
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
          
        private:
            
            Grammar m_grammar;
            
            ParsingState m_state;
            
            Diagnostic m_diagnostic;
        };
    }
}

#endif
