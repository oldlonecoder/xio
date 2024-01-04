//
// Created by oldlonecoder on 11/2/23.
//

#include "Spp/Compiler/Compiler.h"

namespace Spp
{





/*!
 * @brief Execute compilation. Main entry.
 *
 * Soon, we will have to TreeBegin Parsing Amu scope bloc and descend into sibling bloc/stacks.
 *
 * @return  Book::Result::Success or Book::Result::Failed.
 * @author &copy; 2023/2024 oldlonecoder (serge.lussier@oldlonecoder.club)
 *
 * @note How to branch on correct rule and context would be to create a dictionary of (Context/Scope -> Rules)
 *  then ( Context/Scope -> {key[Token::Type/Mnemonic/SemanticBits]->rule(s)->parser} )
 */
Book::Result Compiler::operator()()
{
//    if(!Lang::Grammar::IsBuilt())
//    {
//        AppBook::Error() << " Gammar rules not built yet!.";
//        return Book::Result::Failed;
//    }

    Ctx = {Data.Tokens->begin(), Data.Tokens->begin(), Data.Tokens->end(), Data.RootBloc};
    AppBook::Debug() << " Tokens sequence begins :'" << Book::Fn::Endl << Color::Yellow << Ctx.Token().Details(true);
    Ctx.Rule = Lang::Grammar()["expression"]; // Ctx.Rule = Lang::Grammar()["amu"];
    //AppBook::Error() << " Compile Amu: " << Book::Fn::Endl << (*Ctx.Cur).Details(true) << Book::Fn::Endl<< " is not implemented yet... ";
    if(auto* x = ParseExpression(); x)
    {
        return Book::Result::Success;
    }
    return Book::Result::Null_ptr;
}


/**
 *  \brief parse_expr  Explicitely parses rule 'expression' using the current context data.
 *
 * \return book::code::accepted, or book::code::error if failed.
 * \author &copy; August 23, 2023; oldlonecoder, (serge.lussier@oldlonecoder.club)
 * \note   Stop conditions are yet to be determined clearly. The current is
 *         1 - eof
 *         2 - eos ( end of statement mnemonic or simply the ';' mnemonic )
 *         3 - Expression ends on invalid relational token or invalid mnemonic when in condition expr context for ex.
 *         -- Other ways that are not yet explored/implemented: ( [condition] expr ) [expr] {expr} syntax.
 */
[[maybe_unused]] xio* Compiler::ParseExpression()
{
    AppBook::Debug() << " ==> Entering on token:" << Book::Fn::Endl << Ctx.Token().Details (true);
    xio* x{nullptr};

    if(SkipComments() == Book::Result::Eof)
    {
        AppBook::Message() << "eof on commented contents;  no expression; leaving";
        return nullptr;
    }

    if(!(x = xio::TreeBegin(Ctx.Bloc, &Ctx.Token(), [this](SppToken *T) -> xio * { return NewExprNode(T); })))
    {
        AppBook::Status() << Color::Yellow << " : seems not an expression at all at " << Book::Fn::Endl << Ctx.Token().Details(true) ;
        return nullptr;
    }

    Ctx++;

    while((Ctx.Cur < Data.Tokens->end()) && (Ctx.Cur->M != Mnemonic::Semicolon)){
        (void)SkipComments();
        x = x->TreeInput(Ctx.Bloc, &Ctx.Token(), [this](SppToken* T)-> xio*{ return NewExprNode(T); });
        if(!x)
        {
            AppBook::Out() << Color::Yellow << " arithmetic expression inputs stopped  by unexpected token - returning." ;
            break;
        }
        Ctx.InstructionsSeq.push_back(x);
        Ctx++;
    }

    if(xio* root = Ctx.InstructionsSeq.back()->CloseTree(); root)
    {
        AppBook::Info() << " Returning accepted." ;
        Ctx.Instruction = root;
        Ctx.Accept();
        return Ctx.Instruction;
    }
    Ctx.Reject();
    return nullptr;
}

std::pair<SppToken::Iterator, SppToken::Iterator> Compiler::GetLineText(SppToken::Iterator Token)
{

    // Seek beginning of the line:
    auto Start = Token;
    while((Start > Config().Tokens->begin()) && (Start->Location.Linenum == Token->Location.Linenum)) --Start;
    if(Start->Location.Linenum != Token->Location.Linenum) ++Start;

    auto End = Token;
    // Seek end of the line:
    while((End != Config().Tokens->end()) && (End->Location.Linenum == Token->Location.Linenum)) ++End;
    --End;

    return {Start, End};
}


[[maybe_unused]] xio *Compiler::CCAmu()
{

    AppBook::Error() << " Compile Amu: " << Book::Fn::Endl << (*Ctx.Cur).Details(true) << Book::Fn::Endl<< " is not implemented yet... ";
    return nullptr;
}

/**
 * \brief make_xio_node callback function is normally invoked from xio::input.
 *
 * Will create the proper instance of the instruction from the token_data infos.
 * \param token  pointer to the current token.
 * \return pointer to newly created xio;
 * \note As of 2023-08-28, only xio's POD ( Plain Old Data ) variable types are created on identifier token restricted to arithmetic expressions.
 *       Object ID and Function ID are not handled yet. So this implementation will systematically raises a syntax error...
 * \author &copy; August 28, 2023; oldlonecoder, (serge.lussier@oldlonecoder.club)
 */
xio* Compiler::NewExprNode(SppToken* Token)
{
    // "Branch" on token type
    AppBook::Debug() << " Entering xio producer with "<< Book::Fn::Endl << Token->Mark(true) ;

    switch(Token->T)
    {
        case Type::Binary:
        case Type::Prefix:
        case Type::Postfix:
        case Type::Unary:
        case Type::Assign:
        case Type::OpenPair:
        case Type::ClosePair:
            return new xio(Ctx.Bloc, Token, nullptr);
        case Type::Keyword:
            return ParseRightValueKeyword();
        case Type::Id:
            // As of 09-2023, Handling only POD variables.
            return Ctx.Bloc->NewVar(Token);
        case Type::Number:
            return new xio(Ctx.Bloc,Token);
        default:
            AppBook::Message() << "unhandled token:" << Book::Fn::Endl << Token->Mark(true);
            break;
    }
    return nullptr;
}

Book::Result Compiler::SkipComments()
{
    while(!Ctx.Eof() && Ctx.Token().IsComment())
        ++Ctx;
    if(Ctx.Eof())
        return Book::Result::Eof;

    Ctx.StartSeq = Ctx.EndSeq = Ctx.Cur;
    return Book::Result::Accepted;
}

xio *Compiler::ParseRightValueKeyword()
{
    if(!Ctx.Token().IsValue())
        throw AppBook::Exception()[AppBook::Syntax() << "Expected right-value token. Got :" << Book::Fn::Endl << Ctx.Token().Details(true)];

    AppBook::Debug() << Book::Result::Notimplemented;
    return nullptr;
}


Book::Result Compiler::ExecuteLexer()
{
    if(Data.SourceText.empty())
    {
        AppBook::Error() << " Source content is empty.";
        return Book::Result::Empty;
    }

    Lexer Lex;
    Lex.Config() = {Data.SourceText.data(),Data.Tokens};
    if(auto R = Lex.Lex(); R != Book::Result::Success)
    {
        AppBook::Error() << " Lexer failed:";
        for(auto const& Token: *Data.Tokens)
            AppBook::Out() << Token.Mark();
        return R;
    }

    return Book::Result::Success;
}


/*!
 * @brief Recursive; Initiate a parsing rule then iterate its sequences list.
 * @param None - The Rule is already ( must be ) set in the Context.
 * @return Result: Accepted or Rejected or any other error Result values.
 *
 * @note   Ctx.Rule = The Rule to Descent-Parse;
 */
Book::Result Compiler::EnterRule()
{
    for(auto Seq = Ctx.Rule->Begin(); !Ctx.Rule->End(Seq); Seq++)
    {
        if(auto Result = EnterElementSequence(Seq); Result == Book::Result::Accepted) return Result;



    }
    return Book::Result::Ok;
}

Book::Result Compiler::EnterElementSequence(Lang::Grammar::ElementSeq::const_iterator SeqIt)
{
    for(auto TermIt = SeqIt->terms.begin(); TermIt != SeqIt->terms.end(); TermIt++)
    {
        if(ParseElement(*TermIt) != Book::Result::Accepted)
        {
            if(TermIt->a.IsOneof())
            {
                ++TermIt;
                continue;
            }
        }

    }
    return Book::Result::Accepted;
}

Book::Result Compiler::ParseElement(const Lang::Grammar::Element& El)
{
    if(El.IsMnemonic())
    {
        if(Ctx.Cur->M == El.Mem.M)
        {
            //..

        }

    }

    return Book::Result::Rejected;
}




// ---------------- Compiler::Context: ----------------------------------------------
#pragma region Context

bool Compiler::ContextData::operator++()
{
    if(Cur >= EndStream) return false;
    ++Cur;
    return Cur < EndStream;
}

bool Compiler::ContextData::operator++(int)
{
    if(Cur >= EndStream) return false;
    ++Cur;
    return Cur < EndStream;
}



/*!
 * @brief --
 */
void Compiler::ContextData::Accept()
{
    StartSeq = EndSeq = Cur;
    InstructionsSeq.clear();
    if(! Instruction) return;

    Bloc->AppendInstruction(Instruction);
    CurType = Type::Null;
}


/*!
 * @brief --
 */
void Compiler::ContextData::Reject()
{
    Cur = EndSeq = StartSeq; // No forward sequence ????
    for(auto* X: InstructionsSeq)
    {
        X->Detach(); // Remove from Stack's children Array
        delete X;
    }
    InstructionsSeq.clear();
    CurType = Type::Null;
}

Compiler::ContextData::ContextData(SppToken::Iterator BeginStream, SppToken::Iterator StartSeq, SppToken::Iterator EndStream, Stack *Bloc)
    : BeginStream(BeginStream), StartSeq(StartSeq), Cur(StartSeq), EndSeq(StartSeq), EndStream(EndStream), Bloc(Bloc)
{}




/*
 * SppToken::Iterator  BeginStream,
                            StartSeq,
                            Cur,
                            EndSeq,
                            EndStream;
        Stack*      Bloc;
        xio*        Instruction;
        xio::Array  InstructionsSeq;
        SppToken::Array TokensSeq;

        Type::T     CurType { Type::Null };

 */
Compiler::ContextData::ContextData(const Compiler::ContextData &CD)
{
    BeginStream = CD.BeginStream;
    StartSeq    = CD.StartSeq;
    Cur         = CD.Cur;
    EndSeq      = CD.EndSeq;
    EndStream   = CD.EndStream;
    Bloc        = CD.Bloc;
    Instruction = CD.Instruction;
    InstructionsSeq = CD.InstructionsSeq;

}

Compiler::ContextData::ContextData(Compiler::ContextData &&CD) noexcept
{
    BeginStream     = CD.BeginStream;
    StartSeq        = CD.StartSeq;
    Cur             = CD.Cur;
    EndSeq          = CD.EndSeq;
    EndStream       = CD.EndStream;
    Bloc            = CD.Bloc;
    Instruction     = CD.Instruction;
    InstructionsSeq = std::move(CD.InstructionsSeq);
}

Compiler::ContextData &Compiler::ContextData::operator=(const Compiler::ContextData &CD)
{
    BeginStream     = CD.BeginStream;
    StartSeq        = CD.StartSeq;
    Cur             = CD.Cur;
    EndSeq          = CD.EndSeq;
    EndStream       = CD.EndStream;
    Bloc            = CD.Bloc;
    Instruction     = CD.Instruction;
    InstructionsSeq = CD.InstructionsSeq;
    return *this;
}


Compiler::ContextData &Compiler::ContextData::operator=(Compiler::ContextData &&CD)
{
    BeginStream     = CD.BeginStream;
    StartSeq        = CD.StartSeq;
    Cur             = CD.Cur;
    EndSeq          = CD.EndSeq;
    EndStream       = CD.EndStream;
    Bloc            = CD.Bloc;
    Instruction     = CD.Instruction;
    InstructionsSeq = std::move(CD.InstructionsSeq);
    return *this;
}


#pragma endregion Context
// ---------------- Compiler::Context: END-------------------------------------------


} // Spp
