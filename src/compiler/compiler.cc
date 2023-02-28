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

compiler::~compiler()
{
    _tokens.clear();
    ctx.ins_seq.clear();
}


void compiler::init_context()
{
    //_global = interpreter::global();
    //_global = new xiobloc;
    ctx = {_global, _tokens.begin()};
    ctx.m_st = context::mstate::_global;
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

code::T compiler::load_file()
{
    if(_cfg.filename.empty())
        throw diagnostic::error()  << " compiler: filename/path is " << code::empty;
    return code::notimplemented;
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

code::T compiler::cc()
{
    if(_cfg.src.empty())
        load_file();

    push_ctx();
    cc_expr();
    pop_ctx();
    return code::accepted;
}




} // namespace xio
