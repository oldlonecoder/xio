//
// Created by oldlonecoder on 11/1/23.
//

#include "Spp/Stack.h"

using Core::Object;
using Core::Color;

namespace Spp
{




Stack::Stack()
{

}

Stack::Stack(const std::string &_Id): xio(nullptr, nullptr, nullptr)
{
    SetId(_Id);
}

Stack::Stack(Core::Object *_Parent, const std::string &_Id) : xio(_Parent, nullptr, nullptr)
{
    SetId(_Id);
}

Stack::Stack(Stack *_Parent, SppToken *_Token, Alu *A): xio(_Parent, _Token, A)
{
    if(_Token)
        SetId(_Token->Text());

}

Stack::~Stack()
{
    LocalVariables.clear();
    Instructions.clear();

    if(!Mem.Ref)
    {
        if(!Instructions.empty())
        {
            //...
            Instructions.clear();
        }
        if(!LocalVariables.empty())
        {
            //...
            LocalVariables.clear();
        }
//        if(LocalFunctions)
//        {
//            LocalFunctions->clear();
//            delete LocalFunctions;
//            LocalFunctions = nullptr;
//        }
    }
}


Stack &Stack::operator=(const Stack &SIO)
{
    Mem.Ref = 1;
    LocalVariables = SIO.LocalVariables;
    Instructions = SIO.Instructions;
    //LocalFunctions = SIO.LocalFunctions; /// LocalFunctions are Function bloc declarations.

    return *this;
}


Alu Stack::JSR()
{
    if(Instructions.empty())
    {
        AppBook::Warning() << " --> Executing Bloc/Stack that has no Instructions - Returning Alu' s current value";\
        return *A;
    }
    for(auto* I: Instructions)
    {
        xio* SI = I->As<xio>();
        *A = SI->JSR();
        if(Mem.LV) return *A;
    }
    return *A;
}

Variable *Stack::NewVar(SppToken *_Token)
{
    AppBook::Debug() << ": ...NewVar(" << Color::Yellow << SToken->Text() << Color::Reset << "):";
    auto* v = GetVariableById(SToken->Text());
    if(!v)
    {
        AppBook::Info()  << ": NewVar(" << Color::Yellow << SToken->Text() << Color::Reset << ") new local variable.";
        LocalVariables.push_back(new Variable(this, SToken));
        Variable* xv = LocalVariables.back()->As<Variable>();
        xv->_StackIndex = LocalVariables.size()-1;
        return xv;
    }
    Variable* var = new Variable(this, SToken, v->A);
    var->_StackIndex = v->_StackIndex;

    AppBook::Debug() << "NewVar(" << Color::Yellow << SToken->Text() << Color::Reset << "): created new ref to variable.";
    return var;
}

Variable *Stack::GetLocalVariableById(const std::string &_ID)
{
    if(LocalVariables.empty()) return nullptr;
    for(auto* V : LocalVariables)
    {
        if(V)
            if(V->Id() == _ID) return V;
    }
    return nullptr;
}

Variable *Stack::GetVariableById(const std::string &_ID)
{
    auto * Var = GetLocalVariableById(_ID);
    if(Var) return Var;
    auto* Par = Parent<Stack>();
    if(Par) return Par->GetVariableById(_ID);

    return nullptr;
}

Book::Enums::Code Stack::RemoveInstruction(xio *_Obj)
{
    if(Instructions.empty()) return Book::Enums::Code::Rejected;

    auto It = std::find(Instructions.begin(), Instructions.end(),_Obj);
    if(It != Instructions.end()) Instructions.erase(It);
    return Book::Enums::Code::Accepted;
}

Book::Enums::Code Stack::RemoveVariable(Variable *_Obj)
{
    if(LocalVariables.empty()) return Book::Enums::Code::Rejected;

    auto It = std::find(LocalVariables.begin(), LocalVariables.end(),_Obj);
    if(It != LocalVariables.end()) LocalVariables.erase(It);
    return Book::Enums::Code::Accepted;
}

//Book::Enums::Code Stack::RemoveFunction(xio *_Obj)
//{
//    if(!LocalFunctions) return Book::Enums::Code::Rejected;
//    auto It = std::find(LocalFunctions->TreeBegin(), LocalFunctions->end(),_Obj);
//    if(It != LocalFunctions->end()) LocalFunctions->erase(It);
//
//    return Book::Enums::Code::Ok;
//}

Book::Enums::Code Stack::AppendInstruction(xio *_In)
{
    Instructions.push_back(_In);
    return Book::Enums::Code::Accepted;
}

Stack::Stack(const Stack & Rhs): xio(nullptr, Rhs.SToken, Rhs.A)
{
    if(!Rhs.LocalVariables.empty())
    {
        LocalVariables = Rhs.LocalVariables;
        for(auto* V  : LocalVariables)
        {
            if(V)
            {
                V->Push();
                V->_OwnerBloc = this; // Your Souls are belong to us. [The Borg]
            }
        }
        Instructions = Rhs.Instructions;
        //LocalFunctions = Rhs.LocalFunctions; // On Hold...
    }


    Instructions = Rhs.Instructions;
}


}