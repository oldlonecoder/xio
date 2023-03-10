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

// SPDX-FileCopyrightText: 2022 Serge Lussier <email>
// SPDX-License-Identifier: BSD-3-Clause


#pragma once

#include <xio/xiobject.h>




namespace xio {

/**
 * @todo write docs
 */
class  xiovar : public xiobject
{

public:
    using list = std::vector<xiovar*>;
    // will use sub-template when I will write the interpret objects operations... this var will holds the objet  instance...
    /**
     * Default constructor
     */
    xiovar();

    /**
     * Copy constructor
     *
     * @param other TODO
     */
    xiovar(const xiovar& other);
    xiovar(xiobject* parent_, token_data* info_, alu* v = nullptr);

    /**
     * Destructor
     */
    ~xiovar();

};

}


