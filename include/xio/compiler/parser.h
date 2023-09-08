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
#include <xio/lexer/lexer_color.h>
#include <xio/compiler/grammar/grammar.h>
#include <xio/xiobloc.h>
#include <stack>


namespace xio
{


class XIO_PUBLIC parser
{

    token_data::collection _tokens_stream;
    const char* _filename_or_source{ nullptr };
    std::string _rules_src;
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
        context(parser::context&& cx) noexcept;
        context(parser::context const& cx);

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

    parser::context ctx;

public:

    //----------- Public access & callables: -------------------

    parser() = default;
    parser(const parser&) = delete;
    parser(parser&&) noexcept = delete;

    parser(xiobloc* bloc, const char* source_or_filename);
    parser(xiobloc* bloc, const char* source_or_filename, const std::string& use_this_rules_text);



    parser& operator = (parser&&) noexcept = delete;
    parser& operator = (const parser&) = delete;

    // ------------------ parsers -Cannot be used yet-----------
    book::rem::code parse_expression();
    book::rem::code parse_expr(xiobloc* blk, const char* expr_text);

    // -------------------Cannot be used yet--------------------
    ::xio::xio* make_instruction(token_data* token);
    book::rem::code parse_rule(const std::string& rule_name);
    // ---------------------------------------------------------

  private:
    xio* parse_rvalue_keyword();
    token_data::collection tokens_line_from(token_data* token);

};

}
