//
// Created by oldlonecoder on 11/2/23.
//

#include "Spp/Compiler/Compiler.h"

namespace Spp
{
Compiler::Compiler(Stack *_RBloc, std::string_view _Src)
{
    Data.RootBloc = _RBloc;
    Data.SourceText = _Src;
}




/*!
 * @brief Execute compilation. Main entry.
 *
 * Soon, we will have to TreeBegin Parsing Unit scope bloc and descend into sibling bloc/stacks.
 *
 * @return  Book::Result::Success or Book::Result::Failed.
 * @author &copy; 2023/2024 oldlonecoder (serge.lussier@oldlonecoder.club)
 *
 * @note How to branch on correct rule and context would be to create a dictionary of (Context/Scope -> Rules)
 *  then ( Context/Scope -> {key[Token::Type/Mnemonic/SemanticBits]->rule(s)->parser} )
 */
Book::Result Compiler::operator()()
{
//    if(!Experimentation::Grammar::IsBuilt())
//    {
//        AppBook::Error() << " Gammar rules not built yet!.";
//        return Book::Result::Failed;
//    }

    Ctx = {Data.Tokens.begin(), Data.Tokens.begin(), Data.Tokens.end(), Data.RootBloc};
//    Ctx.Rule = Experimentation::Grammar()["Unit"];

    (void)CCUnit();

    return Book::Result::Notimplemented;
}


/**
 *  \brief parse_expr  Explicitely parses rule 'expression' using the current context data.
 *
 * \return book::code::accepted, or book::code::error if failed.
 * \author &copy; August 23, 2023; oldlonecoder, (serge.lussier@oldlonecoder.club)
 * \note   Stop conditions are yet to be determined clearly. The current is
 *         1 - eof
 *         2 - eos ( end of statement mnemonic or simply the ';' mnemonic )
 *         3 - Expression ends on invalid relationanl token or invalid mnemonic when in condition expr context
 *         -- Other ways that are not yet explored/implemented: ( [condition] expr ) [expr] {expr} syntaxes.
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

    if(!(x = xio::TreeBegin(Ctx.Bloc, &Ctx.Token(), [this](SppToken *T) -> xio * { return NewXio(T); })))
    {
        AppBook::Status() << Color::Yellow << " : seems not an expression at all at " << Book::Fn::Endl << Ctx.Token().Details(true) ;
        return nullptr;
    }

    Ctx++;
    while((Ctx.Cur < Data.Tokens.end()) && (Ctx.Cur->M != Mnemonic::Semicolon)){
        x = x->TreeInput(Ctx.Bloc, &Ctx.Token(), [this](SppToken* T)-> xio*{ return NewXio(T); });
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



xio *Compiler::CCUnit()
{

    AppBook::Error() << " Compile Experimentation::Grammar::Rule[" << Core::Color::Yellow << Ctx.Rule->Id << Core::Color::Reset << "'] not implemented yet... ";
    return nullptr;
}

/**
 * \brief make_xio_node callback function is normally invoked from xio::input.
 *
 * Will create the proper instance of the instruction from the token_data infos.
 * \param token  pointer to the current token.
 * \return pointer to newly created xio;
 * \note As of 2023-08-28, only xio's POD ( Plain Old Data ) variable types are created on identifier token restricted to arithmetic expressions.
 * \author &copy; August 28, 2023; oldlonecoder, (serge.lussier@oldlonecoder.club)
 */
xio* Compiler::NewXio(SppToken* Token)
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
    AppBook::Debug() << "Entering with token: " << Book::Fn::Endl << Ctx.Token().Details(true);

    while(!Ctx.Eof() && (((Ctx.Token().M == Mnemonic::BlocComment)) || ((Ctx.Token().M == Mnemonic::CommentCpp)))) ++Ctx;
    if(Ctx.Eof()) return Book::Result::Eof;
    AppBook::Message() << " accepting skip comment.";
    Ctx.Accept();
    return Book::Result::Ok;
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
    Lex.Config() = {Data.SourceText.data(),&Data.Tokens};
    if(auto R = Lex.Lex(); R != Book::Result::Success)
        return R;

    return Book::Result::Accepted;
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

void Compiler::ContextData::Accept()
{
    StartSeq = EndSeq = Cur;
    InstructionsSeq.clear();
    if(Instruction) Bloc->AppendInstruction(Instruction);
    CurType = Type::Null;
}




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
    : BeginStream(BeginStream), StartSeq(StartSeq), EndStream(EndStream), Bloc(Bloc)
{}


#pragma endregion Context
// ---------------- Compiler::Context: END-------------------------------------------


} // Spp