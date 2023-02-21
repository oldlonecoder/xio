#include "xio/compiler/parser.h"

namespace xio {

void parser::init_context()
{
    ctx.ins_seq.clear();
    ctx.stop = ctx.cursor = ctx.start;
}

parser::parser()
{

}

parser::parser(token_data::collection *_tkstream):
    _stream(_tkstream)
{

}

parser::~parser()
{

}

bool parser::eof()
{
    return ctx.cursor == _stream->end();
}

} // namespace xio
