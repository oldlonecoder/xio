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


#include <Spp/Compiler/Lexer.h>
#include "Spp/Stack.h"
// ---- On Hold : Grammar::Rules
#include "Spp/Compiler/Lang/Grammar.h"

#include <stack>

//--------------------------------
namespace Spp
{

class SPP_EXPORT Compiler
{

    friend class Amu;
    friend class Interpreter;



public:
    struct SPP_EXPORT ConfigData
    {
        std::string_view SourceText;
        SppToken::Array*  Tokens;
        Stack*           RootBloc;
    };


    Compiler() =default;

    Compiler::ConfigData& Config() { return Data; }

    Book::Result ExecuteLexer();

private:
    struct ContextData
    {
        SppToken::Iterator  BeginStream,
                            StartSeq,
                            Cur,
                            EndSeq,
                            EndStream;
        Stack*      Bloc;
        xio*        Instruction;
        xio::Array  InstructionsSeq;
        SppToken::Array TokensSeq;

        Type::T     CurType { Type::Null };

        SppToken& Token() { return *Cur; }
        Lang::Grammar::Rule const* Rule;
        ContextData() = default;
        ContextData(const ContextData& CD);
        ContextData(ContextData&& CD) noexcept;

        ContextData& operator = (const ContextData& CD);
        ContextData& operator = (ContextData&& CD);


        ~ContextData() = default;
        ContextData(SppToken::Iterator BeginStream, SppToken::Iterator StartSeq, SppToken::Iterator EndStream, Stack *Bloc);

        bool operator ++();
        bool operator ++(int);
        void Accept();
        void Reject();
        [[nodiscard]] bool Eof() const { return Cur >= EndStream; };

        using CTXStack = std::stack<ContextData>;

    };

    Compiler::ConfigData  Data;
    Compiler::ContextData Ctx;

    using Maker = xio*(Compiler::*)();
    using RuleParserPairs = std::map<std::string_view, Compiler::Maker>;
    using MnemonicCompilerPairs = std::map<Mnemonic, Compiler::Maker>;



    // Compilers :

    Book::Result operator()();
    Book::Result SkipComments();
    xio* NewExprNode(SppToken* Token);

    [[maybe_unused]] xio* CCAmu();

    xio *ParseRightValueKeyword();

    [[maybe_unused]] xio *ParseExpression();

    Book::Result EnterRule();
    Book::Result EnterElementSequence(Lang::Grammar::ElementSeq::const_iterator SeqIt);
    Book::Result ParseElement(Lang::Grammar::Element::Iterator& EI);

    std::pair<SppToken::Iterator, SppToken::Iterator> GetLineText(SppToken::Iterator Token);
    Compiler::ContextData::CTXStack CStack;


};

} // Spp


