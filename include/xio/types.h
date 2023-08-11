/***************************************************************************
*   Copyright (C) 2005/2023 by Serge Lussier                               *
*   2023: serge.lussier@oldlonecoder.club                                  *
***************************************************************************/

#pragma once

#include <cstdint>
#include <string>
#include <xio/public.h>


namespace xio
{

namespace type
{
using T = uint64_t;

constexpr T        Null         = 0x1000000000000000;
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
constexpr T        Integer      = xio::type::I8|xio::type::I16|xio::type::I32|xio::type::I64|xio::type::U8|xio::type::U16|xio::type::U32|xio::type::U64;
constexpr T        Unsigned     = xio::type::U8|xio::type::U16|xio::type::U32|xio::type::U64;


std::string name(T T_);
T           from_str(const std::string&);

} // namespace type


enum class mnemonic : uint16_t
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
    Module,
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
    Noop
};

std::string mnemonic_name(mnemonic);

namespace distance
{
using T = uint8_t;
constexpr T scope      = 0;
constexpr T assign_rhs = 1;
constexpr T deref      = 2;
constexpr T prefix     = 3;
constexpr T unary      = 3;
constexpr T paranthese = 5;
constexpr T exponent   = 6;
constexpr T bits       = 7;
constexpr T product    = 8;
constexpr T addition   = 9;
constexpr T shift      = 10;
constexpr T equality   = 11;
constexpr T comp       = 12;
constexpr T bool_and   = 13;
constexpr T bool_xor   = 14;
constexpr T bool_or    = 15;
constexpr T boolean    = 16;
constexpr T ternary    = 17;
constexpr T comma      = 18;
constexpr T identifier = 19;
constexpr T assign     = 20; // a + b * m = 0 - 1 ( assign is right hand side association so the operator has lowest priority)
constexpr T noop_      = 21;
}

namespace lexem
{

using T = const char *;
constexpr T Null          = "null";
constexpr T LeftShift    = "<<";
constexpr T Radical       = "^/";
constexpr T Exponent      = "^";
constexpr T RightShift   = ">>";
constexpr T Decr          = "--";
constexpr T Incr          = "++";
constexpr T AssignAdd    = "+=";
constexpr T AssignSub    = "-=";
constexpr T AssignMul    = "*=";
constexpr T AssignDiv    = "/=";
constexpr T AssignMod    = "%=";
constexpr T AssignAnd    = "&=";
constexpr T AssignOr     = "|=";
constexpr T AssignXor    = "><=";
constexpr T AssignC1     = "`=";
constexpr T AssignLshift = "<<=";
constexpr T AssignRshift = ">>=";
constexpr T Deref         = "->";
constexpr T LessEqual    = "<=";
constexpr T GreaterEqual = ">=";
constexpr T Equal         = "==";
constexpr T NotEqual     = "!=";  // != <>
constexpr T Addition      = "+";
constexpr T Sub           = "-";
constexpr T Multiply      = "*";
constexpr T Indirection   = "*";
constexpr T CommentCpp   = "//";
constexpr T Modulo        = "%";
constexpr T Xor           = "><";
constexpr T LessThan     = "<";
constexpr T GreaterThan  = ">";
constexpr T Assign        = "=";
constexpr T BinaryAnd    = "&";
constexpr T BinaryOr     = "|";
constexpr T C1            = "`"; ///< compl&eacute;ment &agrave; 1
constexpr T C2            = "``";///< compl&eacute;ment &agrave; 2
constexpr T Not           = "!";
constexpr T BoolAnd      = "&&";
constexpr T BoolOr       = "||";
constexpr T AbsBegin     = "|<";// |< expr >|
constexpr T AbsEnd       = ">|";
constexpr T OpenPar      = "(";
constexpr T ClosePar     = ")";
constexpr T OpenIndex    = "[";
constexpr T CloseIndex   = "]";
constexpr T BraceBegin   = "{";
constexpr T BraceEnd     = "}";
constexpr T CommentBegin = "/*";
constexpr T CommentEnd   = "*/";
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
constexpr T Module        = "module";
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
constexpr T LowHex       = "0x";
constexpr T UpHex        = "0X";
constexpr T Cosinus       = "cos";
constexpr T ArcCosinus   = "acos";
constexpr T Tangent       = "tan";// tan(4*a*m)  - sin(4ac) sina
constexpr T ArcTangent   = "atan";
constexpr T Sinus         = "sin";
constexpr T ArcSinus     = "asin";
constexpr T Object        = "object";
constexpr T Static        = "static";
constexpr T This          = "me";
constexpr T Unshadow      = ".::";
constexpr T Catch         = "catch";
constexpr T Throw         = "throw";

mnemonic from_str(const std::string &M_);

} // namespace lexem
} // namespece xio

