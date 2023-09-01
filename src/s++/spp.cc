#include "xio/s++/spp.h"
#include "xio/compiler/parser.h"

namespace xio::spp
{

interpretr::interpretr(const std::string& interp_name): xiobloc(),
    name(interp_name)
{
}

interpretr::~interpretr()
{
}

/**
 * \brief operator[] Quick arithmetic expression evaluator (small source code)
 *
 * \param expr
 * \return
 * \author &copy; August 24, 2023; oldlonecoder, (serge.lussier@oldlonecoder.club)

 */
alu interpretr::operator[](const std::string& expr)
{
    grammar g;
    g.build();
    g.dump();
    parser expr_parser(this, expr.c_str());
    auto R = expr_parser.parse_expr(this, expr.c_str());
    if(!R)
      return alu(1.42f);
    return *R;
}

void interpretr::syntax(token_data* token_ptr, token_data::collection const& stream)
{
    lexer_color lc;
    std::string code = token_ptr->text_line();
    auto token = token_ptr->back_to_startof_line(stream);
    lc.process(code, _tokens_stream);
    book::rem::push_syntax() << " in arithmetic expression parsing context. " << book::rem::endl << lc.mark(*token);
}



}
