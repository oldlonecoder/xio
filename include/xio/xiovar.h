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


