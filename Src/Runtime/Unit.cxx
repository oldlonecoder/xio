//
// Created by oldlonecoder on 11/2/23.
//

#include "Spp/Runtime/Unit.h"
//#include "Spp/Compiler/Compiler.h"

namespace Spp
{


Unit::~Unit()
{
    _Source.clear();
    _Tokens.clear();
}

Unit::Unit(Stack *_Parent, std::string _Id): Stack(_Parent, nullptr,nullptr)
{
    AppBook::Debug() << " New Unit['" << Core::Color::Yellow << Id() << Core::Color::Reset << "'] Created.";
    SetId(_Id);

}

Book::Enums::Code Unit::Compile()
{
    if(_Source.empty())
        return Book::Enums::Code::Empty;

    Compiler CC(this, _Source.c_str());

    return Book::Enums::Code::Ok;
}

Alu Unit::JSR()
{
    return Stack::JSR();
}

Book::Enums::Code Unit::LoadSource()
{

    std::ifstream in;
    in.open(_Filename);
    if (in.is_open())
    {
        char  Line[256];
        while (!in.eof())
        {
            in.getline(Line, 255);
            _Source += Line;
            _Source += '\n';
        }
        in.close();
    }
    else
    {
        AppBook::Error() << '\'' << Core::Color::Yellow << _Filename << Core::Color::White <<'\'' << Core::Color::White << " " << strerror(errno);
        return Book::Enums::Code::Failed;
    }

    return Book::Enums::Code::Accepted;
}


} // Spp