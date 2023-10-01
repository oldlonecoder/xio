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
#include <xio/compiler/lexer.h>
#include <xio/compiler/grammar.h>
#include <xio/xiobloc.h>
#include <stack>


namespace xio
{


class XIO_PUBLIC compiler
{
    xiobloc* _bloc{ nullptr };


    //void push_context();
    //void pop_context();
    //void reset_context();

    //void accept();
    //void reject();

    struct context
    {
        token_data::iterator start, end_stream, end, cur;
        xiobloc*             bloc{nullptr};
        ::xio::type::T       current_type = ::xio::type::Number;
        ///...

        book::rem::code state = book::rem::none;

        xio::list instructions; ///< filling the temp xio buffer before the commit into the instructions list of the scope-bloc ( local stack ).

        context();
        context(compiler::context&& cx) noexcept;
        context(compiler::context const& cx);

        context(xiobloc* blk, token_data::iterator start, token_data::iterator i_end, token_data::iterator i_endstream);

        ~context();

        context& operator = (context&& cx) noexcept;
        context &operator = (context const & cx);

        void accept(xio* instruction);
        void reject();
        token_data* token() { return &(*cur); }
        bool operator++(int);
        bool operator++();

        bool eof();
    };

    compiler::context ctx;

    using parser_pairs_t = std::map<mnemonic, xio*(compiler::*)()>; ///< Paired mnemonic to parser.
    using parser_rules_t = std::map<std::string_view, xio*(compiler::*)()>; ///< Paired rule name to parser.

    static parser_pairs_t   mnemonic_parsers;
    static parser_rules_t   rule_parsers;

public:
    
    struct XIO_PUBLIC config_data
    {
        std::string_view source_content;
        token_data::list* tokens_stream;
        std::string_view grammar_text;
        bool operator !() { return source_content.empty() || (!tokens_stream || tokens_stream->empty()); }
    };
    
    compiler::config_data& config() { return cnf;  }


    //----------- Public access & callables: -------------------

    compiler() = default;
    compiler(const compiler&) = delete;
    compiler(compiler&&) noexcept = delete;

    compiler(xiobloc* bloc);
    

    ~compiler();

    compiler& operator = (compiler&&) noexcept = delete;
    compiler& operator = (const compiler&) = delete;
    xiobloc* bloc() { return _bloc; }
    // ------------------ parsers -Cannot be used yet-----------
    xio* parse_expression();
    book::rem::code evaluate_expr(xiobloc* blk, const char* expr_text);


    xio* input(token_data* token); // Let's explore the same principle as in xio::input.


    // -------------------Cannot be used yet--------------------
    ::xio::xio* make_xio_node(token_data* token);
    // ---------------------------------------------------------
    book::rem::code compile();

private:
    book::rem::code lexical_analyse();
    



    xio* parse_rvalue_keyword();
    token_data::list tokens_line_from(token_data* token);

    compiler::config_data cnf;

    xio* parse_if();
    xio* parse_do();
    xio* parse_while();
    xio* parse_until();
    xio* parse_for();

    xio* return_stmt();

};

}
