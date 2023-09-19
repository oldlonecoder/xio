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
    compiler*        cc   { nullptr };
    module_unit*     next { nullptr };
    module_unit*     prev { nullptr };
public:
    struct Argc
    {

        stracc::list           args;
        stracc::list::iterator arg;
        module_unit*           mu { nullptr };
    public:
        Argc(module_unit* munit, int argc, char** argv);
        Argc(module_unit* munit, stracc::list&& argc);

        Argc() = default;
        ~Argc();


        bool operator >> (std::string_view& str);
        void reset() { arg = args.begin(); }

        book::rem::code process();
        std::string     usage();

    };

    module_unit() = default;
    module_unit(xiobloc* parent_bloc, const std::string& mname, int argc, char** argv);
    
    ~module_unit() override;


private:
    module_unit::Argc Arg;

};

}