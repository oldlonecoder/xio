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
            // @todo have maker here too:
            ins = xiobject::begin(ctx.bloc, cursor());

            ctx.cursor ++;
            continue;
        }

        //@todo Must provide lambda function to let the parsers create the proper xiobject from within the arithmetic expression tree.
        ins = ins->input(ctx.bloc,cursor(),[&](token_data* token)-> xiobject*
        {
            // As said, for now we create only xio-P.O.D. types vairables. and PI keyword const
            switch(token->t)
            {
                case xio::type::Id:
                {
                    auto* v = ctx.bloc->query_var(cursor()->text());
                    if(v)
                        return new xiovar(ctx.bloc, cursor(), v->acc);

                    return ctx.bloc->new_var(cursor());
                }
                case xio::type::Number:
                {
                    if(ctx.cursor->c == mnemonic::Pi)
                    {
                        return new xiobject(ctx.bloc, cursor());
                    }
                }


            }
            return nullptr;
        });
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
