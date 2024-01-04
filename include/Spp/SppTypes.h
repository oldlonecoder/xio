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

#include "Spp/Exports.h"
#include <AppBook/Util/StrAcc.h>

namespace Spp
{

namespace Type
{
using T = uint64_t;

constexpr T        Number       = 0x000000000001;
constexpr T        Text         = 0x000000000002;
constexpr T        VoidPtr      = 0x000000000004;
constexpr T        Any          = 0x000000000008;
constexpr T        Leaf         = 0x000000000010;
constexpr T        Sign         = 0x000000000020;
constexpr T        I8           = 0x000000000040;
constexpr T        I16          = 0x000000000080; // 0x7F41
constexpr T        I32          = 0x000000000100;
constexpr T        I64          = 0x000000000200;
constexpr T        U8           = 0x000000000400;
constexpr T        U16          = 0x000000000800;
constexpr T        U32          = 0x000000001000;
constexpr T        U64          = 0x000000002000;
constexpr T        Float        = 0x000000004000; // real: f32, f64, f128... ^^
constexpr T        Var          = 0x000000008000; // Ne pas oublier d'ajouter ce type a toutes "variables"
constexpr T        Bloc         = 0x000000010000;
constexpr T        Pipe         = 0x000000020000;
constexpr T        Keyword      = 0x000000040000;
constexpr T        Unary        = 0x000000080000;
constexpr T        Prefix       = 0x000000100000;
constexpr T        Postfix      = 0x000000200000;
constexpr T        Operator     = 0x000000400000;
constexpr T        Binary       = 0x000000800000;
constexpr T        Function     = 0x000001000000;
constexpr T        FunctionPtr  = 0x000002000000;
constexpr T        Object       = 0x000004000000;
constexpr T        Pointer      = 0x000008000000;
constexpr T        Type         = 0x000010000000;
constexpr T        Id           = 0x000020000000;
constexpr T        Ref          = 0x000040000000;
constexpr T        Punc         = 0x000080000000;
constexpr T        Assign       = 0x000100000000;
constexpr T        Bool         = 0x000200000000;
constexpr T        Hex          = 0x000400000000;
constexpr T        OpenPair     = 0x000800000000;
constexpr T        ClosePair    = 0x001000000000;
constexpr T        Const        = 0x002000000000;
constexpr T        Static       = 0x008000000000; ///< Flag set at the parser phase when applicable.
constexpr T        F32          = 0x010000000000;
constexpr T        F64          = 0x020000000000;
constexpr T        F128         = 0x040000000000;
constexpr T        OctalFormat  = 0x080000000000;
constexpr T        BinFormat    = 0x100000000000;
constexpr T        LineComment  = 0x200000000000;
constexpr T        BlocComment  = 0x400000000000;
constexpr T        Litteral     = 0x800000000000; ///< Litteral types { number, string };
constexpr T        Null         = 0x1000000000000000;
constexpr T        Integer      = Type::I8|Type::I16|Type::I32|Type::I64|Type::U8|Type::U16|Type::U32|Type::U64;
constexpr T        Unsigned     = Type::U8|Type::U16|Type::U32|Type::U64;


std::string SPP_EXPORT Name(T T_);
T           FromStr(const std::string& TName);
//
//struct Text;
//
//struct Integer;
//struct Uint8;
//struct Uint16;
//struct Uint32;
//struct Uint64;
//struct Int8;
//struct Int16;
//struct Int32;
//struct Int64;
//struct Float;
//struct Double;
//struct String;
//struct Pointer;


} // namespace Type


enum class Mnemonic : uint16_t
{
    Null = 0,
    LeftShift,
    Radical,
    Exponent,
    RightShift,
    Decr,
    Incr,
    AssignAdd,
    AssignSub,
    AssignMul,
    AssignDiv,
    AssignMod,
    AssignAnd,
    AssignOr,
    AssignXor,
    AssignC1,
    AssignLeftShift,
    AssignRightShift,
    Deref,
    LessEq,
    GreaterEq,
    Equal,
    NotEqual,  // != <>
    Add,
    Sub,
    Mul,
    Indirection,
    CommentCpp,
    Modulo,
    LessThan,
    GreaterThan,
    Assign,
    BinaryAnd,
    BinaryOr,
    BinaryXor,
    C1,           // complement a 1
    C2,
    BinaryNot,          //
    LogicalAnd,
    LogicalOr,
    OpenAbsOp,
    CloseAbsOp,
    OpenPar,
    ClosePar,
    Openindex,
    Closeindex,
    Openbrace,
    Closebrace,
    BeginComment,
    EndComment,
    Div,
    Comma,
    Scope,
    Semicolon,
    Colon,
    Range,
    Factorial,
    Positive,
    Negative,
    Squote, // '
    Dquote, // "
    Ternary, // ? :
    Hash, // #
    Eos,
    Dot,
    Return,
    If,
    Pi,
    Number,
    U8,
    U16,
    U32,
    U64,
    I8,
    I16,
    I32,
    I64,
    Float,
    String,
    Then,
    Else,
    Const,
    Include,
    Unit,
    At,
    Prime,
    Do,
    While,
    For,
    Until,
    Repeat,
    Switch,
    Case,
    Type,
    LowHex,
    UpHex,
    Cosinus,
    ArcCosinus,
    Tangent,
    ArcTangent,
    Sinus,
    ArcSinus,
    Object,
    Static,
    This,
    Unshadow,
    Catch,
    Throw,
    Noop,
    LineComment,
    BlocComment
};

std::string SPP_EXPORT MnemonicName(Mnemonic);

namespace Distance
{
using T = uint8_t;
constexpr T Scope      = 0;
constexpr T Assign_rhs = 1; //  A = B = ER!
constexpr T Deref      = 2;
constexpr T Prefix     = 3;
constexpr T Postfix    = 3;
constexpr T Unary      = 3;
constexpr T Paranthese = 5;
constexpr T Exponent   = 6;
constexpr T Bits       = 7;
constexpr T Product    = 8;
constexpr T Addition   = 9;
constexpr T Shift      = 10;
constexpr T Equality   = 11;
constexpr T Comp       = 12;
constexpr T Bool_and   = 13;
constexpr T Bool_xor   = 14;
constexpr T Bool_or    = 15;
constexpr T Boolean    = 16;
constexpr T Ternary    = 17;
constexpr T Comma      = 18;
constexpr T Identifier = 19;
constexpr T Assign     = 20; // a + b * m = 0 - 1 ( assign is right hand side association so the operator has lowest priority)
constexpr T Noop_      = 21;
}

namespace Lexem
{

using T = const char *;
constexpr T Null          = "null";
constexpr T LeftShift     = "<<";
constexpr T Radical       = "^/";
constexpr T Exponent      = "^";
constexpr T RightShift    = ">>";
constexpr T Decr          = "--";
constexpr T Incr          = "++";
constexpr T AssignAdd     = "+=";
constexpr T AssignSub     = "-=";
constexpr T AssignMul     = "*=";
constexpr T AssignDiv     = "/=";
constexpr T AssignMod     = "%=";
constexpr T AssignAnd     = "&=";
constexpr T AssignOr      = "|=";
constexpr T AssignXor     = "><=";
constexpr T AssignC1      = "`="; // Insérer AssignCompletment à 2 =>  { ``=  }
constexpr T AssignLshift  = "<<=";
constexpr T AssignRshift  = ">>=";
constexpr T Deref         = "->";
constexpr T LessEqual     = "<=";
constexpr T GreaterEqual  = ">=";
constexpr T Equal         = "==";
constexpr T NotEqual      = "!=";  // != <>
constexpr T Addition      = "+";
constexpr T Sub           = "-";
constexpr T Multiply      = "*";
constexpr T Indirection   = "*";
constexpr T CommentCpp    = "//";
constexpr T Modulo        = "%";
constexpr T Xor           = "><";
constexpr T LessThan      = "<";
constexpr T GreaterThan   = ">";
constexpr T Assign        = "=";
constexpr T BinaryAnd     = "&";
constexpr T BinaryOr      = "|";
constexpr T C1            = "`"; ///< compl&eacute;ment &agrave; 1
constexpr T C2            = "``";///< compl&eacute;ment &agrave; 2
constexpr T Not           = "!";
constexpr T BoolAnd       = "&&";
constexpr T BoolOr        = "||";
constexpr T AbsBegin      = "|<";// |< absolute value expr >|
constexpr T AbsEnd        = ">|";
constexpr T OpenPar       = "(";
constexpr T ClosePar      = ")";
constexpr T OpenIndex     = "[";
constexpr T CloseIndex    = "]";
constexpr T BraceBegin    = "{";
constexpr T BraceEnd      = "}";
constexpr T CommentBegin  = "/*";
constexpr T CommentEnd    = "*/";
constexpr T Division      = "/";
constexpr T Comma         = ",";
constexpr T Scope         = "::";
constexpr T Semicolon     = ";";
constexpr T Colon         = ":";
constexpr T Range         = "..";
constexpr T Factorial     = "!";
constexpr T Positive      = "+";
constexpr T Negative      = "-";
constexpr T Squote        = "'";  // '
constexpr T Dquote        = "\""; // "
constexpr T Ternary       = "?";  // ? :
constexpr T Hash          = "#";  // #
constexpr T Eos           = "$";
constexpr T Dot           = ".";
constexpr T Return        = "return";
constexpr T If            = "if";
constexpr T Pi            = "pi";
constexpr T Number        = "number";
constexpr T U8            = "u8";
constexpr T U16           = "u16";
constexpr T U32           = "u32";
constexpr T U64           = "u64";
constexpr T I8            = "i8";
constexpr T I16           = "i16";
constexpr T I32           = "i32";
constexpr T I64           = "i64";
constexpr T Float         = "real";
constexpr T String        = "string";
constexpr T Then          = "then";
constexpr T Else          = "else";
constexpr T Const         = "const";
constexpr T Include       = "include";
constexpr T Unit          = "Amu";
constexpr T At            = "@";
constexpr T Prime         = "`"; ///@todo change...
constexpr T Do            = "do";
constexpr T While         = "while";
constexpr T For           = "for";
constexpr T Until         = "until";
constexpr T Repeat        = "repeat";
constexpr T Switch        = "switch";
constexpr T Case          = "case";
constexpr T Type          = "type";
constexpr T LowHex        = "0x";
constexpr T UpHex         = "0X";
constexpr T Cosinus       = "cos";
constexpr T ArcCosinus    = "acos";
constexpr T Tangent       = "tan";// tan(4*a*m)  - sin(4ac) sina
constexpr T ArcTangent    = "atan";
constexpr T Sinus         = "sin";
constexpr T ArcSinus      = "asin";
constexpr T Object        = "object";
constexpr T Static        = "static";
constexpr T This          = "me";
constexpr T Unshadow      = ".::";
constexpr T Catch         = "catch";
constexpr T Throw         = "throw";
constexpr T LineComment   = "//";
constexpr T BlocComment   = "/*";

Mnemonic SPP_EXPORT  FromStr(const std::string &M_);

} // namespace Lexem
} // namespece Spp


