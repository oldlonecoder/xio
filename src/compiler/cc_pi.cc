#include "xio/compiler/compiler.h"
#include <xio/rti/kpi.h>


namespace xio
{

xiobject* compiler::cc_pi()
{
    if(ctx.cursor->c != mnemonic::Pi) return nullptr;
    return new k_pi(ctx.bloc, cursor());
}

}
