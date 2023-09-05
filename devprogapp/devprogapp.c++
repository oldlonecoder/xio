#include <xio/s++/spp.h>
#include <signal.h>

using book::rem;
using xio::spp::interpretr;









void sig_int( int s )
{
    rem::push_interrupted() << " dump messages stream and exit:";
    rem::clear([](rem& r){ std::cerr << r.cc(); });
    std::cerr << " sig interrupt caugh...\n";
    exit(3);
}

void sig_fault( int s)
{
    rem::push_segfault() << " dump messages stream and exit:";
    rem::clear([](rem& r){ std::cerr << r.cc(); });
    std::cerr << " sigfault caught...\n";
    exit(127);
}

void sig_abort( int s)
{
   rem::push_aborted() << " dump messages stream and exit:";
   rem::clear([](rem& r){ std::cerr << r.cc(); });
   std::cerr << s << '\n';

  exit(127);
}



auto main(int argc, char** argv) -> int
{

    ::signal(SIGINT, sig_int);
    ::signal(SIGSEGV, sig_fault);
    ::signal(SIGABRT, sig_abort);

    interpretr i;
    //auto alu = i["z = ax + 4ac;"];
    auto alu = i["x == 42(3+4*8) return 0;"];

    rem::out() << " Result: " << color::Yellow << alu();
    rem::push_message() << rem::success << "\\O/ !!!";

    rem::clear(nullptr);
    return 0;
}
