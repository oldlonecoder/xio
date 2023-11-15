
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

#include "Spp/SppTypes.h"
#include <AppBook/Util/StrAcc.h>

//#include "logbook/expect.h"
#include <vector>


namespace Spp
{

struct SPP_EXPORT SppToken
{
    Mnemonic    M{Mnemonic::Noop};
    Type::T     T{Type::Null};    ///< Primitive Type bit.
    Type::T     S{Type::Null};    ///< Semantic Type bits field
    Spp::Distance::T D{Spp::Distance::Noop_};
    void* Data{nullptr};

//        // --------------------- Add link pointers to previous and forward tokens because there situations where giving the std container is so overload and overbloated!
//        SppToken* __prev{nullptr};
//        SppToken* __next{nullptr};
//        // --------------------------------------------------------------------------------------------------------------------------------------------------------------

    using Array     = std::vector<SppToken>;
    // ----------------------------------------
    using ArrayPtr    = std::vector<SppToken*>;
    using Ptr  = ArrayPtr::iterator;
    // ----------------------------------------
    using Iterator = Array::iterator;


    struct SPP_EXPORT LocationData
    {
        const char* begin   = nullptr;
        const char* end     = nullptr;
        int         Linenum = -1; ///< line number
        int         Colnum  = -1; ///< Column number
        int64_t     Offset  = -1; ///< Absolute Offset from the beginning of the Source Text.
        size_t      Length      = 0; ///< Length of this TokenPtr Attribute.
        std::string operator()() const;
        std::string Text() const;
        std::string Position() const;
    }Location;

    struct SPP_EXPORT FlagsBit
    {
        uint8_t V : 1; ///< Pre-parsed as a Value Token;
        uint8_t S : 1; ///< Post parsed as assignable
        uint8_t M : 1; ///< Virtual multiplication operator. (...4ac...)
    }   Flags = { 0, 0, 0 };

    static SppToken NullToken;

    SppToken() = default;
    ~SppToken() = default;

    SppToken(Mnemonic Code_, Type::T Type_, Type::T Sem_, Spp::Distance::T Delta_, Lexem::T aLexem, uint8_t V);
    SppToken(Mnemonic Code_, Type::T Type_, Type::T Sem_, Spp::Distance::T Delta_, SppToken::LocationData LocationData_, SppToken::FlagsBit Flags_, void* Ptr_ = nullptr);
    SppToken(const SppToken& Token_);
    SppToken(SppToken&& Token_) noexcept;
    SppToken& operator=(SppToken&& Token_) noexcept;
    SppToken& operator=(const SppToken& Token_);
    SppToken operator[](Mnemonic CC);
    [[nodiscard]] std::string Mark(int nspc = 0) const;
    [[nodiscard]] std::string TextLine() const;
    //SppToken* back_to_startof_line();

    explicit operator bool() const
    {
        return Location.begin != nullptr;
    }
    bool operator||(Type::T Sem_) const
    {
        return S || Sem_;
    }

    [[nodiscard]] std::string Text() const
    {
        if ((Flags.M) && (M == Mnemonic::Mul)) return Lexem::Multiply; // Overwrite source location.
        return Location.Text();
    }
    [[nodiscard]] std::string LocationText() const;
    [[nodiscard]] std::string SemanticText() const;
    [[nodiscard]] std::string TypeName() const;
    [[nodiscard]] std::string Details(bool mark_it = false) const;

    static SppToken Scan(const char* C_);
    static std::string DumpTokenTable();

    [[nodiscard]] bool IsText() const
    {
        return S & Type::Text;
    }
    [[nodiscard]] bool IsBinary() const
    {
        return S & Type::Binary;
    }
    [[nodiscard]] bool IsUnary() const
    {
        return S & Type::Unary;
    }
    [[nodiscard]] bool IsIdentifier() const
    {
        return S & Type::Id;
    }
    [[nodiscard]] bool IsLeaf() const
    {
        return S & Type::Leaf;
    }
    [[nodiscard]] bool IsOperator() const
    {
        return S & Type::Operator;
    }
    [[nodiscard]] bool IsClosingPair() const
    {
        return S & Type::ClosePair;
    }
    [[nodiscard]] bool IsPunctuation() const
    {
        return S & Type::Punc;
    }
    [[nodiscard]] bool IsNumber() const
    {
        return S & Type::Number;
    }
    [[nodiscard]] bool IsHexadecimal() const
    {
        return S & Type::Hex;
    }
    [[nodiscard]] bool IsPrefix() const
    {
        return S & Type::Prefix;
    }
    [[nodiscard]] bool IsPostfix() const
    {
        return S & Type::Postfix;
    }
    [[nodiscard]] bool IsInstruction() const
    {
        return !(S & (Type::Operator | Type::Leaf));
    }
    [[nodiscard]] bool IsValue() const
    {
        return Flags.V;
    }

    static Type::T TypeOf(Mnemonic m);
    };
}

