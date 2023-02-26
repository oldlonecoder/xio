#include <xio/rti/kpi.h>

constexpr auto _PI_ = 3.14159265358979323846;
namespace xio
{


k_pi::k_pi(xiobloc *blk, token_data* _token):xiobject(blk, _token)
{
    *acc = _PI_;
}

k_pi::~k_pi()
{

}


/// Already assigned, calling k_pi::jsr() is overhead.
alu k_pi::jsr()
{
    return *acc;
}

}
