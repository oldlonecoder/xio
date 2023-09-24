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

    return book::rem::ok;
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
    auto& fexpr = cmdargs << book::cmd::argdata{ "Expression", "-e", "--expression", "Evaluate arithmetic expression from the command-line", 1};
    auto& farg  = cmdargs << book::cmd::argdata{ "Source File", "-f", "--file", "Loads and compile source file", 1 };
    auto& jarg  = cmdargs << book::cmd::argdata{ "Journal", "-j", "--journal", "Set the logger journal file", 1 };

    fexpr.callback.connect(this, &amu::eval_expression);
    farg.callback.connect(this, &amu::source_file);
    cmdargs.set_default_callback([this](const book::cmd::argdata& a)-> book::expect<> { return cmdline_invalid_args(a); });

    auto R = cmdargs.process(argc, argv);
    // ...

    return book::rem::code();
}




book::expect<> amu::eval_expression(const book::cmd::argdata& arg)
{
    book::rem::push_info(HERE) << " Evaluate: '" << color::Yellow << arg.arguments[0] << color::Reset << "' :" << book::rem::commit;
    cc = new compiler(this);
    if(rem::code result; ( result = cc->evaluate_expr(this, arg.arguments[0].data())) != rem::accepted) return result;

    export_expr_ast(std::string(arg.arguments[0]));

    return rem::accepted;
}


book::expect<> amu::source_file(const book::cmd::argdata& arg)
{
    book::rem::push_info(HERE) << " compile file: '" << color::Yellow << arg.arguments[0] << color::Reset << "' :" << book::rem::commit;
    return book::rem::notimplemented;
}

book::expect<> amu::cmdline_invalid_args(const book::cmd::argdata& a)
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
