#pragma once
#include <xio/xiobloc.h>
#include <xio/compiler/compiler.h>
#include <logbook/cargs.h>

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

    book::cmd::cargs<amu> cmdargs;

    token_data::list tokens_stream;
    compiler*        cc   { nullptr };
    amu*             next { nullptr };
    amu*             prev { nullptr };
public:



    amu() = default;
    amu(xiobloc* parent_bloc, const std::string& mname);
    
    ~amu() override;
    book::rem::code open_file();
    book::rem::code compile();

    virtual book::rem::code process_cmdline(int arc, char **argv);
    stracc export_expr_ast(const std::string& expr);
private:

    rem::code eval_expression(std::string_view expr_str);
    rem::code source_file(std::string_view src);

    rem::code cmdline_invalid_args(std::string_view arg);

};

}
