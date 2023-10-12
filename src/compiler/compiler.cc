/***************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                         *
 *   serge.lussier@oldlonecoder.club                                       *
 *                                                                         *
 *                                                                         *
 *   Unless otherwise specified, all code in this project is written       *
 *   by the author (Serge Lussier)                                         *
 *   and no one else then not even {copilot, chatgpt, or any other AI}     *
 *   --------------------------------------------------------------------- *
 *   Copyrights from authors other than Serge Lussier also apply       *
 ***************************************************************************/




/**
 * \file parser.cc
 * \defgroup compiler
 * \author &copy; 2023, #USERNAME; oldlonecoder (serge.lussier@oldlonecoder.club)
 */


#include <xio/compiler/compiler.h>
#include "xio/s++/spp.h"




namespace xio
{


compiler::parser_pairs_t   compiler::mnemonic_parsers=
{
    {mnemonic::Return, &compiler::return_stmt}
};

compiler::parser_rules_t   compiler::rule_parsers=
{
    {"expression", &compiler::parse_expression}
};




compiler::compiler(xiobloc* bloc):_bloc(bloc)
{}


compiler::~compiler()
{
}




/*!
 * \brief compiler::evaluate_epr  - Directly invokable independently. Will not be called from within the normal source content compiling...
 *
 *
 * \param blk
 * \param expr_text
 * \return book::code::accepted or rejected;
 * \author &copy; 2023, oldlonecoder (serge.lussier@goldlonecoder.club) ;)
 */
book::code compiler::evaluate_expr(xiobloc *blk, const char *expr_text)
{
    _bloc = blk; ///< amu's bloc address likelly...

    lexer lex;
    lex.config() = {expr_text, cnf.tokens_stream};

    auto R = lex();
    if(R!=book::code::accepted)
    {
        Book::error() << R ;
        return book::code::rejected;
    }


    Book::debug() << " lexer terminated successfully : prepare (coloured) tokens with details [lexer_color]:" << book::functions::endl ;

    std::string code = expr_text;
    stracc text = lex.colorize();
    Book::info() <<  color::BlueViolet << "xio" << color::White << "::" <<
      color::BlueViolet << "compiler" << color::White << "::" <<
      color::BlueViolet << "evaluate_expr" << color::White << "(" << text << color::White << ") :" ;

    ctx = context(_bloc, cnf.tokens_stream->begin(), cnf.tokens_stream->end(), cnf.tokens_stream->end());

    Book::info() << "begin parse and build expr binary tree ( of xio nodes ): " ;
    xio* x{nullptr};
    if(!(x = xio::begin(ctx.bloc, ctx.token(), [this](token_data* t)->xio*{ return make_xio_node(t); })))
    {
        Book::error() << color::Yellow << " arithmetic expression: source code seems not be an expression at all..." ;
        return book::code::rejected;
    }

    ctx++;
    while((ctx.cur < cnf.tokens_stream->end()) && (ctx.cur->m != ::xio::mnemonic::Semicolon)){
        x = x->input(ctx.bloc, ctx.token(), [this](token_data* token)-> xio*{ return make_xio_node(token); });
        if(!x)
        {
            Book::out() << color::Yellow << " arithmetic expression inputs stopped  by unexpected token - returning." ;
            break;
        }
        ctx.instructions.push_back(x);
        ctx++;
    }

    xio* root = ctx.instructions.back()->tree_close();
    if(!root)
        ctx.reject();

    Book::info() << " Returning accepted." ;
    ctx.accept(root);
    Book::debug() << " root instruction :" << root->token()->details(true) ;
    // jnl.info() << ....
    // jnl.error() << ....
    // jnl.warning() << ....
    // jnl.debug() << ....
    return book::code::success;
}


/*!
 * \brief compiler::input Exploring; Implement token (sub-)sequence input and switch to the appropriate parser for the :
 *        syntax-rule or term of statement in the current rule context
 * \param address of the current (input) token.
 * \return pointer to the new "xio'ed" token.
 * \author &copy; 2023, oldlonecoder (serge.lussier@oldlonecoder.club)

xio *compiler::input(token_data *token)
{
    return nullptr;
}

*/

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
xio* compiler::parse_expression()
{
    Book::debug() << " ==> Entering on token:" << book::functions::endl << ctx.token()->details (true);
    xio* x{nullptr};
    if(!(x = xio::begin(ctx.bloc, ctx.token(), [this](token_data* t)->xio*{ return make_xio_node(t); })))
    {
        Book::status() << color::Yellow << " : seems not an expression at all at " << book::functions::endl << ctx.token()->details(true) ;
        return nullptr;
    }

    ctx++;
    while((ctx.cur < cnf.tokens_stream->end()) && (ctx.cur->m != ::xio::mnemonic::Semicolon)){
        x = x->input(ctx.bloc, ctx.token(), [this](token_data* token)-> xio*{ return make_xio_node(token); });
        if(!x)
        {
            Book::out() << color::Yellow << " arithmetic expression inputs stopped  by unexpected token - returning." ;
            break;
        }
        ctx.instructions.push_back(x);
        ctx++;
    }

    xio* root = ctx.instructions.back()->tree_close();
    if(!root)
        ctx.reject();

    Book::info() << " Returning accepted." ;
    ctx.accept(root);

    return root;
}



book::code compiler::compile()
{
    auto R = lexical_analyse();
    if(R != book::code::accepted)
        return R;
    Book::debug() << " returning because this request is not implemented yet..."  ;
    //ctx = context(_bloc, cnf.tokens_stream->begin(), cnf.tokens_stream->end(), cnf.tokens_stream->end());

    return book::code::notimplemented;
}

book::code compiler::lexical_analyse()
{

    lexer lex;
    lex.config() =
      {
          cnf.source_content.data(),
          cnf.tokens_stream
      };

    auto R = lex();
    if(R!=book::code::accepted)
    {
        Book::error() << R ;
        return book::code::rejected;
    }
    Book::test() << " lexer terminate successfully : prepare (lexical highlight) text of the source code:" << book::functions::endl ;
    Book::out() << lex.colorize() ;
    Book::message() << " will test lexer::mark(...):" << book::functions::endl ;
    Book::test() << lex.mark((*cnf.tokens_stream)[5], true) ;
    //book::code::test() << "";

    return book::code::accepted;
}

xio* compiler::parse_rvalue_keyword()
{
    Book::debug() << book::code::notimplemented << ctx.token()->mark()  ;
    return nullptr;
}


token_data::list compiler::tokens_line_from(token_data* token)
{
    token_data::list tokens;
    for(auto it = cnf.tokens_stream->begin(); it != cnf.tokens_stream->end(); it++)
    {
        if(it->loc.linenum == token->loc.linenum) tokens.push_back(*it);
    }

    return tokens;
}

xio *compiler::parse_if()
{
    return nullptr;
}

xio *compiler::parse_do()
{
    return nullptr;
}

xio *compiler::parse_while()
{
    return nullptr;
}

xio *compiler::parse_until()
{
    return nullptr;
}

xio *compiler::parse_for()
{
    return nullptr;
}

xio *compiler::return_stmt()
{
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
::xio::xio* compiler::make_xio_node(token_data* token)
{
    // "Branch" on token type
    Book::debug() << " Entering xio producer with "<< book::functions::endl << token->mark() ;

    switch(token->t)
    {
        case ::xio::type::Binary:
        case ::xio::type::Prefix:
        case ::xio::type::Postfix:
        case ::xio::type::Unary:
        case ::xio::type::Assign:
        case ::xio::type::OpenPair:
        case ::xio::type::ClosePair:
            return new xio(ctx.bloc, token, nullptr);
        break;
        case ::xio::type::Keyword:
            return parse_rvalue_keyword();
        break;
        case type::Id:
            // As of 09-2023, Handling only POD variables.
            return ctx.bloc->new_var(token);
            //...
        break;
        case  type::Number:
            return new xio(ctx.bloc,token);
        break;
        default:
            Book::message() << "unhandled token:" ;
        break;
    }
        return nullptr;
}






// ------------------------------------- parser::context -----------------------------------------------

compiler::context::context()
{

}

compiler::context::context(context &&cx) noexcept
{

}

compiler::context::context(const context &cx)
{

}


/*!
 * \brief parser::context::context New context with specific iterators and initial scope bloc.
 * \param blk
 * \param start
 * \param i_end
 * \param i_endstream
 */
compiler::context::context(xiobloc *blk, token_data::iterator i_start, token_data::iterator i_end, token_data::iterator i_endstream):
  bloc(blk), start(i_start), cur(i_start), end(i_end), end_stream(i_endstream)
{}

compiler::context::~context()
{

}

compiler::context &compiler::context::operator =(compiler::context &&cx) noexcept
{
    start = std::move(cx.start);
    cur   = std::move(cx.cur);
    end   = std::move(cx.end);
    end_stream = std::move(cx.end_stream);
    bloc = cx.bloc;
    current_type = cx.current_type;
    return *this;
}


compiler::context &compiler::context::operator = (compiler::context const & cx)
{
    start = cx.start;
    cur   = cx.cur;
    end   = cx.end;
    end_stream = cx.end_stream;
    bloc = cx.bloc;
    current_type = cx.current_type;

    return *this;
}


// ------------------------------------- parser::context ^^^--------------------------------------------




void compiler::context::accept(xio* instruction)
{

    start = cur; ///< Pass the consumed tokens
    bloc->append_instruction(instruction);
    instructions.clear(); // Reset the xio::list buffer
    state = book::code::accepted;
}

void compiler::context::reject()
{
    cur = start;
    for(auto* x : instructions)
    {
        bloc->detach(x);
        delete x;
    }

    state = book::code::rejected;
}

bool compiler::context::operator++(int)
{
    if( cur >= end_stream) return false;
    ++cur;
    return cur < end;
}

bool compiler::context::eof()
{
    return cur == end;
}

bool compiler::context::operator++()
{
    if( cur >= end_stream) return false;
    ++cur;
    ++end;
    return cur < end;
}


}


