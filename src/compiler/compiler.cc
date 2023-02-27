#include "xio/compiler/compiler.h"


namespace xio {


compiler::compiler()
{

}



compiler::compiler(xiobloc *_bloc):
    _global(_bloc)
{
    init_context();
}


void compiler::init_context()
{
    //_global = interpreter::global();
    //_global = new xiobloc;
    ctx = {_global, _tokens.begin()};
    //ctx.stop = ctx.cursor = ctx.start;
}

token_data *compiler::cursor()
{
    return ctx.cursor < _tokens.end() ? &(*ctx.cursor) : nullptr;
}

void compiler::push_ctx()
{
    _ctx_stack.push(ctx);
}

code::T compiler::pop_ctx()
{
    if(_ctx_stack.empty())
    {
        diagnostic::error() << code::empty << " the context stack is empty";
        return code::empty;
    }
    ctx = _ctx_stack.top();
    return code::accepted;
}

xiobject *compiler::generate_instruction()
{
    //xiobject* x = nullptr;
    if(ctx.m_st == context::mstate::_expr)
    {
        if(cursor()->t == xio::type::Id) return cc_identifier();
        if(cursor()->c == mnemonic::Pi) return cc_pi();

        return new xiobject(ctx.bloc,cursor());
    }

    //...
    diagnostic::warning() << code::notimplemented;
    return nullptr;
}

bool compiler::eof()
{
    return ctx.cursor == _tokens.end();
}




} // namespace xio
