#include <xio/s++/spp.h>
#include <logbook/cargs.h>

#include <signal.h>

using book::rem;
using xio::spp::interpretr;






void sig_int( int s )
{
    //rem::push_interrupted() << " dump messages stream and exit:";
    //rem::clear(nullptr);
    std::cerr << " sig interrupt caugh...\n";
    exit(3);
}

void sig_fault( int s)
{
    //rem::push_segfault() << " dump messages stream and exit:";
    //rem::clear(nullptr);
    std::cerr << " sigfault caught...\n";
    exit(127);
}

void sig_abort( int s)
{
    //rem::push_aborted() << " dump messages stream and exit:";
    //rem::clear(nullptr);
    std::cerr << s << '\n';

    exit(127);
}

// --------------------------------------------------------------------------
namespace book
{


class app
{
    cmd::cargs<app> args;
public:

    app() = default;
    ~app();

    app(std::string_view title);

    rem::code eval_expression(const cmd::argdata<app>& arg);
    rem::code compile_source(const cmd::argdata<app>& arg);
    rem::code set_draw_ast(const cmd::argdata<app>& arg);
    rem::code cmdline_args(const cmd::argdata<app>& arg);


    rem::code process(int argc, char **argv);
};

app::app(std::string_view title)
{
    args << book::cmd::argdata<app>{ this, &app::eval_expression,"Evaluate Expression", "-e", "--expression", "Evaluate arithmetic expression from the command-line", 1};
    args << book::cmd::argdata<app>{ this, &app::compile_source, "Source File", "-f", "--file", "Loads and compile source file", 1 };
    args << book::cmd::argdata<app>{ this, 0,"Journal", "-j", "--journal", "Set the logger journal file", 1 };
    args << book::cmd::argdata<app>{ this, 0,"Expression AST", "-t", "--expression-ast", "Create the expression AST in the dot file", 1 };

    //targ.callback.conntect(this, &amu::gen_expr_ast);
    args.set_default_callback(&app::cmdline_args);

}

rem::code app::eval_expression(const cmd::argdata<app> &arg)
{
    interpretr i;
    try{
        auto alu = i[arg.arguments[0].data()];
    }
    catch(book::rem&){}
    catch(std::exception e)
    {
        rem::push_except() << e.what() << rem::commit;
    }

    book::rem::clear([](book::rem&){;});
    return rem::success;
}

rem::code app::compile_source(const cmd::argdata<app> &arg)
{
    return rem::notimplemented;
}

rem::code app::cmdline_args(const cmd::argdata<app> &arg)
{
    return rem::notimplemented;
}



rem::code app::process(int argc, char **argv)
{
    return args.process(argc,argv);
}


}




auto main(int argc, char** argv) -> int
{

    ::signal(SIGINT, sig_int);
    ::signal(SIGSEGV, sig_fault);
    ::signal(SIGABRT, sig_abort);

    book::app application("testing the app and the interpreter...");

    (void)application.process(argc, argv);



    return 0;
}

