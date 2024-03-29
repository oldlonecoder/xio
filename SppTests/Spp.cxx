#include <Spp/Runtime/Interpreter.h>


#include <AppBook/Book/AppBook.h>
#include <AppBook/Book/StmlText.h>

#include <csignal>


void SigInt(int s )
{
    //rem::push_interrupted() << " Dump messages stream and exit:";
    //rem::clear(nullptr);
    std::cerr << " sig interrupt caugh...\n";
    AppBook::Close();
    exit(3);
}

void SigFault(int s)
{
    //rem::push_segfault() << " Dump messages stream and exit:";
    //rem::clear(nullptr);
    std::cerr << " sigfault caught...\n";
    AppBook::Close();
    exit(127);
}

void SigAbort(int s)
{
    //rem::push_aborted() << " Dump messages stream and exit:";
    //rem::clear(nullptr);
    std::cerr << s << '\n';
    AppBook::Close();
    exit(127);
}

// --------------------------------------------------------------------------





auto main(int argc, char** argv) -> int
{

    ::signal(SIGINT, SigInt);
    ::signal(SIGSEGV, SigFault);
    ::signal(SIGABRT, SigAbort);

    try
    {
        AppBook& Livre = AppBook::Init("SppTests");
        AppBook::Open();
        Livre.Descriptions = 
R"(
<Icon:School; fg:Yellow>This is the development of the <fg:White>[xio & interpreter API explorations]<fg/>
------------------------------------------------------------------------
)";

        std::string head;
        Book::STMLText ml_description;
        auto c = ml_description << Livre.Descriptions >> head;
        if(c != Book::Enums::Code::Success)
            std::cerr << " text processing failed.";
        auto & SppDev= AppBook::CreateSection("xio.dev");
        SppDev.Open().CreateSectionContents("Interpreter");

        Livre["xio.dev"]["Interpreter"];

        AppBook::Out() << Book::Enums::Fn::Endl << head;
        Spp::Interpreter Interpreter("testing the SppTest and the interpreter...", argc, argv);
        auto R =  Interpreter.ProcessArgs();
        if(R != Book::Action::Continue)
            AppBook::Message() << " Interpreter tests failed...";

    }
    catch(AppBook::Exception& be)
    {
        std::cerr << " Caught Book::exception: " << be.what() << '\n';
        AppBook::Close();
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

