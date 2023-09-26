#include "xio/s++/amu.h"
#include <errno.h>
#include <fstream>




namespace xio
{

std::string_view str_code_none = ";";






rem::code amu::open_file()
{

    //int a = access(_filename.c_str(), F_OK|R_OK);
    //if(a)
    //{
    //    book::rem::push_error(HEREF) << strerror(errno) << book::rem::commit;
    //    return book::rem::notexist;
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
        book::rem::push_error() << "amu::open_file '" << filename << "'" << book::rem::commit;
        return book::rem::failed;
    }

    return book::rem::accepted;
}

book::rem::code amu::compile()
{
    cc = new compiler();
    cc->config() = {source_content, &tokens_stream};
    auto result = cc->compile();
    //...
    return book::rem::notimplemented;
}

book::rem::code amu::process_cmdline(int argc, char** argv)
{
    cmdargs << book::cmd::argdata<amu>{ this, &amu::eval_expression,"Evaluate Expression", "-e", "--expression", "Evaluate arithmetic expression from the command-line", 1};
    cmdargs << book::cmd::argdata<amu>{ this, &amu::source_file, "Source File", "-f", "--file", "Loads and compile source file", 1 };
    cmdargs << book::cmd::argdata<amu>{ this, 0,"Journal", "-j", "--journal", "Set the logger journal file", 1 };
    cmdargs << book::cmd::argdata<amu>{ this, 0,"Expression AST", "-t", "--expression-ast", "Create the expression AST in the dot file", 1 };

    //targ.callback.conntect(this, &amu::gen_expr_ast);
    cmdargs.set_default_callback(&amu::cmdline_invalid_args);

    return cmdargs.process(argc, argv);

}




rem::code amu::eval_expression(const book::cmd::argdata<amu>& arg)
{
    book::rem::push_info(HERE) << " Evaluate: '" << color::Yellow << arg.arguments[0] << color::Reset << "' :" << book::rem::commit;
    cc = new compiler(this);
    cc->config() = { arg.arguments[0].data(), &tokens_stream };

    // - a little mis config here in that the compiler requires the expression source text here:
    if(rem::code result; ( result = cc->evaluate_expr(this, arg.arguments[0].data())) != rem::accepted) return result;


    export_expr_ast(std::string(arg.arguments[0]));
    auto a=jsr();
    lexer lex;
    lex.config() = { arg.arguments[0].data(), &tokens_stream };
    stracc text = lex.colorize();
    book::rem::out() << "Evaluate expression: [" << text << color::Reset << "] - result : " << book::rem::commit;
    for(auto* v : *_xiovars)
    {
        book::rem::out() << "%-10s" << v->attribute() << color::White << " = " << color::Yellow << value()() << book::rem::commit;
    }
    return rem::accepted;
}


/*!
 * \brief amu::source_file
 * \param  book::cmd::argdata<amu>& arg
 * \return rem::accepted;
 * \note Likely the most used option when invoking the interpreter.
 * \author &copy; 2023, oldlonecoder (serge.lussier@oldlonecoder.club)
 */
rem::code amu::source_file(const book::cmd::argdata<amu>& arg)
{
    book::rem::push_debug(HERE) << " Source file '" << color::Yellow << arg.arguments[0] << color::Reset << "':" << book::rem::commit;
    filename = arg.arguments[0].data();
    if(open_file() != book::rem::accepted) return rem::failed;


    book::rem::push_info(HERE) << " ready to compile file: '" << color::Yellow << arg.arguments[0] << color::Reset << "' :" << book::rem::commit;

    compiler cc(this);
    cc.config() = { source_content, &tokens_stream };

    return cc.compile();
}

rem::code amu::cmdline_invalid_args(const book::cmd::argdata<amu>& a)
{
    book::rem::push_error() << book::rem::endl << cmdargs.usage() << book::rem::commit;

    return book::rem::failed;
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
    book::rem::push_debug(HERE) << "expr: '" << expr << "':" << book::rem::commit;
    xio::dot_tree_start(str, expr);
    xio::dot_tree(_instructions->front(), str);
    str << "}";
    //book::rem::push_debug(HERE) << " tree output: ";
    //book::rem::out() << str;
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
