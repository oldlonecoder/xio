#include "xio/compiler/compiler.h"

namespace xio
{
code::T compiler::parse_expr()
{
    init_context();
    //...
    xiobject* ins = nullptr;
    do{
        if(!ctx.cursor->_flags.V)
        {
            if(!ins)
            {
                ctx.roll_back();
                return code::rejected;
            }
            break;
        }
        if(!ins)
        {
            //...
            // 1 - identifer { var, function_call, ... }
            // 2 - (const) val Keyword
            // Ex.: u8 A = 0;
            //           =
            //          / \
            //        u8   0
            //          \
            //           A

            ins = new xiobject(ctx.bloc, &(*ctx.cursor));
            ctx.cursor ++;
            continue;
        }

        ins = ins->input(ctx.bloc,&(*ctx.cursor));
        if(!ins)
        {
            ctx.roll_back();
            return code::rejected;
        }
        ctx.cursor++;
        ctx.bloc->append_instruction(ins);
    }
    while(!eof());
    // A;
    ins = ins->tree_close();
    if(!ins)
    {
        ctx.roll_back();
        return code::rejected;
    }
    return code::accepted;
}

}
