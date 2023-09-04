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




/**
 * \file parser.cc
 * \defgroup compiler
 * \author &copy; 2023, #USERNAME; oldlonecoder (serge.lussier@oldlonecoder.club)
 */


#include <xio/compiler/parser.h>
#include "xio/s++/spp.h"

namespace xio
{



std::map<std::string, book::rem::code(parser::*)()> extern_parsers =
{
    {"expression", &parser::parse_expression},
    {"functionid", nullptr},
    {"newvar", nullptr},
    {"obj_instance",nullptr},
    {"objectid", nullptr},
    {"var_id", nullptr}
};





parser::parser(xiobloc* bloc, const char* source_or_filename): _filename_or_source(source_or_filename),_bloc(bloc)
{}

parser::parser(xiobloc* bloc, const char* source_or_filename, const std::string& use_this_rules_text):
    _filename_or_source(source_or_filename),
    _rules_src(use_this_rules_text),
    _bloc(bloc)
{}

book::expect<alu> parser::parse_expr(xiobloc *blk, const char *expr_text)
{
    _bloc = blk; ///< Interpreter's bloc address likelly...
    lexer lex;
    lex.config() =
    {
        expr_text,
        &_tokens_stream
    };

    auto R = lex();
    if(R!=book::rem::accepted)
    {
        return book::rem::push_error() << R;
    }


    book::rem::push_debug(HERE) << " parser::context initialized : dumping tokens (coloured) details";

    lexer_color lc;
    std::string code = expr_text;
    lc.process(code, _tokens_stream);
    for(auto & token: _tokens_stream) book::rem::out() << lc.mark(token);
    rem::push_debug() << " returning dummy float alu";

    book::rem::push_debug(HERE) << "\\O/ - Now let's parse:";
     ctx = context(_bloc, _tokens_stream.begin(), _tokens_stream.end(), _tokens_stream.end());

    xio* x{nullptr};
    x = xio::begin(ctx.bloc, ctx.token(), [this](token_data* t)->xio*{ return make_instruction(t); });

    if(!x)
    {
        auto tokens = tokens_line_from(ctx.token());
        spp::interpretr::trace_line(ctx.cur, tokens);
        return alu(1.42f);
    }


    do{

    }while(ctx.cur < _tokens_stream.end());


    return alu(1.42f);
}






/**
 *  \brief parse_expr  Explicitely parses rule 'expression'.
 *
 * \return book::rem::accepted, or book::rem::error if failed.
 * \author &copy; August 23, 2023; oldlonecoder, (serge.lussier@oldlonecoder.club)

 */
book::rem::code parser::parse_expression()
{

    return book::rem::notimplemented;
}


/**
 * \brief parse_rule
 *
 * \param rule_name
 * \return book::rem::accepted, or book::rem::rejected if <i>rule name</i> does not matches the code.
 * \author &copy; August 23, 2023; oldlonecoder, (serge.lussier@oldlonecoder.club)

 */
book::rem::code parser::parse_rule(const std::string& rule_name)
{
    grammar::rule* rule = grammar::rules[rule_name];
    if (!rule)
    {
        book::rem::push_error() << " the rule identified by '" << rule_name << "' doe not exist.";
        return book::rem::notexist;
    }

    if (rule->a.is_parserctrl())
    {
        for (auto& [rid, fn] : extern_parsers)
        {
            if (rid == rule_name)
            {
                if (!fn)
                {
                    book::rem::push_error() << "[parser rule]: The external rule identified by '" << rule->a() << color::Reset << '\'' << color::Yellow << rule_name << color::Reset << "' is " << book::rem::notimplemented;
                    return book::rem::notimplemented;
                }
                return (this->*fn)();
            }
        }
        book::rem::push_error() << "[parser rule]: The external rule identified by '" << color::Yellow << rule_name << color::Reset << "' is out of bounds : " << book::rem::notexist;
    }

    //...
    book::rem::push_message() << "[parser rule]: The sequential rule identified by " << rule->a() << color::Reset << '\''
        << color::Yellow
        << rule_name
        << color::Reset << "' is"
        << book::rem::notimplemented;

    return book::rem::notimplemented;
}

xio* parser::parse_expr_keyword(token_data*)
{
    book::rem::push_info(HERE) << book::rem::notimplemented;
    return nullptr;
}

token_data::collection parser::tokens_line_from(token_data* token)
{
    token_data::collection tokens;
    for(auto it = _tokens_stream.begin(); it != _tokens_stream.end(); it++)
    {
        if(it->mLoc.linenum == token->mLoc.linenum) tokens.push_back(*it);
    }

    return tokens;
}



/**
 * \brief make_instruction callback function invoked from xio::input.
 *
 * Will create the proper instance of the instruction from the token_data infos.
 * \param token  pointer to the current token.
 * \return pointer to newly created xio;
 * \note As of 2023-08-28, only xio's POD variable types are created on identifier token restricted to arithmetic expressions.
 * \author &copy; August 28, 2023; oldlonecoder, (serge.lussier@oldlonecoder.club)
 */
::xio::xio* parser::make_instruction(token_data* token)
{
    // "Branch" on token type
    book::rem::push_debug(HERE) << " Entering xio producer with " << token->mark();

    switch(token->t)
    {
        case ::xio::type::Operator:
            return new xio(_bloc, token, nullptr);
        break;
        case ::xio::type::Keyword:
        {
            if(token->_flags.V)
            {
                return parse_expr_keyword(token);
            }
        }
        break;
        case type::Id:
        {
            // Handling only POD variables.
            xiovar* var = ctx.bloc->query_var(token->text());
            if(!var)
                return ctx.bloc->new_var(token);
            else
                return new xio(ctx.bloc, token, var->aluptr());

            //...
        }
        break;
        case  type::Number:
            return new xio(ctx.bloc,token);
        break;
        default: break;
    }
    lexer_color lc;
    std::string code = _filename_or_source;
    lc.process(code, _tokens_stream);
    book::rem::push_syntax() << " in arithmetic expression parsing context. " << book::rem::endl << lc.mark(*token);
    return nullptr;
}






// ------------------------------------- parser::context -----------------------------------------------

parser::context::context()
{

}

parser::context::context(context &&cx) noexcept
{

}

parser::context::context(const context &cx)
{

}


/*!
 * \brief parser::context::context New context with specific iterators and initial scope bloc.
 * \param blk
 * \param start
 * \param i_end
 * \param i_endstream
 */
parser::context::context(xiobloc *blk, token_data::iterator i_start, token_data::iterator i_end, token_data::iterator i_endstream):
  bloc(blk), start(i_start), cur(i_start), end(i_end), end_stream(i_endstream)
{}

parser::context::~context()
{

}

parser::context &parser::context::operator =(parser::context &&cx) noexcept
{
    start = std::move(cx.start);
    cur   = std::move(cx.cur);
    end   = std::move(cx.end);
    end_stream = std::move(cx.end_stream);
    bloc = cx.bloc;
    current_type = cx.current_type;
    return *this;
}


parser::context &parser::context::operator = (parser::context const & cx)
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




void parser::context::accept(context &cx)
{

}

void parser::context::reject(context &cx)
{

}

bool parser::context::operator++(int)
{
    if( cur >= end_stream) return false;
    ++cur;
    ++end;
    return cur < end_stream;
}



}


