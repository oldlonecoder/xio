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

#include "xio/xio.h"




namespace xio
{




/**
@brief

 */
class  XIO_PUBLIC xiovar : public xio
{
    std::size_t  _index{0ul}; ///< index to get to the local instance;
    type::T _type{type::F32}; ///< default s++ value type.
    friend class xiobloc;
    friend class function_descriptor;


public:
    using list = std::vector<xiovar*>;
    xiovar();


    xiovar(const xiovar&) = delete;
    xiovar(xio* parent_, token_data* info_, std::size_t i_= 0, alu* v = nullptr);

    /**
     * Destructor
     */
    ~xiovar();

};

}


