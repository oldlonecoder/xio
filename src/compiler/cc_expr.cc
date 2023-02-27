#include "xio/compiler/compiler.h"

namespace xio
{

xiobject* compiler::cc_expr()
{
    ctx.m_st = context::mstate::_expr;
    //...
    xiobject* ins = nullptr;
    do{
        if(!ctx.cursor->_flags.V)
        {
            if(!ins)
            {
                if(!ctx.ins_seq.empty())
                    ctx.roll_back();
                return nullptr;
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
            // @todo have maker here too:
            ins = generate_instruction();

            ctx.cursor ++;
            continue;
        }

        //@todo Must provide lambda function to let the parsers create the proper xiobject from within the arithmetic expression tree.
        ins = ins->input(ctx.bloc,cursor(),[&](token_data* token)-> xiobject*
        {
            // As said, for now we create only xio-P.O.D. types vairables. and PI keyword const
            return generate_instruction();

        });

        if(!ins) break;


        ctx.cursor++;
        ctx.ins_seq.push_back(ins);
    }
    while((ins != nullptr) && !eof());

    if(!ins)
    {
        //check
        if(ctx.ins_seq.empty())
        {
            diagnostic::error() << " expression parser invoked, but there is no such expression:" << ctx.start->mark();
            return nullptr;
        }
        ins = *ctx.ins_seq.begin();
    }
    ins = ins->tree_close();
    if(!ins)
        ctx.roll_back(); // xiobject already have put diagnostic message log entry

   return ins;
}

}
