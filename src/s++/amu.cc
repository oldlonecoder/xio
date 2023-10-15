#include "xio/s++/amu.h"
#include <errno.h>
#include <fstream>



namespace xio
{

std::string_view str_code_none = ";";






book::code amu::open_file()
{

    //int a = access(_filename.c_str(), F_OK|R_OK);
    //if(a)
    //{
    //    Book::error(F) << strerror(errno);
    //    return book::notexist;
    //}

    std::ifstream in; // std::iobase::in
    in.open(filename);
    if (in.is_open())
    {
        char  line[256];
        while (!in.eof())
        {
            in.getline(line, 255);
            source_content += line;
            source_content += '\n';
        }
        in.close();
    }
    else
    {
        Book::error() << '\'' << color::Yellow << filename << color::White <<'\'' << color::White << " " << strerror(errno);
        return book::code::failed;
    }

    return book::code::accepted;
}

book::code amu::compile()
{
    cc = new compiler();
    cc->config() = {source_content, &tokens_stream};
    auto result = cc->compile();
    //...
    return book::code::notimplemented;
}




book::code amu::eval_expression(std::string_view  expr_str)
{
    Book::info() << " Evaluate: '" << color::Yellow << expr_str << color::Reset << "' :";
    cc = new compiler(this);
    cc->config() = { expr_str.data(), &tokens_stream };

    // - a little mis config  in that the compiler requires the expression source text :
    if(book::code result; ( result = cc->evaluate_expr(this, expr_str.data())) != book::code::accepted) return result;


    export_expr_ast(std::string(expr_str.data()));
    auto a=jsr();
    lexer lex;
    lex.config() = {expr_str.data() , &tokens_stream };
    stracc text = lex.colorize();
    Book::out() << "Evaluate expression: [" << text << color::Reset << "] - result : ";
    for(auto* v : *_xiovars)
    {
        Book::out() << "%-10s" << v->attribute() << color::White << " = " << color::Yellow << value()();
    }
    return book::code::accepted;
}


/*!
 * \brief amu::source_file
 * \param  book::cmd::argdata<amu>& arg
 * \return book::code::accepted;
 * \note Likely the most used option when invoking the interpreter.
 * \author &copy; 2023, oldlonecoder (serge.lussier@oldlonecoder.club)
 */
book::code amu::source_file(std::string_view arg)
{
    Book::debug() << " Source file '" << color::Yellow << arg << color::Reset << "':";
    filename = arg.data();
    if(open_file() != book::code::accepted) return book::code::failed;


    Book::info() << " ready to compile file: '" << color::Yellow << arg << color::Reset << "' :";

    compiler cc(this);
    cc.config() = { source_content, &tokens_stream };

    return cc.compile();
}


amu::amu(xiobloc* parent_bloc, const std::string& mname) : xiobloc(parent_bloc, mname)
{
}

amu::~amu()
{
}






stracc amu::export_expr_ast(const std::string& expr)
{
    stracc str = "";
    Book::debug() << "expr: '" << expr << "':";
    xio::dot_tree_start(str, expr);
    xio::dot_tree(_instructions->front(), str);
    str << "}";
    //Book::debug() << " tree output: ";
    //Book::out() << str;
    std::ofstream of("./xio.dot");
    of << str();
    of.close();
//#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    //(void)system("dot -O xio.dot -Tpng && start xio.dot.png");

//#else // !
    //(void)system("dot -O xio.dot -Tpng && xdg-open xio.dot.png");
    (void)system("dot -O xio.dot -Tpng");
//#endif
    return str;
}









}
