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


#include "Spp/SppToken.h"
#include <AppBook/Utf/Glyphes.h>

#include <map>
#include <AppBook/Book/AppBook.h>
//#include "logbook/expect.h"


//static const char* fmt = "'%s'(%d,%d) %s[%s]";



namespace Spp
{

static std::map<Mnemonic, Type::T> key_to_type = {   {Mnemonic::I8,   Type::I8},
                                                          {Mnemonic::I16,  Type::I16},
                                                          {Mnemonic::I32,  Type::I32},
                                                          {Mnemonic::I64,  Type::I64},
                                                          {Mnemonic::U8,   Type::U8},
                                                          {Mnemonic::U16,  Type::U16},
                                                          {Mnemonic::U32,  Type::U32},
                                                          {Mnemonic::U64,  Type::U64},
                                                          {Mnemonic::Float,Type::Float}

};

std::string SppToken::LocationData::operator()() const
{
    // line y, col x: `Attribute`
    return Text();
}

std::string SppToken::LocationData::Text() const
{
    if (begin == nullptr)
        return "std::string()";


    std::string str;
    const char* ptr = begin;
    str += *ptr++;
    if (!end)
    {
        while (*ptr)
            str += *ptr++;
        return str;
    }
    while (ptr <= end)
        str += *ptr++;
    //str += '\'';
    return str;
}

std::string SppToken::LocationData::Position() const
{
    StrAcc Str = "%d,%d";
    Str << Linenum << Colnum;
    return Str();
}

SppToken SppToken::NullToken = SppToken();

SppToken::SppToken(Mnemonic aCode, Type::T aType, Type::T aSem, Distance::T aDelta, Lexem::T aLexem, uint8_t V_Flag)
{
    M = aCode;
    T = aType;
    S = aSem;
    D = aDelta;
    Location = { aLexem, nullptr, 0, 0, -1 };
    Flags = { V_Flag, 0, 0 };

}

SppToken::SppToken(Mnemonic aCode, Type::T aType, Type::T aSem, Spp::Distance::T aDelta, SppToken::LocationData aLoc, SppToken::FlagsBit aFlag, void* aPtr)
{
    M = aCode;
    T = aType;
    S = aSem;
    D = aDelta;
    Location = aLoc;
    Flags = aFlag;
    Data = aPtr;
}


SppToken::SppToken(const SppToken& aToken) // Consider using "default" ...
{
    M = aToken.M;
    T = aToken.T;
    S = aToken.S;
    D = aToken.D;
    Location = aToken.Location;
    Flags = aToken.Flags;
    Data = aToken.Data;
}

SppToken::SppToken(SppToken&& aToken) noexcept
{
    std::swap(M, aToken.M);
    std::swap(T, aToken.T);
    std::swap(S, aToken.S);
    std::swap(D, aToken.D);
    std::swap(Location, aToken.Location);
    std::swap(Flags, aToken.Flags);
    std::swap(Data, aToken.Data);

}

SppToken& SppToken::operator=(SppToken&& aToken) noexcept
{
    std::swap(M, aToken.M);
    std::swap(T, aToken.T);
    std::swap(S, aToken.S);
    std::swap(D, aToken.D);
    std::swap(Location, aToken.Location);
    std::swap(Flags, aToken.Flags);
    std::swap(Data, aToken.Data);
    return *this;
}

SppToken& SppToken::operator=(const SppToken& aToken)
{
    M = aToken.M;
    T = aToken.T;
    S = aToken.S;
    D = aToken.D;
    Location = aToken.Location;
    Flags = aToken.Flags;
    Data = aToken.Data;
    return *this;
}

std::string SppToken::Mark(int nspc) const
{
    StrAcc str;
    if(nspc)
        str.fill(0x20, nspc);
    const char* B = Location.begin - Location.Offset;
    const char* cc = Location.begin;
    // localiser le debut de la ligne;
    while (*cc && (cc > B) && (*cc != '\n') && (*cc != '\r'))
        --cc;
    // debut de la ligne ou de la source:
    if (cc >= B)
    {
        if ((*cc == '\n') || (*cc == '\r'))
            ++cc;
        while (*cc && (*cc != '\n') && (*cc != '\r'))
            str += *cc++;
    }
    StrAcc tstr;

    tstr << str() << '\n';
    if(nspc)
        tstr.fill(0x20, nspc);

    for (int x = 1; x < Location.Colnum; x++)
        tstr << ' ';
    tstr << Utf::Glyph::CArrowUp;

    return tstr();
}




/*!
 * \brief SppToken::TextLine
 * \return std::string the string content of the line
 */
std::string SppToken::TextLine() const
{
    StrAcc str;
    const char* B = Location.begin - Location.Offset;
    const char* cc = Location.begin;
    // localiser le debut de la ligne;
    while (*cc && (cc > B) && (*cc != '\n') && (*cc != '\r'))
        --cc;
    // debut de la ligne ou de la source:
    if (cc >= B)
    {
        if ((*cc == '\n') || (*cc == '\r'))
            ++cc;
        while (*cc && (*cc != '\n') && (*cc != '\r'))
            str += *cc++;
    }
    //AppBook::Debug() << Book::Enums::Fn::Endl << str();
    return str();
}


std::string SppToken::LocationText() const
{
    if(Location.Linenum <= 0) return "[]";
    StrAcc Str = "Line :%d, Col:%d";
    Str << Location.Linenum << Location.Colnum;
    return Str();
}


std::string SppToken::SemanticText() const
{
    return Type::Name(S);
}

std::string SppToken::TypeName() const
{
    std::string Str;
    return Type::Name(T);
}


static SppToken::Array TokensTable =
    {
        {Mnemonic::Null,                Type::Keyword,   Type::Keyword                                              ,Distance::Identifier, Lexem::Null,        1},
        {Mnemonic::LeftShift,           Type::Binary,    Type::Operator|Type::Binary                                ,Distance::Shift,      Lexem::LeftShift,  1},
        {Mnemonic::Unshadow,            Type::Prefix,    Type::Unary|Type::Prefix|Type::Operator                    ,Distance::Scope,      Lexem::Unshadow,    1},
        {Mnemonic::OpenAbsOp,           Type::OpenPair,  Type::Operator|Type::Binary|Type::Punc                     ,Distance::Paranthese, Lexem::AbsBegin,   0},
        {Mnemonic::CloseAbsOp,          Type::ClosePair, Type::Operator|Type::Punc                                  ,Distance::Paranthese, Lexem::AbsEnd,     0},
        {Mnemonic::Radical,             Type::Binary,    Type::Operator|Type::Binary                                ,Distance::Exponent,   Lexem::Radical,     1},
        {Mnemonic::Exponent,            Type::Binary,    Type::Operator|Type::Binary                                ,Distance::Exponent,   Lexem::Exponent,    1},
        {Mnemonic::RightShift,          Type::Binary,    Type::Operator|Type::Binary                                ,Distance::Shift,      Lexem::RightShift, 1},
        {Mnemonic::BinaryXor,           Type::Binary,    Type::Operator|Type::Binary                                ,Distance::Bits,       Lexem::Xor,         1},
        {Mnemonic::Decr,                Type::Unary,     Type::Unary|Type::Prefix|Type::Operator                    ,Distance::Unary,      Lexem::Decr,        1},
        {Mnemonic::Incr,                Type::Unary,     Type::Unary|Type::Prefix|Type::Operator                    ,Distance::Unary,      Lexem::Incr,        1},
        {Mnemonic::AssignAdd,           Type::Assign,    Type::Keyword|Type::Operator|Type::Binary|Type::Assign     ,Distance::Assign,     Lexem::AssignAdd,  1},
        {Mnemonic::AssignSub,           Type::Assign,    Type::Keyword|Type::Operator|Type::Binary|Type::Assign     ,Distance::Assign,     Lexem::AssignSub,  1},
        {Mnemonic::AssignMul,           Type::Assign,    Type::Keyword|Type::Operator|Type::Binary|Type::Assign     ,Distance::Assign,     Lexem::AssignMul,  1},
        {Mnemonic::AssignDiv,           Type::Assign,    Type::Keyword|Type::Operator|Type::Binary|Type::Assign     ,Distance::Assign,     Lexem::AssignDiv,  1},
        {Mnemonic::AssignMod,           Type::Assign,    Type::Keyword|Type::Operator|Type::Binary|Type::Assign     ,Distance::Assign,     Lexem::AssignMod,  1},
        {Mnemonic::AssignAnd,           Type::Assign,    Type::Keyword|Type::Operator|Type::Binary|Type::Assign     ,Distance::Assign,     Lexem::AssignAnd,  1},
        {Mnemonic::AssignOr,            Type::Assign,    Type::Keyword|Type::Operator|Type::Binary|Type::Assign     ,Distance::Assign,     Lexem::AssignOr,   1},
        {Mnemonic::AssignXor,           Type::Assign,    Type::Keyword|Type::Operator|Type::Binary|Type::Assign     ,Distance::Assign,     Lexem::AssignXor,  1},
        {Mnemonic::AssignC1,            Type::Assign,    Type::Keyword|Type::Operator|Type::Binary|Type::Assign     ,Distance::Assign,     Lexem::AssignC1,     1},
        {Mnemonic::AssignLeftShift,     Type::Assign,    Type::Keyword|Type::Operator|Type::Binary|Type::Assign     ,Distance::Assign,     Lexem::AssignLshift, 1},
        {Mnemonic::AssignRightShift,    Type::Assign,    Type::Keyword|Type::Operator|Type::Binary|Type::Assign     ,Distance::Assign,     Lexem::AssignRshift, 1},
        {Mnemonic::Deref,               Type::Prefix,    Type::Unary|Type::Prefix|Type::Operator                    ,Distance::Deref,      Lexem::Deref        , 1},
        {Mnemonic::LessEq,              Type::Binary,    Type::Operator|Type::Binary|Type::Bool                     ,Distance::Equality,   Lexem::LessEqual,    1},
        {Mnemonic::GreaterEq,           Type::Binary,    Type::Operator|Type::Binary|Type::Bool                     ,Distance::Equality,   Lexem::GreaterEqual, 1},
        {Mnemonic::Equal,               Type::Binary,    Type::Operator|Type::Binary|Type::Bool                     ,Distance::Equality,   Lexem::Equal,        1},
        {Mnemonic::NotEqual,            Type::Binary,    Type::Operator|Type::Binary|Type::Bool                     ,Distance::Equality,   Lexem::NotEqual,    1},
        {Mnemonic::NotEqual,            Type::Binary,    Type::Operator|Type::Binary|Type::Bool                     ,Distance::Equality,   "<>",                1},
        {Mnemonic::Add,                 Type::Binary,    Type::Operator|Type::Binary                                ,Distance::Addition,   Lexem::Addition,     1},
        {Mnemonic::Sub,                 Type::Binary,    Type::Operator|Type::Binary                                ,Distance::Addition,   Lexem::Sub,          1},
        {Mnemonic::Mul,                 Type::Binary,    Type::Operator|Type::Binary                                ,Distance::Product,    Lexem::Multiply,     1},
        {Mnemonic::CommentCpp,          Type::LineComment, Type::LineComment|Type::Operator                                        ,Distance::Noop_,      Lexem::CommentCpp,  0},
        {Mnemonic::Modulo,              Type::Binary,    Type::Operator|Type::Binary                                ,Distance::Product,    Lexem::Modulo,       1},
        {Mnemonic::LessThan,            Type::Binary,    Type::Operator|Type::Binary|Type::Bool                     ,Distance::Equality,   Lexem::LessThan,    1},
        {Mnemonic::GreaterThan,         Type::Binary,    Type::Operator|Type::Binary|Type::Bool                     ,Distance::Equality,   Lexem::GreaterThan, 1},
        {Mnemonic::Assign,              Type::Assign,    Type::Keyword|Type::Operator|Type::Binary|Type::Assign     ,Distance::Assign,     Lexem::Assign,     1},
        {Mnemonic::BinaryAnd,           Type::Binary,    Type::Operator|Type::Binary                                ,Distance::Bits,       Lexem::BinaryAnd,  1},
        {Mnemonic::BinaryOr,            Type::Binary,    Type::Operator|Type::Binary                                ,Distance::Bits,       Lexem::BinaryOr,   1},
        {Mnemonic::C2,                  Type::Prefix,    Type::Unary|Type::Prefix|Type::Operator                    ,Distance::Prefix,     Lexem::C2,          1},
        {Mnemonic::C1,                  Type::Prefix,    Type::Unary|Type::Prefix|Type::Operator                    ,Distance::Prefix,     Lexem::C1,            1},
        {Mnemonic::BinaryNot,           Type::Unary,     Type::Unary|Type::Prefix|Type::Operator                    ,Distance::Prefix,     Lexem::Not,           1},
        {Mnemonic::LogicalAnd,          Type::Binary,    Type::Operator|Type::Binary|Type::Bool                     ,Distance::Bool_and,   Lexem::BoolAnd,      1},
        {Mnemonic::LogicalOr,           Type::Binary,    Type::Operator|Type::Binary|Type::Bool                     ,Distance::Bool_or,    Lexem::BoolOr,       1},
        {Mnemonic::OpenPar,             Type::OpenPair,  Type::Operator|Type::Punc|Type::OpenPair                   ,Distance::Paranthese, Lexem::OpenPar,      1},
        {Mnemonic::ClosePar,            Type::ClosePair, Type::Operator|Type::Punc|Type::ClosePair                  ,Distance::Paranthese,  Lexem::ClosePar,     1},
        {Mnemonic::Openindex,           Type::OpenPair,  Type::Operator|Type::Binary|Type::Punc|Type::OpenPair      ,Distance::Paranthese, Lexem::OpenIndex,    1},
        {Mnemonic::Closeindex,          Type::ClosePair, Type::Operator|Type::Punc|Type::Binary|Type::ClosePair     ,Distance::Paranthese, Lexem::CloseIndex,   1},
        {Mnemonic::Openbrace,           Type::Binary,    Type::Operator|Type::Binary|Type::Punc|Type::OpenPair      ,Distance::Paranthese, Lexem::BraceBegin,  1},
        {Mnemonic::Closebrace,          Type::ClosePair, Type::Operator|Type::Binary|Type::Punc|Type::ClosePair     ,Distance::Paranthese, Lexem::BraceEnd,     1},
        {Mnemonic::BeginComment,        Type::BlocComment, Type::BlocComment|Type::Operator                         ,Distance::Noop_,      Lexem::CommentBegin, 0},
        {Mnemonic::EndComment,          Type::BlocComment, Type::BlocComment|Type::Operator                         ,Distance::Noop_,      Lexem::CommentEnd, 0},
        {Mnemonic::Div,                 Type::Binary,    Type::Operator|Type::Binary                                ,Distance::Product,    Lexem::Division,  1},
        {Mnemonic::Comma,               Type::Punc,      Type::Operator|Type::Binary|Type::Punc                     ,Distance::Comma,      Lexem::Comma,     1},
        {Mnemonic::Scope,               Type::Punc,      Type::Operator|Type::Binary|Type::Punc                     ,Distance::Scope,      Lexem::Scope,     1},
        {Mnemonic::Semicolon,           Type::Punc,      Type::Punc                                                 ,Distance::Noop_,      Lexem::Semicolon, 0},
        {Mnemonic::Colon,               Type::Punc,      Type::Operator|Type::Binary|Type::Punc                     ,Distance::Noop_,      Lexem::Colon,     0},
        {Mnemonic::Range,               Type::Binary,    Type::Operator|Type::Binary|Type::Punc                     ,Distance::Scope,      Lexem::Range,     1},
        {Mnemonic::Factorial,           Type::Postfix,   Type::Unary|Type::Postfix|Type::Operator                   ,Distance::Unary,      Lexem::Factorial, 1},
        {Mnemonic::Positive,            Type::Prefix,    Type::Sign|Type::Unary|Type::Prefix|Type::Operator         ,Distance::Unary,      Lexem::Positive, 1},
        {Mnemonic::Negative,            Type::Prefix,    Type::Sign|Type::Unary|Type::Prefix|Type::Operator         ,Distance::Unary,      Lexem::Negative, 1},
        {Mnemonic::Squote,              Type::Text,      Type::Text|Type::Leaf|Type::Operator                       ,Distance::Identifier, Lexem::Squote,  1},
        {Mnemonic::Dquote,              Type::Text,      Type::Text|Type::Leaf|Type::Operator                       ,Distance::Identifier, Lexem::Dquote,  1},
        {Mnemonic::Ternary,             Type::Keyword,   Type::Keyword|Type::Operator|Type::Binary                  ,Distance::Unary,      Lexem::Ternary, 1},
        {Mnemonic::Hash,                Type::Prefix,    Type::Unary|Type::Prefix|Type::Operator                    ,Distance::Unary,      Lexem::Hash,   1},
        {Mnemonic::Eos,                 Type::Prefix,    Type::Unary|Type::Prefix|Type::Operator                    ,Distance::Unary,      Lexem::Eos,    1},
        {Mnemonic::Dot,                 Type::Punc,      Type::Punc|Type::Operator                     ,Distance::Scope,      Lexem::Dot,    1},
        {Mnemonic::Return,              Type::Keyword,   Type::Keyword                                              ,Distance::Identifier, Lexem::Return, 0},
        {Mnemonic::If,                  Type::Keyword,   Type::Keyword                                              ,Distance::Identifier, Lexem::If,     0},
        {Mnemonic::Pi,                  Type::Number,    Type::Number|Type::Leaf|Type::Float|Type::Keyword|Type::Const ,Distance::Identifier, Lexem::Pi,     1},
        {Mnemonic::Number,              Type::Prefix,    Type::Number|Type::Unary|Type::Keyword|Type::Prefix|Type::Operator     ,Distance::Identifier, Lexem::Number, 1},
        {Mnemonic::U8,                  Type::Prefix,    Type::U8|Type::Keyword|Type::Prefix|Type::Operator         ,Distance::Identifier, Lexem::U8,     1},
        {Mnemonic::U16,                 Type::Prefix,    Type::U16|Type::Unary|Type::Keyword|Type::Prefix|Type::Operator        ,Distance::Identifier, Lexem::U16,    1},
        {Mnemonic::U32,                 Type::Prefix,    Type::U32|Type::Unary|Type::Keyword|Type::Prefix|Type::Operator        ,Distance::Identifier, Lexem::U32,    1},
        {Mnemonic::U64,                 Type::Prefix,    Type::U64|Type::Unary|Type::Keyword|Type::Prefix|Type::Operator        ,Distance::Identifier, Lexem::U64,     1},
        {Mnemonic::I8,                  Type::Prefix,    Type::I8|Type::Unary|Type::Keyword|Type::Prefix|Type::Operator         ,Distance::Identifier, Lexem::I8,      1},
        {Mnemonic::I16,                 Type::Prefix,    Type::I16|Type::Unary|Type::Keyword|Type::Prefix|Type::Operator        ,Distance::Identifier, Lexem::I16,     1},
        {Mnemonic::I32,                 Type::Prefix,    Type::I32|Type::Unary|Type::Keyword|Type::Prefix|Type::Operator        ,Distance::Identifier, Lexem::I32,     1},
        {Mnemonic::I64,                 Type::Prefix,    Type::I64|Type::Unary|Type::Keyword|Type::Prefix|Type::Operator        ,Distance::Identifier, Lexem::I64,     1},
        {Mnemonic::Float,               Type::Prefix,    Type::Float|Type::Unary|Type::Keyword|Type::Prefix|Type::Operator      ,Distance::Identifier, Lexem::Float,   1},
        {Mnemonic::String,              Type::Prefix,    Type::Text|Type::Unary|Type::Keyword|Type::Prefix|Type::Operator       ,Distance::Identifier, Lexem::String,  1},
        {Mnemonic::Then,                Type::Keyword,   Type::Keyword                                              ,Distance::Identifier, Lexem::Then,    0},
        {Mnemonic::Else,                Type::Keyword,   Type::Keyword                                              ,Distance::Identifier, Lexem::Else,    0},
        {Mnemonic::Const,               Type::Keyword,   Type::Keyword|Type::Const                                  ,Distance::Identifier, Lexem::Const,   0},
        {Mnemonic::Include,             Type::Keyword,   Type::Keyword                                              ,Distance::Identifier, Lexem::Include, 0},
        {Mnemonic::Unit,                Type::Keyword,   Type::Keyword                                              ,Distance::Identifier, Lexem::Unit, 0},
        {Mnemonic::At,                  Type::Prefix,    Type::Unary|Type::Prefix|Type::Operator                    ,Distance::Unary,      Lexem::At,      1},
        {Mnemonic::Prime,               Type::Prefix,    Type::Unary|Type::Prefix|Type::Operator                    ,Distance::Unary,      Lexem::Prime,   1},
        {Mnemonic::Do,                  Type::Keyword,   Type::Keyword                                              ,Distance::Noop_,      Lexem::Do,      0},
        {Mnemonic::While,               Type::Keyword,   Type::Keyword                                              ,Distance::Noop_,      Lexem::While,       0},
        {Mnemonic::For,                 Type::Keyword,   Type::Keyword                                              ,Distance::Noop_,      Lexem::For,         0},
        {Mnemonic::Until,               Type::Keyword,   Type::Keyword                                              ,Distance::Noop_,      Lexem::Until,       0},
        {Mnemonic::Repeat,              Type::Keyword,   Type::Keyword                                              ,Distance::Noop_,      Lexem::Repeat,      0},
        {Mnemonic::Switch,              Type::Keyword,   Type::Keyword                                              ,Distance::Noop_,      Lexem::Switch,      0},
        {Mnemonic::Case,                Type::Keyword,   Type::Keyword                                              ,Distance::Noop_,      Lexem::Case,        0},
        {Mnemonic::Type,                Type::Keyword,   Type::Keyword                                              ,Distance::Noop_,      Lexem::Type,        0},
        {Mnemonic::LowHex,              Type::Prefix,    Type::Unary|Type::Prefix|Type::Leaf|Type::Keyword|Type::Operator|Type::Hex  ,Distance::Noop_,      Lexem::LowHex,     1},
        {Mnemonic::UpHex,               Type::Prefix,    Type::Unary|Type::Prefix|Type::Leaf|Type::Keyword|Type::Operator|Type::Hex  ,Distance::Noop_,      Lexem::UpHex,      1},
        {Mnemonic::Cosinus,             Type::Prefix,    Type::Unary|Type::Prefix|Type::Operator                    ,Distance::Unary,      Lexem::Cosinus,     1},
        {Mnemonic::ArcCosinus,          Type::Prefix,    Type::Keyword|Type::Unary|Type::Prefix|Type::Operator      ,Distance::Unary,      Lexem::ArcCosinus, 1},
        {Mnemonic::Tangent,             Type::Prefix,    Type::Keyword|Type::Unary|Type::Prefix|Type::Operator      ,Distance::Unary,      Lexem::Tangent,     1},
        {Mnemonic::ArcTangent,          Type::Prefix,    Type::Keyword|Type::Unary|Type::Prefix|Type::Operator      ,Distance::Unary,      Lexem::ArcTangent, 1},
        {Mnemonic::Sinus,               Type::Prefix,    Type::Keyword|Type::Unary|Type::Prefix|Type::Operator      ,Distance::Unary,      Lexem::Sinus,       1},
        {Mnemonic::ArcSinus,            Type::Prefix,    Type::Keyword|Type::Unary|Type::Prefix|Type::Operator      ,Distance::Unary,      Lexem::ArcSinus,   1},
        {Mnemonic::Object,              Type::Keyword,   Type::Keyword                                              ,Distance::Identifier, Lexem::Object,      0},
        {Mnemonic::Static,              Type::Keyword,   Type::Keyword                                              ,Distance::Identifier, Lexem::Static,      0},
        {Mnemonic::This,                Type::Keyword,   Type::Keyword                                              ,Distance::Identifier, Lexem::This,        1}
    };

// It was the right one ... duh...
SppToken SppToken::Scan(const char* C_)
{
    int      unicode = 0;
    if (!*C_)
        return SppToken::NullToken;

    for (auto token : TokensTable)
    {
        const char* crs = C_;
        const char* rtxt = token.Location.begin;
        unicode = 0; // oops...
        //std::size_t sz = std::strlen(rtxt);

        if(*crs != *token.Location.begin) continue;

        while ((*crs && *rtxt) && (*crs == *rtxt))
        {
            if (*crs < 0)
                ++unicode; ///< @note oui/ yes; Soon/Bientot je supporte quelques symboles UTF-8 (pi, xor,...)
            ++crs;
            ++rtxt;
        }
        if (*rtxt == 0)
        {
            if (*crs && !isspace(*crs))
            {
                if ((isalnum(*crs) || (*crs == '_')) && !token.IsOperator())
                    continue;
            }

            token.Location.begin = C_;
            token.Location.end = crs - 1;
            return token;
        }
    }
    return SppToken::NullToken;

}

std::string SppToken::DumpTokenTable()
{
    StrAcc acc = "-----------------------Tokens Table --------------------------\n";

    for(auto const& token: TokensTable)
    {
        acc = "[%-16s] %s\n";
        acc << MnemonicName(token.M) << token.SemanticText();
        std::cout << acc();
    }
    return acc();
}

Type::T SppToken::TypeOf(Mnemonic m)
{
    return key_to_type[m];
}

SppToken SppToken::operator[](Mnemonic CC)
{
    for (SppToken Tk : TokensTable)
    {
        if (CC == Tk.M)
            return Tk;
    }
    return SppToken::NullToken;
}

std::string SppToken::Details(bool Mark_) const
{
    StrAcc Str;
    Str << '\'' << Text() << '\'';
    Str += "[%s]: offset: %d line:%d, col:%d, %s/{%s}";
    Str
    << MnemonicName(M)
    << Location.Offset
    << Location.Linenum
    << Location.Colnum
    << TypeName()
    << SemanticText();

    if (Mark_)
        Str << '\n' << Mark();
    return Str();
}
}


