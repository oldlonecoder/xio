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

    book::cmd::cargs cmdargs;

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

    book::expect<> eval_expression(const book::cmd::argdata& arg);
    book::expect<> source_file(const book::cmd::argdata& arg);

    book::expect<> cmdline_invalid_args(const book::cmd::argdata& a);

};

}
