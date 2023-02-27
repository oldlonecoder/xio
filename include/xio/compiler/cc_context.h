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

#include<xio/xiobloc.h>


namespace xio {
// x + calcule_distance( sin x/ ) - r;
struct context
{


    enum class mstate : uint8_t
    {
        _stmt_start,    ///< begin statement state; ex.:'if' in this state is accepted and invoke cc_if.
        _stmt,          ///< in (middle of) statement  state ;ex.:'if' in this state is rejected and so fatal,error,unexpected.
        _stmt_end,      ///< statement sequence is terminated and accepted.
        _expr,          ///< Explicitely Invoked, pushes condition state for accepting only value tokens, invoking xiobject::input
                        //   to build punctual arithmetic expression AST until oef, error, non-value token.
        _global,        ///< only declarations and init constructs.
        _bloc_enter,    ///< go to _bloc state.
        _bloc,          ///< can go to _stmt_start-_declaration-_stmt-_stmt_end cycles.
        _bloc_end,      ///< terminating bloc. - Switch context to parent bloc.
        _declaration,   ///< Explicit.
        _function_decl, ///< Indirect & Explicit...
        _function_call, ///< Indirect & Explicit...
        _params,        ///< Explicit decl phase...
        _param,
        _args,          ///< Explicit instanciation call phase ...
        _arg,
        // === === === === === === === === === === === === === === === === === === === === === === === ===
        _error,
        _warning,
        _fatal
        //...
    }m_st {context::mstate::_global};

    // ------- Input data : ----------------
    xiobloc*                bloc = nullptr;
    token_data::iterator    start;

    // ------- local data : ----------------
    token_data::iterator    cursor;
    token_data::iterator    stop;
    xiobject::list          ins_seq;
    //..
    friend class compiler;
    using stack = std::stack<context>;



    context();
    context(xiobloc* _bloc, token_data::iterator _start);

    context(context&&) noexcept = default;
    context(const context&) = default;

    ~context();

    context& operator = (context&&) noexcept = default;
    context& operator = (const context&) = default;

    void roll_back();

};

} // namespace xio


