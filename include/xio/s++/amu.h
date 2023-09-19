#pragma once
#include <xio/xiobloc.h>
#include <xio/compiler/compiler.h>

namespace xio
{



/**
 * \brief Accumulator Module Unit  ( I know, it's wierd but it is wht it is... )
 * 
 *  
 * \author &copy; 2023, Serge Lussier; oldlonecoder (serge.lussier@oldlonecoder.club)
 */
class XIO_PUBLIC amu : public xiobloc
{
protected:
    std::string      source_content;
    std::string      filename;
    token_data::list tokens_stream;
    compiler*        cc   { nullptr };
    amu*             next { nullptr };
    amu*             prev { nullptr };
public:

    struct Argc
    {
        using list = std::vector<std::string_view>;
        using iterator = Argc::list::iterator;
        Argc::list     args;
        Argc::iterator arg;

    public:
        Argc(int argc, char** argv);
        //Argc(amu* munit, stracc::list&& argc);

        Argc() = default;
        ~Argc();


        bool operator >> (std::string_view& str);
        void reset() { arg = args.begin(); }

        
        std::string     usage();

    };

    amu() = default;
    amu(xiobloc* parent_bloc, const std::string& mname, int argc, char** argv);
    
    ~amu() override;
    book::rem::code open_file();
    book::rem::code compile();
private:
    amu::Argc Arg;

};

}