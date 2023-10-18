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
    friend class spp::interpretr;
protected:
    std::string      source_content;
    std::string      filename;

    token_data::list tokens_stream;
    compiler*        cc   { nullptr };
    amu*             next { nullptr };
    amu*             prev { nullptr };
public:



    amu() = default;
    amu(xiobloc* parent_bloc, const std::string& mname);
    
    ~amu() override;
    book::code open_file();
    book::code compile();

    stracc export_expr_ast(const std::string& expr);
private:

    book::code eval_expression(std::string_view expr_str);
    book::code source_file(std::string_view src);

    book::code cmdline_invalid_args(std::string_view arg);

};

}
