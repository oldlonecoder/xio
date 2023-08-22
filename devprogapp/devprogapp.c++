#include <xio/compiler/parser.h>


using book::rem;

auto main(int argc, char** argv) -> int
{
 
    xio::cc::parser parser;

    rem::push_message() << rem::success << "\\O/ !!!";

    rem::clear(nullptr);
    return 0;
}