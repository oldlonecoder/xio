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
#include "Spp/Runtime/Amu.h"
#include "Spp/Runtime/RTObject.h"
#include <AppBook/Util/CArgs.h>
#include "Spp/Styles/ColorScheme.h"


namespace Spp
{
/**
 * \brief  Amu-Interpreter  ( I know, it's wierd but it is what it is... )
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


class SPP_EXPORT Interpreter : public Amu
{
    Cmd::CArgs    Args;
    std::vector<std::string_view> CmdArgs;

    RTObject::Dictionary  RuntimeObjects;
    RTObject::Dictionary  RuntimeFunctions; ///< No matter if function or class method;

    Amu::Array         Units;
    static Interpreter* InterpreterInstance;



    ColorScheme Styles;
public:
    static Interpreter *Instance();
    static ColorScheme& ColorsStyle();
    Interpreter() = default;
    ~Interpreter() override;

    Interpreter(std::string _Id, int argc, char** argv);

    Book::Action ProcessArgs();
    Book::Action SourceFile(Cmd::ArgumentData& Arg);
    Book::Action Expression(Cmd::ArgumentData &);

    static std::string MarkToken(std::pair<SppToken::Iterator, SppToken::Iterator> const& BE, const SppToken& Token);

//    template<typename T> RTVariable<T>& Assign(T& V, const std::string& VarID)
//    {
//        if(GetLocalVariableById(VarID))
//            throw AppBook::Exception() [ AppBook::Except() << " Variable '" << VarID << "' already exists."];
//
//        LocalVariables.emplace_back(new Variable(this,nullptr,nullptr));
//        LocalVariables.back()->SetId(VarID);
//
//    }


    /*!
     * @brief Base runtime object;
     *
     * //...
     */
    class SPP_EXPORT RTBase : public Object
    {
        void *ClassInstance{nullptr};
        void *RTObject{nullptr};

    public:
        RTBase() = default;

        RTBase(Stack* Scope, const std::string& ID);
        RTBase(Stack* Scope, const std::string& ID, void* Inst, void* Obj);

        ~RTBase() override = default;
        virtual Alu Invoke(std::vector<Alu> Args) = 0;
    };


    /*!
     * @brief Runtime Function.
     * @tparam Class
     */
    template<typename Class> class RTFunction : RTBase
    {
    public:
        using Dictionary = std::map<std::string, RTBase*>;
        using ParamType = Alu::Array;
        using RTFunctionPtr = Alu(Class::*)(typename RTFunction::ParamType);
        typename RTFunction::RTFunctionPtr FunctionPtr{nullptr};

        RTFunction() = default;

        RTFunction(Stack* Scope, const std::string& ID, Class* ObjInst, RTFunction::ParamType FPtr)
            : RTBase(Scope, ID,ObjInst, FPtr){}

        ~RTFunction() override = default;


        Alu Invoke(std::vector<Alu> Args) override
        {

            return {};
        }


        Alu operator << (std::vector<Alu> Args)
        {
            return {};
        }

        Alu operator >> (std::vector<Alu>& Args)
        {
            return {};
        }



        /*!
            @brief rt calls interpreter script source function.

        Ret operator()(const Params_ &...args)
        {
            auto param = [](auto A) {
                return Alu(A);
            };
            Alu::Array Params = {param(args)...};
            // alu a = call_script_function(_name, params);
            // return a.value<R>();
            return Ret();
        }

        template<std::size_t ... Is> Alu Accumulate(Alu::Array const &params, std::index_sequence<Is...> const &)
        {
            return (RTInstance->*FunctionPtr)(params[Is].value<Params_>()...);
        }

        Alu operator()(const Alu::Array &params) override
        {
            for (auto &a: params)
            {
                AppBook::Debug() << "arg: [" << Color::Yellow << a() << Color::Reset << "]";
                //...
            }
            return Accumulate(params, std::index_sequence_for<Params_...>{});
        }

        template<std::size_t ... Is> Alu Accumulate(Alu::Array &params, std::index_sequence<Is...> const &)
        {
            return (RTInstance->*FunctionPtr)(params[Is].value<Params_>()...);
        }
    */
    };



};

} // Spp


