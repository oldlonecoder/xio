//
// Created by oldlonecoder on 11/1/23.
//

#include "Spp/Runtime/Variable.h"

namespace Spp
{
Variable::Variable(): xio(){}

Variable::Variable(Core::Object *_Parent, SppToken *_Token, Alu *_A) : xio(_Parent, _Token, _A){}

Variable::~Variable(){}

void Variable::Push()
{
    _Stack.push(*A);
}

void Variable::Pop()
{
    if(!_Stack.empty())
    {
        *A = _Stack.top();
        _Stack.pop();
    }
}



}