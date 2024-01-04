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


#include "Spp/SppTypes.h"
#include <AppBook/Util/StrBreak.h>
#include <AppBook/Util/StrAcc.h>


#include <map>

namespace Spp
{
namespace Type
{

std::map<T, std::string> T_STR = {{Null,         "Null"},
                                  {Number,       "Number"},
                                  {Text,         "Text"},
                                  {VoidPtr,      "VoidPtr"},
                                  {Any,          "Any"},
                                  {Leaf,         "Leaf"},
                                  {Sign,         "Signed"},
                                  {I8,           "I8"},
                                  {I16,          "I16"},
                                  {I32,          "I32"},
                                  {I64,          "I64"},
                                  {U8,           "U8"},
                                  {U16,          "U16"},
                                  {U32,          "U32"},
                                  {U64,          "U64"},
                                  {Float,        "Float"},
                                  {Var,          "Var"},
                                  {Bloc,         "Bloc"},
                                  {Pipe,         "Pipe"},
                                  {Keyword,      "Keyword"},
                                  {Unary,        "Unary"},
                                  {Prefix,       "Prefix"},
                                  {Postfix,      "Postfix"},
                                  {Operator,     "Operator"},
                                  {Binary,       "Binary"},
                                  {Function,     "Function"},
                                  {FunctionPtr,  "FunctionPtr"},
                                  {Object,       "Object"},
                                  {Pointer,      "Pointer"},
                                  {Type,         "Type"},
                                  {Id,           "Id"},
                                  {Ref,          "Ref"},
                                  {Punc,         "Punc"},
                                  {Assign,       "Assign"},
                                  {Bool,         "Bool"},
                                  {Hex,          "Hex"},
                                  {BinFormat,    "BinFormat"},
                                  {OctalFormat,  "OctalFormat"},
                                  {F32,          "F32"},
                                  {F64,          "F64"},
                                  {F128,         "F128"},
                                  {OpenPair,     "OpenPair"},
                                  {ClosePair,    "ClosePair"},
                                  {Static,       "Static"},
                                  {Const,        "Const"},
                                  {Integer,      "Integer"},
                                  {Unsigned,     "Unsigned"},
                                  {LineComment,   "LineComment"},
                                  {BlocComment,   "BlocComment"},
                                  {Litteral,      "Litteral"}
};

std::map<std::string, T> STR_T = {{"Null",          Null},
                                  {"Number",        Number},
                                  {"Text",          Text},
                                  {"String",        Text},
                                  {"VoidPtr",       VoidPtr},
                                  {"VoidPointer",   VoidPtr},
                                  {"Any",           Any},
                                  {"Leaf",          Leaf},
                                  {"Signed",        Sign},
                                  {"I8",            I8},
                                  {"I16",           I16},
                                  {"I32",           I32},
                                  {"I64",           I64},
                                  {"U8",            U8},
                                  {"U16",           U16},
                                  {"U32",           U32},
                                  {"U64",           U64},
                                  {"OctalFormat",   OctalFormat},
                                  {"F32",           F32},
                                  {"F64",           F64},
                                  {"F128",          F128},
                                  {"Float",         Float},
                                  {"Var",           Var},
                                  {"Bloc",          Bloc},
                                  {"Pipe",          Pipe},
                                  {"Stack",         Pipe},
                                  {"Keyword",       Keyword},
                                  {"Unary",         Unary},
                                  {"Prefix",        Prefix},
                                  {"Postfix",       Postfix},
                                  {"Operator",      Operator},
                                  {"Binary",        Binary},
                                  {"Function",      Function},
                                  {"FunctionPtr",   FunctionPtr},
                                  {"Object",        Object},
                                  {"Pointer",       Pointer},
                                  {"Type",          Type},
                                  {"Id",            Id},
                                  {"Identifier",    Id},
                                  {"Ref",           Ref},
                                  {"Reference",     Ref},
                                  {"Punc",          Punc},
                                  {"Punctuation",   Punc},
                                  {"Assign",        Assign},
                                  {"Bool",          Bool},
                                  {"Hex",           Hex},
                                  {"BinFormat",     BinFormat},
                                  {"OpenPair",      OpenPair},
                                  {"ClosePair",     ClosePair},
                                  {"Const",         Const},
                                  {"Integer",       Integer},
                                  {"Unsigned",      Unsigned},
                                  {"LineComment",   LineComment},
                                  {"BlocComment",   BlocComment},
                                  {"Litteral",      Litteral}
};

std::string Name(T T_)
{
    std::string Str_;

//    if(T_ > Spp::Type::BlocComment)
//    {
//        Str_ += "*.*";
//        return Str_;
//    }

    Type::T tbm = T_;
    //int                a, c, b;

    for(const auto &t: T_STR)
    {
        if(!tbm)
            break;
        if(t.first & tbm)
        {
            Str_ += t.second;
            tbm &= ~t.first;  // Remove the Bit from the "Sem" field.
            if(tbm)
                Str_ += '/';
        }
    }
    if(Str_.empty())
        Str_ += "Null";
    return Str_;
}


T FromStr(const std::string& Str )
{
    T Types = 0;
    StrBreak Words;
    StrBreak::ConfigData Data = {Str, "/", StrBreak::Discard};
    auto count = Words(Data);

    for(auto I: Data.Words)
        Types |= STR_T[I()];
    return Types;
}

} //namespace Type


std::string MnemonicName(Mnemonic M)
{
    std::map<Mnemonic, std::string_view> _ = {
        {Mnemonic::Null,             "Null"},
        {Mnemonic::LeftShift,        "LeftShift"},
        {Mnemonic::Radical,          "Radical"},
        {Mnemonic::Exponent,         "Exponent"},
        {Mnemonic::RightShift,       "RightShift"},
        {Mnemonic::Indirection,      "Indirection"},
        {Mnemonic::Deref,            "Deref"},
        {Mnemonic::Decr,             "Decr"},
        {Mnemonic::Incr,             "Incr"},
        {Mnemonic::AssignAdd,        "AssignAdd"},
        {Mnemonic::AssignSub,        "AssignSub"},
        {Mnemonic::AssignMul,        "AssignMul"},
        {Mnemonic::AssignDiv,        "AssignDiv"},
        {Mnemonic::AssignMod,        "AssignMod"},
        {Mnemonic::AssignAnd,        "AssignAnd"},
        {Mnemonic::AssignOr,         "AssignOr"},
        {Mnemonic::AssignXor,        "AssignXor"},
        {Mnemonic::AssignC1,         "AssignC1"},
        {Mnemonic::AssignLeftShift,  "AssignLeftShift"},
        {Mnemonic::AssignRightShift, "AssignRightShift"},
        {Mnemonic::LessEq,           "LessEq"},
        {Mnemonic::GreaterEq,        "GreaterEq"},
        {Mnemonic::Equal,            "Equal"},
        {Mnemonic::NotEqual,         "NotEqual"},
        {Mnemonic::Add,              "Add"},
        {Mnemonic::Sub,              "Sub"},
        {Mnemonic::Mul,              "Mul"},
        {Mnemonic::Indirection,      "Indirection"},
        {Mnemonic::CommentCpp,       "CommentCpp"},
        {Mnemonic::Modulo,           "Modulo"},
        {Mnemonic::LessThan,         "LessThan"},
        {Mnemonic::GreaterThan,      "GreaterThan"},
        {Mnemonic::Assign,           "Assign"},
        {Mnemonic::BinaryAnd,        "BinaryAnd"},
        {Mnemonic::BinaryOr,         "BinaryOr"},
        {Mnemonic::BinaryXor,        "BinaryXor"},
        {Mnemonic::C1,               "C1"},// complement a 1
        {Mnemonic::C2,               "C2"},
        {Mnemonic::BinaryNot,        "BinaryNot"},   //
        {Mnemonic::LogicalAnd,       "LogicalAnd"},
        {Mnemonic::LogicalOr,        "LogicalOr"},
        {Mnemonic::OpenAbsOp,        "OpenAbsOp"},
        {Mnemonic::CloseAbsOp,       "CloseAbsOp"},
        {Mnemonic::OpenPar,          "OpenPar"},
        {Mnemonic::ClosePar,         "ClosePar"},
        {Mnemonic::Openindex,        "OpenIndex"},
        {Mnemonic::Closeindex,       "CloseIndex"},
        {Mnemonic::Openbrace,        "BraceBegin"},
        {Mnemonic::Closebrace,       "BraceEnd"},
        {Mnemonic::BeginComment,     "BeginComment"},
        {Mnemonic::EndComment,       "EndComment"},
        {Mnemonic::Div,              "Div"},
        {Mnemonic::Comma,            "Comma"},
        {Mnemonic::Scope,            "Scope"},
        {Mnemonic::Semicolon,        "Semicolon"},
        {Mnemonic::Colon,            "Colon"},
        {Mnemonic::Range,            "Range"},
        {Mnemonic::Factorial,        "Factorial"},
        {Mnemonic::Positive,         "Positive"},
        {Mnemonic::Negative,         "Negative"},
        {Mnemonic::Squote,           "Squote"},
        {Mnemonic::Dquote,           "Dquote"},
        {Mnemonic::Ternary,          "Ternary"},
        {Mnemonic::Hash,             "Hash"},
        {Mnemonic::Eos,              "Eos"},
        {Mnemonic::Dot,              "Dot"},
        {Mnemonic::Return,           "Return"},
        {Mnemonic::If,               "If"},
        {Mnemonic::Pi,               "Pi"},
        {Mnemonic::Number,           "Number"},
        {Mnemonic::U8,               "U8"},
        {Mnemonic::U16,              "U16"},
        {Mnemonic::U32,              "U32"},
        {Mnemonic::U64,              "U64"},
        {Mnemonic::I8,               "I8"},
        {Mnemonic::I16,              "I16"},
        {Mnemonic::I32,              "I32"},
        {Mnemonic::I64,              "I64"},
        {Mnemonic::Float,            "Float"}, // Important: F32 : float; F64: double; F128: long double;
        {Mnemonic::String,           "String"},
        {Mnemonic::Then,             "Then"},
        {Mnemonic::Else,             "Else"},
        {Mnemonic::Const,            "Const"},
        {Mnemonic::Include,          "Include"},
        {Mnemonic::Unit,           "Amu"},
        {Mnemonic::At,               "At"},
        {Mnemonic::Prime,            "Prime"},
        {Mnemonic::Do,               "Do"},
        {Mnemonic::While,            "While"},
        {Mnemonic::For,              "For"},
        {Mnemonic::Until,            "Until"},
        {Mnemonic::Repeat,           "Repeat"},
        {Mnemonic::Switch,           "Switch"},
        {Mnemonic::Case,             "Case"},
        {Mnemonic::Type,             "Type"},
        {Mnemonic::LowHex,           "LowHex"},
        {Mnemonic::UpHex,            "UpHex"},
        {Mnemonic::Cosinus,          "Cosinus"},
        {Mnemonic::ArcCosinus,       "ArcCosinus"},
        {Mnemonic::Tangent,          "Tangent"},
        {Mnemonic::ArcTangent,       "ArcTangent"},
        {Mnemonic::Sinus,            "Sinus"},
        {Mnemonic::ArcSinus,         "ArcSinus"},
        {Mnemonic::Object,           "Object"},
        {Mnemonic::Static,           "Static"},
        {Mnemonic::This,             "This"},
        {Mnemonic::Unshadow,         "Unshadow"}, // Take the address of the local var instance identifier, from the parent bloc insSppd.
        {Mnemonic::Noop,             "noop"},
        {Mnemonic::LineComment,      "//"},
        {Mnemonic::BlocComment,      "/*"},
    };
    return std::string(_[M]);
}



Mnemonic Lexem::FromStr(const std::string &M_)
{
    std::map<Mnemonic, std::string> _ = {
        {Mnemonic::Null,             "Null"},
        {Mnemonic::LeftShift,        "LeftShift"},
        {Mnemonic::Radical,          "Radical"},
        {Mnemonic::Exponent,         "Exponent"},
        {Mnemonic::RightShift,       "RightShift"},
        {Mnemonic::Deref,            "Deref"},
        {Mnemonic::Decr,             "Decr"},
        {Mnemonic::Incr,             "Incr"},
        {Mnemonic::AssignAdd,        "AssignAdd"},
        {Mnemonic::AssignSub,        "AssignSub"},
        {Mnemonic::AssignMul,        "AssignMul"},
        {Mnemonic::AssignDiv,        "AssignDiv"},
        {Mnemonic::AssignMod,        "AssignMod"},
        {Mnemonic::AssignAnd,        "AssignAnd"},
        {Mnemonic::AssignOr,         "AssignOr"},
        {Mnemonic::AssignXor,        "AssignXor"},
        {Mnemonic::AssignC1,         "AssignC1"},
        {Mnemonic::AssignLeftShift,  "AssignLeftShift"},
        {Mnemonic::AssignRightShift, "AssignRightShift"},
        {Mnemonic::LessEq,           "LessEq"},
        {Mnemonic::GreaterEq,        "GreaterEq"},
        {Mnemonic::Equal,            "Equal"},
        {Mnemonic::NotEqual,         "Neq"},
        {Mnemonic::Add,              "Add"},
        {Mnemonic::Sub,              "Sub"},
        {Mnemonic::Mul,              "Mul"},
        {Mnemonic::Indirection,      "Indirection"},
        {Mnemonic::CommentCpp,       "CommentCpp"},
        {Mnemonic::Modulo,           "Modulo"},
        {Mnemonic::LessThan,         "LessThan"},
        {Mnemonic::GreaterThan,      "GreaterThan"},
        {Mnemonic::Assign,           "Assign"},
        {Mnemonic::BinaryAnd,        "BinaryAnd"},
        {Mnemonic::BinaryOr,         "BinaryOr"},
        {Mnemonic::BinaryXor,        "BinaryXor"},
        {Mnemonic::C1,               "C1"},// complement a 1
        {Mnemonic::C2,               "C2"},
        {Mnemonic::BinaryNot,        "BinaryNot"},   //
        {Mnemonic::LogicalAnd,       "LogicalAnd"},
        {Mnemonic::LogicalOr,        "LogicalOr"},
        {Mnemonic::OpenAbsOp,        "OpenAbsOp"},
        {Mnemonic::CloseAbsOp,       "CloseAbsOp"},
        {Mnemonic::OpenPar,          "OpenPar"},
        {Mnemonic::ClosePar,         "ClosePar"},
        {Mnemonic::Openindex,        "OpenIndex"},
        {Mnemonic::Closeindex,       "CloseIndex"},
        {Mnemonic::Openbrace,        "BraceBegin"},
        {Mnemonic::Closebrace,       "BraceEnd"},
        {Mnemonic::BeginComment,     "BeginComment"},
        {Mnemonic::EndComment,       "EndComment"},
        {Mnemonic::Div,              "Div"},
        {Mnemonic::Comma,            "Comma"},
        {Mnemonic::Scope,            "Scope"},
        {Mnemonic::Semicolon,        "Semicolon"},
        {Mnemonic::Colon,            "Colon"},
        {Mnemonic::Range,            "Range"},
        {Mnemonic::Factorial,        "Factorial"},
        {Mnemonic::Positive,         "Positive"},
        {Mnemonic::Negative,         "Negative"},
        {Mnemonic::Squote,           "Squote"},
        {Mnemonic::Dquote,           "Dquote"},
        {Mnemonic::Ternary,          "Ternary"},
        {Mnemonic::Hash,             "Hash"},
        {Mnemonic::Eos,              "Eos"},
        {Mnemonic::Dot,              "Dot"},
        {Mnemonic::Return,           "Return"},
        {Mnemonic::If,               "If"},
        {Mnemonic::Pi,               "Pi"},
        {Mnemonic::Number,           "Number"},
        {Mnemonic::U8,               "U8"},
        {Mnemonic::U16,              "U16"},
        {Mnemonic::U32,              "U32"},
        {Mnemonic::U64,              "U64"},
        {Mnemonic::I8,               "I8"},
        {Mnemonic::I16,              "I16"},
        {Mnemonic::I32,              "I32"},
        {Mnemonic::I64,              "I64"},
        {Mnemonic::Float,            "Float"},
        {Mnemonic::String,           "String"},
        {Mnemonic::Then,             "Then"},
        {Mnemonic::Else,             "Else"},
        {Mnemonic::Const,            "Const"},
        {Mnemonic::Include,          "Include"},
        {Mnemonic::Unit,           "Amu"},
        {Mnemonic::At,               "At"},
        {Mnemonic::Prime,            "Prime"},
        {Mnemonic::Do,               "Do"},
        {Mnemonic::While,            "While"},
        {Mnemonic::For,              "For"},
        {Mnemonic::Until,            "Until"},
        {Mnemonic::Repeat,           "Repeat"},
        {Mnemonic::Switch,           "Switch"},
        {Mnemonic::Case,             "Case"},
        {Mnemonic::Type,             "Type"},
        {Mnemonic::LowHex,           "LowHex"},
        {Mnemonic::UpHex,            "UpHex"},
        {Mnemonic::Cosinus,          "Cosinus"},
        {Mnemonic::ArcCosinus,       "ArcCosinus"},
        {Mnemonic::Tangent,          "Tangent"},
        {Mnemonic::ArcTangent,       "ArcTangent"},
        {Mnemonic::Sinus,            "Sinus"},
        {Mnemonic::ArcSinus,         "ArcSinus"},
        {Mnemonic::Object,           "Object"},
        {Mnemonic::Static,           "Static"},
        {Mnemonic::This,             "This"},
        {Mnemonic::Unshadow,         "Unshadow"},
        {Mnemonic::Noop,             "Noop"},
        {Mnemonic::LineComment,      "//"},
        {Mnemonic::BlocComment,      "/*"},
    };
    for(const auto &P: _)
    {
        if(P.second == M_)
            return P.first;
    }
    return Mnemonic::Noop;
}
}
