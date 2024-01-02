//
// Created by oldlonecoder on 11/2/23.
//

#include "Spp/Runtime/Amu.h"
//#include "Spp/Compiler/Compiler.h"

namespace Spp
{


Amu::~Amu()
{
    _Source.clear();
    _Tokens.clear();
}

Amu::Amu(Stack *_Parent, std::string _Id): Stack(_Parent, nullptr, nullptr)
{
    AppBook::Debug() << " New Amu['" << Color::Yellow << Id() << Color::Reset << "'] Created.";
    SetId(_Id);

}

Book::Enums::Code Amu::Compile()
{
    if(_Source.empty())
        return Book::Enums::Code::Empty;

    Compiler CC;
    CC.Config() = { _Source, &_Tokens, this };


    if(auto R = CC.ExecuteLexer(); R != Book::Result::Success)
    {
        AppBook::Debug() << "Lex rejected";

        return Book::Result::Rejected;
    }

    return CC();
}

Alu Amu::JSR()
{
    return Stack::JSR();
}

Book::Enums::Code Amu::LoadSource()
{
    char  Line[256];
    std::ifstream in;
    in.open(_Filename);
    if (!in.is_open())
    {
        AppBook::Error() << '\'' << Color::Yellow << _Filename << Color::White <<'\'' << Color::White << " " << strerror(errno);
        return Book::Enums::Code::Failed;
    }

    while (!in.eof())
    {
        in.getline(Line, 255);
        _Source += Line;
        _Source += '\n';
    }
    in.close();
    return Book::Enums::Code::Accepted;
}


} // Spp