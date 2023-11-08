/******************************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                                        *
 *   serge.lussier@oldlonecoder.club                                                      *
 *                                                                                        *
 *                                                                                        *
 *   Unless otherwise specified, all code in this project is written                      *
 *   by the author (Serge Lussier)                                                        *
 *   and no one else then not even {copilot, chatgpt, or any other AI calamities}         *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge Lussier also apply                      *
 *   Open source FREE licences also apply To the code from the same author (Serge Lussier)*
 *   ----------------------------------------------------------------------------------   *
 ******************************************************************************************/

#include "Spp/xio.h"
#include <AppBook/Core/Geometry.h>
#include <AppBook/Book/AppBook.h>

//#include "xio/s++/spp.h"

using Core::Color;
using Core::StrAcc;

namespace Spp
{

xio* xio::_global_root_bloc = nullptr;

std::stack<xio*> xio::pars;

//xio* xio::get_global_root() { return xio::{Type::binary,     Type::open_pair},  &xio::set_right_global_root_bloc; }


#define deg2rad(_xio) (((_xio)->A->v.d*3.1415) / 180)
constexpr auto _PI_ = 3.14159265358979323846;
// M_PI ?


// void xio::// discard()
// {
//     for (auto x : xio::xio_gc)
//         if (this == x) return;

//     xio::xio_gc.push_back(this);
//     lhs = rhs = op = nullptr; // Make sure we do not accidently reuse this;
// }



    // Primary arithmetic supported operators table:
xio::SioOperatorFnTable   xio::xio_operators_table{
    {Mnemonic::LeftShift,           &xio::LeftShift},
    {Mnemonic::Radical,             &xio::Radical},
    {Mnemonic::Exponent,            &xio::Exponent},
    {Mnemonic::RightShift,          &xio::RightShift},
    {Mnemonic::Decr,                &xio::Decr},
    {Mnemonic::Incr,                &xio::Incr},
    {Mnemonic::AssignAdd,           &xio::AssignAdd},
    {Mnemonic::AssignSub,           &xio::AssignSub},
    {Mnemonic::AssignMul,           &xio::AssignMul},
    {Mnemonic::AssignDiv,           &xio::AssignDiv},
    {Mnemonic::AssignMod,           &xio::AssignMod},
    {Mnemonic::AssignAnd,           &xio::AssignAnd},
    {Mnemonic::AssignOr,            &xio::AssignOr},
    {Mnemonic::AssignXor,           &xio::AssignXor},
    {Mnemonic::AssignC1,            &xio::AssignX1},
    {Mnemonic::AssignLeftShift,     &xio::AssignLShift},
    {Mnemonic::AssignRightShift,    &xio::AssignRShift},
    {Mnemonic::LessEq,              &xio::Leq},
    {Mnemonic::GreaterEq,           &xio::Geq},
    {Mnemonic::Equal,               &xio::Eq},
    {Mnemonic::NotEqual,            &xio::Neq},
    {Mnemonic::Add,                 &xio::Add},
    {Mnemonic::Sub,                 &xio::Sub},
    {Mnemonic::Mul,                 &xio::Mul},
    {Mnemonic::Modulo,              &xio::Modulo},
    {Mnemonic::LessThan,            &xio::LessThan},
    {Mnemonic::GreaterThan,         &xio::GreaterThan},
    {Mnemonic::Assign,              &xio::Assign},
    {Mnemonic::BinaryAnd,           &xio::BinAnd},
    {Mnemonic::BinaryOr,            &xio::BinOr},
    {Mnemonic::BinaryXor,           &xio::BitXor},
    {Mnemonic::C1,                  &xio::X1},
    {Mnemonic::C2,                  &xio::X2},
    {Mnemonic::BinaryNot,           &xio::BitNot},
    {Mnemonic::LogicalAnd,          &xio::BoolAnd},
    {Mnemonic::LogicalOr,           &xio::BoolOr},
    {Mnemonic::Div,                 &xio::Division},
    {Mnemonic::Factorial,           &xio::Factorial},
    {Mnemonic::Positive,            &xio::Positive},
    {Mnemonic::Negative,            &xio::Negative},
    {Mnemonic::Pi,                  &xio::KPi},
    {Mnemonic::Cosinus,             &xio::KCos},
    {Mnemonic::ArcCosinus,          &xio::KAcos},
    {Mnemonic::Tangent,             &xio::KTan},
    {Mnemonic::ArcTangent,          &xio::KAtan},
    {Mnemonic::Sinus,               &xio::KSin},
    {Mnemonic::ArcSinus,            &xio::KAsin},
    {Mnemonic::Number,              &xio::KNumber},
    {Mnemonic::U8,                  &xio::KU8   },
    {Mnemonic::U16,                 &xio::KU16  },
    {Mnemonic::U32,                 &xio::KU32  },
    {Mnemonic::U64,                 &xio::KU64  },
    {Mnemonic::I8,                  &xio::KI8   },
    {Mnemonic::I16,                 &xio::KI16  },
    {Mnemonic::I32,                 &xio::KI32  },
    {Mnemonic::I64,                 &xio::KI64  },
    {Mnemonic::Float,               &xio::KReal },
    {Mnemonic::String,              &xio::KString}
};


// xio::xio()
// {
//     //acc = std::make_shared<Alu>(0.0);
// }



xio::xio(): Core::Object()
{
    A = new Alu(0.0f); // Shit!
}

xio::xio(xio* parent_bloc): Core::Object(parent_bloc, "xio")
{
    //token_info = new SppToken;
    A = new Alu;
}


xio::~xio()
{
    if (!Mem.Ref) delete A;
}

xio::xio(Core::Object* parent_bloc, SppToken* atoken, Alu* a_alu): Core::Object(parent_bloc, atoken ? atoken->Text() : "xio")
{
    SToken = atoken;

    //AppBook::Debug() << "xio::xio => token details:[" << (t0 ? t0->Details() : "nullptr") << "]"<< Book::Enums::Fn::Endl ;

    if (a_alu != nullptr)
    {
        A = a_alu;
        Mem.PRef = 0;
        return;
    }

    A = new Alu(0.0f);

    if (!SToken)
        return;

    switch (SToken->T) {
    case Type::Text:
        *A = atoken->Text();
        return;
    case Type::Number:
    {
        ///@todo Attention enlever le court-circuit du type-size!!!
        if (SToken->S & Type::Number)
        {
            double d;
            (Core::StrAcc(atoken->Text())) >> d;
            *A = d;
            AppBook::Debug() << " acc: " << Color::Yellow << (*A)();
        }
    }
    return;
    case Type::Hex:
    {
        uint64_t d;
        StrAcc(atoken->Text()).Hex(d);
        *A = d;
        return;
    }
    return;
    case Type::Any:
    case Type::VoidPtr:
        *A = (void*)nullptr;
        return;
    default:

        break;
    }

    auto i = xio::xio_operators_table.find(atoken->M);
    xio_fn = i != xio::xio_operators_table.end() ? i->second : nullptr;
    //AppBook::Debug() << " acc: '" << Color::Yellow << (*acc)() << Color::White << "'";
}

//Core::Object::Iterator xio::GetChildIterator(xio* c)
//{
//    auto It = _children.TreeBegin();
//
//    for(; It != _children.end(); It++)
//    {
//        if(*It == c)
//            break;
//    }
//    return It;
//}

#pragma region JSR

// ------------------ ARITHMETIC OPERATORS IMPLEMENTATION ---------------------------------------------

Alu xio::JSR()
{
    //...

    AppBook::Debug() << Color::White << Attribute() << " Value:" << Color::Yellow << A->number<uint64_t>() << Book::Enums::Fn::Endl << SToken->Details(true) ;
    if(SToken->IsOperator())
    {
        if(SToken->IsBinary())
            AppBook::Out() << xio::TraceConnectBinaryOperands(this) << Book::Enums::Fn::Endl ;

        if (Lhs) *A = Lhs->JSR(); // Always catch the lhs value so we return that one if t is no rhs operand.
        if (Rhs) *A = Rhs->JSR(); // Always catch the rhs value because it is the value to be returned after being applied to the lhs (if applicable).

        if (xio_fn)
            return (this->*xio_fn)();// All operators assign acc.
        else
            AppBook::Warning() << "operator xio [" << Color::Yellow << SToken->Text() << Color::Reset << "] has no implementation (yet?).:\n" << SToken->Mark() << Book::Enums::Fn::Endl ;
    }
    SToken->S |= A->T; ///< Why is that ?
    // It's because our actual token::type (xio::Type::T) has been changed by the type of the resulting operation Alu::T ( acc->T is put into t0->s )...
    // Ceci est le changement apporté au champs sémantique du token qui est modifié par le type résultant de l'opération.  ^       ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ...

    return *A;
}

//Book::Enums::Code xio::AppendChild(xio*c)
//{
//    auto e = GetChildIterator(c);
//    if(e != _children.end())
//        _children.push_back(c);
//    else
//        return Book::Enums::Code::Exist;
//
//    return Book::Enums::Code::Accepted;
//}


//Book::Enums::Code xio::Detach(xio* c)
//{
//    auto i = GetChildIterator(c);
//    if(i != _children.end())
//        _children.erase(i);
//    else
//        return Book::Enums::Code::Notexist;
//
//    return Book::Enums::Code::Accepted;
//}
//
//Book::Enums::Code xio::detach()
//{
//    xio* p = Parent<xio>();
//    if(p)
//    {
//        p->Detach(this);
//    }
//    return Book::Enums::Code::Accepted;
//}



Alu xio::LeftShift()
{
    if((Lhs->SToken->S & Type::Float) || (Rhs->SToken->S & Type::Float))
    {
        *A = 0.f;
        AppBook::Warning() << Lhs->SToken->TypeName() << " " << SToken->Text() << " " << Rhs->SToken->TypeName() << " are incompatible" ;
    }

    *A = Lhs->A->number<uint64_t>() << Rhs->A->number<uint64_t>();
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}

Alu xio::Radical()
{
    *A = std::pow(Lhs->A->number<double>(), 1 / Rhs->A->number<double>());
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}

Alu xio::Exponent()
{
    *A = std::pow(Lhs->A->number<double>(), Rhs->A->number<double>());
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}

Alu xio::RightShift()
{
    *A = Lhs->A->number<uint64_t>() >> Rhs->A->number<uint64_t>();
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}


Alu xio::Decr()
{
    *A = SToken->IsPrefix() ? --(*Rhs->A) : (*Lhs->A)--;
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}


Alu xio::Incr()
{
    *A = SToken->IsPrefix() ? ++(*Rhs->A) : (*Lhs->A)++;
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}


Alu xio::AssignAdd()
{
    *A = *(Lhs->A) += *(Rhs->A);
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return  *A;
}


Alu xio::AssignSub()
{
    *A = *(Lhs->A) -= *(Rhs->A);
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return  *A;
}


Alu xio::AssignMul()
{
    *A = *(Lhs->A) *= *(Rhs->A);
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}


Alu xio::AssignDiv()
{
    *A = *(Lhs->A) /= *(Rhs->A);
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}
Alu xio::AssignMod()
{
    *A = *(Lhs->A) %= *(Rhs->A);
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;

}
Alu xio::AssignAnd()
{
    *A = *(Lhs->A) &= *(Rhs->A);
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;

}
Alu xio::AssignOr()
{
    *A = *(Lhs->A) |= *(Rhs->A);
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;

}
Alu xio::AssignXor()
{
    *A = *(Lhs->A) ^= *(Rhs->A);
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;

}
Alu xio::AssignX1()
{
    *A = ~(*Rhs->A);
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}
Alu xio::AssignLShift()
{
    *A = *Lhs->A <<= *Rhs->A;
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}


Alu xio::AssignRShift()
{
    *A = *Lhs->A >>= *Rhs->A;
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}


Alu xio::Leq()
{
    *A = *Lhs->A <= *Rhs->A;
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}


Alu xio::Geq()
{
    *A = *Lhs->A >= *Rhs->A;
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}


Alu xio::Eq()
{
    *A = (*Lhs->A) == (*Rhs->A);
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}


Alu xio::Neq()
{
    *A = *Lhs->A != *Rhs->A;
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}

Alu xio::Add()
{
    AppBook::Debug() << Color::Yellow << Lhs->Value()() << " " << Color::CornflowerBlue << Attribute() << " " << Color::Yellow << Rhs->Value()() << ":";
    *A = *Lhs->A + *Rhs->A;
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}
Alu xio::Sub()
{
    // hack... en attendant :
    if (SToken->S & Type::Sign)
        return Negative();
    AppBook::Debug() << Color::Lime
                     << Color::Yellow << Lhs->Value()() << " " << Color::CornflowerBlue << Attribute() << " " << Color::Yellow << Rhs->Value()() << ":";
    *A = *Lhs->A - *Rhs->A;
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();

    return *A;
}
Alu xio::Mul()
{
    AppBook::Debug()
        << Color::Yellow << Lhs->Attribute()
        << Color::CornflowerBlue << Attribute()
        << Color::Yellow << Rhs->Attribute()
        << Color::White;
    *A = *Lhs->A * *Rhs->A;
    AppBook::Out() << Color::CornflowerBlue << " => " << Color::Lime << (*A)() ;
    return *A;
}
Alu xio::Modulo()
{
    *A = *Lhs->A % *Rhs->A;
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}
Alu xio::LessThan()
{
    *A = *Lhs->A < *Rhs->A;
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}
Alu xio::GreaterThan()
{
    *A = *Lhs->A > *Rhs->A;
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}
Alu xio::Assign()
{
    AppBook::Debug() << Color::Lime
                     << Color::Aquamarine3 << Lhs->Attribute() << Color::Reset << " "
                     << " " << Color::CornflowerBlue << Attribute() << " "
                     << Color::Yellow
                     << Rhs->Value()() << ":";

    *Lhs->A = *Rhs->A;
    *A = *Rhs->A;

    AppBook::Out() << Color::CornflowerBlue << " => " << Color::Lime << (*A)();
    return *A;
}


Alu xio::BinAnd()
{
    *A = *Lhs->A & *Rhs->A;
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}


Alu xio::BinOr()
{
    *A = *Lhs->A | *Rhs->A;
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}


Alu xio::BitXor()
{
    *A = *Lhs->A ^ *Rhs->A;
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}

Alu xio::X1()
{
    *A = ~(*Lhs->A);
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}

Alu xio::X2()
{
    *A = ~(*Lhs->A) + Alu(1);
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}


Alu xio::BitNot()
{
    *A = !Rhs->A->number<double>();
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}


Alu xio::BoolAnd()
{
    *A = *Lhs->A && *Rhs->A;
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}
Alu xio::BoolOr()
{
    *A = *Lhs->A || *Rhs->A;
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;

}

Alu xio::Division()
{
    AppBook::Debug() << Color::Lime
                     << Color::Yellow << Lhs->Value()()
                     << " " << Color::CornflowerBlue << Attribute() << " "
                     << Color::Yellow
                     << Rhs->Value()() << ":";

    *A = *Lhs->A / *Rhs->A;
    AppBook::Out() << Color::CornflowerBlue << " => " << Color::Lime << (*A)();
    return *A;
    //AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*acc)();
}


Alu xio::Factorial()
{
    //*acc = acc->factorial(*lhs->acc);
    AppBook::Debug() << Color::Lime << Lhs->Attribute()
                     << Color::Yellow << " " << (*Lhs->A)() << " "
                     << Color::CornflowerBlue << Attribute() << Color::White << ":" ;

    AppBook::Out() << xio::trace_connect_postfix_operands(this) ;

    *A = Lhs->A->Factorial();

    AppBook::Out() << Color::CornflowerBlue << " => " << Color::Yellow << (*A)();
    return *A;
}



Alu xio::Positive()
{
    if (Rhs->A->number<double>() < 0)
        *Rhs->A *= -1;

    *A = *Rhs->A;
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}


Alu xio::Negative()
{
    AppBook::Debug() << Color::Lime
                     << Color::CornflowerBlue << Attribute()
                     << Color::Yellow << Rhs->Attribute()
        << Color::White << "=" ;

    if (Rhs->A->number<double>() > 0) // ==> a = -1;  -a = ?
        *Rhs->A *= -1;

    *A = *Rhs->A;
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}


Alu xio::KPi()
{
    *A = Alu(_PI_);
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}


Alu xio::KCos()
{
    *A = std::cos(deg2rad(Rhs));
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}


Alu xio::KAcos()
{
    *A = std::acos(deg2rad(Rhs));
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}
Alu xio::KTan()
{
    *A = std::tan(deg2rad(Rhs));
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}


Alu xio::KAtan()
{
    *A = std::atan(deg2rad(Rhs));
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();

    return *A;
}

Alu xio::KSin()
{
    *A = std::sin(deg2rad(Rhs));
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}

Alu xio::KAsin()
{
    *A = std::asin(deg2rad(Rhs));
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}

Alu xio::KNumber()
{

    return *A;
}

Alu xio::KU8()
{
    *A = Rhs->A->number<uint64_t>() & 0xFF;
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}

Alu xio::KU16()
{
    *A = Rhs->A->number<uint64_t>() & 0xFFFF;
    AppBook::Out() << Color::CornflowerBlue << " = " << Color::Lime << (*A)();
    return *A;
}

Alu xio::KU32()
{
    *A = Rhs->A->number<uint64_t>() & 0xFFFFFFFF;
    return *A;
}

Alu xio::KU64()
{
    *A = (uint64_t)(Rhs->A->number<uint64_t>());// & 0xFFFFFFFFFFFFFFFF);
    return *A;
}

Alu xio::KI8()
{
    *A = (int8_t)(Rhs->A->number<int64_t>() & 0xFF);
    return *A;
}

Alu xio::KI16()
{
    AppBook::Debug() << Rhs->SToken->Text();
    *A = (int16_t)(Rhs->A->number<uint64_t>() & 0xFFFF);
    return *A;
}

Alu xio::KI32()
{
    *A = (int32_t)(Rhs->A->number<uint64_t>() & 0xFFFFFFFF);
    return *A;
}

Alu xio::KI64()
{
    //*acc = (int64_t)(rhs->acc->number<int64_t>() & 0xFFFFFFFFFFFFFFFF);
    *A = Rhs->A->number<int64_t>();
    return *A;
}

Alu xio::KReal()
{
    *A = Rhs->A->number<double>();
    return *A;
}

Alu xio::KString()
{
    *A = std::string(*Rhs->A);
    return *A;
}


[[maybe_unused]] std::string xio::TypeName()
{
    return Type::Name(SToken->T);
}


auto xio::Match(xio* in_lhs, xio* in_rhs)
{
    for (auto [lr, fn] : xio::move_tbl)
    {
        auto [l, r] = lr;
        if ((in_lhs->SToken->T & l) && (in_rhs->SToken->T & r)) return fn;
    }
    return static_cast<xio * (xio::*)(xio*)> (nullptr);
}

#pragma endregion JSR


//---------------------------Arithmetic Expression Tree Building------------------------------------------------
xio::move_table_t xio::move_tbl =
{
    // ToLeft:
        {{Type::OpenPair, Type::Leaf | Type::Prefix | Type::Binary | Type::Id | Type::Number | Type::Const}, &xio::ToLeft},
        {{Type::Binary | Type::Prefix | Type::Sign | Type::Assign,  Type::OpenPair | Type::Leaf | Type::Prefix | Type::Number | Type::Id}, &xio::ToRight},
        // rpar_leaf:
        //    {{Type::close_pair, Type::leaf}, &xio::_rpar_leaf},
        //// _assign:
        //    {{Type::leaf|Type::number|Type::id, Type::assign}, &xio::_assign},
        // _close_par:
        {{Type::ClosePair | Type::Prefix | Type::Postfix | Type::Id | Type::Number | Type::Const, Type::ClosePair }, &xio::_close_par},
        // close_par_seq:
        {{Type::ClosePair, Type::Binary | Type::Postfix | Type::ClosePair}, &xio::collapse_par_pair},
        // _binary:
        {{Type::Number | Type::Binary | Type::Prefix | Type::Postfix | Type::Id, Type::Assign | Type::Binary}, &xio::TreeInputBinary},
        // opo_right:
        {{Type::Number | Type::Id, Type::Postfix}, &xio::OpToRight}
};

xio::input_table_t xio::input_tbl =
{
    {{Type::Id     | Type::Assign,    Type::Assign}, "&xio::_assign"},
    {{Type::Number | Type::Const | Type::Postfix | Type::ClosePair | Type::Id,    Type::Binary}, "&xio::TreeInputBinary"},
    {{Type::Number | Type::Id    | Type::ClosePair | Type::Const,    Type::Postfix},  "&xio::_postfix"},
    {{Type::Binary,  Type::Prefix},     "&xio::_prefix"},
    {{Type::Prefix | Type::Binary | Type::OpenPair | Type::Assign,    Type::Leaf | Type::Number | Type::OpenPair | Type::Id | Type::Prefix},  "&xio::_leaf"},
    {{Type::Prefix | Type::Binary | Type::OpenPair,    Type::OpenPair},    "&xio::_open_pair"},
    {{Type::Postfix| Type::Const | Type::Number | Type::Id | Type::OpenPair | Type::ClosePair,    Type::ClosePair}, "&xio::_close_pair"}
};


void xio::syntax_error(xio* e)
{
    throw AppBook::Syntax() << "at " << e->SToken->LocationText() << Book::Enums::Fn::Endl << e->SToken->Mark() << Book::Enums::Fn::Endl ;
}

xio *xio::Warning(xio*)
{
    return nullptr;
}
void xio::make_error(Book::Enums::Code ErrCode, xio* source_node, xio* input_node)
{
    throw AppBook::Error() << source_node->Attribute()
        << " TokenPtr TreeInput error: "
        << ErrCode
        << input_node->Attribute()
        << Book::Enums::Fn::Endl
        << input_node->SToken->Mark()
        << Book::Enums::Fn::Endl ;
}

void xio::make_error(Book::Enums::Code ErrCode, xio* source_node, SppToken* inputoken)
{
    throw AppBook::Error() << source_node->Attribute()
        << " TokenPtr TreeInput error: "
        << ErrCode
        << inputoken->Text()
        << Book::Enums::Fn::Endl
        << inputoken->Mark()
        << Book::Enums::Fn::Endl ;
}

void xio::Header(xio* input_node, std::source_location&& Loc)
{
    AppBook::Debug(Loc) << Color::Yellow << SToken->Text() << Color::White << "<-" <<
                        Color::Yellow << input_node->SToken->Text() <<
                        Book::Enums::Fn::Endl << input_node->SToken->Mark() << Book::Enums::Fn::Endl ;
}



/*!
 * \brief xio::TreeInput
 *
 * \note Call to this function is stricly, restricted To the Arithmetic Expression Tree/AST Build context.
 * \param parent_bloc
 * \param token
 * \return new Insertion node ( or vertex? ).
 *
 * ex.: if x-1 + 12 ==  0 return 12;
 * * ex.: if a+1+b+c d = 12;
 */
xio* xio::TreeInput(xio* parent_bloc, SppToken* token, xio::maker mk)
{
    for (auto& [lr, fntext] : xio::input_tbl)
    {
        auto [l, r] = lr;
        if ((SToken->T & l) && (token->T & r))
        {
            AppBook::Debug() << Color::Yellow << SToken->Text() << " <- " << Color::Yellow << token->Text() << Color::Reset << " Input TokenPtr validated: '" << Color::Yellow << fntext << Color::Reset << "'";
            ///@todo Check id tokens for function_call and other id-constructs before calling xio::TreeInput(...).

            xio* a;
            if(mk)
                a = mk(token);
            else
                a  = new xio(parent_bloc, token);

            if(!a) return nullptr;
            auto fn = xio::Match(this, a);
            if (!fn)
            {
                a->Detach();
                delete a;
                AppBook::Syntax() << " invalid relational operands at " << token->LocationText() << " - unexpected TokenPtr:" << Book::Enums::Fn::Endl << token->Mark() << Book::Enums::Fn::Endl ;
                return nullptr;
            }
            AppBook::Debug() << SToken->Text() << "::TreeInput(" << token->Text() << "):" << Book::Enums::Fn::Endl << token->Mark() << Book::Enums::Fn::Endl ;

            return (this->*fn)(a);
        }
    }

    AppBook::Info() << Color::White << "'" << Color::Yellow << SToken->Text() << Color::White << "'" << Color::Reset
                    << "::TreeInput(" << Color::Yellow << token->Text() << Color::Reset << ") => invalid relational operands at "
                    << token->LocationText() << " - unexpected TokenPtr."
                    << Book::Enums::Fn::Endl << token->Mark() << Book::Enums::Fn::Endl ;
    AppBook::Out() << SToken->Details() << " || " << token->Details() << Book::Enums::Fn::Endl << "Returning nullptr" ;

    return nullptr;
}

xio* xio::TreeInputBinary(xio* a)
{
    Header(a);

    if (SToken->IsLeaf())
    {
        if (a->SToken->M == Mnemonic::OpenPar)
            syntax_error(a);
    }

    if (SToken->M == Mnemonic::OpenPar)
        return ToLeft(a);

    if (SToken->IsBinary())
    {
        //if (!rhs) syntax_error(a);

        if (a->SToken->D < SToken->D)
            return ToRight(a);
        if (Op)
        {
            auto fn = Match(this, a);
            if (!fn)
                xio::make_error(Book::Enums::Code::Unexpected, this, a);

            return (Op->*fn)(a);
        }

        a->ToLeft(this); // returns "this" but from  we must return a as the new insertion node into the tree...
        // The only case when it is required to return lhs ( left-hand-side node) is on openning par or left-pair(operator) that isolates the
        // inner expression between open-close pair.
        return a;
    }

    if (Op) {
        auto fn = Match(this, a);
        if (!fn)
            xio::make_error(Book::Enums::Code::Unexpected, this, a);
        return (Op->*fn)(a);

    }
    a->ToLeft(this);
    return a;
}

xio* xio::_close_pair(xio* a)
{
    Header(a);
    xio* x = xio::PopPar();
    if (!x)
    {
        AppBook::Error() << "Unmatched left paranthese:" << Book::Enums::Fn::Endl << a->SToken->Mark() << Book::Enums::Fn::Endl ;
        return nullptr;
    }
    a->Op = x->Op;
    a->Lhs = x->Lhs;
    if (a->Lhs)
    {
        if (a->Lhs->Op)
            a->Lhs->Op = a;
    }
    if (a->Op)
        a->Op->Rhs = a; // oops!!

    AppBook::Debug()
        << "new TreeInput vertex:["
        << Color::Yellow
        << a->SToken->Text()
        << Color::Reset
        << "]" << Book::Enums::Fn::Endl
        << a->SToken->Mark() << Book::Enums::Fn::Endl ;

    return a;
}

xio* xio::_close_par(xio* a)
{
    Header(a);

    xio* x = xio::PopPar();
    if (!x)
    {
        AppBook::Error() << "Unmatched left paranthese." << a->SToken->Mark() << Book::Enums::Fn::Endl ;
        return nullptr;
    }
    a->Op = x->Op;
    a->Lhs = x->Lhs;
    if (a->Lhs)
    {
        if (a->Lhs->Op)
            a->Lhs->Op = a;
    }
    if (a->Op)
        a->Op->Rhs = a; // oops!!

    AppBook::Out()
        << "new TreeInput vertex:["
        << Color::Yellow
        << a->SToken->Text()
        << Color::Reset
        << "]" << Book::Enums::Fn::Endl
        << a->SToken->Mark() << Book::Enums::Fn::Endl ;

    return a;
}




/*!
 * \brief xio::collapse_par_pair
 * \param a
 * \return
 *
    \code
     [/]                  [/] <- !
    /  \                   \
   42   ) <-- !             -
       /                     \
      -                       4
       \
        4
    \endcode
 */
xio* xio::collapse_par_pair(xio* a)
{
    Header(a);
    //xio* v = lhs;

    // Collapse lhs

    Lhs->Op = Op;
    if (Op)
    {
        Op->Rhs = Lhs;
        if(a->SToken->D < Op->SToken->D)
        {
            return Op->ToRight(a);
        }
    }

    // discard();

    if (Lhs->Op) {
        AppBook::Out()
            << Color::Yellow << Lhs->Op->Attribute() << Color::CornflowerBlue
            << " <-- "
            << Color::Yellow << a->Attribute();

        auto p_fn = Match(Lhs->Op, a);
        if (!p_fn)
            xio::syntax_error(a);

        return (Lhs->Op->*p_fn)(a);
    }

    Lhs->Op = a;
    a->Lhs = Lhs;
    return a;
}

xio* xio::OpToRight(xio* a)
{
    Header(a);
    if (!Op) {
        a->Lhs = this;
        Op = a;
        return a;
    }
    return Op->ToRight(a);
}


int xio::PushPar(xio* a)
{
    xio::pars.push(a);
    return static_cast<int>(xio::pars.size());
}

xio* xio::PopPar()
{
    if (xio::pars.empty()) return nullptr;
    xio* x = xio::pars.top();
    xio::pars.pop(); // Tabarnak!
    return x;
}


xio* xio::TreeBegin(xio* ParentObj, SppToken* Token, const xio::maker& Maker)
{
    AppBook::Debug() << Color::Yellow << Book::Enums::Fn::Endl << Token->Text() << Book::Enums::Fn::Endl ;
    xio* a;

    if (!Token->Flags.V)
        return nullptr;

    if(Maker)
        a = Maker(Token);
    else
        a = new xio(ParentObj, Token);

    if(!a)
        return nullptr;

    if (a->SToken->M == Mnemonic::OpenPar) PushPar(a);
    return a;
}


xio* xio::CloseTree()
{
    Header(this);

    if (SToken->M == Mnemonic::OpenPar)
    {
        AppBook::Error() << " unexpected End of file." ;
        return nullptr;
    }

    if (!xio::pars.empty())
    {
        xio* x = xio::pars.top();
        xio::pars.pop();
        AppBook::Error() << " umatched closing parenthese from:" << Book::Enums::Fn::Endl << x->SToken->Mark() << Book::Enums::Fn::Endl ;
        return nullptr;
    }


    if (SToken->M == Mnemonic::ClosePar) {
        AppBook::Debug() << "Closing the tree on close parenthese:";
        if (Lhs)
        {
            xio* x = Lhs;
            AppBook::Debug() << "left hand side operand: " << Lhs->SToken->Details() << ":" << Book::Enums::Fn::Endl << Lhs->SToken->Mark() << Book::Enums::Fn::Endl ;

            Lhs->Op = Op;

            if (Op)
            {
                Op->Rhs = Lhs;
                xio::TraceConnectBinaryOperands(Op);
            }

            // discard();
            return x->TreeRoot(false);
        }
    }
    return TreeRoot(false);
}

xio* xio::TreeRoot(bool skip_syntax)
{
    AppBook::Debug() << "Match tree ins from xio node:" << Book::Enums::Fn::Endl << SToken->Mark() << Book::Enums::Fn::Endl ;
    xio* x = this;
    xio* p = x;
    do {
        x = p;
        if (!skip_syntax)
        {
            switch (x->SToken->T) {
            case Type::Assign:
            case Type::Binary:
                if (!x->Lhs)
                {
                    AppBook::Error() << "Syntax error: binary operator has no left operand." << Book::Enums::Fn::Endl << x->SToken->Mark() << Book::Enums::Fn::Endl ;
                    return nullptr;
                }
                if (!x->Rhs)
                {
                    AppBook::Error() << "Syntax error: binary operator has no right operand." << Book::Enums::Fn::Endl << x->SToken->Mark() << Book::Enums::Fn::Endl ;
                    return nullptr;
                }
            case Type::Prefix:
                if (!x->Rhs)
                {
                    AppBook::Error() << "Syntax error: prefix unary operator has no (right) operand." << Book::Enums::Fn::Endl << x->SToken->Mark() << Book::Enums::Fn::Endl ;
                    return nullptr;
                }
                break;
            case Type::Postfix:
                if (!x->Lhs)
                {
                    AppBook::Error() << "Syntax error: postfix unary operator has no (left) operand." << Book::Enums::Fn::Endl << x->SToken->Mark() << Book::Enums::Fn::Endl ;
                    return nullptr;
                }
                break;
            }
        }
        p = p->Op;
    } while (p);

    AppBook::Debug() << "Match tree ins returning node: " << x->SToken->Details() << Book::Enums::Fn::Endl << x->SToken->Mark() << Book::Enums::Fn::Endl ;
    return  x;
}

xio* xio::ToRight(xio* in_rhs)
{
    Header(in_rhs);

    // Temporary hack....
    if (in_rhs->SToken->M == Mnemonic::OpenPar)
        xio::PushPar(in_rhs);

    if (Rhs) {
        /*
           this
              \
               x <- nextoken
              /
            rhs
        */
        AppBook::Debug() << SToken->Text() << " -> " << Rhs->SToken->Text()
                         << Color::Lime << "tree_set_right "
                         << Color::White << " <- "
                         << Color::Yellow << in_rhs->SToken->Text();
        Rhs->Op = in_rhs;
        in_rhs->Lhs = Rhs;
    }
    Rhs = in_rhs;
    in_rhs->Op = this;
    if (SToken->IsBinary())
    {
        AppBook::Debug() << xio::TraceConnectBinaryOperands(this);
    }
    return in_rhs;
}

xio* xio::ToLeft(xio* in_lhs)
{
    Header(in_lhs);
    /*
    (; [; {   // Appliqué sur aucun autre type de token car l'appel de tree_set_left ne se fait qu'� partir de tree_input qui r�soud l'associativit�.
              /
             x < -nextoken
            /
            lhs
    */

    if (Lhs) {
        //  we are supposed to be the openning par/index/bracket/bloc. So the interior will become right hand side of the parent op of this.
        Lhs->Op = in_lhs;
        in_lhs->Lhs = Lhs;
    }
    in_lhs->Op = this;

    Lhs = in_lhs;
    return in_lhs;
}


#pragma region ast-digraph


/*!
 * \brief xio::dot_tree_start
 * \param a_out
 * \param Title
 * \note For now the colours are the halloween theme ;)
 */
void xio::dot_tree_start(StrAcc& a_out, const StrAcc& Title)
{
    a_out << "digraph arithmetic_expressionree {\n";
    a_out << "ratio=compress; ranksep=.55; size = \"6.5,6.5\"; bgcolor=\"#606060\"; \n";
    //a_out << "    node [fontname=\"Source Code Pro\", fontsize=12];\n";
    a_out << "    node [fontname=\"JetBrainsMono NF\", fontsize=18, fontcolor=\"#ff500f\"];\n";
    a_out << "    label = < <u> arithmetic expresion ast : </u> <br/> <br/>" << Title() << ">; fontsize = 16; fontcolor=\"#00d787\"\n"; // #0FAEFF
}

void xio::dot_node(xio* A, StrAcc& a_out)
{
    //static int nullcount = 0;
    if (!A) return;
    if ((!A->Lhs) && (!A->Rhs)) return;

    if (A->Lhs) {
        a_out << "    xioNode" << A << " -> xioNode" << A->Lhs << " [dir = none, Color=\"#A07060\", penwidth=5];\n";
        xio::dot_node(A->Lhs, a_out);
    }

    if (A->Rhs) {
        a_out << "    xioNode" << A << " -> xioNode" << A->Rhs << " [dir = none, Color=\"#A07060\", penwidth=5];\n";
        xio::dot_node(A->Rhs, a_out);
    }

}

void xio::dot_null_node(xio*, int, StrAcc&)
{

}

void xio::dot_tree(xio* a_root, StrAcc& a_out)
{
    a_root->dot_attr(a_out);
    xio::dot_node(a_root, a_out);
}

void xio::dot_tree_close(StrAcc& a_out)
{
    a_out << "}\n";
}

void xio::dot_attr(StrAcc& a_out)
{
    StrAcc attr;
    attr << SToken->Text();
    StrAcc Shape;
    if (SToken->T & Type::Text)
        Shape << "none";
    else
        if (SToken->T & Type::Assign)
            Shape << "none";
        else
            Shape << "none";

    a_out << "    xioNode" << this << " [shape=%s, label=\"%s\"]\n";
    a_out << Shape() << attr();

    if (Lhs)
        Lhs->dot_attr(a_out);
    if (Rhs)
        Rhs->dot_attr(a_out);
}

using Core::Point;
using Core::Rect;



std::string xio::TraceConnectBinaryOperands(xio* x)
{
    // assume this binary operator already has its lhs rhs operands !!
    //StrAcc str;
    auto lw = x->Lhs->Attribute().length();
    auto rw = x->Rhs->Attribute().length();
    auto ow = x->Attribute().length();
    auto w = lw + rw + 3; // total width
    w -= lw % 2 == 0;


    auto m_lhs = lw - (lw > 1 ? 1 : 0);

    Point oper_xy = Point(static_cast<int>(m_lhs) + 1, 0);
    oper_xy.X -= ow % 2 == 0 ? 1 : 0;

    Core::WinBuffer area;
    area.SetGeometry(static_cast<int>(w), 3); // pour l'instant c'est hardcodé.
    area.GotoXY(oper_xy.X, 0);
    area << x->Attribute();
    area << Point{ static_cast<int>(m_lhs),1 } << "/ \\";

    area.GotoXY(0, 2);
    area << x->Lhs->Attribute();

    area.GotoXY(static_cast<int>(m_lhs) + 2 + (rw <= 1 ? 1 : 0), 2);
    area << x->Rhs->Attribute();
    return area;
}


std::string xio::trace_connect_postfix_operands(xio* x)
{
    // assume this binary operator already has its lhs rhs operands !!
    //StrAcc str;
    auto lw = x->Lhs->Attribute().length();
    //auto rw = x->rhs->Attribute().length();
    auto ow = x->Attribute().length();
    auto w = lw + 3; // total width
    w -= lw % 2 == 0;


    auto m_lhs = lw - (lw > 1 ? 1 : 0);

    Point oper_xy = Point(static_cast<int>(m_lhs) + 1, 0);
    oper_xy.X -= ow % 2 == 0 ? 1 : 0;

    Core::WinBuffer area;
    area.SetGeometry(static_cast<int>(w), 3); // pour l'instant c'est hardcodé.
    area.GotoXY(oper_xy.X, 0);
    area << x->Attribute();
    area << Point{ static_cast<int>(m_lhs),1 } << "/ \\";

    area.GotoXY(0, 2);
    area << x->Lhs->Attribute();

    //area.GotoXY(static_cast<int>(m_lhs) + 2;
    //area << x->rhs->Attribute();
    return area;
}
#pragma endregion ast-digraph





}


