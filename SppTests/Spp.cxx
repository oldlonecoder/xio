#include <Spp/Runtime/Interpreter.h>


#include <AppBook/Book/AppBook.h>
#include <AppBook/Book/StmlText.h>

#include <csignal>


void SigInt(int s )
{
    //rem::push_interrupted() << " dump messages stream and exit:";
    //rem::clear(nullptr);
    std::cerr << " sig interrupt caugh...\n";
    AppBook::Close();
    exit(3);
}

void SigFault(int s)
{
    //rem::push_segfault() << " dump messages stream and exit:";
    //rem::clear(nullptr);
    std::cerr << " sigfault caught...\n";
    AppBook::Close();
    exit(127);
}

void SigAbort(int s)
{
    //rem::push_aborted() << " dump messages stream and exit:";
    //rem::clear(nullptr);
    std::cerr << s << '\n';
    AppBook::Close();
    exit(127);
}

// --------------------------------------------------------------------------


class SppTest
{
    Core::Cmd::CArgs Args;
public:

    SppTest() = default;
    ~SppTest();

    explicit SppTest(std::string_view title);

    Book::Enums::Code EvalExpression(Core::Cmd::ArgumentData& Arg);
    Book::Enums::Code CompileSource (Core::Cmd::ArgumentData& Arg);
    Book::Enums::Code SetDrawAst    (Core::Cmd::ArgumentData& Arg);
    Book::Enums::Code CmdlineArgs   (Core::Cmd::ArgumentData& Arg);


    Book::Enums::Code Process(int argc, char **argv);
};

SppTest::SppTest(std::string_view title)
{
    Args << Core::Cmd::ArgumentData { "Evaluate Expression", "-e", "--expression", "Evaluate arithmetic expression from the command-line", 1};
    Args << Core::Cmd::ArgumentData { "Source File", "-s", "--source-file", "Loads and compile source file", 1 };
    Args << Core::Cmd::ArgumentData { "Journal", "-j", "--journal", "Set the logger journal file", 1 };
    Args << Core::Cmd::ArgumentData { "Expression AST", "-t", "--expression-ast", "Create the expression AST in the dot file", 1 };

    Args["Evaluate Expression"].Connect(this, &SppTest::EvalExpression);

    Args.SetDefaultCallback(this, &SppTest::CmdlineArgs);
}

SppTest::~SppTest()
{
    ;
}

Book::Enums::Code SppTest::EvalExpression(Core::Cmd::ArgumentData& Arg)
{
//    interpretr i;
//    try{
//        auto A = i[Arg.arguments[0].data()];
//    }
//    catch(Book::section::bloc_stack::element&){}
//    catch(Book::exception& e)
//    {
//        Book::except() << e.what();
//    }


    return Book::Enums::Code::Notimplemented;
}

Book::Enums::Code SppTest::CompileSource(Core::Cmd::ArgumentData& Arg)
{

//    interpretr i;
//    auto r = i.source(Arg.arguments[0]);
//    if(r==Book::Enums::Code::success)
//        Book::debug() << " result:" << color::Yellow << i.value()();
//
//    return r;

//    Book::debug() << book::functions::endl << " As of " << book::functions::weekday << ", " << book::functions::monthname
//                  << ' ' << book::functions::day << ' ' << book::functions::year
//                  << ", xio project is still exploring its logistics. Thus, this option:" << book::functions::endl
//                  << color::Yellow << Arg.description << color::White << book::functions::endl
//                  << "has been unimplemented ...Temporary...";

    return Book::Enums::Code::Notimplemented;
}

Book::Enums::Code SppTest::CmdlineArgs(Core::Cmd::ArgumentData& Arg)
{
    Args.Usage();
    return Book::Enums::Code::Notimplemented;
}



Book::Enums::Code SppTest::Process(int argc, char **argv)
{
    Args.InputCmdLineData(argc,argv);
    return Args.Execute();

}

Book::Enums::Code SppTest::SetDrawAst(Core::Cmd::ArgumentData& Arg)
{

    return Book::Enums::Code::Notimplemented;
}




auto main(int argc, char** argv) -> int
{

    ::signal(SIGINT, SigInt);
    ::signal(SIGSEGV, SigFault);
    ::signal(SIGABRT, SigAbort);

    try
    {
        AppBook& Livre = AppBook::Init("XioDev.Book");
        AppBook::Open();
        Livre.Descriptions = R"(
<Icon:School; fg:Yellow>This is the development of the <fg:White>[xio & interpreter API explorations]<fg/>
------------------------------------------------------------------------
)";

        std::string head;
        Book::STMLText ml_description;
        auto c = ml_description << Livre.Descriptions >> head;
        if(c != Book::Enums::Code::Success)
            std::cerr << " text processing failed.";
        auto & SppDev= AppBook::CreateSection("xio.dev");
        SppDev.Open().CreateSectionContents("xio-EvalExpression-dev");
        /*auto & stackdev =*/
        Livre["xio.dev"]["xio-EvalExpression-dev"];

        AppBook::Out() << Book::Enums::Fn::Endl << head;
        SppTest TestApplication("testing the SppTest and the interpreter...");
        (void) TestApplication.Process(argc, argv);
    }
    catch(AppBook::Exception& be)
    {
        std::cerr << " Cauch Book::exception: " << be.what() << '\n';
        return 127;
    }
        /// xio and interpreter compiling stuff:
    catch(AppBook::Section::Contents::Element& bel)
    {
        std::cerr << " Caught event exception held in Book element contents file. Closing the Book now";
    }

    AppBook::Close();

    return 0;
}

