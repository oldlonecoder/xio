
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
#include <Spp/xio.h>
#include <AppBook/Book/AppBook.h>

namespace Spp
{



class SPP_EXPORT RTBase : public xio
{
public:
    using Dictionary = std::map<std::string, RTBase *>;

    RTBase() : xio()
    {}

    RTBase(xio *a_parent, const std::string &a_id) : xio(a_parent, nullptr, nullptr)
    {
        SetId(a_id);
    }

    ~RTBase() override;

    std::string Name(){ return Id(); }

    virtual Alu operator()(const Alu::Array &args) = 0;
};



template<typename Class, typename Ret, typename ...Params_> class RTFunction : public RTBase
{
    Class *rt_obj = nullptr;
public:
    using rtf_ptr_t = Ret(Class::*)(Params_...);

    typename RTFunction::rtf_ptr_t rtf_ptr = nullptr;

    RTFunction() = default;

    RTFunction(xio *a_parent, const std::string &a_name, Class &rt_inst, rtf_ptr_t a_fn)
        : RTBase(a_parent, a_name), rt_obj(&rt_inst), rtf_ptr(a_fn)
    {}

    ~RTFunction() override = default;

    /*!
        @brief rt calls interpreter script source function.
    */
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

    template<std::size_t ... Is> Alu accumulate(Alu::Array const &params, std::index_sequence<Is...> const &)
    {
        return (rt_obj->*rtf_ptr)(params[Is].value<Params_>()...);
    }

    Alu operator()(const Alu::Array &params) override
    {
        for (auto &a: params)
        {
            AppBook::Debug() << "arg: [" << Core::Color::Yellow << a() << Core::Color::Reset << "]";
            //...
        }
        return accumulate(params, std::index_sequence_for<Params_...>{});
    }

    template<std::size_t ... Is> Alu accumulate(Alu::Array &params, std::index_sequence<Is...> const &)
    {
        return (rt_obj->*rtf_ptr)(params[Is].value<Params_>()...);
    }

};



}