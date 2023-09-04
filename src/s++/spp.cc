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




/*!
 * \brief interpretr::error
 * \param ertype
 * \param ercode
 * \param token token the object of the error
 * \param line_seq tokens collection sequence part of the line of text/code.
 * \author &copy;2023, oldlonecoder serge.lussier@oldlonecoder.club
 */
void interpretr::error(book::rem::type ertype, book::rem::code ercode, token_data::iterator token, const token_data::collection& line_seq)
{

}

/*!
 * \brief interpretr::warning
 * \param token
 * \param line_seq
 */
void interpretr::warning(rem::type, rem::code, token_data::iterator token, const token_data::collection& line_seq)
{

}

/*!
 * \brief interpretr::trace_line
 * \param token token target
 * \param tokens tokens collection sequence that are part of the line of text/code.
 * \param colour set to true if colourize tokens.
 * \param mark set to true if the token is to be "marked"
 */
void interpretr::trace_line(token_data::iterator token, const token_data::collection& tokens, bool colour, bool mark)
{
    if(colour)
    {

      lexer_color lc;
      lc.process(token, tokens);
      if(mark)
          book::rem::out() << lc.mark(*token);
      //...
      return;
    }

    //...




}


}
