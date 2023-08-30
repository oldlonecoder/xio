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
    auto R = expr_parser.parse_rule("expression");
    return 0;
}



}