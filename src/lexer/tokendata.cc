#include <xio/lexer/tokendata.h>
#include <tlux/utf/Icons.h>
#include <map>
#include <tlux/diagnostic.h>

static const char* fmt = "'%s'(%d,%d) %s[%s]";
using namespace tux;
namespace xio
{

static std::map<mnemonic, xio::type::T> key_to_type = {   {mnemonic::I8,   xio::type::I8},
                                                          {mnemonic::I16,  xio::type::I16},
                                                          {mnemonic::I32,  xio::type::I32},
                                                          {mnemonic::I64,  xio::type::I64},
                                                          {mnemonic::U8,   xio::type::U8},
                                                          {mnemonic::U16,  xio::type::U16},
                                                          {mnemonic::U32,  xio::type::U32},
                                                          {mnemonic::U64,  xio::type::U64},
                                                          {mnemonic::Float,xio::type::Float}

};

std::string token_data::location_data::operator()() const
{
    // line y, col x: `attribute`
    return text();
}

std::string token_data::location_data::text() const
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

std::string token_data::location_data::position() const
{
    tux::stracc Str = "%d,%d";
    Str << linenum << colnum;
    return Str();
}

token_data token_data::mNull = token_data();

token_data::token_data(mnemonic aCode, xio::type::T aType, xio::type::T aSem, xio::type::distance::T aDelta, lexem::T aLexem, uint8_t V_Flag)
{
    c = aCode;
    t = aType;
    s = aSem;
    d = aDelta;
    mLoc = { aLexem, 0, 0, 0, -1 };
    _flags = { V_Flag, 0, 0 };

}

token_data::token_data(mnemonic aCode, xio::type::T aType, xio::type::T aSem, xio::type::distance::T aDelta, token_data::location_data aLoc, token_data::Flag aFlag, void* aPtr)
{
    c = aCode;
    t = aType;
    s = aSem;
    d = aDelta;
    mLoc = aLoc;
    _flags = aFlag;
    vdata = aPtr;
}


token_data::token_data(const token_data& aToken) // Consider using "default" ...
{
    c = aToken.c;
    t = aToken.t;
    s = aToken.s;
    d = aToken.d;
    mLoc = aToken.mLoc;
    _flags = aToken._flags;
    vdata = aToken.vdata;
}

token_data::token_data(token_data&& aToken) noexcept
{
    std::swap(c, aToken.c);
    std::swap(t, aToken.t);
    std::swap(s, aToken.s);
    std::swap(d, aToken.d);
    std::swap(mLoc, aToken.mLoc);
    std::swap(_flags, aToken._flags);
    std::swap(vdata, aToken.vdata);

}

token_data& token_data::operator=(token_data&& aToken) noexcept
{
    std::swap(c, aToken.c);
    std::swap(t, aToken.t);
    std::swap(s, aToken.s);
    std::swap(d, aToken.d);
    std::swap(mLoc, aToken.mLoc);
    std::swap(_flags, aToken._flags);
    std::swap(vdata, aToken.vdata);
    return *this;
}

token_data& token_data::operator=(const token_data& aToken)
{
    c = aToken.c;
    t = aToken.t;
    s = aToken.s;
    d = aToken.d;
    mLoc = aToken.mLoc;
    _flags = aToken._flags;
    vdata = aToken.vdata;
    return *this;
}

std::string token_data::mark(int nspc) const
{
    stracc str;
    if(nspc)
        str.fill(0x20, nspc);
    const char* B = mLoc.begin - mLoc.offset;
    //std::string str;
    int l = 1;
    const char* cc = mLoc.begin;
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
    stracc tstr;

    tstr << str() << '\n';
    if(nspc)
        tstr.fill(0x20, nspc);

    for (int x = 1; x < mLoc.colnum; x++)
        tstr << ' ';
    tstr << Icon::CArrowUp;

    return tstr();
}

std::string token_data::location() const
{
    stracc Str = "Line :%d, Col:%d";
    Str << mLoc.linenum << mLoc.colnum;
    return Str();
}


std::string token_data::semantic_types() const
{
    std::string Str;
    return xio::type::name(s);
}

std::string token_data::type_name() const
{
    std::string Str;
    return xio::type::name(t);
}


/*!
    @note !IMPORTANT : Decompress token_data::s values!
*/
static token_data::collection tokens_table =
{
{mnemonic::Null,                type::Keyword,    0x000040000, type::distance::identifier, lexem::Null,        1},
{mnemonic::LeftShift,           type::Binary,     0x000C00000, type::distance::shift,      lexem::LeftShift,  1},
{mnemonic::Unshadow,            type::Prefix,     0x000580000, type::distance::scope,      lexem::Unshadow,    1},
{mnemonic::OpenAbsOp,           type::OpenPair,   0x080C00000, type::distance::paranthese, lexem::AbsBegin,   0},
{mnemonic::CloseAbsOp,          type::ClosePair,  0x080400000, type::distance::paranthese, lexem::AbsEnd,     0},
{mnemonic::Radical,             type::Binary,     0x000C00000, type::distance::exponent,   lexem::Radical,     1},
{mnemonic::Exponent,            type::Binary,     0x000C00000, type::distance::exponent,   lexem::Exponent,    1},
{mnemonic::RightShift,          type::Binary,     0x000400000, type::distance::shift,      lexem::RightShift, 1},
{mnemonic::BinaryXor,           type::Binary,     0x000C00000, type::distance::bits,       lexem::Xor,         1},
{mnemonic::Decr,                type::Unary,      0x000580000, type::distance::unary,      lexem::Decr,        1},
{mnemonic::Incr,                type::Unary,      0x000580000, type::distance::unary,      lexem::Incr,        1},
{mnemonic::AssignAdd,           type::Assign,     0x100C40000, type::distance::assign,     lexem::AssignAdd,  1},
{mnemonic::AssignSub,           type::Assign,     0x100C40000, type::distance::assign,     lexem::AssignSub,  1},
{mnemonic::AssignMul,           type::Assign,     0x100C40000, type::distance::assign,     lexem::AssignMul,  1},
{mnemonic::AssignDiv,           type::Assign,     0x100C40000, type::distance::assign,     lexem::AssignDiv,  1},
{mnemonic::AssignMod,           type::Assign,     0x100C40000, type::distance::assign,     lexem::AssignMod,  1},
{mnemonic::AssignAnd,           type::Assign,     0x100C40000, type::distance::assign,     lexem::AssignAnd,  1},
{mnemonic::AssignOr,            type::Assign,     0x100C40000, type::distance::assign,     lexem::AssignOr,   1},
{mnemonic::AssignXor,           type::Assign,     0x100C40000, type::distance::assign,     lexem::AssignXor,  1},
{mnemonic::AssignC1,            type::Assign,     0x100C40000, type::distance::assign,     lexem::AssignC1,     1},
{mnemonic::AssignLeftShift,     type::Assign,     0x100C40000, type::distance::assign,     lexem::AssignLshift, 1},
{mnemonic::AssignRightShift,    type::Assign,     0x100C40000, type::distance::assign,     lexem::AssignRshift, 1},
{mnemonic::Deref,               type::Prefix,     0x000580000, type::distance::deref,      lexem::Deref        , 1},
{mnemonic::LessEq,              type::Binary,     0x200C00000, type::distance::equality,   lexem::LessEqual,    1},
{mnemonic::GreaterEq,           type::Binary,     0x200C00000, type::distance::equality,   lexem::GreaterEqual, 1},
{mnemonic::Equal,               type::Binary,     0x200C00000, type::distance::equality,   lexem::Equal,        1},
{mnemonic::NotEqual,            type::Binary,     0x200C00000, type::distance::equality,   lexem::NotEqual,    1},
{mnemonic::NotEqual,            type::Binary,     0x200C00000, type::distance::equality,   "<>",                    1},
{mnemonic::Add,                 type::Binary,     0x000C00000, type::distance::addition,   lexem::Addition,     1},
{mnemonic::Sub,                 type::Binary,     0x000C00000, type::distance::addition,   lexem::Sub,          1},
{mnemonic::Mul,                 type::Binary,     0x000C00000, type::distance::product,    lexem::Multiply,     1},
{mnemonic::CommentCpp,          type::Punc,       0x000400000, type::distance::noop_,      lexem::CommentCpp,  0},
{mnemonic::Modulo,              type::Binary,     0x000C00000, type::distance::product,    lexem::Modulo,       1},
{mnemonic::LessThan,            type::Binary,     0x200C00000, type::distance::equality,   lexem::LessThan,    1},
{mnemonic::GreaterThan,         type::Binary,     0x200C00000, type::distance::equality,   lexem::GreaterThan, 1},
{mnemonic::Assign,              type::Assign,     0x100C40000, type::distance::assign,     lexem::Assign,     1},
{mnemonic::BinaryAnd,           type::Binary,     0x000C00000, type::distance::bits,       lexem::BinaryAnd,  1},
{mnemonic::BinaryOr,            type::Binary,     0x000C00000, type::distance::bits,       lexem::BinaryOr,   1},
{mnemonic::C2,                  type::Prefix,     0x000580000, type::distance::prefix,     lexem::C2,          1},
{mnemonic::C1,                  type::Prefix,     0x000580000, type::distance::prefix,     lexem::C1,            1},
{mnemonic::BinaryNot,           type::Unary,      0x000580000, type::distance::prefix,     lexem::Not,           1},
{mnemonic::LogicalAnd,          type::Binary,     0x200C00000, type::distance::bool_and,   lexem::BoolAnd,      1},
{mnemonic::LogicalOr,           type::Binary,     0x200C00000, type::distance::bool_or,    lexem::BoolOr,       1},
{mnemonic::OpenPar,             type::OpenPair,   0x0880400000,type::distance::paranthese, lexem::OpenPar,      1},
{mnemonic::ClosePar,            type::ClosePair,  0x1080400000,type::distance::paranthese,  lexem::ClosePar,     1},
{mnemonic::Openindex,           type::Binary,     0x2080C00000,type::distance::paranthese, lexem::OpenIndex,    1},
{mnemonic::Closeindex,          type::ClosePair,  0x4080400000,type::distance::paranthese, lexem::CloseIndex,   1},
{mnemonic::Openbrace,           type::Binary,     0x0880C00000, type::distance::paranthese, lexem::BraceBegin,  1},
{mnemonic::Closebrace,          type::ClosePair,  0x1080400000,type::distance::paranthese, lexem::BraceEnd,     1},
{mnemonic::BeginComment,        type::Punc,       0x00000,     type::distance::noop_,      lexem::CommentBegin, 0},
{mnemonic::EndComment,          type::Punc,       0x00000,     type::distance::noop_,      lexem::CommentEnd, 0},
{mnemonic::Div,                 type::Binary,     0x000C00000, type::distance::product,    lexem::Division,  1},
{mnemonic::Comma,               type::Binary,     0x080C00000, type::distance::comma,      lexem::Comma,     1},
{mnemonic::Scope,               type::Binary,     0x080C00000, type::distance::scope,      lexem::Scope,     1},
{mnemonic::Semicolon,           type::Punc,       0x080000000, type::distance::noop_,      lexem::Semicolon, 0},
{mnemonic::Colon,               type::Binary,     0x080C00000, type::distance::noop_,      lexem::Colon,     0},
{mnemonic::Range,               type::Binary,     0x080C00000, type::distance::scope,      lexem::Range,     1},
{mnemonic::Factorial,           type::Postfix,    0x000580000, type::distance::unary,      lexem::Factorial, 1},
{mnemonic::Positive,            type::Prefix,     0x000580020, type::distance::unary,      lexem::Positive, 1},
{mnemonic::Negative,            type::Prefix,     0x000580020, type::distance::unary,      lexem::Negative, 1},
{mnemonic::Squote,              type::Text,       0x000400012, type::distance::identifier, lexem::Squote,  1},
{mnemonic::Dquote,              type::Text,       0x000400012, type::distance::identifier, lexem::Dquote,  1},
{mnemonic::Ternary,             type::Keyword,    0x000C40000, type::distance::unary,      lexem::Ternary, 1},
{mnemonic::Hash,                type::Prefix,     0x000580000, type::distance::unary,      lexem::Hash,   1},
{mnemonic::Eos,                 type::Prefix,     0x000580000, type::distance::unary,      lexem::Eos,    1},
{mnemonic::Dot,                 type::Punc,       0x080400000, type::distance::scope,      lexem::Dot,    1},
{mnemonic::Return,              type::Keyword,    0x000040000, type::distance::identifier, lexem::Return, 0},
{mnemonic::If,                  type::Keyword,    0x000040000, type::distance::identifier, lexem::If,     0},
{mnemonic::Pi,                  type::Number,     0x2000044011, type::distance::identifier, lexem::Pi,     1},
{mnemonic::Number,              type::Prefix,     0x000540001, type::distance::identifier, lexem::Number, 1},
{mnemonic::U8,                  type::Prefix,     0x000540400, type::distance::identifier, lexem::U8,     1},
{mnemonic::U16,                 type::Prefix,     0x000540800, type::distance::identifier, lexem::U16,    1},
{mnemonic::U32,                 type::Prefix,     0x000541000, type::distance::identifier, lexem::U32,    1},
{mnemonic::U64,                 type::Prefix,     0x000542000, type::distance::identifier, lexem::U64,     1},
{mnemonic::I8,                  type::Prefix,     0x000540040, type::distance::identifier, lexem::I8,      1},
{mnemonic::I16,                 type::Prefix,     0x000540080, type::distance::identifier, lexem::I16,     1},
{mnemonic::I32,                 type::Prefix,     0x000540100, type::distance::identifier, lexem::I32,     1},
{mnemonic::I64,                 type::Prefix,     0x000540200, type::distance::identifier, lexem::I64,     1},
{mnemonic::Float,               type::Prefix,     0x000544000, type::distance::identifier, lexem::Float,   1},
{mnemonic::String,              type::Prefix,     0x000540002, type::distance::identifier, lexem::String,  1},
{mnemonic::Then,                type::Keyword,    0x000040000, type::distance::identifier, lexem::Then,    0},
{mnemonic::Else,                type::Keyword,    0x000040000, type::distance::identifier, lexem::Else,    0},
{mnemonic::Const,               type::Keyword,    0x2000040000,type::distance::identifier, lexem::Const,   0},
{mnemonic::Include,             type::Keyword,    0x000040000, type::distance::identifier, lexem::Include, 0},
{mnemonic::Module,              type::Keyword,    0x000040000, type::distance::identifier, lexem::Module, 0},
{mnemonic::At,                  type::Prefix,     0x000580000, type::distance::unary,      lexem::At,      1},
{mnemonic::Prime,               type::Prefix,     0x000580000, type::distance::unary,      lexem::Prime,   1},
{mnemonic::Do,                  type::Keyword,    0x000040000, type::distance::noop_,      lexem::Do,      0},
{mnemonic::While,               type::Keyword,    0x000040000, type::distance::noop_,      lexem::While,       0},
{mnemonic::For,                 type::Keyword,    0x000040000, type::distance::noop_,      lexem::For,         0},
{mnemonic::Until,               type::Keyword,    0x000040000, type::distance::noop_,      lexem::Until,       0},
{mnemonic::Repeat,              type::Keyword,    0x000040000, type::distance::noop_,      lexem::Repeat,      0},
{mnemonic::Switch,              type::Keyword,    0x000040000, type::distance::noop_,      lexem::Switch,      0},
{mnemonic::Case,                type::Keyword,    0x000040000, type::distance::noop_,      lexem::Case,        0},
{mnemonic::Type,                type::Keyword,    0x000040000, type::distance::noop_,      lexem::Type,        0},
{mnemonic::LowHex,              type::Hex,        0x400440011, type::distance::noop_,      lexem::LowHex,     1},
{mnemonic::UpHex,               type::Hex,        0x400440011, type::distance::noop_,      lexem::UpHex,      1},
{mnemonic::Cosinus,             type::Prefix,     0x000580000, type::distance::unary,      lexem::Cosinus,     1},
{mnemonic::ArcCosinus,          type::Prefix,     0x0005C0000, type::distance::unary,      lexem::ArcCosinus, 1},
{mnemonic::Tangent,             type::Prefix,     0x0005C0000, type::distance::unary,      lexem::Tangent,     1},
{mnemonic::ArcTangent,          type::Prefix,     0x0005C0000, type::distance::unary,      lexem::ArcTangent, 1},
{mnemonic::Sinus,               type::Prefix,     0x0005C0000, type::distance::unary,      lexem::Sinus,       1},
{mnemonic::ArcSinus,            type::Prefix,     0x0005C0000, type::distance::unary,      lexem::ArcSinus,   1},
{mnemonic::Object,              type::Keyword,    0x000040000, type::distance::identifier, lexem::Object,      0},
{mnemonic::Static,              type::Keyword,    0x000040000, type::distance::identifier, lexem::Static,      0},
{mnemonic::This,                type::Keyword,    0x000040000, type::distance::identifier, lexem::This,        1}
};

// It was the right one ... duh...
token_data token_data::scan(const char* C_)
{
    int      unicode = 0;
    if (!*C_)
        return token_data::mNull;

    for (auto token : tokens_table)
    {
        const char* crs = C_;
        const char* rtxt = token.mLoc.begin;
        unicode = 0; // oops...
        std::size_t sz = std::strlen(rtxt);

        if (*crs != *token.mLoc.begin) continue;

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
                if ((isalnum(*crs) || (*crs == '_')) && !token.is_operator())
                    continue;
            }

            token.mLoc.begin = C_;
            token.mLoc.end = crs - 1;
            return token;
        }
    }
    return token_data::mNull;

}

xio::type::T token_data::ktype(mnemonic m)
{
    return key_to_type[m];
}

token_data token_data::operator[](mnemonic CC)
{
    for (token_data T : tokens_table)
    {
        if (CC == T.c)
            return T;
    }
    return token_data::mNull;
}

std::string token_data::details(bool Mark_) const
{
    stracc Str = "'%s'[%s]: offset: %d line:%d, col:%d, %s/{%s}";
    Str << text() << mnemonic_name(c) << mLoc.offset << mLoc.linenum << mLoc.colnum << type_name() << semantic_types();
    if (Mark_)
        Str << '\n' << mark(diagnostic::indentation()*4);
    return Str();
}
}


