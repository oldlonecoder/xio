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

#include "Spp/Alu.h"
#include <AppBook/Book/AppBook.h>




namespace Spp
{

using Core::Color;
using Core::StrAcc;


std::string  Alu::mNil = "";

#define UCTor(rhs) { T |= Spp::Type::Number|Spp::Type::Leaf; v.d=(rhs); }
#define SCTor(rhs) { T |= Spp::Type::Number|Spp::Type::Sign|Spp::Type::Leaf;v.d=(rhs); }
#define _assign_u T |= Spp::Type::Number|Spp::Type::Leaf; v.d = (rhs);
#define _assign_s T |= Spp::Type::Number|Spp::Type::Sign|Spp::Type::Leaf;v.d = (rhs);

Alu::Alu(int8_t rhs) : T(Spp::Type::I8)
SCTor(rhs)
Alu::Alu(int16_t rhs) : T(Spp::Type::I16)
SCTor(rhs)
Alu::Alu(int32_t rhs) : T(Spp::Type::I32)
SCTor(rhs)
Alu::Alu(int64_t rhs) : T(Spp::Type::I64)
SCTor(static_cast<double>(rhs))
Alu::Alu(uint8_t rhs) : T(Spp::Type::U8)
UCTor(rhs)
Alu::Alu(uint16_t rhs) : T(Spp::Type::U16)
UCTor(rhs)
Alu::Alu(uint32_t rhs) : T(Spp::Type::U32)
UCTor(rhs)
Alu::Alu(uint64_t rhs) : T(Spp::Type::U64)
UCTor(static_cast<double>(rhs))
Alu::Alu(bool rhs) : T(Spp::Type::Bool)
SCTor(rhs)

#define cast(rhs)\
(rhs).T & Spp::Type::Bool  ? (rhs).v.i != false :\
(rhs).T & Spp::Type::I8       ? (rhs).v.i:\
(rhs).T & Spp::Type::I16      ? (rhs).v.i:\
(rhs).T & Spp::Type::I32      ? (rhs).v.i:\
(rhs).T & Spp::Type::I64      ? (rhs).v.i:\
(rhs).T & Spp::Type::U8       ? (rhs).v.u:\
(rhs).T & Spp::Type::U16      ? (rhs).v.u:\
(rhs).T & Spp::Type::U32      ? (rhs).v.u:\
(rhs).T & Spp::Type::U64      ? (rhs).v.u:\
(rhs).T & Spp::Type::Float    ? (rhs).v.d :0




Alu::Alu(double rhs)
{
    T = Spp::Type::Number | Spp::Type::Float | Spp::Type::Leaf;
    v.d = rhs;
}

Alu::Alu() {}

Alu::Alu(float rhs)
{
    T = Spp::Type::Number | Spp::Type::Float | Spp::Type::Leaf;
    v.d = (double)rhs;
}

Alu::Alu(const std::string& rhs)
{
    T = Spp::Type::Text | Spp::Type::Leaf;
    v.t = &text; text = rhs;
}

Alu::Alu(const char* rhs)
{

    T = Spp::Type::Text | Spp::Type::Leaf;
    v.t = &text; text = rhs;
}

Alu::Alu(void* rhs)
{
    T = Spp::Type::Any | Spp::Type::VoidPtr;
    v.p = rhs;
}

bool Alu::TypeSize(const Alu& Rhs_) const
{
    using namespace Spp::Type;
    Spp::Type::T me = T & (I8 | I16 | I32 | I64 | U8 | U16 | U32 | U64 | Float);
    Spp::Type::T r = Rhs_.T & (I8 | I16 | I32 | I64 | U8 | U16 | U32 | U64 | Float);
    return me - r >= 0;
}

Alu::operator bool()
{
    if (!v.i)
        return false;
    if (!T)
        return false;
    if (T & Spp::Type::Text)
        return !text.empty();
    if (T & Spp::Type::Number)
        return v.d != 0.0f;
    if (T & Spp::Type::VoidPtr)
        return value<void *>() != nullptr;
    return false;
}

#define lr_number(rv) if ((T&Spp::Type::Number) && ((rv).T&Spp::Type::Number))
#define lrtext(rv)   if ((T&Spp::Type::Text) && ((rv).T&Spp::Type::Text))
#define lr_any(rv)    if ((T&Spp::Type::Any)  && ((rv).T&Spp::Type::Any))
#define is_integrale(rv) if (!((rv).T&Spp::Type::Float))
#define istext(i)    if((i).T & Spp::Type::Text)
#define is_number if ((T&(Spp::Type::Float|Spp::Type::Number)))
// #define delstring_if(rhs) if ((T&Spp::Type::Text) && (!(rhs).T & Spp::Type::Text)) delete v..t;  Vieux stuff

Alu& Alu::operator=(const Alu& rhs)
{
    if (&rhs != this)
    {
        T = rhs.T;
        v = rhs.v;
        return *this; // WTF???????
    }

    if (T & Spp::Type::Text)
    {
        text = rhs.text;
        return *this;
    }
    if (T & Spp::Type::Any)
    {
        v.p = rhs.v.p;
        return *this;
    }
    *this = false;
    return *this;
}

[[maybe_unused]] std::string Alu::Types() const
{
    std::string str;
    return Spp::Type::Name(T);
}

Alu Alu::operator+(const Alu& rhs) const
{
    lrtext(rhs)
        return {text + rhs.text};
    else lr_number(rhs)
        return {v.d + rhs.v.d}; // Slow?

    std::string str;
    throw AppBook::Except() << " [" << Type::Name(T) << "] and [" << Type::Name(rhs.T) << "] are incompatible." ;
    return {false};
}


Alu Alu::operator==(const Alu & lf) const
{
    Alu a;
    lrtext(lf)
    {
        a = lf.text == text;
        return a;
    }
    lr_number(lf)
    {
        return lf.number<double>() == number<double>();
    }
    throw AppBook::Except() << " Equality comparison between different types(" << Type::Name(T) << " == " << Type::Name(lf.T) << ')' ;
    return {false};
}

Alu Alu::operator[](Alu::CNV c)
{
    istext(*this)
    {
        return {0.0f};
    }
    auto d = v.d;

    Alu    a;
    switch (c)
    {
        case Alu::DEG:
            a = d * 180 / 3.141592653589793;
            break;
        case Alu::RAD:
            a = d * 3.141592653589793 / 180;
            break;
        default:
            a = 0.0;
            break;
    }
    return a;
}

Alu Alu::operator-(const Alu& rv)
{

    lrtext(rv)
    {
        throw AppBook::Except() << " substraction operator strings is not yet supported" ;
        //return remove_substr_copy(rv.text);
        return *this;
    }

    lr_number(rv)
        return {v.d - rv.v.d};
    lr_any(rv)
        return {reinterpret_cast<uint64_t>(value<void *>()) - reinterpret_cast<uint64_t>(rv.value<void *>())};

    throw AppBook::Except() << "cannot apply sign on " << Type::Name(T) ;
    return {false};
}


Alu Alu::operator*(const Alu& rv) const
{
    Alu f;
    lrtext(rv)
    {
        std::size_t pos = rv.text.find(text);
        return {pos != std::string::npos};
    }

    //else
    lr_number(rv)
        return {v.d * rv.v.d};

    throw AppBook::Except() << "cannot multiply" << Type::Name(T) << " and " << Type::Name(rv.T) << "." ;
    return {};
}

Alu Alu::operator/(const Alu& rv)
{
    lrtext(rv)
    {
        throw AppBook::Except() << "cannot divide strings" ;
        return {};
    }
    //    f = text + rv.text;
    //else
    lr_number(rv)
    {
        double vr = rv.v.d;
        double lr = v.d;

        if (vr == 0.0L)
        {
            throw AppBook::Except() << " In Alu operation: division by zero error. (" << (*this)() << " / " << rv() << ")" ;
            return "Inf";
        }
        return {lr / vr};
    }

    throw AppBook::Except() << "cannot divide " << Type::Name(T) << " and " << Type::Name(rv.T) << "." ;
    return {};
}

Alu Alu::operator%(const Alu& rv) const
{
    Alu f;
    lrtext(rv)
    {
        throw  AppBook::Except() << "cannot modulo strings" ;
        return {};
    }
    lr_number(rv)
        return {v.u % rv.v.u};

    throw AppBook::Except() << "cannot modulo " << Type::Name(T) << " and " << Type::Name(rv.T) << "." ;
    return {};
}

Alu Alu::operator^(const Alu& rv) const
{
    Alu f;
    lrtext(rv)
    {
        throw AppBook::Except() << "cannot xor strings" ;
        return {};
    }

    lr_number(rv)
        return {v.u ^ rv.v.u};

    throw AppBook::Except() << "cannot xor " << Type::Name(T) << " and " << Type::Name(rv.T) << "." ;
    return {};
}

Alu Alu::operator<<(const Alu& rhs) const
{
    Alu f;
    lrtext(rhs)
    {
        Core::StrAcc str = text;
        str << rhs.text;
        return {str()};
    }

    lr_number(rhs)
        return {v.u << rhs.v.u};

    throw AppBook::Except() << "cannot execute bitwise left-shift or insert with " << Type::Name(T) << " and " << Type::Name(rhs.T) << "." ;
    return f;
}
Alu Alu::operator>>(const Alu& rhs) const
{
    Alu f;
    lrtext(rhs)
    {
        throw AppBook::Except() << "cannot execute bitwise right-shift or extract/write/send string on string " << Type::Name(T) << " and " << Type::Name(rhs.T) << "." ;
    }
    lr_number(rhs)
        return {v.u >> rhs.v.u};

    throw AppBook::Except() << "cannot execute bitwise right-shift or extract/write/send with " << Type::Name(T) << " and " << Type::Name(rhs.T) << "." ;
    return f;
}

Alu Alu::operator|(const Alu& rhs) const
{

    lrtext(rhs)
    {
        return {text + rhs.text};
    }
    lr_number(rhs)
        return {v.u | rhs.v.u};

    throw AppBook::Except() << "cannot execute bitwise or with " << Type::Name(T) << " and " << Type::Name(rhs.T) << "." ;
    return {};
}

Alu Alu::operator&(const Alu& rhs) const
{
    lrtext(rhs)
    {
        throw AppBook::Except() << "cannot execute bitwise and on string " ;
    }
    lr_number(rhs)
        return {v.u & rhs.v.u};

    throw AppBook::Except() << "cannot execute bitwise and between " << Type::Name(T) << " and " << Type::Name(rhs.T) << "." ;
    return {};
}

Alu Alu::operator~()
{
    istext(*this)
    {
        throw AppBook::Except() << "cannot execute bitwise invert on string " ;
    }

    is_number
    {
        if (T & Spp::Type::Float)
            return {1 / v.d};
        return {~v.u};
    }

    throw AppBook::Except() << "cannot execute bitwise invert on " << Type::Name(T) ;
    return {};
}

// assign operators:
Alu& Alu::operator+=(const Alu& rv)
{
    lrtext(rv)
    {
        text = text + rv.text;
        return *this;
    }

    lr_number(rv)
    {
        v.d += rv.v.d;
        return *this;
    }

    throw AppBook::Except() << "cannot execute assign add between " << Type::Name(T) << " and " << Type::Name(rv.T);
    return *this;
}

Alu& Alu::operator-=(const Alu& rv)
{
    lrtext(rv) return *this;
    //return remove_substr(rv.text);

    lr_number(rv)
    {
        v.d  -= rv.v.d;
        return *this;
    }

    throw AppBook::Except() << "cannot execute assign substraction between " << Type::Name(T) << " and " << Type::Name(rv.T) << "." ;;
    return *this;
}

Alu& Alu::operator*=(const Alu& rv)
{
    lrtext(rv)
    {
        throw AppBook::Except() << "cannot execute assign multiply on strings [" << (*this)() << " *= " << rv() << "]." ;
        return *this;
    }

    lr_number(rv)
    {
        AppBook::Debug()
            << Color::Orange5 << "Alu"
            << Color::White << "::"
            << Color::White << Book::Enums::Fn::Endl << ":" << Book::Enums::Fn::Endl << "{" << Book::Enums::Fn::Endl
            << Color::Yellow << v.d << Color::White << '(' << Color::LightSeaGreen << Type::Name(T) << Color::White << ')'
            << Color::CornflowerBlue << " *= "
            << Color::Yellow << rv.v.d << Color::White << '(' << Color::LightSeaGreen << Type::Name(rv.T) << Color::White << ')' ;

        v.d *= rv.v.d;
        AppBook::Out() << Color::White << " = " << Color::Yellow << v.d  << Color::White <<  Book::Enums::Fn::Endl << '}' <<  Book::Enums::Fn::Endl ;
        return *this;
    }

    throw AppBook::Except() << "cannot execute assign multiply between " << Type::Name(T) << " and " << Type::Name(rv.T) << "." ;
    return *this;
}

Alu& Alu::operator/=(const Alu& rv)
{
    lrtext(rv)
    {
        throw AppBook::Except() << "cannot execute assign divide on strings [" << (*this)() << " /= " << rv() << "]." ;
        return *this;
    }

    lr_number(rv)
    {

        if (v.d==0.0f)
        {
            throw AppBook::Except() << "Alu: cannot divide by zero." ; // Oops plutôt erreur fatale!!
            v.d = 0.0;
            T = Spp::Type::Null;
            return *this;
        }
        v.d /= rv.v.d;
        return *this;
    }
    throw AppBook::Except() << "cannot execute assign divide between " << Type::Name(T) << " and " << Type::Name(rv.T) << "." ;
    return *this;
}

Alu::~Alu(){}

Alu& Alu::operator%=(const Alu& rv)
{
    lrtext(rv) return *this;
    //return completely_remove_substr(rv.text);

    lr_number(rv)
    {
        v.u %= rv.v.u;
        return *this;
    }
    return *this;
}

Alu& Alu::operator|=(const Alu& rv)
{

    lrtext(rv)
    {
        throw AppBook::Except() << "cannot execute assign bitwise or on strings [" << (*this)() << " |= " << rv() << "]." ;
        return *this;
    }

    lr_number(rv)
    {
        v.u |= rv.v.u;
        return *this;
    }
    throw AppBook::Except() << "cannot execute assign bitwise or between " << Type::Name(T) << " and " << Type::Name(rv.T) << "." ;
    return *this;
}

Alu& Alu::operator&=(const Alu& rv)
{
    lrtext(rv)
    {
        throw AppBook::Except() << "cannot execute assign bitwise and on strings [" << (*this)() << " &= " << rv() << "]." ;
        return *this;
    }
    //    a = std::string(text + rv.text);
    //else
    lr_number(rv)
    {
        v.u &=  rv.v.u;
        return *this;
    }
    throw AppBook::Except() << "cannot execute assign bitwise and between " << Type::Name(T) << " and " << Type::Name(rv.T) << "." ;
    return *this;
}

Alu& Alu::operator^=(const Alu& rv)
{
    lrtext(rv)
    {
        throw AppBook::Except() << "cannot execute assign xor on strings [" << (*this)() << " ><= " << rv() << "]." ;
        return *this;
    }

    lr_number(rv)
    {
        /// @note xor ?
        v.u ^= rv.v.u;
        return *this;
    }

    throw AppBook::Except() << "cannot execute assign xor between " << Type::Name(T) << " and " << Type::Name(rv.T) << "." ;
    return *this;
}

// boolean operators:
Alu Alu::operator>(const Alu& rv)
{
    lrtext(rv)
        return {text > rv.text};

    lr_number(rv)
        return {v.d > rv.v.d};

    throw AppBook::Except() << "cannot execute relational operations between " << Type::Name(T) << " and " << Type::Name(rv.T) << "." ;
    return {false};
}

Alu Alu::operator<(const Alu& rv)
{
    lrtext(rv)
        return {text < rv.text};

    lr_number(rv)
        return {v.d < rv.v.d};

    throw AppBook::Except() << "cannot execute relational operations between " << Type::Name(T) << " and " << Type::Name(rv.T) << "." ;
    return {false};

}
Alu Alu::operator>=(const Alu& rv)
{
    lrtext(rv)
        return {text >= rv.text};

    lr_number(rv)
        return {v.d >= rv.v.d};

    throw AppBook::Except() << "cannot execute relational operations between " << Type::Name(T) << " and " << Type::Name(rv.T) << "." ;
    return {false};

}

Alu Alu::operator<=(const Alu& rv)
{
    lrtext(rv)
        return {text <= rv.text};

    lr_number(rv)
        return {v.d <= rv.v.d};

    throw AppBook::Except() << "cannot execute relational operations between " << Type::Name(T) << " and " << Type::Name(rv.T) << "." ;
    return {false};

}

Alu Alu::operator!=(const Alu& rv)
{
    lrtext(rv)
        return {text != rv.text};

    lr_number(rv)
        return {v.d != rv.v.d};

    throw AppBook::Except() << "cannot execute relational operations between " << Type::Name(T) << " and " << Type::Name(rv.T) << "." ;
    return {false};
}

Alu Alu::operator||(const Alu& rv)
{
    lrtext(rv)
        return {!text.empty() || (!rv.text.empty())};

    lr_number(rv)
        return {v.d || rv.v.d};

    throw AppBook::Except() << "cannot execute relational operations between " << Type::Name(T) << " and " << Type::Name(rv.T) ;
    return {false};
}

Alu Alu::operator&&(const Alu& rv)
{
    lrtext(rv)
        return {!text.empty() && (!rv.text.empty())};

    lr_number(rv)
        return {v.u && rv.v.u};

    throw AppBook::Except() << "cannot execute relational operations between " << Type::Name(T) << " and " << Type::Name(rv.T) << "." ;
    return {false};
}

bool Alu::operator!()
{
    return v.d ==0.0f;
}

Alu Alu::operator-()
{
    istext(*this)
    {
        throw AppBook::Except() << "cannot execute sign operator on string -(\"" << (*this)() << "\")." ;
    }
    throw AppBook::Except() << "cannot execute relational operations on " << Type::Name(T) ;
    return -v.d;
}

Alu Alu::operator+()
{
    istext(*this)
    {
        throw AppBook::Except() << "cannot execute sign operator on string +(\"" << (*this)() << "\")." ;
    }

    is_number
        if (v.d < 0.0f)
            return {v.d * -1};

    throw AppBook::Except() << "cannot execute relational operations on " << Type::Name(T) ;
    return {false};
}

Alu Alu::operator++(int)
{

    istext(*this)
    {
        throw AppBook::Except() << "cannot increment string (\"" << (*this)() << "\")++." ;
    }

    is_number
    {
        auto f = v.d;
        Alu    ff(f);
        ++f;
        v.d = f;
        return ff;
    }
    throw AppBook::Except() << "cannot execute increment operations on " << Type::Name(T) ;
    return {false};
}

Alu Alu::operator++()
{
    istext(*this)
    {
        throw AppBook::Except() << "cannot increment string ++(\"" << (*this)() << "\")." ;
    }

    is_number
    {
        auto f = v.d;
        ++f;
        v.d = f;
        return *this;
    }
    throw AppBook::Except() << "cannot execute increment operations on " << Type::Name(T) ;
    return {false};
}

Alu Alu::operator--()
{

    istext(*this)
    {
        throw AppBook::Except() << "cannot decrement string --(\"" << (*this)() << "\")." ;
    }

    is_number
    {
        auto f = v.d;
        ++f;
        v.d = f;
        return *this;
    }
    throw AppBook::Except() << "cannot execute decrement operations on " << Type::Name(T) ;
    return {false};
}

Alu Alu::operator--(int)
{
    istext(*this)
    {
        throw AppBook::Except() << "cannot decrement string (\"" << (*this)() << "\")++." ; // oops... What if long text string?
    }

    is_number
    {
        auto f = v.d;
        Alu    ff(f);
        ++f;
        v.d = f;
        return ff;
    }
    throw AppBook::Except() << "cannot execute decrement operations on " << Type::Name(T) ;
    return {false};
}

Alu Alu::Radical(const Alu& Lhs_) const
{
    Alu al;
    lr_number(Lhs_)
    {
        al = Alu(std::pow(v.d, 1 / Lhs_.v.d));
        return al;
    }

    throw AppBook::Except() << "cannot execute Radical operator on " << Type::Name(T) ;
    return {false};
}

//    Alu Alu::factorial(const Alu& Lhs_) const
//    {
//        Book::debug() << Color::Yellow << (*this)() << Color::Reset << ":" ;
//        lr_number(Lhs_)
//        {
//            int fac = 1;
//            int N = Lhs_.number<int>();
//            if (N >= 10)
//                return {};
//            for (int j = 1; j <= N; j++)
//                fac *= j;
//            Book::debug() << Color::Yellow << (*this)() << Color::Reset << ":" ;
//            return {fac};
//        }
//        throw AppBook::Except() << "cannot execute factorial operator on " << Type::Name(T) ;
//        return {false};
//    }

Alu Alu::Factorial() const
{
    AppBook::Debug() << Color::Yellow << (*this)() <<Color::White << ":"  ;
    lr_number(*this)
    {
        if(v.i < 0)
        {
            throw AppBook::Except() << " Invalid Base number :" << Color::Yellow << (*this)() << Color::Reset ;
            return {};
        }
        int fac = 1;
        int N = number<int>();
        if (N >= 10)
        {
            throw AppBook::Except() << " Base number too high." << Color::Yellow << (*this)() << Color::Reset ;
            return {};
        }
        for (int j = 1; j <= N; j++)
            fac *= j;
        AppBook::Debug() << Color::Yellow << fac <<Color::White << ":"  ;
        return { fac };
    }
    throw AppBook::Except() << "cannot execute Factorial operator on " << Type::Name(T) ;
    return { false };

}

Alu& Alu::operator<<=(const Alu& rv)
{

    lrtext(rv)
    {
        throw AppBook::Except() << "cannot execute assign extraction/bitwise left-shift / output on strings."  ;
        return *this;
    }

    lr_number(rv)
    {
        v.u = v.u << rv.v.u;
        return *this;
    }

    throw AppBook::Except() << "cannot execute assign bitwise left-shift or insert with " << Type::Name(T) << " and " << Type::Name(rv.T) << "." ;
    return *this;
}

Alu& Alu::operator>>=(const Alu& rv)
{

    lrtext(rv)
    {
        throw AppBook::Except() << "cannot execute assign extraction/bitwise right-shift / output on strings."  ;
        return *this;
    }

    lr_number(rv)
    {
        v.u = v.u << rv.v.u;
        return *this;
    }

    throw AppBook::Except() << "cannot execute assign bitwise right-shift or insert with " << Type::Name(T) << " and " << Type::Name(rv.T) << "." ;
    return *this;
}

//Alu Alu::remove_substr_copy(const std::string &to_erase)
//{
//
//    // J'suis paresseux :: copy-paste  (from thispointer :: how to remove substr: )
//    auto   cp  = std::any_cast<std::string>(a);
//    size pos = cp.find(to_erase);
//
//    if(pos != std::string::npos)
//    {
//        // If found then erase it from string
//
//        cp.erase(pos<< to_erase.length());
//    }
//    return Alu(cp);
//}
//
//Alu Alu::remove_all_substr_copy(const std::string &to_erase)
//{
//    // J'suis paresseux :: copy-paste  (from thispointer :: how to remove substr: )
//
//
//    size pos = std::string::npos;
//    auto   cp  = std::any_cast<std::string>(a);
//    // Search for the substring in string in a loop untill nothing is found
//    while((pos = cp.find(to_erase)) != std::string::npos)
//    {
//        // If found then erase it from string
//        cp.erase(pos<< to_erase.length());
//    }
//
//    return Alu(cp);
//}
//
//Alu &Alu::remove_substr(const std::string &to_erase)
//{
//
//    // J'suis paresseux :: copy-paste  (from thispointer :: how to remove substr: )
//    auto   cp  = std::any_cast<std::string &>(a);
//    size pos = cp.find(to_erase);
//
//    if(pos != std::string::npos)
//    {
//        // If found then erase it from string
//
//        cp.erase(pos<< to_erase.length());
//    }
//    return *this;
//}

[[maybe_unused]] bool Alu::IsNum() const
{
    return (T & (Spp::Type::Float | Spp::Type::Number));
}

//Alu &Alu::completely_remove_substr(const std::string &to_erase)
//{
//    // J'suis paresseux :: copy-paste  (from thispointer :: how to remove substr: )
//
//
//    size pos = std::string::npos;
//    auto   cp  = std::any_cast<std::string &>(a);
//    // Search for the substring in string in a loop untill nothing is found
//    while((pos = cp.find(to_erase)) != std::string::npos)
//    {
//        // If found then erase it from string
//        cp.erase(pos<< to_erase.length());
//    }
//
//    return *this;
//}

[[maybe_unused]] std::string Alu::ToStr() const
{
    return (std::string)*this;
}

Alu::operator std::string() const
{
    Core::StrAcc str;
    if (T & Spp::Type::Bool)
        str << (v.u ? "true" : "false");
    else
    if (T & (Spp::Type::Number|Spp::Type::Float))
        str << v.d;
    else
    if (T & Spp::Type::Text)
        return text;
    else
    if (T & Spp::Type::VoidPtr) {
        str = "@[%ld]";
        str << v.p;
    }
    else
    if (T & Spp::Type::Null)
        str << "null";
    return str();
}


}


