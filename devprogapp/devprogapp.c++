#include <xio/s++/spp.h>


using book::rem;
using xio::spp::interpretr;

auto main(int argc, char** argv) -> int
{
 
    
    interpretr i;
    auto alu = i["z = ax + 4ac;"];

    rem::push_message() << rem::success << "\\O/ !!!";

    rem::clear(nullptr);
    return 0;
}