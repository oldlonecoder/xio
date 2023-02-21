#include <xio/alu.h>

using namespace tux;
namespace xio
{

std::string  alu::mNil = "";

#define UCTor(rhs) { T |= xio::type::Number|xio::type::Leaf; v.d=(rhs); }
#define SCTor(rhs) { T |= xio::type::Number|xio::type::Sign|xio::type::Leaf;v.d=(rhs); }
#define _assign_u T |= xio::type::Number|xio::type::Leaf; v.d = (rhs);
#define _assign_s T |= xio::type::Number|xio::type::Sign|xio::type::Leaf;v.d = (rhs);

    alu::alu(int8_t rhs) : T(xio::type::I8)
    SCTor(rhs)
    alu::alu(int16_t rhs) : T(xio::type::I16)
    SCTor(rhs)
    alu::alu(int32_t rhs) : T(xio::type::I32)
    SCTor(rhs)
    alu::alu(int64_t rhs) : T(xio::type::I64)
    SCTor(static_cast<double>(rhs))
    alu::alu(uint8_t rhs) : T(xio::type::U8)
    UCTor(rhs)
    alu::alu(uint16_t rhs) : T(xio::type::U16)
    UCTor(rhs)
    alu::alu(uint32_t rhs) : T(xio::type::U32)
    UCTor(rhs)
    alu::alu(uint64_t rhs) : T(xio::type::U64)
    UCTor(static_cast<double>(rhs))
    alu::alu(bool rhs) : T(xio::type::Bool)
    SCTor(rhs)

#define cast(rhs)\
(rhs).T & xio::type::Bool  ? (rhs).v.i != false :\
(rhs).T & xio::type::I8       ? (rhs).v.i:\
(rhs).T & xio::type::I16      ? (rhs).v.i:\
(rhs).T & xio::type::I32      ? (rhs).v.i:\
(rhs).T & xio::type::I64      ? (rhs).v.i:\
(rhs).T & xio::type::U8       ? (rhs).v.u:\
(rhs).T & xio::type::U16      ? (rhs).v.u:\
(rhs).T & xio::type::U32      ? (rhs).v.u:\
(rhs).T & xio::type::U64      ? (rhs).v.u:\
(rhs).T & xio::type::Float    ? (rhs).v.d :0




    alu::alu(double rhs)
    {
        T = xio::type::Number | xio::type::Float | xio::type::Leaf;
        v.d = rhs;
    }

    alu::alu() {}

    alu::alu(float rhs)
    {
        T = xio::type::Number | xio::type::Float | xio::type::Leaf;
        v.d = (double)rhs;
    }

    alu::alu(const std::string& rhs)
    {
        T = xio::type::Text | xio::type::Leaf;
        v.t = &text; text = rhs;
    }

    alu::alu(const char* rhs)
    {

        T = xio::type::Text | xio::type::Leaf;
        v.t = &text; text = rhs;
    }

    alu::alu(void* rhs)
    {
        T = xio::type::Any | xio::type::VoidPtr;
        v.p = rhs;
    }

    bool alu::type_size(const alu& Rhs_) const
    {
        using namespace xio::type;
        xio::type::T me = T & (I8 | I16 | I32 | I64 | U8 | U16 | U32 | U64 | Float);
        xio::type::T r = Rhs_.T & (I8 | I16 | I32 | I64 | U8 | U16 | U32 | U64 | Float);
        return me - r >= 0;
    }

    alu::operator bool()
    {
        if (!v.i)
            return false;
        if (!T)
            return false;
        if (T & xio::type::Text)
            return !text.empty();
        if (T & xio::type::Number)
            return v.d != 0.0f;
        if (T & xio::type::VoidPtr)
            return value<void *>() != nullptr;
        return false;
    }

#define lr_number(rv) if ((T&xio::type::Number) && ((rv).T&xio::type::Number))
#define lrext(rv)   if ((T&xio::type::Text) && ((rv).T&xio::type::Text))
#define lr_any(rv)    if ((T&xio::type::Any)  && ((rv).T&xio::type::Any))
#define is_integrale(rv) if (!((rv).T&xio::type::Float))
#define isext(i)    if((i).T & xio::type::Text)
#define is_number if ((T&(xio::type::Float|xio::type::Number)))
    // #define delstring_if(rhs) if ((T&xio::type::Text) && (!(rhs).T & xio::type::Text)) delete v..t;  Vieux stuff

    alu& alu::operator=(const alu& rhs)
    {
        if (&rhs != this)
        {
            T = rhs.T;
            v = rhs.v;
        }
        return *this;
    }

    alu& alu::operator=(alu&& rhs) noexcept
    {
        std::swap(T, rhs.T);
        std::swap(v, rhs.v);
        return *this;
    }

    alu alu::operator==(const alu& rhs) const
    {

        if (T & xio::type::Number)
            return {(bool)(((cast(*this)) == (cast(rhs))))};
        if (T & xio::type::Text)
            return {text == rhs.text};

        if (T & xio::type::Any)
            return v.p == rhs.v.p;
        return {false};
    }

[[maybe_unused]] std::string alu::types() const
    {
        std::string str;
        return xio::type::name(T);
    }

    alu alu::operator+(const alu& rhs) const
    {
        lrext(rhs)
            return {text + rhs.text};
        else lr_number(rhs)
            return {v.d + rhs.v.d}; // Slow?

        std::string str;
        diagnostic::warning() << " [" << xio::type::name(T) << "] and [" << xio::type::name(rhs.T) << "] are incompatible.";
        return {false};
    }

    alu alu::operator[](alu::CNV c)
    {
        isext(*this)
        {
            return {0.0f};
        }
        auto d = v.d;

        alu    a;
        switch (c)
        {
            case alu::DEG:
                a = d * 180 / 3.141592653589793;
                break;
            case alu::RAD:
                a = d * 3.141592653589793 / 180;
                break;
            default:
                a = 0.0;
                break;
        }
        return a;
    }

    alu alu::operator-(const alu& rv)
    {

        lrext(rv)
        {
            diagnostic::warning() << " substraction operator strings is not yet supported";
            //return remove_substr_copy(rv.text);
            return *this;
        }

        lr_number(rv)
            return {v.d - rv.v.d};
        lr_any(rv)
            return {reinterpret_cast<uint64_t>(value<void *>()) - reinterpret_cast<uint64_t>(rv.value<void *>())};

        diagnostic::warning() << "cannot apply sign on " << xio::type::name(T);
        return {false};
    }


    alu alu::operator*(const alu& rv) const
    {
        alu f;
        lrext(rv)
        {
            std::size_t pos = rv.text.find(text);
            return {pos != std::string::npos};
        }

        //else
        lr_number(rv)
            return {v.d * rv.v.d};

        diagnostic::warning() << "cannot multiply" << xio::type::name(T) << " and " << xio::type::name(rv.T) << ".";
        return {};
    }

    alu alu::operator/(const alu& rv)
    {
        lrext(rv)
        {
            diagnostic::warning() << "cannot divide strings";
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
                diagnostic::except() << " In alu operation: division by zero error. (" << (*this)() << " / " << rv() << ")";
                return "Inf";
            }
            return {lr / vr};
        }

        diagnostic::warning() << "cannot divide " << xio::type::name(T) << " and " << xio::type::name(rv.T) << ".";
        return {};
    }

    alu alu::operator%(const alu& rv) const
    {
        alu f;
        lrext(rv)
        {
            diagnostic::warning() << "cannot modulo strings";
            return {};
        }
        lr_number(rv)
            return {v.u % rv.v.u};

        diagnostic::warning() << "cannot modulo " << xio::type::name(T) << " and " << xio::type::name(rv.T) << ".";
        return {};
    }

    alu alu::operator^(const alu& rv) const
    {
        alu f;
        lrext(rv)
        {
            diagnostic::warning() << "cannot xor strings";
            return {};
        }

        lr_number(rv)
            return {v.u ^ rv.v.u};

        diagnostic::warning() << "cannot xor " << xio::type::name(T) << " and " << xio::type::name(rv.T) << ".";
        return {};
    }

    alu alu::operator<<(const alu& rhs) const
    {
        alu f;
        lrext(rhs)
        {
            stracc str = text;
            str << rhs.text;
            return {str()};
        }

        lr_number(rhs)
            return {v.u << rhs.v.u};

        diagnostic::warning() << "cannot execute bitwise left-shift or insert with " << xio::type::name(T) << " and " << xio::type::name(rhs.T) << ".";
        return f;
    }
    alu alu::operator>>(const alu& rhs) const
    {
        alu f;
        lrext(rhs)
        {
            diagnostic::warning() << "cannot execute bitwise right-shift or extract/write/send string on string " << xio::type::name(T) << " and " << xio::type::name(rhs.T) << ".";
        }
        lr_number(rhs)
            return {v.u >> rhs.v.u};

        diagnostic::warning() << "cannot execute bitwise right-shift or extract/write/send with " << xio::type::name(T) << " and " << xio::type::name(rhs.T) << ".";
        return f;
    }

    alu alu::operator|(const alu& rhs) const
    {

        lrext(rhs)
        {
            return {text + rhs.text};
        }
        lr_number(rhs)
            return {v.u | rhs.v.u};

        diagnostic::warning() << "cannot execute bitwise or with " << xio::type::name(T) << " and " << xio::type::name(rhs.T) << ".";
        return {};
    }

    alu alu::operator&(const alu& rhs) const
    {
        lrext(rhs)
        {
            diagnostic::warning() << "cannot execute bitwise and on string ";
        }
        lr_number(rhs)
            return {v.u & rhs.v.u};

        diagnostic::warning() << "cannot execute bitwise and between " << xio::type::name(T) << " and " << xio::type::name(rhs.T) << ".";
        return {};
    }

    alu alu::operator~()
    {
        isext(*this)
        {
            diagnostic::warning() << "cannot execute bitwise invert on string ";
        }

        is_number
        {
            if (T & xio::type::Float)
                return {1 / v.d};
            return {~v.u};
        }

        diagnostic::warning() << "cannot execute bitwise invert on " << xio::type::name(T);
        return {};
    }

    // assign operators:
    alu& alu::operator+=(const alu& rv)
    {
        lrext(rv)
        {
            text = text + rv.text;
            return *this;
        }

        lr_number(rv)
        {
            v.d += rv.v.d;
            return *this;
        }

        diagnostic::warning() << "cannot execute assign add between " << xio::type::name(T) << " and " << xio::type::name(rv.T) << ".";
        return *this;
    }

    alu& alu::operator-=(const alu& rv)
    {
        lrext(rv) return *this;
        //return remove_substr(rv.text);

        lr_number(rv)
        {
            v.d  -= rv.v.d;
            return *this;
        }

        diagnostic::warning() << "cannot execute assign substraction between " << xio::type::name(T) << " and " << xio::type::name(rv.T) << ".";
        return *this;
    }

    alu& alu::operator*=(const alu& rv)
    {
        lrext(rv)
        {
            diagnostic::warning() << "cannot execute assign multiply on strings [" << (*this)() << " *= " << rv() << "].";
            return *this;
        }

        lr_number(rv)
        {
            diagnostic::debug()
                << color::Orange5 << "alu"
                << color::White << "::"

                << color::White << code::endl << ":" << code::endl << "{" << code::endl
                << color::Yellow << v.d << color::White << '(' << color::LightSeaGreen << xio::type::name(T) << color::White << ')'
                << color::CornflowerBlue << " *= "
                << color::Yellow << rv.v.d << color::White << '(' << color::LightSeaGreen << xio::type::name(rv.T) << color::White << ')';

            v.d *= rv.v.d;
            diagnostic::output() << color::White << " = " << color::Yellow << v.d  << color::White << code::endl << '}' << code::endl;
            return *this;
        }

        diagnostic::warning() << "cannot execute assign multiply between " << xio::type::name(T) << " and " << xio::type::name(rv.T) << ".";
        return *this;
    }

    alu& alu::operator/=(const alu& rv)
    {
        lrext(rv)
        {
            diagnostic::warning() << "cannot execute assign divide on strings [" << (*this)() << " /= " << rv() << "].";
            return *this;
        }

        lr_number(rv)
        {

            if (v.d==0.0f)
            {
                diagnostic::warning() << "alu: cannot divide by zero."; // Oops plutôt erreur fatale!!
                v.d = 0.0;
                T = xio::type::Null;
                return *this;
            }
            v.d /= rv.v.d;
            return *this;
        }
        diagnostic::warning() << "cannot execute assign divide between " << xio::type::name(T) << " and " << xio::type::name(rv.T) << ".";
        return *this;
    }

    alu::~alu(){}

    alu& alu::operator%=(const alu& rv)
    {
        lrext(rv) return *this;
        //return completely_remove_substr(rv.text);

        lr_number(rv)
        {
            v.u %= rv.v.u;
            return *this;
        }
        return *this;
    }

    alu& alu::operator|=(const alu& rv)
    {

        lrext(rv)
        {
            diagnostic::warning() << "cannot execute assign bitwise or on strings [" << (*this)() << " |= " << rv() << "].";
            return *this;
        }

        lr_number(rv)
        {
            v.u |= rv.v.u;
            return *this;
        }
        diagnostic::warning() << "cannot execute assign bitwise or between " << xio::type::name(T) << " and " << xio::type::name(rv.T) << ".";
        return *this;
    }

    alu& alu::operator&=(const alu& rv)
    {
        lrext(rv)
        {
            diagnostic::warning() << "cannot execute assign bitwise and on strings [" << (*this)() << " &= " << rv() << "].";
            return *this;
        }
        //    a = std::string(text + rv.text);
        //else
        lr_number(rv)
        {
            v.u &=  rv.v.u;
            return *this;
        }
        diagnostic::warning() << "cannot execute assign bitwise and between " << xio::type::name(T) << " and " << xio::type::name(rv.T) << ".";
        return *this;
    }

    alu& alu::operator^=(const alu& rv)
    {
        lrext(rv)
        {
            diagnostic::warning() << "cannot execute assign xor on strings [" << (*this)() << " ><= " << rv() << "].";
            return *this;
        }

        lr_number(rv)
        {
            /// @note xor ?
            v.u ^= rv.v.u;
            return *this;
        }

        diagnostic::warning() << "cannot execute assign xor between " << xio::type::name(T) << " and " << xio::type::name(rv.T) << ".";
        return *this;
    }

    // boolean operators:
    alu alu::operator>(const alu& rv)
    {
        lrext(rv)
            return {text > rv.text};

        lr_number(rv)
            return {v.d > rv.v.d};

        diagnostic::warning() << "cannot execute relational operations between " << xio::type::name(T) << " and " << xio::type::name(rv.T) << ".";
        return {false};
    }

    alu alu::operator<(const alu& rv)
    {
        lrext(rv)
            return {text < rv.text};

        lr_number(rv)
            return {v.d < rv.v.d};

        diagnostic::warning() << "cannot execute relational operations between " << xio::type::name(T) << " and " << xio::type::name(rv.T) << ".";
        return {false};

    }
    alu alu::operator>=(const alu& rv)
    {
        lrext(rv)
            return {text >= rv.text};

        lr_number(rv)
            return {v.d >= rv.v.d};

        diagnostic::warning() << "cannot execute relational operations between " << xio::type::name(T) << " and " << xio::type::name(rv.T) << ".";
        return {false};

    }

    alu alu::operator<=(const alu& rv)
    {
        lrext(rv)
            return {text <= rv.text};

        lr_number(rv)
            return {v.d <= rv.v.d};

        diagnostic::warning() << "cannot execute relational operations between " << xio::type::name(T) << " and " << xio::type::name(rv.T) << ".";
        return {false};

    }

    alu alu::operator!=(const alu& rv)
    {
        lrext(rv)
            return {text != rv.text};

        lr_number(rv)
            return {v.d != rv.v.d};

        diagnostic::warning() << "cannot execute relational operations between " << xio::type::name(T) << " and " << xio::type::name(rv.T) << ".";
        return {false};
    }

    alu alu::operator||(const alu& rv)
    {
        lrext(rv)
            return {!text.empty() || (!rv.text.empty())};

        lr_number(rv)
            return {v.d || rv.v.d};

        diagnostic::warning() << "cannot execute relational operations between " << xio::type::name(T) << " and " << xio::type::name(rv.T) << ".";
        return {false};
    }

    alu alu::operator&&(const alu& rv)
    {
        lrext(rv)
            return {!text.empty() && (!rv.text.empty())};

        lr_number(rv)
            return {v.u && rv.v.u};

        diagnostic::warning() << "cannot execute relational operations between " << xio::type::name(T) << " and " << xio::type::name(rv.T) << ".";
        return {false};
    }

    bool alu::operator!()
    {
        return v.d ==0.0f;
    }

    alu alu::operator-()
    {
        isext(*this)
        {
            diagnostic::warning() << "cannot execute sign operator on string -(\"" << (*this)() << "\").";
        }
        diagnostic::warning() << "cannot execute relational operations on " << xio::type::name(T);
        return -v.d;
    }

    alu alu::operator+()
    {
        isext(*this)
        {
            diagnostic::warning() << "cannot execute sign operator on string +(\"" << (*this)() << "\").";
        }

        is_number
            if (v.d < 0.0f)
                return {v.d * -1};

        diagnostic::warning() << "cannot execute relational operations on " << xio::type::name(T);
        return {false};
    }

    alu alu::operator++(int)
    {

        isext(*this)
        {
            diagnostic::warning() << "cannot increment string (\"" << (*this)() << "\")++.";
        }

        is_number
        {
            auto f = v.d;
            alu    ff(f);
            ++f;
            v.d = f;
            return ff;
        }
        diagnostic::warning() << "cannot execute increment operations on " << xio::type::name(T);
        return {false};
    }

    alu alu::operator++()
    {
        isext(*this)
        {
            diagnostic::warning() << "cannot increment string ++(\"" << (*this)() << "\").";
        }

        is_number
        {
            auto f = v.d;
            ++f;
            v.d = f;
            return *this;
        }
        diagnostic::warning() << "cannot execute increment operations on " << xio::type::name(T);
        return {false};
    }

    alu alu::operator--()
    {

        isext(*this)
        {
            diagnostic::warning() << "cannot decrement string --(\"" << (*this)() << "\").";
        }

        is_number
        {
            auto f = v.d;
            ++f;
            v.d = f;
            return *this;
        }
        diagnostic::warning() << "cannot execute decrement operations on " << xio::type::name(T);
        return {false};
    }

    alu alu::operator--(int)
    {
        isext(*this)
        {
            diagnostic::warning() << "cannot decrement string (\"" << (*this)() << "\")++."; // oops... What if long text string?
        }

        is_number
        {
            auto f = v.d;
            alu    ff(f);
            ++f;
            v.d = f;
            return ff;
        }
        diagnostic::warning() << "cannot execute decrement operations on " << xio::type::name(T);
        return {false};
    }

    alu alu::radical(const alu& Lhs_) const
    {
        alu al;
        lr_number(Lhs_)
        {
            al = alu(std::pow(v.d, 1 / Lhs_.v.d));
            return al;
        }

        diagnostic::warning() << "cannot execute radical operator on " << xio::type::name(T);
        return {false};
    }

    alu alu::factorial(const alu& Lhs_) const
    {
        lr_number(Lhs_)
        {
            int fac = 1;
            int N = Lhs_.number<int>();
            if (N >= 10)
                return {};
            for (int j = 1; j <= N; j++)
                fac *= j;
            return {fac};
        }
        diagnostic::warning() << "cannot execute factorial operator on " << xio::type::name(T);
        return {false};
    }

    alu alu::factorial() const
    {
        lr_number(*this)
        {
            int fac = 1;
            int N = number<int>();
            if (N >= 10)
                return {};
            for (int j = 1; j <= N; j++)
                fac *= j;
            return { fac };
        }
        diagnostic::warning() << "cannot execute factorial operator on " << xio::type::name(T);
        return { false };

    }

    alu& alu::operator<<=(const alu& rv)
    {

        lrext(rv)
        {
            diagnostic::warning() << "cannot execute assign extraction/bitwise left-shift / output on strings.";
            return *this;
        }

        lr_number(rv)
        {
            v.u = v.u << rv.v.u;
            return *this;
        }

        diagnostic::warning() << "cannot execute assign bitwise left-shift or insert with " << xio::type::name(T) << " and " << xio::type::name(rv.T) << ".";
        return *this;
    }

    alu& alu::operator>>=(const alu& rv)
    {

        lrext(rv)
        {
            diagnostic::warning() << "cannot execute assign extraction/bitwise right-shift / output on strings.";
            return *this;
        }

        lr_number(rv)
        {
            v.u = v.u << rv.v.u;
            return *this;
        }

        diagnostic::warning() << "cannot execute assign bitwise right-shift or insert with " << xio::type::name(T) << " and " << xio::type::name(rv.T) << ".";
        return *this;
    }

    //alu alu::remove_substr_copy(const std::string &to_erase)
    //{
    //
    //    // J'suis paresseux :: copy-paste here (from thispointer :: how to remove substr: )
    //    auto   cp  = std::any_cast<std::string>(a);
    //    size pos = cp.find(to_erase);
    //
    //    if(pos != std::string::npos)
    //    {
    //        // If found then erase it from string
    //
    //        cp.erase(pos<< to_erase.length());
    //    }
    //    return alu(cp);
    //}
    //
    //alu alu::remove_all_substr_copy(const std::string &to_erase)
    //{
    //    // J'suis paresseux :: copy-paste here (from thispointer :: how to remove substr: )
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
    //    return alu(cp);
    //}
    //
    //alu &alu::remove_substr(const std::string &to_erase)
    //{
    //
    //    // J'suis paresseux :: copy-paste here (from thispointer :: how to remove substr: )
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

[[maybe_unused]] bool alu::isnum() const
    {
        return (T & (xio::type::Float | xio::type::Number));
    }

    //alu &alu::completely_remove_substr(const std::string &to_erase)
    //{
    //    // J'suis paresseux :: copy-paste here (from thispointer :: how to remove substr: )
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

[[maybe_unused]] std::string alu::to_str() const
    {
        return (std::string)*this;
    }

    alu::operator std::string() const
    {
        stracc str;
        if (T & xio::type::Bool)
            str << (v.u ? "true" : "false");
        else
            if (T & (xio::type::Number|xio::type::Float))
                str << v.d;
            else
                if (T & xio::type::Text)
                    return text;
                else
                    if (T & xio::type::VoidPtr) {
                        str = "@[%ld]";
                        str << v.p;
                    }
                    else
                        if (T & xio::type::Null)
                            str << "null";
        return str();
    }
}

