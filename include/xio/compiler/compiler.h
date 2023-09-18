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

    token_data::list* _tokens_stream{ nullptr };
    std::string_view _filename;
    std::string_view source_content;

    std::string_view _rules_src;
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

public:
    /*!
     * \brief The Argc class
     *
     * -e "expr"
     * -f path/filename.s++
     *
     * \author &copy;2023, Serge Lussier ( serge.lussier@oldlonecoder.club )
     *
     */
    


    //----------- Public access & callables: -------------------

    compiler() = default;
    compiler(const compiler&) = delete;
    compiler(compiler&&) noexcept = delete;

    compiler(xiobloc* bloc, const char* source_or_filename);
    compiler(xiobloc* bloc, const char* source_or_filename, const std::string& use_this_rules_text);

    ~compiler();

    compiler& operator = (compiler&&) noexcept = delete;
    compiler& operator = (const compiler&) = delete;
    xiobloc* bloc() { return _bloc; }
    // ------------------ parsers -Cannot be used yet-----------
    book::rem::code parse_expression();
    book::rem::code compile_expr(xiobloc* blk, const char* expr_text);

    // -------------------Cannot be used yet--------------------
    ::xio::xio* make_xio_node(token_data* token);
    book::rem::code parse_rule(const std::string& rule_name);
    // ---------------------------------------------------------
    book::rem::code compile();
    void set_source(const char* c) { source_content = c; }
    void set_source_file(const std::string& sf) { _filename = sf; }
    const char* source_code() { return source_content.data(); }
private:
    book::rem::code lexical_analyse();
    book::rem::code open_file();
    xio* parse_rvalue_keyword();
    token_data::list tokens_line_from(token_data* token);

};

}
