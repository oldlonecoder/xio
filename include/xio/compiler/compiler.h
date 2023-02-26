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

//program {
//    start: rt::Class::Start;
//    name : Connect Station;
//    ...
//}



class compiler
{
    context   ctx;
    token_data::collection _tokens;
    const char* _source;
    lexer       lex;
    lexer_color* _lc = nullptr; ///< When: diagnostic;

    token_data::iterator _cursor;
    xiobloc*    _global = nullptr;

    context::stack _ctx_stack;

    using maker = std::function<xiobject*()>;



// === === === === === === === === === === === === === === === === === === ===

    xiobject* cc_expr();
    xiobject* cc_pi();
    xiobject* generate_instruction();
    xiobject* cc_identifier();

//    xiobject* cc_if();
//    xiobject* cc_for();
//    xiobject* cc_return();
//    xiobject* cc_decl_var();
//    xiobject* cc_decl_function();
//    xiobject* cc_function_call();

// === === === === === === === === === === === === === === === === === === ===

public:

    compiler();
    compiler(xiobloc* _bloc);
    ~compiler();
    bool eof();

private:

    void init_context();


    token_data* cursor();
    void push_ctx();
    code::T pop_ctx();


};

} // namespace xio

