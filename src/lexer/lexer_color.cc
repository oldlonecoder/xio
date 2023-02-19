#include <xio/lexer/lexer_color.h>
#include <xio/lexer/lexer.h>

using namespace tux;

namespace xio
{

    //Normal Types:
    std::map<xio::type::T, color::type> lexer_color::Types =
    {
        {xio::type::Assign,    color::CadetBlue},
        {xio::type::Number,    color::Cyan2},
        {xio::type::Keyword,   color::CornflowerBlue},
        {xio::type::Operator,  color::LightCoral},
        {xio::type::Binary,    color::Cornsilk1},
        {xio::type::Text,      color::DarkSlateGray3},
        {xio::type::Id,        color::White},
        {xio::type::Punc,      color::Yellow},
        {xio::type::Prefix,    color::CadetBlue},

        //...
    };


    ////Affined particular Lexem:
    //std::map<mnemonic, color::Type> LexerColor::Mnemonics =
    //{

    //};

lexer_color::affine_set lexer_color::affine_db =
{
    {xio::mnemonic::Null,                     color::White},
    {xio::mnemonic::LeftShift,               color::SkyBlue1},
    {xio::mnemonic::Radical,                  color::DeepSkyBlue7},
    {xio::mnemonic::Exponent,                 color::DeepSkyBlue7},
    {xio::mnemonic::RightShift,              color::SkyBlue1},
    {xio::mnemonic::Decr,                     color::DodgerBlue1},
    {xio::mnemonic::Incr,                     color::DodgerBlue1},
    {xio::mnemonic::AssignAdd,               color::Salmon1},
    {xio::mnemonic::AssignSub,               color::Salmon1},
    {xio::mnemonic::AssignMul,               color::Salmon1},
    {xio::mnemonic::AssignDiv,               color::Salmon1},
    {xio::mnemonic::AssignMod,               color::Salmon1},
    {xio::mnemonic::AssignAnd,               color::Salmon1},
    {xio::mnemonic::AssignOr,                color::Salmon1},
    {xio::mnemonic::AssignXor,               color::Salmon1},
    {xio::mnemonic::AssignC1,                color::Salmon1},
    {xio::mnemonic::AssignLeftShift,        color::Salmon1},
    {xio::mnemonic::AssignRightShift,       color::Salmon1},
    {xio::mnemonic::Deref,                    color::White},
    {xio::mnemonic::LessEq,                  color::LighcoreateBlue},
    {xio::mnemonic::GreaterEq,               color::LighcoreateBlue},
    {xio::mnemonic::Equal,                    color::LighcoreateBlue},
    {xio::mnemonic::NotEqual,                color::LighcoreateBlue},
    {xio::mnemonic::Add,                      color::DarkOrange3},
    {xio::mnemonic::Sub,                      color::DarkOrange3},
    {xio::mnemonic::Mul,                      color::DeepSkyBlue7},
    {xio::mnemonic::Indirection,              color::White},
    {xio::mnemonic::CommentCpp,              color::White},
    {xio::mnemonic::Modulo,                   color::LighcoreateBlue},
    {xio::mnemonic::LessThan,                color::LighcoreateBlue},
    {xio::mnemonic::GreaterThan,             color::LighcoreateBlue},
    {xio::mnemonic::Assign,                   color::Salmon1},
    {xio::mnemonic::BinaryAnd,               color::SkyBlue1},
    {xio::mnemonic::BinaryOr,                color::SkyBlue1},
    {xio::mnemonic::BinaryXor,               color::SkyBlue1},
    {xio::mnemonic::C1,                       color::SkyBlue1},
    {xio::mnemonic::C2,                       color::SkyBlue1},
    {xio::mnemonic::BinaryNot,               color::SkyBlue1},
    {xio::mnemonic::LogicalAnd,              color::LighcoreateBlue},
    {xio::mnemonic::LogicalOr,               color::LighcoreateBlue},
    {xio::mnemonic::OpenAbsOp,              color::LightSteelBlue},
    {xio::mnemonic::CloseAbsOp,             color::LightSteelBlue},
    {xio::mnemonic::OpenPar,                 color::Gold4},
    {xio::mnemonic::ClosePar,                color::Gold4},
    {xio::mnemonic::Openindex,                  color::DarkOliveGreen2},
    {xio::mnemonic::Closeindex,                 color::DarkOliveGreen2},
    {xio::mnemonic::Openbrace,                  color::Grey69},
    {xio::mnemonic::Closebrace,                 color::Grey69},
    {xio::mnemonic::BeginComment,            color::Orange3},
    {xio::mnemonic::EndComment,              color::Orange3},
    {xio::mnemonic::Div,                      color::DeepSkyBlue7},
    {xio::mnemonic::Comma,                    color::Violet},
    {xio::mnemonic::Scope,                    color::Violet},
    {xio::mnemonic::Semicolon,                color::Violet},
    {xio::mnemonic::Colon,                    color::Violet},
    {xio::mnemonic::Range,                    color::Violet},
    {xio::mnemonic::Factorial,                color::DeepSkyBlue4},
    {xio::mnemonic::Positive,                 color::White},
    {xio::mnemonic::Negative,                 color::White},
    {xio::mnemonic::Squote,                   color::White},
    {xio::mnemonic::Dquote,                   color::White},
    {xio::mnemonic::Ternary,                  color::White},
    {xio::mnemonic::Hash,                     color::White},
    {xio::mnemonic::Eos,                      color::White},
    {xio::mnemonic::Dot,                      color::Violet},
    {xio::mnemonic::Return,                   color::DodgerBlue2},
    {xio::mnemonic::If,                       color::DodgerBlue2},
    {xio::mnemonic::Pi,                       color::Gold1},
    {xio::mnemonic::Number,                   color::Aquamarine2},
    {xio::mnemonic::U8,                       color::Aquamarine2},
    {xio::mnemonic::U16,                      color::Aquamarine2},
    {xio::mnemonic::U32,                      color::Aquamarine2},
    {xio::mnemonic::U64,                      color::Aquamarine2},
    {xio::mnemonic::I8,                       color::Aquamarine2},
    {xio::mnemonic::I16,                      color::Aquamarine2},
    {xio::mnemonic::I32,                      color::Aquamarine2},
    {xio::mnemonic::I64,                      color::Aquamarine2},
    {xio::mnemonic::Float,                    color::Aquamarine2},
    {xio::mnemonic::String,                   color::Aquamarine2},
    {xio::mnemonic::Then,                     color::DodgerBlue2},
    {xio::mnemonic::Else,                     color::DodgerBlue2},
    {xio::mnemonic::Const,                    color::White},
    {xio::mnemonic::Include,                  color::White},
    {xio::mnemonic::Module,                   color::White},
    {xio::mnemonic::At,                       color::White},
    {xio::mnemonic::Prime,                    color::White},
    {xio::mnemonic::Do,                       color::DodgerBlue2},
    {xio::mnemonic::While,                    color::DodgerBlue2},
    {xio::mnemonic::For,                      color::DodgerBlue2},
    {xio::mnemonic::Until,                    color::DodgerBlue2},
    {xio::mnemonic::Repeat,                   color::DodgerBlue2},
    {xio::mnemonic::Switch,                   color::DodgerBlue2},
    {xio::mnemonic::Case,                     color::DodgerBlue2},
    {xio::mnemonic::Type,                     color::White},
    {xio::mnemonic::LowHex,                  color::White},
    {xio::mnemonic::UpHex,                   color::White},
    {xio::mnemonic::Cosinus,                  color::SpringGreen4},
    {xio::mnemonic::ArcCosinus,              color::SpringGreen4},
    {xio::mnemonic::Tangent,                  color::SpringGreen4},
    {xio::mnemonic::ArcTangent,              color::SpringGreen4},
    {xio::mnemonic::Sinus,                    color::SpringGreen4},
    {xio::mnemonic::ArcSinus,                color::SpringGreen4},
    {xio::mnemonic::Object,                   color::SpringGreen4},
    {xio::mnemonic::Static,                   color::Grey70},
    {xio::mnemonic::This,                     color::SpringGreen4},
    {xio::mnemonic::Unshadow,                 color::White},
    {xio::mnemonic::Catch,                    color::Yellow5},
    {xio::mnemonic::Throw,                    color::Yellow5},
    {xio::mnemonic::Noop,                      color::Yellow5}
};


lexer_color::lexer_color()
{
}


lexer_color::~lexer_color()
{
    //...
    _product_data.clear();
    tokens.clear();
    //...
}

code::T lexer_color::init()
{
    return code::accepted;
}

code::T lexer_color::operator<<(const std::string& aSource)
{
    lexer lex;
    _product_data = aSource;
    std::string _color;
    size_t Spacing = 0, // Current color String::Length();
           Offset = 0; // Cummulative ( Offset += Spacing )


    lex.config() = { aSource.c_str(),  & tokens}; // oop!!!
    code::T E = lex();
    if (E != code::accepted) return E;


    //...
    for (auto const& Token : tokens)
    {
        _color.clear();
        _color = attr<textattr::format::ansi256>::fg(lexer_color::affine_db[Token.c]);
        Spacing = _color.length();
        if (!_color.empty())
        {
            _product_data.insert(Token.mLoc.offset + Offset, _color);
            Offset += Spacing;
        }
    }
    return code::ok;
}

std::string lexer_color::mark(token_data& token, int indentation)
{
    stracc Str = "%s%s[%s]: offset: %d line:%d, col:%d, %s/{%s}";
    Str << Product() << color::Reset << mnemonic_name(token.c) << token.mLoc.offset << token.mLoc.linenum << token.mLoc.colnum << token.type_name() << token.semantic_types();
    Str << '\n';
    Str.fill(0x20, token.mLoc.colnum-1 + indentation);
    Str << Icon::ArrowUp;
    return Str();
}




}
