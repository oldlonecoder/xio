/******************************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                                        *
 *   serge.lussier@oldlonecoder.club                                                      *
 *                                                                                        *
 *                                                                                        *
 *   Unless otherwise specified, all code in this project is written                      *
 *   by the author (Serge Lussier)                                                        *
 *   and no one else then not even {copilot, chatgpt, or any other AI calamities}         *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge Lussier also apply here                     *
 *   Open source FREE licences also apply To the code from the same author (Serge Lussier)*
 *   ----------------------------------------------------------------------------------   *
 ******************************************************************************************/


#pragma once


//#include "logbook/expect.h"
#include "Spp/SppTypes.h"
#include <memory>
#include <vector>
#include <map>
#include <stack>


namespace Spp
{
/*!
 * @brief Arithmetic Logical Unit ( Sort of...)
 *
 *
 * One local instance of/or pointer-To alu is held by each and every Spp; alu takes the role of the Spp accumulator
 * holding one and only one Value (accumulator) /arithmetic logical unit. On the active role, alu also performs the base arithmetic operations between alu's within
 * Spp execution unit.
 * @author &copy; 2014(using union),2020(using std::any),2022(back To union); Serge Lussier, oldlonecoder/bretzel/Bretzelus/Bretzel Spp, (lussier.serge@gmail.com)
 *
 */

struct SPP_EXPORT Alu
{
    using Array = std::vector<Alu>;
    using LinkArray = std::vector<Alu*>;
    using Shared = std::shared_ptr<Alu>;

    static std::string mNil;

    Spp::Type::T  T = Spp::Type::Float; ///< Default Type.

    std::string text;
    union piudt
    {// Create std::string pool for managing assign-release cycle of std::string* in alu;
        std::string* t;
        double       d;
        uint64_t     u;
        int64_t      i;
        void*        p;

        piudt& operator=(double v_) { d = v_; return *this;}
        piudt& operator=(uint64_t v_) { u = v_; return *this;}
        piudt& operator=(int64_t v_) { i = v_; return *this;}
        piudt& operator=(void* v_) { p = v_; return *this;}
        piudt& operator=(std::string* v_) { t = v_; return *this;}
    }v = {0};

    enum SPP_EXPORT CNV {
        DEG,
        RAD
    };

    Alu();

    Alu(const Alu&) = default;
    Alu(double A_);
    Alu(float A_);
    //alu(token* rhs);

    Alu(const std::string & A_);
    Alu(const char* A_);
    Alu(void* A_);
    Alu(Alu&&) noexcept = default;
    Alu(int8_t A_);
    Alu(int16_t A_);
    Alu(int32_t A_);
    Alu(int64_t A_);
    Alu(uint8_t A_);
    Alu(uint16_t A_);
    Alu(uint32_t A_);
    Alu(uint64_t A_);
    Alu(bool A_);



    Alu& operator =(const Alu & Lhs_);// { a = lf.a; T = lf.T; return *this; }
    Alu& operator =(Alu && rhs) noexcept = default;
    Alu operator==(const Alu & lf) const;

    [[nodiscard]] Spp::Type::T TypeOf() const { return T; } // [const] Sous r�serve; [const] provided that it is not screwing access...

#pragma region Huge_Overload
    template<typename ofType> [[nodiscard]]ofType value() const {

        if constexpr ((std::is_same<ofType, std::string>::value) || (std::is_same<ofType, const std::string&>::value))
            return (T & Spp::Type::Text ? text : "");
        else
        if constexpr (std::is_same<ofType, const char*>::value)
            return (T & Spp::Type::Text ? text.c_str() : nullptr);
        else
        if constexpr (std::is_same<ofType, void*>::value)
            return v.p;
        else
            return static_cast<ofType>(
                T & Spp::Type::I8 ? v.i :
                T & Spp::Type::I16 ? v.i :
                T & Spp::Type::I32 ? v.i :
                T & Spp::Type::I64 ? v.i :
                T & Spp::Type::U8 ? v.u :
                T & Spp::Type::U16 ? v.u :
                T & Spp::Type::U32 ? v.u :
                T & Spp::Type::U64 ? v.u :
                T & Spp::Type::Bool ? v.u :
                T & Spp::Type::Float ? v.d : 0
            );
    }

    template<typename ofype> [[nodiscard]] ofype number() const
    {
        if constexpr (std::is_same<ofype, void*>::value)
            return v.p;
        else
        if constexpr (std::is_same<ofype, std::string>::value)
            return (std::string)*this;
        else
            return static_cast<ofype>(
                T & Spp::Type::I8 ? v.i :
                T & Spp::Type::I16 ? v.i :
                T & Spp::Type::I32 ? v.i :
                T & Spp::Type::I64 ? v.i :
                T & Spp::Type::U8 ? v.u :
                T & Spp::Type::U16 ? v.u :
                T & Spp::Type::U32 ? v.u :
                T & Spp::Type::U64 ? v.u :
                T & Spp::Type::Bool ? v.u :
                T & Spp::Type::Float ? v.d : 0
            );

    }

    template<Spp::Type::T V> bool is() {
        return T == V;
    }

#pragma endregion Huge_Overload
    [[maybe_unused]]bool IsText() const { return T & Spp::Type::Text; }
    [[maybe_unused]] std::string ToStr() const;
    [[maybe_unused]] bool IsNum() const;
    [[maybe_unused]] [[nodiscard]]std::string Types() const;
    virtual ~Alu();



    // Arithmetic operations overload between two alu's:

    // Binary operators:

    Alu operator +(const Alu & Rhs_) const;
    Alu operator -(const Alu & Rhs_);
    Alu operator *(const Alu & Rhs_) const;
    Alu operator [](Alu::CNV Cnv_);

    Alu operator /(const Alu & Rhs_);
    Alu operator %(const Alu & Rhs_) const;
    Alu operator ^(const Alu & Rhs_) const;
    Alu operator <<(const Alu & Lhs_) const;
    Alu operator >>(const Alu & Lhs_) const;
    Alu operator |(const Alu & Lhs_) const;
    Alu operator &(const Alu & Lhs_) const;
    Alu operator ~();
    // assign operators:
    Alu& operator +=(const Alu & Rhs_);
    Alu& operator -=(const Alu & Rhs_);
    Alu& operator *=(const Alu & Rhs_);
    Alu& operator /=(const Alu & Rhs_);
    Alu& operator %=(const Alu & Rhs_);
    Alu& operator |=(const Alu & Rhs_);
    Alu& operator &=(const Alu & Rhs_);
    Alu& operator ^=(const Alu & Rhs_);

    Alu& operator <<=(const Alu & Rhs_);
    Alu& operator >>=(const Alu & Rhs_);


    // boolean operators:
    Alu operator >(const Alu & Rhs_);
    Alu operator <(const Alu & Rhs_);
    Alu operator >=(const Alu & Rhs_);
    Alu operator <=(const Alu & Rhs_);
    Alu operator !=(const Alu & Rhs_);
    Alu operator ||(const Alu & Rhs_);
    Alu operator &&(const Alu & Rhs_);

    bool operator !();
    //bool IsSet() { return r.has_value(); }
    operator bool();

    Alu operator -();
    Alu operator +();

    Alu operator ++(int);
    Alu operator ++();
    Alu operator --();
    Alu operator --(int);

    Alu Radical(const Alu & Lhs_) const;
    //alu factorial(const alu & Lhs_) const; //?????
    Alu Factorial() const;
    bool TypeSize(const Alu & Rhs_) const;
    std::string operator()()const { return (std::string) *this; }
    explicit operator std::string() const;

    //    private:
    //    alu remove_substr_copy(const std::string& to_erase);
    //    alu completely_remove_substr_copy(const std::string& to_erase);
    //    alu& remove_substr(const std::string& to_erase);
    //    alu& completely_remove_substr(const std::string& to_erase);


};
}

