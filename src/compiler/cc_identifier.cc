#include <xio/compiler/compiler.h>



namespace xio
{

xiobject* compiler::cc_identifier()
{
    ///@note Will have to check function-id for function-call soon
    auto* v = ctx.bloc->query_var(cursor()->text());
    if(v)
        return new xiovar(ctx.bloc, cursor(), v->acc);

//    v = ctx.bloc->query_function(cursor()->text());
//    if(v)
//       return cc_function_call();

    return ctx.bloc->new_var(cursor());
}


}
