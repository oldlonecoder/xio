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

#include <xio/compiler/cc_context.h>


namespace xio {

context_t::context_t()
{

}


context_t::context_t(xiobloc *_bloc, token_data::iterator _start):
    bloc(_bloc),
    start(_start),
    stop(_start),
    cursor(_start)
{

}

void context_t::roll_back()
{
    for(auto* x : ins_seq)
    {
        //...
        bloc->detach(x);
        delete x;
        //...
    }
    ins_seq.clear();
}

code::T context_t::operator ++()
{
    return code::notimplemented;
}

code::T context_t::operator ++(int)
{
    return code::notimplemented;
}


} // namespace xio
