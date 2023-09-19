#include "xio/s++/spp.h"
#include "xio/compiler/compiler.h"
#include <fstream>



namespace xio::spp
{



interpretr::interpretr(const std::string& interp_name, int argc, char** argv): amu(nullptr, interp_name, argc,argv),
    name(interp_name)
{
}

interpretr::~interpretr()
{

}






/*!
 * \brief interpretr::process Load sources from the file or expression given on the commandline
 * \param argc
 * \param argv
 * \return
 */
book::rem::code interpretr::process(int argc, char** argv)
{
    grammar g;
    g.build();
    g.dump();
    compiler cc(this);


  //  if(R != book::rem::accepted) return R;
    // this amu's scope instructions set ( init stuff such as local variables... )
    alu a = jsr(); 

    // At this global level, find and execute the 'main' function:
    //...

    export_expr_ast(source_content);
    return book::rem::accepted;
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
    compiler expr_parser(this);
    token_data::list tokens;
    //expr_parser.config() = {expr.c_str(), &tokens};
    
    auto R = expr_parser.compile_expr(this, expr.c_str());
    //auto R = expr_parser.parse_rule("expression");
    if(R != book::rem::accepted)
      return alu(1.42f);

    stracc str = "";
    xio::dot_tree_start(str, expr);
    xio::dot_tree(_instructions->front(), str);
    str << "}";
    //book::rem::push_debug(HERE) << " tree output: ";
    //book::rem::out() << str;
    std::ofstream of("./xio.dot");
    of << str();
    of.close();
    (void)system("dot -O xio.dot -Tpng && xdg-open xio.dot.png");

    alu r = jsr();
    book::rem::push_debug(HERE) << " result: " << color::Yellow << r() << ": " << book::rem::endl;
    if(_xiovars)
    {
      for(auto* v : *_xiovars)
          book::rem::out() << color::White << "'" << v->t0->text() << color::White << "' :" <<color::Yellow << v->value()();
    }
    return r;
}



stracc interpretr::export_expr_ast(const std::string& expr)
{
    stracc str = "";
    book::rem::push_debug(HERE) << "expr: '" << expr << "':" << book::rem::commit;
    xio::dot_tree_start(str, expr);
    xio::dot_tree(_instructions->front(), str);
    str << "}";
    //book::rem::push_debug(HERE) << " tree output: ";
    //book::rem::out() << str;
    std::ofstream of("./xio.dot");
    of << str();
    of.close();
#if defined(_MSC_VER) || !defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    (void)system("dot -O xio.dot -Tpng && start xio.dot.png");
#else // !
    (void)system("dot -O xio.dot -Tpng && xdg-open xio.dot.png");
#endif
    return str;
}

/*!
 * \brief interpretr::error
 * \param ertype
 * \param ercode
 * \param token token the object of the error
 * \param line_seq tokens collection sequence part of the line of text/code.
 * \author &copy;2023, oldlonecoder serge.lussier@oldlonecoder.club
 */
void interpretr::error(book::rem::type ertype, book::rem::code ercode, token_data::iterator token, const token_data::list& line_seq)
{

}



/*!
 * \brief interpretr::warning
 * \param token
 * \param line_seq
 */
void interpretr::warning(rem::type, rem::code, token_data::iterator token, const token_data::list& line_seq)
{

}



}
