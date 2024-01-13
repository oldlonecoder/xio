/***************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                         *
 *   serge.lussier@oldlonecoder.club                                       *
 *                                                                         *
 *                                                                         *
 *   Unless otherwise specified, all code in this project is written       *
 *   by the author (Serge Lussier)                                         *
 *   and no one else then not even {copilot, chatgpt, or any other AI}     *
 *   --------------------------------------------------------------------- *
 *   Copyrights from authors other than Serge Lussier also apply here      *
 ***************************************************************************/


#pragma once
#include <AppBook/Book/BookEnums.h>
#include <AppBook/Book/Expect.h>
#include <Spp/SppToken.h>
#include <map>
#include <stack>
#include "Spp/Stack.h"
#include "Spp/Compiler/Lang/Grammar.h"

namespace Spp::Lang
{


class SPP_EXPORT Ast
{
    Stack* Bloc{ nullptr };


public:

    struct SPP_EXPORT Node
    {
        Ast* Tree{ nullptr };

        Ast::Node* R0{ nullptr }; ///< Operator         | TAC ( Three Address Code ): A0
        Ast::Node* R1{ nullptr }; ///< Left Hand Side   | TAC ( Three Address Code ): A1
        Ast::Node* R2{ nullptr }; ///< Right Hand Side  | TAC ( Three Address Code ): A2
        Ast::Node* R3{ nullptr }; ///< Return Point     | TAC ( Three Address Code ): JMP to Ast::Node / xio* address
        
        using List = std::vector<Ast::Node*>;
        using Iterator = Ast::Node::List::iterator;

        

    };

    Node::List Nodes{};

    Ast::Node* Vertex{ nullptr };

    virtual Book::Expect <Ast::Node*> Input(SppToken* aToken);


};


class SPP_EXPORT MathAST : public Ast
{

public:
    
    static Book::Expect <Ast::Node*> TreeBegin(SppToken* aToken);

    Book::Expect <Ast::Node*> Input(SppToken* aToken) override;
    Book::Expect <Ast::Node*> Root();
    Book::Expect <Ast::Node*> Close();

    using InputValidation = std::vector<std::pair<std::pair<Type::T, Type::T>, std::string>>;
    using InsertAssoc = std::vector<std::pair<std::pair<Type::T, Type::T>, Ast::Node* (MathAST::*)(Ast::Node*)>>;


};

}



