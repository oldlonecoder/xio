/***************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                         *
 *   serge.lussier@oldlonecoder.club                                       *
 *                                                                         *
 *                                                                         *
 *   Unless otherwise specified, all code in this project is written       *
 *   by the author (Serge Lussier)                                         *
 *   and no one else then not even {copilot, chatgpt, or any other AI}     *
 *   --------------------------------------------------------------------- *
 *   Copyrights from authors other than Serge Lussier also apply here      *
 ***************************************************************************/

#pragma once
#include "Spp/Runtime/Unit.h"
#include "Spp/Runtime/RTBase.h"
#include <AppBook/Core/CArgs.h>



namespace Spp
{
/**
 * \brief  Unit-Interpreter  ( I know, it's wierd but it is wht it is... )
 *
 *
 * \author &copy; 2023, Serge Lussier; oldlonecoder (serge.lussier@oldlonecoder.club)
 *
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
*/


class SPP_EXPORT Interpreter : public Unit
{
    Core::Cmd::CArgs    Args;
    std::vector<std::string_view> CmdArgs;

    RTBase::Dictionary  RuntimeObjects;
    RTBase::Dictionary  RuntimeFunctions; ///< No matter if function or class method;

    Unit::Array         Units;
    static Interpreter* InterpreterInstance;

public:
    static Interpreter *Instance();

public:

    Interpreter() = default;
    ~Interpreter() override;

    Interpreter(std::string _Id, int argc, char** argv);

    Book::Result ProcessArgs();

    Book::Result SourceFile(Core::Cmd::ArgumentData& Arg);


    Book::Enums::Code Expression(Core::Cmd::ArgumentData &);
};

} // Spp

