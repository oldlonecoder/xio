//
// Created by oldlonecoder on 11/2/23.
//

#include "Spp/Compiler/Compiler.h"

namespace Spp
{
Compiler::Compiler(Stack *_RBloc, std::string_view _Src)
{
    Data.RootBloc = _RBloc;
    Data.SourceText = _Src;
}




/*!
 * @brief Execute compilation.
 * @return  Book::Result::Success or Book::Result::Failed.
 */
Book::Result Compiler::operator()()
{
//    if(!Experimentation::Grammar::IsBuilt())
//    {
//        AppBook::Error() << " Gammar rules not built yet!.";
//        return Book::Result::Failed;
//    }

    Ctx = {Data.Tokens.begin(), Data.Tokens.begin(), Data.Tokens.end(), Data.RootBloc};
    Ctx.Rule = Experimentation::Grammar()["Unit"];

    (void)CCUnit();

    return Book::Result::Notimplemented;
}

xio *Compiler::CCUnit()
{

    AppBook::Error() << " Compile Experimentation::Grammar::Rule[" << Core::Color::Yellow << Ctx.Rule->Id << Core::Color::Reset << "'] not implemented yet... ";
    return nullptr;
}


// ---------------- Compiler::Context: ----------------------------------------------
#pragma region Context

bool Compiler::ContextData::operator++()
{
    if(Cur >= EndStream) return false;
    ++Cur;
    return Cur < EndStream;
}

bool Compiler::ContextData::operator++(int)
{
    if(Cur >= EndStream) return false;
    ++Cur;
    return Cur < EndStream;
}

void Compiler::ContextData::Accept()
{
    StartSeq = EndSeq = Cur;
    InstructionsSeq.clear();
    if(Instruction) Bloc->AppendInstruction(Instruction);
    CurType = Type::Null;
}




void Compiler::ContextData::Reject()
{
    Cur = EndSeq = StartSeq; // No forward sequence ????
    for(auto* X: InstructionsSeq)
    {
        X->Detach(); // Remove from Stack's children Array
        delete X;
    }
    InstructionsSeq.clear();
    CurType = Type::Null;
}

Compiler::ContextData::ContextData(SppToken::Iterator BeginStream, SppToken::Iterator StartSeq, SppToken::Iterator EndStream, Stack *Bloc)
    : BeginStream(BeginStream), StartSeq(StartSeq), EndStream(EndStream), Bloc(Bloc)
{}


#pragma endregion Context
// ---------------- Compiler::Context: END-------------------------------------------


} // Spp