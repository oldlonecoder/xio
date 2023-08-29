#include "xio/s++/spp.h"
#include <xio/compiler/parser.h>

namespace xio::spp
{

interpretr::interpretr(const std::string interp_name):
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
    cc::parser parser;
    auto R = parser.parse_rule("expression");
    return 0;
}



}