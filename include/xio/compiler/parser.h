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

        context();
        context(parser::context&& cx) noexcept;
        context(parser::context const& cx);

        context(xiobloc* blk, token_data::iterator start, token_data::iterator i_end, token_data::iterator i_endstream);

        ~context();

        context& operator = (context&& cx) noexcept;
        context &operator = (context const & cx);

        void accept(parser::context& cx);
        void reject(parser::context& cx);
        token_data* token() { return &(*cur); }
        bool operator++(int);
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
    book::expect<alu> parse_expr(xiobloc* blk, const char* expr_text);

    // -------------------Cannot be used yet--------------------
    ::xio::xio* make_instruction(token_data* token);
    book::rem::code parse_rule(const std::string& rule_name);
    // ---------------------------------------------------------

  private:
    xio* parse_expr_keyword(token_data*);
    token_data::collection tokens_line_from(token_data* token);

};

}
