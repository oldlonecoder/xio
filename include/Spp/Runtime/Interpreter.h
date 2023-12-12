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
#include "Spp/Runtime/RTObject.h"
#include <AppBook/Util/CArgs.h>



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
    Cmd::CArgs    Args;
    std::vector<std::string_view> CmdArgs;

    RTObject::Dictionary  RuntimeObjects;
    RTObject::Dictionary  RuntimeFunctions; ///< No matter if function or class method;

    Unit::Array         Units;
    static Interpreter* InterpreterInstance;


    struct RuntimeObject: public Util::Object
    {
        using Map = std::map<std::string, RuntimeObject*>;

        void *RTO{nullptr};
        RuntimeObject(Interpreter* Inter, void* RTObj): Util::Object(Inter, Id() + "Node"), RTO(RTObj){}
        virtual ~RuntimeObject() = default;

    };

    template<typename CType, typename ...Args> struct RuntimeFunction : RuntimeObject
    {
        using Function = Alu(CType::*)(Args...);
        //...
    };


public:
    static Interpreter *Instance();

    Interpreter() = default;
    ~Interpreter() override;

    Interpreter(std::string _Id, int argc, char** argv);

    Book::Action ProcessArgs();

    Book::Action SourceFile(Cmd::ArgumentData& Arg);


    Book::Action Expression(Cmd::ArgumentData &);
};

} // Spp


