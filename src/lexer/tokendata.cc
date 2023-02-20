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
    @note !IMPORTANT : Decompress token_data::s values! -==>
    ---------------------------------------------------------------------------------------------------------------------------------------------------------
[Null           ] Keyword
[LeftShift      ] Operator/Binary
[Unshadow       ] Unary/Prefix/Operator
[OpenAbsOp      ] Operator/Binary/Punc
[CloseAbsOp     ] Operator/Punc
[Radical        ] Operator/Binary
[Exponent       ] Operator/Binary
[RightShift     ] Operator
[BinaryXor      ] Operator/Binary
[Decr           ] Unary/Prefix/Operator
[Incr           ] Unary/Prefix/Operator
[AssignAdd      ] Keyword/Operator/Binary/Assign
[AssignSub      ] Keyword/Operator/Binary/Assign
[AssignMul      ] Keyword/Operator/Binary/Assign
[AssignDiv      ] Keyword/Operator/Binary/Assign
[AssignMod      ] Keyword/Operator/Binary/Assign
[AssignAnd      ] Keyword/Operator/Binary/Assign
[AssignOr       ] Keyword/Operator/Binary/Assign
[AssignXor      ] Keyword/Operator/Binary/Assign
[AssignC1       ] Keyword/Operator/Binary/Assign
[AssignLeftShift] Keyword/Operator/Binary/Assign
[AssignRightShift] Keyword/Operator/Binary/Assign
[Deref          ] Unary/Prefix/Operator
[LessEq         ] Operator/Binary/Bool
[GreaterEq      ] Operator/Binary/Bool
[Equal          ] Operator/Binary/Bool
[NotEqual       ] Operator/Binary/Bool
[NotEqual       ] Operator/Binary/Bool
[Add            ] Operator/Binary
[Sub            ] Operator/Binary
[Mul            ] Operator/Binary
[CommentCpp     ] Operator
[Modulo         ] Operator/Binary
[LessThan       ] Operator/Binary/Bool
[GreaterThan    ] Operator/Binary/Bool
[Assign         ] Keyword/Operator/Binary/Assign
[BinaryAnd      ] Operator/Binary
[BinaryOr       ] Operator/Binary
[C2             ] Unary/Prefix/Operator
[C1             ] Unary/Prefix/Operator
[BinaryNot      ] Unary/Prefix/Operator
[LogicalAnd     ] Operator/Binary/Bool
[LogicalOr      ] Operator/Binary/Bool
[OpenPar        ] Operator/Punc/OpenPair
[ClosePar       ] Operator/Punc/ClosePair
[OpenIndex      ] Operator/Binary/Punc/Const
[CloseIndex     ] Operator/Punc/
[BraceBegin     ] Operator/Binary/Punc/OpenPair
[BraceEnd       ] Operator/Punc/ClosePair
[BeginComment   ] Null
[EndComment     ] Null
[Div            ] Operator/Binary
[Comma          ] Operator/Binary/Punc
[Scope          ] Operator/Binary/Punc
[Semicolon      ] Punc
[Colon          ] Operator/Binary/Punc
[Range          ] Operator/Binary/Punc
[Factorial      ] Unary/Prefix/Operator
[Positive       ] Signed/Unary/Prefix/Operator
[Negative       ] Signed/Unary/Prefix/Operator
[Squote         ] Text/Leaf/Operator
[Dquote         ] Text/Leaf/Operator
[Ternary        ] Keyword/Operator/Binary
[Hash           ] Unary/Prefix/Operator
[Eos            ] Unary/Prefix/Operator
[Dot            ] Operator/Punc
[Return         ] Keyword
[If             ] Keyword
[Pi             ] Number/Leaf/Float/Keyword/Const
[Number         ] Number/Keyword/Prefix/Operator
[U8             ] U8/Keyword/Prefix/Operator
[U16            ] U16/Keyword/Prefix/Operator
[U32            ] U32/Keyword/Prefix/Operator
[U64            ] U64/Keyword/Prefix/Operator
[I8             ] I8/Keyword/Prefix/Operator
[I16            ] I16/Keyword/Prefix/Operator
[I32            ] I32/Keyword/Prefix/Operator
[I64            ] I64/Keyword/Prefix/Operator
[Float          ] Float/Keyword/Prefix/Operator
[String         ] Text/Keyword/Prefix/Operator
[Then           ] Keyword
[Else           ] Keyword
[Const          ] Keyword/Const
[Include        ] Keyword
[Module         ] Keyword
[At             ] Unary/Prefix/Operator
[Prime          ] Unary/Prefix/Operator
[Do             ] Keyword
[While          ] Keyword
[For            ] Keyword
[Until          ] Keyword
[Repeat         ] Keyword
[Switch         ] Keyword
[Case           ] Keyword
[Type           ] Keyword
[LowHex         ] Number/Leaf/Keyword/Operator/Hex
[UpHex          ] Number/Leaf/Keyword/Operator/Hex
[Cosinus        ] Unary/Prefix/Operator
[ArcCosinus     ] Keyword/Unary/Prefix/Operator
[Tangent        ] Keyword/Unary/Prefix/Operator
[ArcTangent     ] Keyword/Unary/Prefix/Operator
[Sinus          ] Keyword/Unary/Prefix/Operator
[ArcSinus       ] Keyword/Unary/Prefix/Operator
[Object         ] Keyword
[Static         ] Keyword
[This           ] Keyword
*/
static token_data::collection tokens_table =
{
    {mnemonic::Null,                type::Keyword,   type::Keyword                                              ,type::distance::identifier, lexem::Null,        1},
    {mnemonic::LeftShift,           type::Binary,    type::Operator|type::Binary                                ,type::distance::shift,      lexem::LeftShift,  1},
    {mnemonic::Unshadow,            type::Prefix,    type::Unary|type::Prefix|type::Operator                    ,type::distance::scope,      lexem::Unshadow,    1},
    {mnemonic::OpenAbsOp,           type::OpenPair,  type::Operator|type::Binary|type::Punc                     ,type::distance::paranthese, lexem::AbsBegin,   0},
    {mnemonic::CloseAbsOp,          type::ClosePair, type::Operator|type::Punc                                  ,type::distance::paranthese, lexem::AbsEnd,     0},
    {mnemonic::Radical,             type::Binary,    type::Operator|type::Binary                                ,type::distance::exponent,   lexem::Radical,     1},
    {mnemonic::Exponent,            type::Binary,    type::Operator|type::Binary                                ,type::distance::exponent,   lexem::Exponent,    1},
    {mnemonic::RightShift,          type::Binary,    type::Operator|type::Binary                                ,type::distance::shift,      lexem::RightShift, 1},
    {mnemonic::BinaryXor,           type::Binary,    type::Operator|type::Binary                                ,type::distance::bits,       lexem::Xor,         1},
    {mnemonic::Decr,                type::Unary,     type::Unary|type::Prefix|type::Operator                    ,type::distance::unary,      lexem::Decr,        1},
    {mnemonic::Incr,                type::Unary,     type::Unary|type::Prefix|type::Operator                    ,type::distance::unary,      lexem::Incr,        1},
    {mnemonic::AssignAdd,           type::Assign,    type::Keyword|type::Operator|type::Binary|type::Assign     ,type::distance::assign,     lexem::AssignAdd,  1},
    {mnemonic::AssignSub,           type::Assign,    type::Keyword|type::Operator|type::Binary|type::Assign     ,type::distance::assign,     lexem::AssignSub,  1},
    {mnemonic::AssignMul,           type::Assign,    type::Keyword|type::Operator|type::Binary|type::Assign     ,type::distance::assign,     lexem::AssignMul,  1},
    {mnemonic::AssignDiv,           type::Assign,    type::Keyword|type::Operator|type::Binary|type::Assign     ,type::distance::assign,     lexem::AssignDiv,  1},
    {mnemonic::AssignMod,           type::Assign,    type::Keyword|type::Operator|type::Binary|type::Assign     ,type::distance::assign,     lexem::AssignMod,  1},
    {mnemonic::AssignAnd,           type::Assign,    type::Keyword|type::Operator|type::Binary|type::Assign     ,type::distance::assign,     lexem::AssignAnd,  1},
    {mnemonic::AssignOr,            type::Assign,    type::Keyword|type::Operator|type::Binary|type::Assign     ,type::distance::assign,     lexem::AssignOr,   1},
    {mnemonic::AssignXor,           type::Assign,    type::Keyword|type::Operator|type::Binary|type::Assign     ,type::distance::assign,     lexem::AssignXor,  1},
    {mnemonic::AssignC1,            type::Assign,    type::Keyword|type::Operator|type::Binary|type::Assign     ,type::distance::assign,     lexem::AssignC1,     1},
    {mnemonic::AssignLeftShift,     type::Assign,    type::Keyword|type::Operator|type::Binary|type::Assign     ,type::distance::assign,     lexem::AssignLshift, 1},
    {mnemonic::AssignRightShift,    type::Assign,    type::Keyword|type::Operator|type::Binary|type::Assign    ,type::distance::assign,     lexem::AssignRshift, 1},
    {mnemonic::Deref,               type::Prefix,    type::Unary|type::Prefix|type::Operator                    ,type::distance::deref,      lexem::Deref        , 1},
    {mnemonic::LessEq,              type::Binary,    type::Operator|type::Binary|type::Bool                     ,type::distance::equality,   lexem::LessEqual,    1},
    {mnemonic::GreaterEq,           type::Binary,    type::Operator|type::Binary|type::Bool                     ,type::distance::equality,   lexem::GreaterEqual, 1},
    {mnemonic::Equal,               type::Binary,    type::Operator|type::Binary|type::Bool                     ,type::distance::equality,   lexem::Equal,        1},
    {mnemonic::NotEqual,            type::Binary,    type::Operator|type::Binary|type::Bool                     ,type::distance::equality,   lexem::NotEqual,    1},
    {mnemonic::NotEqual,            type::Binary,    type::Operator|type::Binary|type::Bool                     ,type::distance::equality,   "<>",                    1},
    {mnemonic::Add,                 type::Binary,    type::Operator|type::Binary                                ,type::distance::addition,   lexem::Addition,     1},
    {mnemonic::Sub,                 type::Binary,    type::Operator|type::Binary                                ,type::distance::addition,   lexem::Sub,          1},
    {mnemonic::Mul,                 type::Binary,    type::Operator|type::Binary                                ,type::distance::product,    lexem::Multiply,     1},
    {mnemonic::CommentCpp,          type::Punc,      type::Operator                                             ,type::distance::noop_,      lexem::CommentCpp,  0},
    {mnemonic::Modulo,              type::Binary,    type::Operator|type::Binary                                ,type::distance::product,    lexem::Modulo,       1},
    {mnemonic::LessThan,            type::Binary,    type::Operator|type::Binary|type::Bool                     ,type::distance::equality,   lexem::LessThan,    1},
    {mnemonic::GreaterThan,         type::Binary,    type::Operator|type::Binary|type::Bool                     ,type::distance::equality,   lexem::GreaterThan, 1},
    {mnemonic::Assign,              type::Assign,    type::Keyword|type::Operator|type::Binary|type::Assign     ,type::distance::assign,     lexem::Assign,     1},
    {mnemonic::BinaryAnd,           type::Binary,    type::Operator|type::Binary                                ,type::distance::bits,       lexem::BinaryAnd,  1},
    {mnemonic::BinaryOr,            type::Binary,    type::Operator|type::Binary                                ,type::distance::bits,       lexem::BinaryOr,   1},
    {mnemonic::C2,                  type::Prefix,    type::Unary|type::Prefix|type::Operator                    ,type::distance::prefix,     lexem::C2,          1},
    {mnemonic::C1,                  type::Prefix,    type::Unary|type::Prefix|type::Operator                    ,type::distance::prefix,     lexem::C1,            1},
    {mnemonic::BinaryNot,           type::Unary,     type::Unary|type::Prefix|type::Operator                    ,type::distance::prefix,     lexem::Not,           1},
    {mnemonic::LogicalAnd,          type::Binary,    type::Operator|type::Binary|type::Bool                     ,type::distance::bool_and,   lexem::BoolAnd,      1},
    {mnemonic::LogicalOr,           type::Binary,    type::Operator|type::Binary|type::Bool                     ,type::distance::bool_or,    lexem::BoolOr,       1},
    {mnemonic::OpenPar,             type::OpenPair,  type::Operator|type::Punc|type::OpenPair                   ,type::distance::paranthese, lexem::OpenPar,      1},
    {mnemonic::ClosePar,            type::ClosePair, type::Operator|type::Punc|type::ClosePair                  ,type::distance::paranthese,  lexem::ClosePar,     1},
    {mnemonic::Openindex,           type::OpenPair,  type::Operator|type::Binary|type::Punc|type::OpenPair      ,type::distance::paranthese, lexem::OpenIndex,    1},
    {mnemonic::Closeindex,          type::ClosePair, type::Operator|type::Punc|type::Binary|type::ClosePair     ,type::distance::paranthese, lexem::CloseIndex,   1},
    {mnemonic::Openbrace,           type::Binary,    type::Operator|type::Binary|type::Punc|type::OpenPair      ,type::distance::paranthese, lexem::BraceBegin,  1},
    {mnemonic::Closebrace,          type::ClosePair, type::Operator|type::Binary|type::Punc|type::ClosePair     ,type::distance::paranthese, lexem::BraceEnd,     1},
    {mnemonic::BeginComment,        type::Punc,      type::Null                                                 ,type::distance::noop_,      lexem::CommentBegin, 0},
    {mnemonic::EndComment,          type::Punc,      type::Null                                                 ,type::distance::noop_,      lexem::CommentEnd, 0},
    {mnemonic::Div,                 type::Binary,    type::Operator|type::Binary                                ,type::distance::product,    lexem::Division,  1},
    {mnemonic::Comma,               type::Punc,      type::Operator|type::Binary|type::Punc                     ,type::distance::comma,      lexem::Comma,     1},
    {mnemonic::Scope,               type::Punc,      type::Operator|type::Binary|type::Punc                     ,type::distance::scope,      lexem::Scope,     1},
    {mnemonic::Semicolon,           type::Punc,      type::Punc                                                 ,type::distance::noop_,      lexem::Semicolon, 0},
    {mnemonic::Colon,               type::Punc,      type::Operator|type::Binary|type::Punc                     ,type::distance::noop_,      lexem::Colon,     0},
    {mnemonic::Range,               type::Binary,    type::Operator|type::Binary|type::Punc                     ,type::distance::scope,      lexem::Range,     1},
    {mnemonic::Factorial,           type::Postfix,   type::Unary|type::Postfix|type::Operator                   ,type::distance::unary,      lexem::Factorial, 1},
    {mnemonic::Positive,            type::Prefix,    type::Sign|type::Unary|type::Prefix|type::Operator         ,type::distance::unary,      lexem::Positive, 1},
    {mnemonic::Negative,            type::Prefix,    type::Sign|type::Unary|type::Prefix|type::Operator         ,type::distance::unary,      lexem::Negative, 1},
    {mnemonic::Squote,              type::Text,      type::Text|type::Leaf|type::Operator                       ,type::distance::identifier, lexem::Squote,  1},
    {mnemonic::Dquote,              type::Text,      type::Text|type::Leaf|type::Operator                       ,type::distance::identifier, lexem::Dquote,  1},
    {mnemonic::Ternary,             type::Keyword,   type::Keyword|type::Operator|type::Binary                  ,type::distance::unary,      lexem::Ternary, 1},
    {mnemonic::Hash,                type::Prefix,    type::Unary|type::Prefix|type::Operator                    ,type::distance::unary,      lexem::Hash,   1},
    {mnemonic::Eos,                 type::Prefix,    type::Unary|type::Prefix|type::Operator                    ,type::distance::unary,      lexem::Eos,    1},
    {mnemonic::Dot,                 type::Punc,      type::Operator|type::Punc                                  ,type::distance::scope,      lexem::Dot,    1},
    {mnemonic::Return,              type::Keyword,   type::Keyword                                              ,type::distance::identifier, lexem::Return, 0},
    {mnemonic::If,                  type::Keyword,   type::Keyword                                              ,type::distance::identifier, lexem::If,     0},
    {mnemonic::Pi,                  type::Number,    type::Number|type::Leaf|type::Float|type::Keyword|type::Const ,type::distance::identifier, lexem::Pi,     1},
    {mnemonic::Number,              type::Prefix,    type::Number|type::Keyword|type::Prefix|type::Operator     ,type::distance::identifier, lexem::Number, 1},
    {mnemonic::U8,                  type::Prefix,    type::U8|type::Keyword|type::Prefix|type::Operator         ,type::distance::identifier, lexem::U8,     1},
    {mnemonic::U16,                 type::Prefix,    type::U16|type::Keyword|type::Prefix|type::Operator        ,type::distance::identifier, lexem::U16,    1},
    {mnemonic::U32,                 type::Prefix,    type::U32|type::Keyword|type::Prefix|type::Operator        ,type::distance::identifier, lexem::U32,    1},
    {mnemonic::U64,                 type::Prefix,    type::U64|type::Keyword|type::Prefix|type::Operator        ,type::distance::identifier, lexem::U64,     1},
    {mnemonic::I8,                  type::Prefix,    type::I8|type::Keyword|type::Prefix|type::Operator         ,type::distance::identifier, lexem::I8,      1},
    {mnemonic::I16,                 type::Prefix,    type::I16|type::Keyword|type::Prefix|type::Operator        ,type::distance::identifier, lexem::I16,     1},
    {mnemonic::I32,                 type::Prefix,    type::I32|type::Keyword|type::Prefix|type::Operator        ,type::distance::identifier, lexem::I32,     1},
    {mnemonic::I64,                 type::Prefix,    type::I64|type::Keyword|type::Prefix|type::Operator        ,type::distance::identifier, lexem::I64,     1},
    {mnemonic::Float,               type::Prefix,    type::Float|type::Keyword|type::Prefix|type::Operator      ,type::distance::identifier, lexem::Float,   1},
    {mnemonic::String,              type::Prefix,    type::Text|type::Keyword|type::Prefix|type::Operator       ,type::distance::identifier, lexem::String,  1},
    {mnemonic::Then,                type::Keyword,   type::Keyword                                              ,type::distance::identifier, lexem::Then,    0},
    {mnemonic::Else,                type::Keyword,   type::Keyword                                              ,type::distance::identifier, lexem::Else,    0},
    {mnemonic::Const,               type::Keyword,   type::Keyword|type::Const                                  ,type::distance::identifier, lexem::Const,   0},
    {mnemonic::Include,             type::Keyword,   type::Keyword                                              ,type::distance::identifier, lexem::Include, 0},
    {mnemonic::Module,              type::Keyword,   type::Keyword                                              ,type::distance::identifier, lexem::Module, 0},
    {mnemonic::At,                  type::Prefix,    type::Unary|type::Prefix|type::Operator                    ,type::distance::unary,      lexem::At,      1},
    {mnemonic::Prime,               type::Prefix,    type::Unary|type::Prefix|type::Operator                    ,type::distance::unary,      lexem::Prime,   1},
    {mnemonic::Do,                  type::Keyword,   type::Keyword                                              ,type::distance::noop_,      lexem::Do,      0},
    {mnemonic::While,               type::Keyword,   type::Keyword                                              ,type::distance::noop_,      lexem::While,       0},
    {mnemonic::For,                 type::Keyword,   type::Keyword                                              ,type::distance::noop_,      lexem::For,         0},
    {mnemonic::Until,               type::Keyword,   type::Keyword                                              ,type::distance::noop_,      lexem::Until,       0},
    {mnemonic::Repeat,              type::Keyword,   type::Keyword                                              ,type::distance::noop_,      lexem::Repeat,      0},
    {mnemonic::Switch,              type::Keyword,   type::Keyword                                              ,type::distance::noop_,      lexem::Switch,      0},
    {mnemonic::Case,                type::Keyword,   type::Keyword                                              ,type::distance::noop_,      lexem::Case,        0},
    {mnemonic::Type,                type::Keyword,   type::Keyword                                              ,type::distance::noop_,      lexem::Type,        0},
    {mnemonic::LowHex,              type::Hex,       type::Number|type::Leaf|type::Keyword|type::Operator|type::Hex  ,type::distance::noop_,      lexem::LowHex,     1},
    {mnemonic::UpHex,               type::Hex,       type::Number|type::Leaf|type::Keyword|type::Operator|type::Hex  ,type::distance::noop_,      lexem::UpHex,      1},
    {mnemonic::Cosinus,             type::Prefix,    type::Unary|type::Prefix|type::Operator                    ,type::distance::unary,      lexem::Cosinus,     1},
    {mnemonic::ArcCosinus,          type::Prefix,    type::Keyword|type::Unary|type::Prefix|type::Operator      ,type::distance::unary,      lexem::ArcCosinus, 1},
    {mnemonic::Tangent,             type::Prefix,    type::Keyword|type::Unary|type::Prefix|type::Operator      ,type::distance::unary,      lexem::Tangent,     1},
    {mnemonic::ArcTangent,          type::Prefix,    type::Keyword|type::Unary|type::Prefix|type::Operator      ,type::distance::unary,      lexem::ArcTangent, 1},
    {mnemonic::Sinus,               type::Prefix,    type::Keyword|type::Unary|type::Prefix|type::Operator      ,type::distance::unary,      lexem::Sinus,       1},
    {mnemonic::ArcSinus,            type::Prefix,    type::Keyword|type::Unary|type::Prefix|type::Operator      ,type::distance::unary,      lexem::ArcSinus,   1},
    {mnemonic::Object,              type::Keyword,   type::Keyword                                              ,type::distance::identifier, lexem::Object,      0},
    {mnemonic::Static,              type::Keyword,   type::Keyword                                              ,type::distance::identifier, lexem::Static,      0},
    {mnemonic::This,                type::Keyword,   type::Keyword                                              ,type::distance::identifier, lexem::This,        1}
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

std::string token_data::dump_token_table()
{
    stracc acc = "-----------------------Tokens Table --------------------------\n";

    for(auto token: tokens_table)
    {
        acc = "[%-16s] %s\n";
        acc << mnemonic_name(token.c) << token.semantic_types();
        std::cout << acc();
    }
    return acc();
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
    stracc Str;
    Str << '\'' << text() << '\'';
    Str += "[%s]: offset: %d line:%d, col:%d, %s/{%s}";
    Str << mnemonic_name(c) << mLoc.offset << mLoc.linenum << mLoc.colnum << type_name() << semantic_types();
    if (Mark_)
        Str << '\n' << mark(diagnostic::indentation()*4);
    return Str();
}
}


