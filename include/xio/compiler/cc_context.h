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
struct context_t
{
    // ------- Input data : ----------------
    xiobloc*                bloc = nullptr;
    token_data::iterator    start;

    // ------- local data : ----------------
    token_data::iterator    cursor;
    token_data::iterator    stop;
    xiobject::list          ins_seq;
    //..

public:
    context_t();
    context_t(xiobloc* _bloc, token_data::iterator _start);

    context_t(context_t&&) noexcept = default;
    context_t(const context_t&) = default;

    ~context_t();

    context_t& operator = (context_t&&) noexcept = default;
    context_t& operator = (const context_t&) = default;

    void roll_back();
    code::T operator ++();
    code::T operator ++(int);

};

} // namespace xio


