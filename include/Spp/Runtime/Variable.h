
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
#include <Spp/xio.h>
//#include <Spp/Runtime/RTObject.h>


namespace Spp
{

class SPP_EXPORT Variable : public xio
{
    int64_t _StackIndex = -1;
    Type::T _Type{Type::F32};

    friend class Stack;
    friend class xio;
    using VarStack = std::stack<Alu>;

    using Array = std::vector<Variable *>;

protected:
    xio* _OwnerBloc{nullptr};

    VarStack _Stack;

    void Push();
    void Pop();

public:

    Variable();

    Variable(Util::Object* _Parent, SppToken* _Token=nullptr, Alu* A=nullptr);



    ~Variable() override;
};

} // Spp

