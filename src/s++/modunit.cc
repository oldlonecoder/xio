#include "xio/s++/modunit.h"




namespace xio
{



/*!
 * \brief Argc::Argc Copy command--line arguments into the stracc::list container.
 * \param argc
 * \param argv

compiler::Argc::Argc(compiler* ac, int argc, char** argv) : acc(ac)
{
    for (int a = 1; a < argc; a++) args.push_back(argv[a]);
    arg = args.begin();
    //   book::rem::push_debug(HERE) << "count: " << color::Yellow << args.size() << book::rem::commit;
}

module_unit::Argc::Argc(module_unit* mu, stracc::list&& argc) :
{
}

compiler::Argc::Argc()
{

}

compiler::Argc::~Argc()
{
    args.clear();
}

bool compiler::Argc::operator >>(std::string_view& str)
{
    if (arg >= args.end())
    {
        str = str_code_none;
        return false;
    }
    str = *arg;
    ++arg;

    return arg <= args.end();
}


book::rem::code compiler::Argc::process()
{
    if (args.empty())
    {
        book::rem::push_error() << "expected arguments :" << book::rem::endl << usage() << book::rem::commit;
        return book::rem::rejected;
    }
    reset();
    //    std::string a;
    //    // debug:
    //    while((*this) >> a) book::rem::out() << a << book::rem::commit;

        // Switch:
    reset();
    if (*arg == "-e")
    {
        ++arg;
        if (arg < args.end())
        {
            acc->set_source(*arg);
            return acc->compile_expr(acc->bloc(), (*arg).c_str());
        }

        book::rem::push_error() << "expected expression text" << book::rem::endl << usage() << book::rem::commit;
        return book::rem::rejected;
    }
    if (*arg == "-f")
    {
        ++arg;
        if (arg < args.end())
        {
            book::rem::push_debug(HERE) << color::Reset << "compiling file '" << color::Aquamarine3 << *arg << color::Reset << "':" << book::rem::commit;
            acc->set_source_file(*arg);
            return acc->compile();
        }
        book::rem::push_error() << "-f requires [path]/filename as the source code." << book::rem::endl << usage() << book::rem::commit;

        return book::rem::rejected;
    }

    book::rem::push_error() << "expected arguments :" << book::rem::endl << usage() << book::rem::commit;
    return book::rem::rejected;
}

std::string compiler::Argc::usage()
{
    stracc str;
    str << "----------------------------------------\n";
    str << "-e \"expression\"\n";
    str << "-f [path]/source_file.s++ \n";
    str << "----------------------------------------\n";

    return str();
}

*/

module_unit::Argc::Argc(module_unit* mou, int argc, char** argv)
{
    for (int a = 1; a < argc; a++) args.push_back(argv[a]);
}

module_unit::Argc::Argc(module_unit* munit, stracc::list&& a):mu(munit), args(std::move(a))
{

}

module_unit::Argc::~Argc()
{
    args.clear();
}

module_unit::module_unit(xiobloc* parent_bloc, const std::string& mname, int argc, char** argv): xiobloc(parent_bloc, mname)
{
    Arg = Argc(this, argc, argv);
}

}
