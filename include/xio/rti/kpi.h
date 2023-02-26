#include <xio/xiobloc.h>

#pragma once

namespace xio
{

class k_pi : public xiobject
{
public:
    k_pi(xiobloc* blk, token_data* _token);
    ~k_pi() override;

    alu jsr() override;

};

}
