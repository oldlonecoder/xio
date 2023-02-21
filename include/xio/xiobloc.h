/***************************************************************************
*   Copyright (C) 2005/2023 by Serge Lussier                              *
*   2005: (bretzel@teaccweb.homelinux.net)                                  *
*   2023: lussier.serge@gmail.com, oldlonecoder@gmail.com                 *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#pragma once
#include <xio/xiovar.h>

namespace xio
{

class  rbase : public xiobject
{

    std::string _id;

public:
    using dictionary_t = std::map<std::string, rbase*>;
    rbase() : xiobject(){}
    rbase(xiobject* a_parent, const std::string& a_id) : xiobject(a_parent, 0, 0), _id(a_id) {}
    ~rbase() override;

    std::string& name() { return _id; }
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
    rtf(xiobject* a_parent, const std::string& a_name, class_& rt_inst, rtf_ptr_t a_fn) : rbase(a_parent, a_name), rt_obj(&rt_inst), rtf_ptr(a_fn) {}
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
        for (auto& a : params) {
           diagnostic::debug() << "arg: [" << color::Yellow << a() << color::Reset << "]";
           //...
        }
        return accumulate(params, std::index_sequence_for<params_...>{});
    }

    template <std::size_t ... Is> alu accumulate(alu::list& params, std::index_sequence<Is...> const&){
        return (rt_obj->*rtf_ptr)(params[Is].value<params_>()...);
    }

};

class  xiobloc : public xiobject
{

public:
    using list = std::vector<xiobloc*>;
protected:

    xiobject::list* _instructions  = nullptr;
    xiovar::list*   _xiovars       = nullptr; ///< Represent the stack xiobloc.
    // xiobloc::list* _functions = nullptr; ///< ...
    // xiobloc::list* _structs = nullptr;

public:
    /**
     * Default constructor
     */
    xiobloc();
    xiobloc(xiobloc* parent_, token_data* info_ = nullptr, alu* a_ = nullptr);
    xiobloc(xiobject* parent_, token_data* info_ = nullptr, alu* a_ = nullptr);

    /**
     * Copy constructor
     *
     * @param other TODO
     */
    xiobloc(const xiobloc& other);

    /**
     * Destructor
     */
    ~xiobloc();

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
    xiovar* new_var(xiobject* var_); ///< ?? When parsing (compiling) the xio node is created (...or not?)
    xiovar* new_var(token_data* info_); ///< ?? When parsing (compiling) the xio node is created (...or not?)

    code::T detach(xiobject* x) override;


    virtual code::T instanciate(); ///< Strictly specific to instanciation from the execution of the interpreter.

};



}
