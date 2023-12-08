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
//--------------------------------
namespace Spp
{

class SPP_EXPORT Compiler
{

    friend class Unit;
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
        ~ContextData() = default;
        ContextData(SppToken::Iterator BeginStream, SppToken::Iterator StartSeq, SppToken::Iterator EndStream, Stack *Bloc);

        bool operator ++();
        bool operator ++(int);
        void Accept();
        void Reject();
        bool Eof() { return Cur >= EndStream; };

    };

    Compiler::ConfigData  Data;
    Compiler::ContextData Ctx;

    using Maker = xio*(Compiler::*)();
    using RuleParserPairs = std::map<std::string_view, Compiler::Maker>;
    using MnemonicCompilerPairs = std::map<Mnemonic, Compiler::Maker>;



    // Compilers :

    Book::Result operator()();
    Book::Result SkipComments();
    xio* NewXio(SppToken*);
    xio* CCUnit();

    xio *ParseRightValueKeyword();

    [[maybe_unused]] xio *ParseExpression();

    Book::Result EnterRule(Lang::Grammar::Rule const* Rule);
    Book::Result EnterElementSequence(Lang::Grammar::ElementSeq::Iterator SeqIt);
    Book::Result ParseElement(Lang::Grammar::Element::Iterator& EI);



};

} // Spp


