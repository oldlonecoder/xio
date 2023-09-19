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









/*!
 * \brief Argc::Argc Copy command-line arguments into the stracc::list container.
 * \param argc
 * \param argv
 */
amu::Argc::Argc(int argc, char** argv)
{

    for (int a = 1; a < argc; a++) args.push_back(argv[a]);
    arg = args.begin();
    //   book::rem::push_debug(HERE) << "count: " << color::Yellow << args.size() << book::rem::commit;
}



bool amu::Argc::operator >>(std::string_view& str)
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


std::string amu::Argc::usage()
{
    stracc str;
    str << "----------------------------------------\n";
    str << "-e \"expression\"\n";
    str << "-f [path]/source_file.s++ \n";
    str << "----------------------------------------\n";

    return str();
}


amu::Argc::~Argc()
{
    args.clear();
}

amu::amu(xiobloc* parent_bloc, const std::string& mname, int argc, char** argv): xiobloc(parent_bloc, mname)
{
    Arg = Argc(argc, argv);
}



amu::~amu()
{
}

}
