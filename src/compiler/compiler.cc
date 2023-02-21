#include "xio/compiler/compiler.h"


namespace xio {

compiler::compiler()
{

}

compiler::compiler(xiobloc *_bloc, token_data::collection *tkstream):
    _global(_bloc),
    _tokens(tkstream)
{
    init_context();
}


void compiler::init_context()
{
    //_global = interpreter::global();
    //_global = new xiobloc;
    ctx = {_global, _tokens->begin()};
    //ctx.stop = ctx.cursor = ctx.start;
}




} // namespace xio
