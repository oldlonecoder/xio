// SPDX-FileCopyrightText: 2022 Serge Lussier <email>
// SPDX-License-Identifier: BSD-3-Clause

#include <xio/xiovar.h>

namespace xio
{

xiovar::xiovar() : xiobject()
{

}

xiovar::xiovar(xiobject* parent_, token_data* info_, alu* v): xiobject(parent_, info_, v)
{
}

xiovar::xiovar(const xiovar& other): xiobject()
{

}

xiovar::~xiovar()
{

}


}


