
/******************************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                                        *
 *   serge.lussier@oldlonecoder.club                                                      *
 *                                                                                        *
 *                                                                                        *
 *   Unless otherwise specified, all code in this project is written                      *
 *   by the author (Serge Lussier)                                                        *
 *   and no one else then not even {copilot, chatgpt, or any other AI calamities}         *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge Lussier also apply here                     *
 *   Open source FREE licences also apply to the code from the same author (Serge Lussier)*
 *   ----------------------------------------------------------------------------------   *
 ******************************************************************************************/


#pragma once
#include "xio/xiovar.h"


using book::rem;


namespace xio
{

class  rbase : public xio
{

public:
    using dictionary_t = std::map<std::string, rbase*>;
    rbase() : xio(){}
    rbase(xio* a_parent, const std::string& a_id) : xio(a_parent, 0, 0)
    {
        set_id(_id);
    }

    ~rbase() override;

    std::string name() { return id(); }
    virtual alu operator()(const alu::list& args) = 0;

};

template<typename class_, typename ret_, typename ...params_> class rtf : public rbase
{
    class_*  rt_obj = nullptr;
public:
    using rtf_ptr_t = ret_(class_::*)(params_...);
private:
    typename rtf::rtf_ptr_t rtf_ptr = nullptr;
    rtf() = default;
    rtf(xio* a_parent, const std::string& a_name, class_& rt_inst, rtf_ptr_t a_fn) : rbase(a_parent, a_name), rt_obj(&rt_inst), rtf_ptr(a_fn) {}
    ~rtf();
    /*!
        @brief rt calls interpreter script source function.
    */
    ret_ operator()(const params_& ...args) {
        auto param = [](auto a) {
//                logdebugpfn << " a= '" << logger::Yellow << a << ":\n";
            return alu(a);
        };
        alu::list params = { param(args)... };
        // alu a = call_script_function(_name, params);
        // return a.value<R>();
        return ret_();
    }

    template <std::size_t ... Is> alu accumulate(alu::list const& params, std::index_sequence<Is...> const&){
        return (rt_obj->*rtf_ptr)(params[Is].value<params_>()...);
    }

    alu operator()(const alu::list& params) override{
        //          logdebug << " xio_stack::rt_function(const alu::list_t&):  " << Ends;
        for (auto& a : params)
        {
            rem::push_debug() << "arg: [" << color::Yellow << a() << color::Reset << "]";
           //...
        }
        return accumulate(params, std::index_sequence_for<params_...>{});
    }

    template <std::size_t ... Is> alu accumulate(alu::list& params, std::index_sequence<Is...> const&){
        return (rt_obj->*rtf_ptr)(params[Is].value<params_>()...);
    }

};

class XIO_PUBLIC xiobloc : public xio
{

public:
    using list = std::vector<xiobloc*>;
protected:

    xio::list* _instructions{nullptr};
    xiovar::list*   _xiovars{nullptr}; ///< Represent the stack bloc.
    // xiobloc::list* _functions = nullptr; ///< ...
    // xiobloc::list* _structs = nullptr;
    friend class function_descriptor;
    friend class function_call;
public:

    xiobloc();
    xiobloc(xiobloc* parent_, token_data* info_ = nullptr, alu* a_ = nullptr);
    xiobloc(xio* parent_, token_data* info_ = nullptr, alu* a_ = nullptr);
    xiobloc(xiobloc* parent_, const std::string& bname);
    /**
     * Copy constructor
     *
     * @param other TODO
     */
    xiobloc(const xiobloc& other);

    /**
     * Destructor
     */
    ~xiobloc() override;

    /**
     * Assignment operator
     *
     * @param other TODO
     * @return TODO
     */
    xiobloc& operator=(const xiobloc& other);

    /**
     * @todo write docs
     *
     * @param other TODO
     * @return TODO
     */
    bool operator!=(const xiobloc& other) const;

    alu jsr() override;
    xiovar* query_var(const std::string& id_);
    xiovar* query_local_var(const std::string& id_);

    //xiovar* ref_var(xio* var_); ///< ?? When parsing (compiling) the xio node is created (...or not?)
    xiovar* new_var(token_data* info_); ///< ?? When parsing (compiling) the xio node is created (...or not?)

    rem::code detach(xio* x) override;
    rem::code append_instruction(xio* x);

    virtual rem::code instanciate(); ///< Strictly specific to instanciation from the execution of the interpreter.

};



}
