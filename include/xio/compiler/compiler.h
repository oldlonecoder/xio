/***************************************************************************
*   Copyright (C) 2005/2023 by Serge Lussier                              *
*   2005: (bretzel@teaccweb.homelinux.net)                                  *
*   2023: lussier.serge@gmail.com, oldlonecoder@gmail.com                 *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/


#pragma once


#include <xio/compiler/cc_context.h>
#include <xio/lexer/lexer_color.h>

namespace xio {




class compiler
{
    context_t   ctx;
    token_data::collection _tokens;
    const char* _source;
    lexer       lex;
    lexer_color* _lc = nullptr; ///< When: diagnostic;

    token_data::iterator _cursor;
    xiobloc*    _global = nullptr;

    context_t::stack _ctx_stack;

    enum class rule : uint8_t
    {
        _expr,          ///< Explicitely Invoked.
        _if,            ///< Indirect
        //_type,  ///< Indirect & Explicit // f32 A = 1/3;
        _var,           ///< Indirect & explicit
        _decl_var,      ///< Explicit
        _function_decl, ///< Indirect & Explicit
        _function_call, ///< Indirect & Explicit
        _params,        ///< Explicit decl phase
        _args,          ///< Explicit instanciation call phase ...
        //...
    };


    code::T parse_expr();

public:
    compiler();
    compiler(xiobloc* _bloc);
    ~compiler();

    void init_context();
    code::T parse(const context_t& _ctx);

    token_data* cursor();
    void push_ctx( context_t&& ctx );
    code::T pop_ctx();
    bool eof();
};

} // namespace xio

