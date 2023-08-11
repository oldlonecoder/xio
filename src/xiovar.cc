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

#include "xio/xiovar.h"


namespace xio
{

xiovar::xiovar() : xio()
{

}

xiovar::xiovar(xio *parent_, token_data *info_, std::size_t i_, alu *v) : xio(parent_, info_, v), _index(i_) {}

xiovar::~xiovar()
{

}


}


