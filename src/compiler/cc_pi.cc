#include "xio/compiler/compiler.h"



constexpr auto _PI_ = 3.14159265358979323846;

namespace xio
{

xiobject* compiler::cc_pi()
{
    if(ctx.cursor->c != mnemonic::Pi) return nullptr;
    //..
    auto* x = new xiobject(ctx.bloc, cursor());
    // alu* xiobject::acc is created:
    *(x->acc) = _PI_;

    return x;
}

}
