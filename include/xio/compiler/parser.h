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
#include <xio/xio.h>



namespace xio::cc
{

class xiobloc;

class XIO_PUBLIC parser
{
    
    token_data::collection _tokens_stream;
    const char* _filename_or_source{ nullptr };
    std::string _rules_src;
    xiobloc* _bloc{ nullptr };
    

    struct context_data
    {
        using iterator = token_data::collection::iterator;
        using stack = std::vector<parser::context_data>;
        iterator begin;
        iterator end;
        iterator cursor;
        iterator source_end;

        const ::xio::grammar::rule* current_rule{nullptr};
        xiobloc*    current_scope{nullptr};

        xio::list   build;
        xio*        instruction{nullptr};
        xio*        input{nullptr};

        context_data() = default;
        // sous réserve:
        context_data(context_data&& ) = default;
        context_data(context_data const& ) = default;
        // ---------------------------------------------
        context_data(token_data::collection* token_stream);

        ~context_data();

        // sous réserve:
        context_data& operator=(context_data&& ) = default;
        context_data& operator=(context_data const& ) = default;
        context_data& operator=(token_data::collection* tkptr);
        void assign_token_stream(token_data::collection* tkstream);
        // ---------------------------------------------
        //...
    };

    context_data::stack context_stack;
    //void push_context();
    //void pop_context();
    //void reset_context();

    //void accept();
    //void reject();



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
    book::rem::code parse_expr();
    xio* make_instruction(token_data* token);
    book::rem::code parse_rule(const std::string& rule_name);
    // ---------------------------------------------------------

};

}