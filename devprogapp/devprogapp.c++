#include <xio/s++/spp.h>
#include <logbook/cargs.h>
#include <logbook/text.h>
#include <signal.h>


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

    book::code eval_expression(const cmd::argdata<app>& arg);
    book::code compile_source(const cmd::argdata<app>& arg);
    book::code set_draw_ast(const cmd::argdata<app>& arg);
    book::code cmdline_args(const cmd::argdata<app>& arg);


    book::code process(int argc, char **argv);
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

app::~app()
{
    ;
}

book::code app::eval_expression(const cmd::argdata<app> &arg)
{
    interpretr i;
    try{
        auto alu = i[arg.arguments[0].data()];
    }
    catch(Book::section::bloc_stack::element&){}
    catch(Book::exception e)
    {
        Book::except() << e.what();
    }


    return book::code::success;
}

book::code app::compile_source(const cmd::argdata<app> &arg)
{

    interpretr i;
    auto r = i.source(arg.arguments[0]);
    if(r==book::code::success)
        Book::debug() << " result:" << color::Yellow << i.value()();

    return r;

//    Book::debug() << book::functions::endl << " As of " << book::functions::weekday << ", " << book::functions::monthname
//                  << ' ' << book::functions::day << ' ' << book::functions::year
//                  << ", xio project is still exploring its logistics. Thus, this option:" << book::functions::endl
//                  << color::Yellow << arg.description << color::White << book::functions::endl
//                  << "has been unimplemented ...Temporary...";

//    return book::code::notimplemented;
}

book::code app::cmdline_args(const cmd::argdata<app> &arg)
{
    return book::code::notimplemented;
}



book::code app::process(int argc, char **argv)
{
    return args.process(argc,argv);
}


}




auto main(int argc, char** argv) -> int
{

    ::signal(SIGINT, sig_int);
    ::signal(SIGSEGV, sig_fault);
    ::signal(SIGABRT, sig_abort);

    try
    {
        Book& AppBook = Book::init("eXecutable-Instruction-Objects");
        AppBook.open();
        AppBook.descriptions =
R"(
<Icon:School; fg:Yellow>This is the development of the <fg:White>[xio & interpreter API explorations]<fg/>
------------------------------------------------------------------------
)";

        std::string head;
        book::text ml_description;
        auto c = ml_description << AppBook.descriptions >> head;
        if(c != book::code::success)
            std::cerr << " text processing failed.";
        auto & xiodev_section  = AppBook.create_section("xio.dev");
        xiodev_section.open().create_stack("xio-eval_expression-dev");
        /*auto & stackdev =*/
        AppBook["xio.dev"]["xio-eval_expression-dev"];

        Book::out() << book::functions::endl << head;
        book::app application("testing the app and the interpreter...");
        (void)application.process(argc, argv);
    }
    catch(Book::exception be)
    {
        std::cerr << " Cauch Book::exception: " << be.what() << '\n';
        return 127;
    }
    /// xio and interpreter compiling stuff:
    catch(Book::section::bloc_stack::element& bel)
    {
        std::cerr << " Caugh event exception held in Book element. Closing the Book now";
    }

    Book::close();

    return 0;
}

