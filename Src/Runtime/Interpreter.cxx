//
// Created by oldlonecoder on 11/2/23.
//

#include "Spp/Runtime/Interpreter.h"

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



Book::Result Interpreter::SourceFile(Core::Cmd::ArgumentData &Arg)
{
    AppBook::Message() << "Command '" << Color::Yellow << Arg.Name << Color::Reset <<  "':" << Book::Result::Notimplemented;
    return Book::Result::Notimplemented;
}

Book::Result Interpreter::ProcessArgs()
{
    AppBook::Debug() << " Configuring command line arguments:";
    (Args << Core::Cmd::ArgumentData{"Compile Source","-c","--compile","Compile given [s++] script source File.",1}).Connect(this, &Interpreter::SourceFile);
    (Args << Core::Cmd::ArgumentData{"Eval Expression","-e","--eval","Evaluate Expression",1}).Connect(this, &Interpreter::Expression);
    AppBook::Debug() << ":" << Book::Fn::Endl << Args.Usage();
    //auto& A = Args["Compile Source"];
    Args.ProcessStringArray(CmdArgs);

    return Args.Execute();
}

Book::Enums::Code Interpreter::Expression(Core::Cmd::ArgumentData &A)
{
    AppBook::Message() << Book::Result::Notimplemented;
    return Book::Result::Ok;
}


} // Spp