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

Interpreter::Interpreter(std::string _Id, int argc, char **argv): Amu(nullptr, std::move(_Id))
{
    AppBook::Message() ;
    if(Interpreter::Instance()) throw AppBook::Exception("Cannot instantiate more than one Interpreter.");
    for(int c = 1; c<argc; c++) CmdArgs.emplace_back(argv[c]);

    Interpreter::InterpreterInstance = this;
}

Interpreter *Interpreter::Instance()
{
    return Interpreter::InterpreterInstance;
}



Book::Action Interpreter::SourceFile(Cmd::ArgumentData &Arg)
{
    AppBook::Debug() << "Command '" << Color::Yellow << Arg.Name << Color::Reset <<  "':" << Color::CadetBlue2 <<  Arg.Arguments[0] << Color::Reset << ":";
    _Filename = Arg.Arguments[0];
    auto R = LoadSource();
    if(R != Book::Result::Accepted)
    {
        AppBook::Error() << " Failed to load source.";
        return Book::Action::End;
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

    auto Code = Amu::Compile();
    if(Code != Book::Result::Success)
    {
        AppBook::Error() << " Failed to Compile and execute the source";
        return Book::Action::End;
    }
    if(!Instructions.empty())
    {
        Alu A = Amu::JSR();
        AppBook::Out() << A();
    }
    else
    {
        AppBook::Status() << Book::Class::Error;
    }
    return Book::Action::Continue;
}



Book::Action Interpreter::ProcessArgs()
{
    AppBook::Debug() << " Processing/Executing command line arguments:";
    (Args << Cmd::ArgumentData{"Compile Source","-c","--compile","Compile given [s++] script source File.",1}).Connect(this, &Interpreter::SourceFile);
    (Args << Cmd::ArgumentData{"Eval Expression","-e","--eval","Evaluate Expression",1}).Connect(this, &Interpreter::Expression);
    //auto& A = Args["Compile Source"];
    Args.ProcessStringArray(CmdArgs);
    AppBook::Debug() << ":" << Book::Fn::Endl << Args.Usage();
    return Args.Execute();
}

Book::Action Interpreter::Expression(Cmd::ArgumentData &A)
{
    AppBook::Message() << Book::Result::Notimplemented;
    return Book::Action::End;
}

std::string Interpreter::MarkToken(const std::pair<SppToken::Iterator, SppToken::Iterator> &BE, const SppToken& Token)
{
    auto It = BE.first;
    size_t offset = 0;
    std::string Line = Token.TextLine();
    std::string _color;
    uint8_t U=0;
    auto& Scheme = Interpreter::ColorsStyle();
    for (; It != BE.second; It++)
    {
        if(It->Flags.U && (It->Location.Colnum < Token.Location.Colnum)) ++U;
        _color = It->M == Mnemonic::Noop ? Color::Ansi(Lexer::PrimitiveTypesColors[It->T]) :
                 Color::Ansi(Lexer::MnemonicColors[It->M]);

//        _color = start_token->M == Mnemonic::Noop ? Scheme[start_token->T] : Scheme[start_token->M];
//
        if (!_color.empty())
        {
            Line.insert(It->Location.Colnum - 1 + offset, _color);
            offset += _color.length();
        }
    }
    StrAcc txt = Line;

    Line.clear();
    txt << Color::Reset << " ["
        << Spp::MnemonicName(Token.M) << "; "
        << Token.LocationText() << " ; "
        << Token.TypeName() << " ; "
        << Token.SemanticText() << "]";
    txt << '\n';
    txt.fill(0x20, Token.Location.Colnum - 1 - U);
    txt << Utf::Glyph::ArrowUp;
    return txt();
}

ColorScheme &Interpreter::ColorsStyle()
{
    auto* Ins = Instance();
    if(!Ins)
        throw AppBook::Exception() [AppBook::Fatal() << Book::Result::Null_ptr << "[Interpreter instance is null]"];
    return Ins->Styles;
}


Interpreter::RTBase::RTBase(Stack *Scope, const string &ID)
    : Object(Scope, ID){}

Interpreter::RTBase::RTBase(Stack *Scope, const string &ID, void *Inst, void *Obj)
    : Object(Scope, ID), ClassInstance(Inst),RTObject(Obj){}




} // Spp