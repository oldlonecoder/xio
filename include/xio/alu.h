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
 *   Open source FREE licences also apply to the code from the same author (Serge Lussier)*
 *   ----------------------------------------------------------------------------------   *
 ******************************************************************************************/


#pragma once


//#include "logbook/expect.h"
#include "xio/types.h"
#include <memory>
#include <vector>
#include <map>


namespace xio
{
    /*!
     * @brief Arithmetic Logical Unit ( Sort of...)
     *
     *
     * One local instance of/or pointer-to alu is held by each and every xio; alu takes the role of the xio accumulator
     * holding one and only one value (accumulator) /arithmetic logical unit. On the active role, alu also performs the base arithmetic operations between alu's within
     * xio execution unit.
     * @author &copy; 2014(using union),2020(using std::any),2022(back to union); Serge Lussier, oldlonecoder/bretzel/Bretzelus/Bretzel xio, (lussier.serge@gmail.com)
     *
     */

struct XIO_PUBLIC alu
{
    using list = std::vector<alu>;
    using linkarray = std::vector<alu*>;
    using shared = std::shared_ptr<alu>;

    static std::string mNil;

    xio::type::T  T = xio::type::Float; ///< Default type.

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

    enum XIO_PUBLIC CNV {
        DEG,
        RAD
    };

    alu();

    alu(const alu&) = default;
    alu(double A_);
    alu(float A_);
    //alu(token* rhs);

    alu(const std::string & A_);
    alu(const char* A_);
    alu(void* A_);
    alu(alu&&) noexcept = default;
    alu(int8_t A_);
    alu(int16_t A_);
    alu(int32_t A_);
    alu(int64_t A_);
    alu(uint8_t A_);
    alu(uint16_t A_);
    alu(uint32_t A_);
    alu(uint64_t A_);
    alu(bool A_);



    alu& operator =(const alu & Lhs_);// { a = lf.a; T = lf.T; return *this; }
    alu& operator =(alu && rhs) noexcept = default;
    alu operator==(const alu & lf) const;

    [[nodiscard]] xio::type::T TypeOf() const { return T; } // [const] Sous r�serve; [const] provided that it is not screwing access...

#pragma region Huge_Overload
    template<typename oftype> [[nodiscard]]oftype value() const {

        if constexpr ((std::is_same<oftype, std::string>::value) || (std::is_same<oftype, const std::string&>::value))
            return (T & xio::type::Text ? text : "");
        else
            if constexpr (std::is_same<oftype, const char*>::value)
                return (T & xio::type::Text ? text.c_str() : nullptr);
            else
                if constexpr (std::is_same<oftype, void*>::value)
                    return v.p;
                else
                    return static_cast<oftype>(
                        T & xio::type::I8 ? v.i :
                        T & xio::type::I16 ? v.i :
                        T & xio::type::I32 ? v.i :
                        T & xio::type::I64 ? v.i :
                        T & xio::type::U8 ? v.u :
                        T & xio::type::U16 ? v.u :
                        T & xio::type::U32 ? v.u :
                        T & xio::type::U64 ? v.u :
                        T & xio::type::Bool ? v.u :
                        T & xio::type::Float ? v.d : 0
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
                            T & xio::type::I8 ? v.i :
                            T & xio::type::I16 ? v.i :
                            T & xio::type::I32 ? v.i :
                            T & xio::type::I64 ? v.i :
                            T & xio::type::U8 ? v.u :
                            T & xio::type::U16 ? v.u :
                            T & xio::type::U32 ? v.u :
                            T & xio::type::U64 ? v.u :
                            T & xio::type::Bool ? v.u :
                            T & xio::type::Float ? v.d : 0
                    );

    }

    template<xio::type::T V> bool is() {
        return T == V;
    }

#pragma endregion Huge_Overload
    [[maybe_unused]]bool isext() const { return T & xio::type::Text; }
    [[maybe_unused]] std::string to_str() const;
    [[maybe_unused]] bool isnum() const;
    [[maybe_unused]] [[nodiscard]]std::string types() const;
    virtual ~alu();



    // Arithmetic operations overload between two alu's:

    // Binary operators:

    alu operator +(const alu & Rhs_) const;
    alu operator -(const alu & Rhs_);
    alu operator *(const alu & Rhs_) const;
    alu operator [](alu::CNV Cnv_);

    alu operator /(const alu & Rhs_);
    alu operator %(const alu & Rhs_) const;
    alu operator ^(const alu & Rhs_) const;
    alu operator <<(const alu & Lhs_) const;
    alu operator >>(const alu & Lhs_) const;
    alu operator |(const alu & Lhs_) const;
    alu operator &(const alu & Lhs_) const;
    alu operator ~();
    // assign operators:
    alu& operator +=(const alu & Rhs_);
    alu& operator -=(const alu & Rhs_);
    alu& operator *=(const alu & Rhs_);
    alu& operator /=(const alu & Rhs_);
    alu& operator %=(const alu & Rhs_);
    alu& operator |=(const alu & Rhs_);
    alu& operator &=(const alu & Rhs_);
    alu& operator ^=(const alu & Rhs_);

    alu& operator <<=(const alu & Rhs_);
    alu& operator >>=(const alu & Rhs_);


    // boolean operators:
    alu operator >(const alu & Rhs_);
    alu operator <(const alu & Rhs_);
    alu operator >=(const alu & Rhs_);
    alu operator <=(const alu & Rhs_);
    alu operator !=(const alu & Rhs_);
    alu operator ||(const alu & Rhs_);
    alu operator &&(const alu & Rhs_);

    bool operator !();
    //bool IsSet() { return r.has_value(); }
    operator bool();

    alu operator -();
    alu operator +();

    alu operator ++(int);
    alu operator ++();
    alu operator --();
    alu operator --(int);

    alu radical(const alu & Lhs_) const;
    //alu factorial(const alu & Lhs_) const; //?????
    alu factorial() const;
    bool type_size(const alu & Rhs_) const;
    std::string operator()()const { return (std::string) *this; }
    explicit operator std::string() const;

    //    private:
    //    alu remove_substr_copy(const std::string& to_erase);
    //    alu completely_remove_substr_copy(const std::string& to_erase);
    //    alu& remove_substr(const std::string& to_erase);
    //    alu& completely_remove_substr(const std::string& to_erase);


};
}

