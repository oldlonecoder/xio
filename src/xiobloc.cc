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

#include "xio/xiobloc.h"


namespace xio
{

xiobloc::xiobloc()
{

}

xiobloc::xiobloc(const xiobloc&)
{

}

xiobloc::xiobloc(xiobloc* parent_, token_data* info_, alu* a_): xio(parent_,info_,a_)
{
}

xiobloc::xiobloc(xio* parent_, token_data* info_, alu* a_) :xio(parent_,info_,a_)
{
}




xiobloc::~xiobloc()
{
    if(!mem.xshared)
    {
        if(_instructions)
            _instructions->clear();
        if(_xiovars)
            _xiovars->clear();
        // if(_functions)
        //     for(auto* f : *_functions) delete f;
        // if(_structs)
        //     for(auto s :*_structs) delete s;
    }

    //...
}



/*!
    @brief deep copy the rhs xiobloc for duplicating its vars and sub-xiobloc stack content.
    At this level,  it is implicitely preparing generation of struct and function calls

    @note - Call this strickly for generate copy of stack such as function calls and other instanciation of xiobloc-based xiovars
*/

xiobloc& xiobloc::operator=(const xiobloc& rhs)
{
    mem.xshared = 1;
    if(rhs._xiovars)
    {
        _xiovars = new xiovar::list;
        *_xiovars = *rhs._xiovars;
    }
    _instructions = rhs._instructions;
    ///@todo Copy also the local function declarations // Needs precision on what I think
    ///@todo Copy also the local struct declarations   // Needs precision on what I think
    //...
    return *this;
}


bool xiobloc::operator!=(const xiobloc& other) const
{
    return true;
}


/*!
    @brief Creates a new xiovar inside this xiobloc scope.

    @return xiovar pointer or a book::book::rem::error/warning if the xiovar already exists in this xiobloc or in the parent scopes...
    @author @copy; 2022, Serge Lussier <oldlonecoder; bretzel> lussier.serge@gmail.com

    @note ( dev note: Make declaration rule syntax mandatory for creating any var, anywhere, thus prevent using new vars into middle of expression )
 */
xiovar* xiobloc::new_var(token_data* info_)
{
    book::rem::push_debug() << __LINE__ << ": xiobloc::new_var(" << color::Yellow << info_->text() << color::Reset << "):";
    auto* v = query_var(info_->text());
    if(!v)
    {
        book::rem::push_info() << __LINE__ << ": xiobloc::new_var(" << color::Yellow << info_->text() << color::Reset << ") new local variable.";
        if(!_xiovars) _xiovars = new xiovar::list;
        _xiovars->push_back(new xiovar(this, info_));
        xiovar* xv = _xiovars->back();
        xv->_index = _xiovars->size()-1;
        return xv;
    }
    xiovar* var = new xiovar(this,info_, v->_index, v->acc);
    book::rem::push_debug() << __LINE__ << ": xiobloc::new_var(" << color::Yellow << info_->text() << color::Reset << "): created new ref to variable.";
    return var;
}

book::rem::code xiobloc::detach(xio *x)
{
    auto r = xio::detach(x);
    if(r != book::rem::accepted) return r;

    if(_instructions)
    {
        auto xi = std::find(_instructions->begin(), _instructions->end(), x);
        if(xi != _instructions->end()) _instructions->erase(xi);
        if(_instructions->empty())
        {
            delete _instructions;
            _instructions = nullptr;
        }
        return book::rem::accepted;
    }

    if(_xiovars)
    {
        auto xi = std::find(_xiovars->begin(), _xiovars->end(), x);
        if(xi != _xiovars->end()) _xiovars->erase(xi);
        if(_xiovars->empty())
        {
            delete _xiovars;
            _xiovars = nullptr;
        }
        return book::rem::accepted;
    }

    //...
    return r;
}

book::rem::code xiobloc::append_instruction(xio *x)
{
    if(!_instructions)
        _instructions = new xio::list;
    _instructions->push_back(x);
    return book::rem::accepted;
}


book::rem::code xiobloc::instanciate()
{

    return book::rem::notimplemented;
}




xiovar *xiobloc::query_local_var(const std::string &id_)
{
    if(!_xiovars) return nullptr;
    for(auto * v : *_xiovars)  if(v) return v;
    return nullptr;
}


xiovar* xiobloc::query_var(const std::string& id_)
{
    auto *p = parent<xiobloc>();
    auto* e = query_local_var(id_);
    if(!e)
    {
        if(p) return p->query_var(id_);
    }

    return e;
}

alu xiobloc::jsr()
{
    if(!_instructions)
    {
        book::rem::push_warning() << " Executing xiobloc with no instructions. returning current alu value.";
        return *acc;
    }

    for(auto * i : *_instructions)
    {
        *acc = i->jsr();
        if(i->mem.leave)
            return *acc;
    }
    return *acc; ///< abritrary; if ever something as a xiobloc returns a value, or the callee needs this value;
}


}











