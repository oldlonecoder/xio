#pragma once
#include <xio/xiobloc.h>
#include <xio/compiler/compiler.h>

namespace xio
{


class XIO_PUBLIC module_unit : public xiobloc
{
    std::string      source;
    std::string      filename;
    token_data::list tokens_stream;
    compiler*        cc { nullptr };

public:
    struct Argc
    {

        stracc::list           args;
        stracc::list::iterator arg;
        compiler*              acc { nullptr };
    public:
        Argc(compiler* ac, int argc, char** argv);
        Argc();
        ~Argc();


        bool operator >> (std::string_view& str);
        void reset() { arg = args.begin(); }

        book::rem::code process();
        std::string     usage();

    };
};

}