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


#include <xio/compiler/compiler.h>
#include "xio/s++/spp.h"
#include <errno.h>
#include <fstream>



namespace xio
{



std::map<std::string, book::rem::code(compiler::*)()> extern_parsers =
{
    {"expression", &compiler::parse_expression},
    {"functionid", nullptr},
    {"newvar", nullptr},
    {"obj_instance",nullptr},
    {"objectid", nullptr},
    {"var_id", nullptr}
};



compiler::compiler(xiobloc* bloc, const char* source_or_filename): _filename(source_or_filename),_bloc(bloc)
{}

compiler::compiler(xiobloc* bloc, const char* source_or_filename, const std::string& use_this_rules_text):
    _filename(source_or_filename),
    _rules_src(use_this_rules_text),
  _bloc(bloc)
{}

compiler::~compiler()
{
    _tokens_stream.clear();
}




/*!
 * \brief parser::parse_expr  - Directly invokable independently. Cannot be called from within the normal rules parsing...
 *
 *
 * \param blk
 * \param expr_text
 * \return rem::code::accepted or rejected;
 * \author &copy; 2023, oldlonecoder (serge.lussier@goldlonecoder.club) ;)
 */
book::rem::code compiler::parse_expr(xiobloc *blk, const char *expr_text)
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
        book::rem::push_error() << R;
        return book::rem::rejected;
    }


    book::rem::push_debug(HERE) << " lexer terminate successfully : prepare (coloured) tokens with details [lexer_color]:" << book::rem::endl << book::rem::commit;

    lexer_color lc;
    std::string code = expr_text;
    lc.process(_tokens_stream);
    book::rem::push_info() <<  color::BlueViolet << "xio" << color::White << "::" <<
      color::BlueViolet << "compiler" << color::White << "::" <<
      color::BlueViolet << "parse_expr" << color::White << "(" <<
      lc.Product() << color::White << ") : dumping tokens: " << book::rem::commit;

    for(auto & token: _tokens_stream) book::rem::out() << lc.mark(token) << book::rem::commit;

    book::rem::push_info(HERE) << "init context data;" << book::rem::commit;
    ctx = context(_bloc, _tokens_stream.begin(), _tokens_stream.end(), _tokens_stream.end());

    book::rem::push_info() << "begin parse and build expr binary tree ( of xio nodes ): " << book::rem::commit;
    xio* x{nullptr};
    x = xio::begin(ctx.bloc, ctx.token(), [this](token_data* t)->xio*{ return make_xio_node(t); });

    if(!x)
    {
        if(!ctx.cur->_flags.V)
        {
            book::rem::out(HERE) << color::Yellow << " arithmetic expression inputs stopped by non-value token - returning." << book::rem::commit;
            goto end_of_expr;
        }
        book::rem::push_message() << "parse arithmetic expression failed." << book::rem::commit;
        ctx.reject();

        return book::rem::accepted;
    }

    ctx++;
    while((ctx.cur < _tokens_stream.end()) && (ctx.cur->m != ::xio::mnemonic::Semicolon)){
        x = x->input(ctx.bloc, ctx.token(), [this](token_data* token)-> xio*{
            return make_xio_node(token);
        });
        if(!x)
        {
            book::rem::out(HERE) << color::Yellow << " arithmetic expression inputs stopped  by unexpected token - returning." << book::rem::commit;
            break;
        }
        ctx.instructions.push_back(x);
        ctx++;
    }
end_of_expr:
    xio* root = ctx.instructions.back()->tree_close();
    if(!root)
        ctx.reject();

    book::rem::push_info() << " Returning accepted." << book::rem::commit;
    ctx.accept(root);
    // jnl.info() << ....
    // jnl.error() << ....
    // jnl.warning() << ....
    // jnl.debug() << ....
    return alu(1.42f);
}



/**
 *  \brief parse_expr  Explicitely parses rule 'expression'.
 *
 * \return book::rem::accepted, or book::rem::error if failed.
 * \author &copy; August 23, 2023; oldlonecoder, (serge.lussier@oldlonecoder.club)

 */
book::rem::code compiler::parse_expression()
{
    book::rem::push_error(HERE) << " This rule parser is not yet implemented. " << book::rem::commit;
    return book::rem::notimplemented;
}


/**
 * \brief parse_rule
 *
 * \param rule_name
 * \return book::rem::accepted, or book::rem::rejected if <i>rule name</i> does not matches the code.
 * \author &copy; August 23, 2023; oldlonecoder, (serge.lussier@oldlonecoder.club)

 */
book::rem::code compiler::parse_rule(const std::string& rule_name)
{
    grammar::rule* rule = grammar::rules[rule_name];
    if (!rule)
    {
        book::rem::push_error() << " the rule identified by '" << rule_name << "' doe not exist."  << book::rem::commit;
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
                    book::rem::push_error() << "[parser rule]: The external rule identified by '" << rule->a() << color::Reset << '\'' << color::Yellow << rule_name << color::Reset << "' is " << book::rem::notimplemented << book::rem::commit;
                    return book::rem::notimplemented;
                }
                return (this->*fn)();
            }
        }
        book::rem::push_error() << "[parser rule]: The external rule identified by '" << color::Yellow << rule_name << color::Reset << "' is out of bounds : " << book::rem::notexist << book::rem::commit;
    }

    //...
    book::rem::push_message() << "[parser rule]: The sequential rule identified by " << rule->a() << color::Reset << '\''
        << color::Yellow
        << rule_name
        << color::Reset << "' is"
        << book::rem::notimplemented << book::rem::commit;

    return book::rem::notimplemented;
}

rem::code compiler::compile()
{
    book::rem::code  R = open_file();
    if(R != book::rem::ok)
        return R;
    R = lexical_analyse();
    if(R != book::rem::accepted)
        return R;
    rem::push_debug(HEREF) << " returning because this request is not implemented yet..."  << book::rem::commit;
    //ctx = context(_bloc, _tokens_stream.begin(), _tokens_stream.end(), _tokens_stream.end());

    return book::rem::notimplemented;
}

rem::code compiler::lexical_analyse()
{

    lexer lex;
    lex.config() =
      {
          source_content().c_str(),
          &_tokens_stream
      };

    auto R = lex();
    if(R!=book::rem::accepted)
    {
        book::rem::push_error() << R << book::rem::commit;
        return book::rem::rejected;
    }
    book::rem::push_test(HERE) << " lexer terminate successfully : prepare (lexical highlight) text of the source code:" << book::rem::endl << book::rem::commit;
    book::rem::out() << lex.colorize() << book::rem::commit;
    book::rem::push_message() << " will test lexer::mark(...):" << book::rem::endl << book::rem::commit;
    book::rem::push_test(HERE) << lex.mark(_tokens_stream[5], true) << book::rem::commit;
    //book::rem::test(HERE) << "";

    return book::rem::accepted;
}

rem::code compiler::open_file()
{

    int a = access(_filename.c_str(), F_OK|R_OK);
    if(a)
    {
        book::rem::push_error(HEREF) << strerror(errno) << book::rem::commit;
        return book::rem::notexist;
    }

    std::ifstream in; // std::iobase::in
    in.open(_filename);
    if(in.is_open())
    {
        char  line[256];
        while(!in.eof())
        {
            in.getline(line,255);
            source_content << line << '\n';
        }
        in.close();
    }
    else
        return book::rem::failed;

    return book::rem::ok;
}

xio* compiler::parse_rvalue_keyword()
{
    book::rem::push_debug(HERE) << book::rem::notimplemented << ctx.token()->mark()  << book::rem::commit;
    return nullptr;
}


token_data::list compiler::tokens_line_from(token_data* token)
{
    token_data::list tokens;
    for(auto it = _tokens_stream.begin(); it != _tokens_stream.end(); it++)
    {
        if(it->loc.linenum == token->loc.linenum) tokens.push_back(*it);
    }

    return tokens;
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
    book::rem::push_debug(HERE) << " Entering xio producer with "<< book::rem::endl << token->mark() << book::rem::commit;

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
            book::rem::push_message(HERE) << "unhandled token:" << book::rem::commit;
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
    state = book::rem::accepted;
}

void compiler::context::reject()
{
    cur = start;
    for(auto* x : instructions)
    {
        bloc->detach(x);
        delete x;
    }

    state = book::rem::rejected;
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


/*!
 * \brief Argc::Argc Copy command--line arguments into the stracc::list container.
 * \param argc
 * \param argv
 */
compiler::Argc::Argc(compiler* ac, int argc, char** argv): acc(ac)
{
    for(int a=1; a<argc ; a++) args.push_back(argv[a]);
    arg = args.begin();
 //   book::rem::push_debug(HERE) << "count: " << color::Yellow << args.size() << book::rem::commit;
}

compiler::Argc::Argc()
{

}

compiler::Argc::~Argc()
{
    args.clear();
}

bool compiler::Argc::operator >>(std::string& str)
{
    if(arg >= args.end())
    {
        str = "eof";
        return false;
    }
    str = *arg;
    ++arg;

    return arg <= args.end();
}


book::rem::code compiler::Argc::process()
{
    if(args.empty())
    {
        book::rem::push_error() << "expected arguments :" << book::rem::endl << usage() << book::rem::commit;
        return book::rem::rejected;
    }
    reset();
//    std::string a;
//    // debug:
//    while((*this) >> a) book::rem::out() << a << book::rem::commit;

    // Switch:
    reset();
    if(*arg == "-e")
    {
        ++arg;
        if(arg < args.end())
        {
            return acc->parse_expr(acc->bloc(), (*arg).c_str());

        }
        book::rem::push_error() << "expected expression text" << book::rem::endl << usage() << book::rem::commit;
        return book::rem::rejected;
    }
    if(*arg == "-f")
    {
        ++arg;
        if(arg < args.end())
        {
            book::rem::push_debug(HERE) << color::Reset << "compiling file '" << color::Aquamarine3 << *arg << color::Reset << "':" << book::rem::commit;
            acc->set_source_file(*arg);
            return acc->compile();
        }
        book::rem::push_error() << "-f requires [path]/filename as the source code." << book::rem::endl << usage() << book::rem::commit;

        return book::rem::rejected;
    }

    book::rem::push_error() << "expected arguments :" << book::rem::endl << usage() << book::rem::commit;
    return book::rem::rejected;
}

std::string compiler::Argc::usage()
{
    stracc str;
    str << "----------------------------------------\n";
    str << "-e \"expression\"\n";
    str << "-f [path]/source_file.s++ \n";
    str << "----------------------------------------\n";

    return str();
}
}


