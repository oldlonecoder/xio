#include <xio/s++/spp.h>
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



auto main(int argc, char** argv) -> int
{

    ::signal(SIGINT, sig_int);
    ::signal(SIGSEGV, sig_fault);
    ::signal(SIGABRT, sig_abort);

    interpretr i;
    try{
        //auto alu = i["x = 42/4 + 5(3+4*3/5+34) - 42 d = 12;"];
        auto alu = i["x = 42/(-4)! + 5(3+4*3/5+34) - 42 d = 12;"];
        rem::out() << " Result: " << color::Yellow << alu();
    }
    catch(book::rem & )
    {
        // no need to do thingss here ...
    }
    book::rem::clear();

    return 0;
}
