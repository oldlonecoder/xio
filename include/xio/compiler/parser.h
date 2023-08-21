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

    struct context_data
    {
        using iterator = token_data::collection::iterator;
        using stack = std::vector<parser::context_data>;
        iterator begin;
        iterator end;
        iterator cursor;
        iterator source_end;

        const rule* current_rule{nullptr};
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
        // ---------------------------------------------
        //...
    };

    context_data::stack context_stack;
    void push_context();
    void pop_context();
    void reset_context();



};

}