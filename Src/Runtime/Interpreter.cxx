//
// Created by oldlonecoder on 11/2/23.
//

#include "Spp/Runtime/Interpreter.h"
#include "Spp/Compiler/Lang/Grammar.h"

namespace Spp
{

Interpreter* Interpreter:: InterpreterInstance = nullptr;


Interpreter::~Interpreter()
{
    CmdArgs.clear();
}

Interpreter::Interpreter(std::string _Id, int argc, char **argv): Unit(nullptr, std::move(_Id))
{
    AppBook::Message() ;
    if(Interpreter::Instance()) throw AppBook::Exception("Cannot instantiate more than on Interpreter which is a singleton class.");
    for(int c = 1; c<argc; c++) CmdArgs.emplace_back(argv[c]);

    Interpreter::InterpreterInstance = this;
}

Interpreter *Interpreter::Instance()
{
    return Interpreter::InterpreterInstance;
}



Book::Result Interpreter::SourceFile(Cmd::ArgumentData &Arg)
{
    AppBook::Debug() << "Command '" << Color::Yellow << Arg.Name << Color::Reset <<  "':" << Color::CadetBlue2 <<  Arg.Arguments[0] << Color::Reset << ":";
    _Filename = Arg.Arguments[0];
    auto R = LoadSource();
    if(R != Book::Result::Accepted)
    {
        AppBook::Error() << " Failed to load source.";
        return R;
    }
    AppBook::Message() << " Source Loaded :";
    AppBook::Out() << Color::White << "----------------------------------------------------------------------------------------";
    AppBook::Out() << Color::CornflowerBlue << _Source;
    AppBook::Out() << Color::White << "----------------------------------------------------------------------------------------";

    AppBook::Info() << " Building embedded Spp Grammar...";
    Lang::Grammar Gr;
    if(R = Gr.Build(); R != Book::Result::Accepted)
        throw AppBook::Exception()[AppBook::Except() << "Error building embedded Spp Grammar :" << R];

    AppBook::Debug() << " Dump Grammar:";
    Gr.Dump();


    return Book::Result::Notimplemented;
}


Book::Result Interpreter::ProcessArgs()
{
    AppBook::Debug() << " Configuring command line arguments:";
    (Args << Cmd::ArgumentData{"Compile Source","-c","--compile","Compile given [s++] script source File.",1}).Connect(this, &Interpreter::SourceFile);
    (Args << Cmd::ArgumentData{"Eval Expression","-e","--eval","Evaluate Expression",1}).Connect(this, &Interpreter::Expression);
    AppBook::Debug() << ":" << Book::Fn::Endl << Args.Usage();
    //auto& A = Args["Compile Source"];
    Args.ProcessStringArray(CmdArgs);

    return Args.Execute();
}

Book::Enums::Code Interpreter::Expression(Cmd::ArgumentData &A)
{
    AppBook::Message() << Book::Result::Notimplemented;
    return Book::Result::Ok;
}


} // Spp