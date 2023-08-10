//
// Created by slussier on 21-08-08.
//

#include "xio/types.h"
#include "logbook/strbrk.h"
#include "logbook/stracc.h"


#include <map>

namespace xio
{
namespace type
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
                                  {"Unsigned",      Unsigned}
};

std::string name(T T_)
{
    std::string Str_;

    if(T_ > xio::type::BinFormat)
    {
        Str_ += "*.*";
        return Str_;
    }

    type::T tbm = T_;
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


T from_str(const std::string& Str )
{
    T types = 0;
    xio::strbrk words = Str.c_str();
    xio::strbrk::token_t::list W;
    (void) words(W, "/", false);

    for(auto I: W)
        types |= STR_T[I()];
    return types;
}

} //namespace type


std::string mnemonic_name(mnemonic M)
{
    std::map<mnemonic, std::string_view> _ = {{    mnemonic::Null,              "Null"},
                                                  {mnemonic::LeftShift,         "LeftShift"},
                                                  {mnemonic::Radical,           "Radical"},
                                                  {mnemonic::Exponent,          "Exponent"},
                                                  {mnemonic::RightShift,        "RightShift"},
                                                  {mnemonic::Decr,              "Decr"},
                                                  {mnemonic::Incr,              "Incr"},
                                                  {mnemonic::AssignAdd,         "AssignAdd"},
                                                  {mnemonic::AssignSub,         "AssignSub"},
                                                  {mnemonic::AssignMul,         "AssignMul"},
                                                  {mnemonic::AssignDiv,         "AssignDiv"},
                                                  {mnemonic::AssignMod,         "AssignMod"},
                                                  {mnemonic::AssignAnd,         "AssignAnd"},
                                                  {mnemonic::AssignOr,          "AssignOr"},
                                                  {mnemonic::AssignXor,         "AssignXor"},
                                                  {mnemonic::AssignC1,          "AssignC1"},
                                                  {mnemonic::AssignLeftShift,   "AssignLeftShift"},
                                                  {mnemonic::AssignRightShift,  "AssignRightShift"},
                                                  {mnemonic::Deref,             "Deref"},
                                                  {mnemonic::LessEq,            "LessEq"},
                                                  {mnemonic::GreaterEq,         "GreaterEq"},
                                                  {mnemonic::Equal,             "Equal"},
                                                  {mnemonic::NotEqual,          "NotEqual"},
                                                  {mnemonic::Add,               "Add"},
                                                  {mnemonic::Sub,               "Sub"},
                                                  {mnemonic::Mul,               "Mul"},
                                                  {mnemonic::Indirection,       "Indirection"},
                                                  {mnemonic::CommentCpp,        "CommentCpp"},
                                                  {mnemonic::Modulo,            "Modulo"},
                                                  {mnemonic::LessThan,          "LessThan"},
                                                  {mnemonic::GreaterThan,       "GreaterThan"},
                                                  {mnemonic::Assign,            "Assign"},
                                                  {mnemonic::BinaryAnd,         "BinaryAnd"},
                                                  {mnemonic::BinaryOr,          "BinaryOr"},
                                                  {mnemonic::BinaryXor,         "BinaryXor"},
                                                  {mnemonic::C1,                "C1"},// complement a 1
                                                  {mnemonic::C2,                "C2"},
                                                  {mnemonic::BinaryNot,         "BinaryNot"},   //
                                                  {mnemonic::LogicalAnd,        "LogicalAnd"},
                                                  {mnemonic::LogicalOr,         "LogicalOr"},
                                                  {mnemonic::OpenAbsOp,         "OpenAbsOp"},
                                                  {mnemonic::CloseAbsOp,        "CloseAbsOp"},
                                                  {mnemonic::OpenPar,           "OpenPar"},
                                                  {mnemonic::ClosePar,          "ClosePar"},
                                                  {mnemonic::Openindex,         "OpenIndex"},
                                                  {mnemonic::Closeindex,        "CloseIndex"},
                                                  {mnemonic::Openbrace,         "BraceBegin"},
                                                  {mnemonic::Closebrace,        "BraceEnd"},
                                                  {mnemonic::BeginComment,      "BeginComment"},
                                                  {mnemonic::EndComment,        "EndComment"},
                                                  {mnemonic::Div,               "Div"},
                                                  {mnemonic::Comma,             "Comma"},
                                                  {mnemonic::Scope,             "Scope"},
                                                  {mnemonic::Semicolon,         "Semicolon"},
                                                  {mnemonic::Colon,             "Colon"},
                                                  {mnemonic::Range,             "Range"},
                                                  {mnemonic::Factorial,         "Factorial"},
                                                  {mnemonic::Positive,          "Positive"},
                                                  {mnemonic::Negative,          "Negative"},
                                                  {mnemonic::Squote,            "Squote"},
                                                  {mnemonic::Dquote,            "Dquote"},
                                                  {mnemonic::Ternary,           "Ternary"},
                                                  {mnemonic::Hash,              "Hash"},
                                                  {mnemonic::Eos,               "Eos"},
                                                  {mnemonic::Dot,               "Dot"},
                                                  {mnemonic::Return,            "Return"},
                                                  {mnemonic::If,                "If"},
                                                  {mnemonic::Pi,                "Pi"},
                                                  {mnemonic::Number,            "Number"},
                                                  {mnemonic::U8,                "U8"},
                                                  {mnemonic::U16,               "U16"},
                                                  {mnemonic::U32,               "U32"},
                                                  {mnemonic::U64,               "U64"},
                                                  {mnemonic::I8,                "I8"},
                                                  {mnemonic::I16,               "I16"},
                                                  {mnemonic::I32,               "I32"},
                                                  {mnemonic::I64,               "I64"},
                                                  {mnemonic::Float,             "Float"},
                                                  {mnemonic::String,            "String"},
                                                  {mnemonic::Then,              "Then"},
                                                  {mnemonic::Else,              "Else"},
                                                  {mnemonic::Const,             "Const"},
                                                  {mnemonic::Include,           "Include"},
                                                  {mnemonic::Module,            "Module"},
                                                  {mnemonic::At,                "At"},
                                                  {mnemonic::Prime,             "Prime"},
                                                  {mnemonic::Do,                "Do"},
                                                  {mnemonic::While,             "While"},
                                                  {mnemonic::For,               "For"},
                                                  {mnemonic::Until,             "Until"},
                                                  {mnemonic::Repeat,            "Repeat"},
                                                  {mnemonic::Switch,            "Switch"},
                                                  {mnemonic::Case,              "Case"},
                                                  {mnemonic::Type,              "Type"},
                                                  {mnemonic::LowHex,            "LowHex"},
                                                  {mnemonic::UpHex,             "UpHex"},
                                                  {mnemonic::Cosinus,           "Cosinus"},
                                                  {mnemonic::ArcCosinus,        "ArcCosinus"},
                                                  {mnemonic::Tangent,           "Tangent"},
                                                  {mnemonic::ArcTangent,        "ArcTangent"},
                                                  {mnemonic::Sinus,             "Sinus"},
                                                  {mnemonic::ArcSinus,          "ArcSinus"},
                                                  {mnemonic::Object,            "Object"},
                                                  {mnemonic::Static,            "Static"},
                                                  {mnemonic::This,              "This"},
                                                  {mnemonic::Unshadow,          "Unshadow"}, // Take the address of the local var instance identifier, from the parent bloc insxiod.
                                                  {mnemonic::Noop,              "noop"}};
    return std::string(_[M]);
}



mnemonic lexem::from_str(const std::string &M_)
{
    std::map<mnemonic, std::string> _ = {
                                             {mnemonic::Null,               "Null"},
                                             {mnemonic::LeftShift,          "LeftShift"},
                                             {mnemonic::Radical,            "Radical"},
                                             {mnemonic::Exponent,           "Exponent"},
                                             {mnemonic::RightShift,         "RightShift"},
                                             {mnemonic::Decr,               "Decr"},
                                             {mnemonic::Incr,               "Incr"},
                                             {mnemonic::AssignAdd,          "AssignAdd"},
                                             {mnemonic::AssignSub,          "AssignSub"},
                                             {mnemonic::AssignMul,          "AssignMul"},
                                             {mnemonic::AssignDiv,          "AssignDiv"},
                                             {mnemonic::AssignMod,          "AssignMod"},
                                             {mnemonic::AssignAnd,          "AssignAnd"},
                                             {mnemonic::AssignOr,           "AssignOr"},
                                             {mnemonic::AssignXor,          "AssignXor"},
                                             {mnemonic::AssignC1,           "AssignC1"},
                                             {mnemonic::AssignLeftShift,    "AssignLeftShift"},
                                             {mnemonic::AssignRightShift,   "AssignRightShift"},
                                             {mnemonic::LessEq,             "LessEq"},
                                             {mnemonic::GreaterEq,          "GreaterEq"},
                                             {mnemonic::Equal,              "Equal"},
                                             {mnemonic::NotEqual,           "Neq"},
                                             {mnemonic::Add,                "Add"},
                                             {mnemonic::Sub,                "Sub"},
                                             {mnemonic::Mul,                "Mul"},
                                             {mnemonic::Indirection,        "Indirection"},
                                             {mnemonic::CommentCpp,         "CommentCpp"},
                                             {mnemonic::Modulo,             "Modulo"},
                                             {mnemonic::LessThan,           "LessThan"},
                                             {mnemonic::GreaterThan,        "GreaterThan"},
                                             {mnemonic::Assign,             "Assign"},
                                             {mnemonic::BinaryAnd,          "BinaryAnd"},
                                             {mnemonic::BinaryOr,           "BinaryOr"},
                                             {mnemonic::BinaryXor,          "BinaryXor"},
                                             {mnemonic::C1,                 "C1"},// complement a 1
                                             {mnemonic::C2,                 "C2"},
                                             {mnemonic::BinaryNot,          "BinaryNot"},   //
                                             {mnemonic::LogicalAnd,         "LogicalAnd"},
                                             {mnemonic::LogicalOr,          "LogicalOr"},
                                             {mnemonic::OpenAbsOp,          "OpenAbsOp"},
                                             {mnemonic::CloseAbsOp,         "CloseAbsOp"},
                                             {mnemonic::OpenPar,            "OpenPar"},
                                             {mnemonic::ClosePar,           "ClosePar"},
                                             {mnemonic::Openindex,          "OpenIndex"},
                                             {mnemonic::Closeindex,         "CloseIndex"},
                                             {mnemonic::Openbrace,          "BraceBegin"},
                                             {mnemonic::Closebrace,         "BraceEnd"},
                                             {mnemonic::BeginComment,       "BeginComment"},
                                             {mnemonic::EndComment,         "EndComment"},
                                             {mnemonic::Div,                "Div"},
                                             {mnemonic::Comma,              "Comma"},
                                             {mnemonic::Scope,              "Scope"},
                                             {mnemonic::Semicolon,          "Semicolon"},
                                             {mnemonic::Colon,              "Colon"},
                                             {mnemonic::Range,              "Range"},
                                             {mnemonic::Factorial,          "Factorial"},
                                             {mnemonic::Positive,           "Positive"},
                                             {mnemonic::Negative,           "Negative"},
                                             {mnemonic::Squote,             "Squote"},
                                             {mnemonic::Dquote,             "Dquote"},
                                             {mnemonic::Ternary,            "Ternary"},
                                             {mnemonic::Hash,               "Hash"},
                                             {mnemonic::Eos,                "Eos"},
                                             {mnemonic::Dot,                "Dot"},
                                             {mnemonic::Return,             "Return"},
                                             {mnemonic::If,                 "If"},
                                             {mnemonic::Pi,                 "Pi"},
                                             {mnemonic::Number,             "Number"},
                                             {mnemonic::U8,                 "U8"},
                                             {mnemonic::U16,                "U16"},
                                             {mnemonic::U32,                "U32"},
                                             {mnemonic::U64,                "U64"},
                                             {mnemonic::I8,                 "I8"},
                                             {mnemonic::I16,                "I16"},
                                             {mnemonic::I32,                "I32"},
                                             {mnemonic::I64,                "I64"},
                                             {mnemonic::Float,              "Float"},
                                             {mnemonic::String,             "String"},
                                             {mnemonic::Then,               "Then"},
                                             {mnemonic::Else,               "Else"},
                                             {mnemonic::Const,              "Const"},
                                             {mnemonic::Include,            "Include"},
                                             {mnemonic::Module,             "Module"},
                                             {mnemonic::At,                 "At"},
                                             {mnemonic::Prime,              "Prime"},
                                             {mnemonic::Do,                 "Do"},
                                             {mnemonic::While,              "While"},
                                             {mnemonic::For,                "For"},
                                             {mnemonic::Until,              "Until"},
                                             {mnemonic::Repeat,             "Repeat"},
                                             {mnemonic::Switch,             "Switch"},
                                             {mnemonic::Case,               "Case"},
                                             {mnemonic::Type,               "Type"},
                                             {mnemonic::LowHex,             "LowHex"},
                                             {mnemonic::UpHex,              "UpHex"},
                                             {mnemonic::Cosinus,            "Cosinus"},
                                             {mnemonic::ArcCosinus,         "ArcCosinus"},
                                             {mnemonic::Tangent,            "Tangent"},
                                             {mnemonic::ArcTangent,         "ArcTangent"},
                                             {mnemonic::Sinus,              "Sinus"},
                                             {mnemonic::ArcSinus,           "ArcSinus"},
                                             {mnemonic::Object,             "Object"},
                                             {mnemonic::Static,             "Static"},
                                             {mnemonic::This,               "This"},
                                             {mnemonic::Unshadow,           "Unshadow"},
                                             {mnemonic::Noop,               "Noop"}
    };
    for(const auto &P: _)
    {
        if(P.second == M_)
            return P.first;
    }
    return mnemonic::Noop;
}
}
