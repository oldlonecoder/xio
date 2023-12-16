/****************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                          *
 *   serge.lussier@oldlonecoder.club                                        *
 *                                                                          *
 *   This is a (self-)learning and exploration code. Pro or advanced        *
 *   academic stadards are not To be expexted.                              *
 *   Unless otherwise specified, all code in this project is written        *
 *   by the author (Serge Lussier)                                          *
 *   ---------------------------------------------------------------------  *
 *   This project is under the GPL.                                         *
 ****************************************************************************/

#include "Spp/Compiler/Lexer.h"
#include <array>
#include <AppBook/Utf/Glyphes.h>

#include <map>
#include <AppBook/Book/AppBook.h>


namespace Spp {


/*!
 * @brief Static local scanners table:
 */
Lexer::ScanTbl ScanTable;



/*!
 * @brief Type::Colors pairs.
 */
std::map<Type::T, Color::Code> PrimitiveTypesColors =
{
    {Type::Assign,      Color::CadetBlue},
    {Type::Number,      Color::Cyan2},
    {Type::Keyword,     Color::CornflowerBlue},
    {Type::Operator,    Color::LightCoral},
    {Type::Binary,      Color::Cornsilk1},
    {Type::Text,        Color::DarkSlateGray3},
    {Type::Id,          Color::White},
    {Type::Punc,        Color::Yellow},
    {Type::Prefix,      Color::CadetBlue},
    {Type::LineComment, Color::Grey66},
    {Type::BlocComment, Color::Grey63}
    //...
};


/*!
 * @brief Affined Mnemonic-Color pairs.
 * @note : The "Affined" is a Work in progress ;)
 */
std::map<Mnemonic, Color::Code> MnemonicColors =
    {
        {Mnemonic::Null,             Color::White},
        {Mnemonic::LeftShift,        Color::SkyBlue1},
        {Mnemonic::Radical,          Color::DeepSkyBlue7},
        {Mnemonic::Exponent,         Color::DeepSkyBlue7},
        {Mnemonic::RightShift,       Color::SkyBlue1},
        {Mnemonic::Decr,             Color::DodgerBlue1},
        {Mnemonic::Incr,             Color::DodgerBlue1},
        {Mnemonic::AssignAdd,        Color::Salmon1},
        {Mnemonic::AssignSub,        Color::Salmon1},
        {Mnemonic::AssignMul,        Color::Salmon1},
        {Mnemonic::AssignDiv,        Color::Salmon1},
        {Mnemonic::AssignMod,        Color::Salmon1},
        {Mnemonic::AssignAnd,        Color::Salmon1},
        {Mnemonic::AssignOr,         Color::Salmon1},
        {Mnemonic::AssignXor,        Color::Salmon1},
        {Mnemonic::AssignC1,         Color::Salmon1},
        {Mnemonic::AssignLeftShift,  Color::Salmon1},
        {Mnemonic::AssignRightShift, Color::Salmon1},
        {Mnemonic::Deref,            Color::White},
        {Mnemonic::LessEq,           Color::LighcoreateBlue},
        {Mnemonic::GreaterEq,        Color::LighcoreateBlue},
        {Mnemonic::Equal,            Color::LighcoreateBlue},
        {Mnemonic::NotEqual,         Color::LighcoreateBlue},
        {Mnemonic::Add,              Color::DarkOrange3},
        {Mnemonic::Sub,              Color::DarkOrange3},
        {Mnemonic::Mul,              Color::LighcoreateBlue},
        {Mnemonic::Indirection,      Color::White},
        {Mnemonic::CommentCpp,       Color::White},
        {Mnemonic::Modulo,           Color::LighcoreateBlue},
        {Mnemonic::LessThan,         Color::LighcoreateBlue},
        {Mnemonic::GreaterThan,      Color::LighcoreateBlue},
        {Mnemonic::Assign,           Color::Salmon1},
        {Mnemonic::BinaryAnd,        Color::SkyBlue1},
        {Mnemonic::BinaryOr,         Color::SkyBlue1},
        {Mnemonic::BinaryXor,        Color::SkyBlue1},
        {Mnemonic::C1,               Color::SkyBlue1},
        {Mnemonic::C2,               Color::SkyBlue1},
        {Mnemonic::BinaryNot,        Color::SkyBlue1},
        {Mnemonic::LogicalAnd,       Color::LighcoreateBlue},
        {Mnemonic::LogicalOr,        Color::LighcoreateBlue},
        {Mnemonic::OpenAbsOp,        Color::LightSteelBlue},
        {Mnemonic::CloseAbsOp,       Color::LightSteelBlue},
        {Mnemonic::OpenPar,          Color::Gold4},
        {Mnemonic::ClosePar,         Color::Gold4},
        {Mnemonic::Openindex,        Color::DarkOliveGreen2},
        {Mnemonic::Closeindex,       Color::DarkOliveGreen2},
        {Mnemonic::Openbrace,        Color::Grey69},
        {Mnemonic::Closebrace,       Color::Grey69},
        {Mnemonic::BeginComment,     Color::Grey53},
        {Mnemonic::EndComment,       Color::Grey54},
        {Mnemonic::Div,              Color::DeepSkyBlue7},
        {Mnemonic::Comma,            Color::Violet},
        {Mnemonic::Scope,            Color::Violet},
        {Mnemonic::Semicolon,        Color::Violet},
        {Mnemonic::Colon,            Color::Violet},
        {Mnemonic::Range,            Color::Violet},
        {Mnemonic::Factorial,        Color::DeepSkyBlue4},
        {Mnemonic::Positive,         Color::White},
        {Mnemonic::Negative,         Color::White},
        {Mnemonic::Squote,           Color::White},
        {Mnemonic::Dquote,           Color::White},
        {Mnemonic::Ternary,          Color::White},
        {Mnemonic::Hash,             Color::White},
        {Mnemonic::Eos,              Color::White},
        {Mnemonic::Dot,              Color::Violet},
        {Mnemonic::Return,           Color::DodgerBlue2},
        {Mnemonic::If,               Color::DodgerBlue2},
        {Mnemonic::Pi,               Color::Gold1},
        {Mnemonic::Number,           Color::Aquamarine2},
        {Mnemonic::U8,               Color::Aquamarine2},
        {Mnemonic::U16,              Color::Aquamarine2},
        {Mnemonic::U32,              Color::Aquamarine2},
        {Mnemonic::U64,              Color::Aquamarine2},
        {Mnemonic::I8,               Color::Aquamarine2},
        {Mnemonic::I16,              Color::Aquamarine2},
        {Mnemonic::I32,              Color::Aquamarine2},
        {Mnemonic::I64,              Color::Aquamarine2},
        {Mnemonic::Float,            Color::Aquamarine2},
        {Mnemonic::String,           Color::Aquamarine2},
        {Mnemonic::Then,             Color::DodgerBlue2},
        {Mnemonic::Else,             Color::DodgerBlue2},
        {Mnemonic::Const,            Color::White},
        {Mnemonic::Include,          Color::White},
        {Mnemonic::Unit,             Color::White},
        {Mnemonic::At,               Color::White},
        {Mnemonic::Prime,            Color::White},
        {Mnemonic::Do,               Color::DodgerBlue2},
        {Mnemonic::While,            Color::DodgerBlue2},
        {Mnemonic::For,              Color::DodgerBlue2},
        {Mnemonic::Until,            Color::DodgerBlue2},
        {Mnemonic::Repeat,           Color::DodgerBlue2},
        {Mnemonic::Switch,           Color::DodgerBlue2},
        {Mnemonic::Case,             Color::DodgerBlue2},
        {Mnemonic::Type,             Color::White},
        {Mnemonic::LowHex,           Color::White},
        {Mnemonic::UpHex,            Color::White},
        {Mnemonic::Cosinus,          Color::SpringGreen4},
        {Mnemonic::ArcCosinus,       Color::SpringGreen4},
        {Mnemonic::Tangent,          Color::SpringGreen4},
        {Mnemonic::ArcTangent,       Color::SpringGreen4},
        {Mnemonic::Sinus,            Color::SpringGreen4},
        {Mnemonic::ArcSinus,         Color::SpringGreen4},
        {Mnemonic::Object,           Color::SpringGreen4},
        {Mnemonic::Static,           Color::Grey70},
        {Mnemonic::This,             Color::SpringGreen4},
        {Mnemonic::Unshadow,         Color::White},
        {Mnemonic::Catch,            Color::Yellow5},
        {Mnemonic::Throw,            Color::Yellow5},
        {Mnemonic::Noop,             Color::Grey66},
        {Mnemonic::LineComment,      Color::Grey66},
        {Mnemonic::BlocComment,      Color::Grey63}
    };


#pragma region InternalCursor

/*!
 * @brief Skips white spaces character, advancing(/consuming) source pointer
 *
 * prefix increment operator
 * @return true if source pointer is not on EOF, false otherwise.
 */
bool Lexer::LexCursor::operator++()
{
    if (C >= E)
        return false;
    ++C;
    return SkipWS();
}

/*!
 * @brief Skips white spaces character, advancing(/consuming) source pointer
 *
 * Postfix increment operator, just calls the prefix increment operator.
 * @return true if source pointer is not on EOF, false otherwise.
 */
bool Lexer::LexCursor::operator++(int)
{
    if (C >= E)
        return false;
    ++C;
    return SkipWS();
}

/*!
* @brief Skips white spaces character, advancing(/consuming) source pointer
*
* Named method, just calls the prefix increment operator.
* @return true if source pointer is not on EOF, false otherwise.
*/
[[maybe_unused]] bool Lexer::LexCursor::SkipWS()
{
    while ((C <= E) && (isspace(*C)))
    {
        ++col;
        if (*C == '\n' || *C == '\r')
        {
            ++line;
            col = 1;
        }

        ++C;
    }
    return C <= E;
}

/*!
 * @brief Tests if P or source pointer is on or past EOF.
 * @param P nullptr if the actual source pointer is to be used.
 * @return true if P or source pointer is EOF, false otherwise.
 */
bool Lexer::LexCursor::EndOfFile(const char *P) const
{
    if (P)
        return P > E;
    return C > E;
}

/*!
 * @brief Synchronize the Location data from the actual source pointer.
 *
 * @return none.
 */
void Lexer::LexCursor::Sync()
{
    const char *C_;
    line = 1;
    col = 1;
    C_ = B;
    while ((C_ >= B) && (C_ <= E) && (C_ < C))
    {
        if ((*C_ == '\n') || (*C_ == '\r'))
        {
            col = 1;
            line++;
        } else
            ++col;
        ++C_;
    }
}

/*!
 * @brief Get the ptrdiff between the source pointer and the beginning of the source Text (B pointer).
 * @return int.
 */
[[maybe_unused]] std::ptrdiff_t Lexer::LexCursor::Index() const
{
    return (std::ptrdiff_t) (C - B);
}

/*!
 * @brief Advances/Consume the source pointer till the Next NewLine{'\r'; '\n'}  code in the source Text
 * @return distinct std::string of the sequence.
 */
[[maybe_unused]] std::string Lexer::LexCursor::ScanToEol()
{
    std::string Str;
    while ((C <= E) && (*C != '\n') && (*C != '\r'))
        Str += *C++;
    --C;
    return Str;
}

/*!
 * @brief Advances/Consumes the source pointer To Skip till SubStr_ match.
 * @param SubStr_
 * @return Expect code.
 */
[[maybe_unused]] [[maybe_unused]] Book::Result Lexer::LexCursor::ScanTo(const char *SubStr_)
{
    // Start from the current position in a temporary string:
    std::string Str = C;
    // use its find utility and skip to the position if SubStr_ is found:
    auto Pos = Str.find(SubStr_,0);
    if(Pos != std::string::npos)
    {
        C += Pos;
        return Book::Result::Accepted;
    }
    return Book::Result::Rejected;
}


Book::Result Lexer::LexCursor::BlocComment()
{
    StrAcc str;
    StrAcc bloc;
    bloc << *C;
    while (C <= E)
    {

        if ((*C == '\'') || (*C == '"'))
        {
            str = ScanString();

            C += str.Len();
            bloc << str;
            continue;
        }

        if (*C == '*')
        {
            ++C;
            bloc << *C;
            if (*C == '/')
            {
//                AppBook::Debug() << Book::Enums::Fn::endl
//                                  << Color::Yellow << '[' << Book::Enums::Fn::endl
//                                  << Color::Reset << bloc << Book::Enums::Fn::endl
//                                  << Color::Yellow << ']';
                Sync();
//                AppBook::Debug() << "Leaving lex_cursor::bloc_comment():" << Book::Enums::Fn::endl
//                                 ;
//                Book::Result::out() << "w *C = '" << Color::Yellow << *C
//                           << Color::Reset << "', column " << Color::Yellow
//                           << col << Color::Reset << ", Line " << Color::Yellow
//                           << line;
                return Book::Result::Accepted;
            }
            continue;
        }
        ++C;
        bloc << *C;
    }
    throw AppBook::Exception() [ AppBook::Error() << Book::Result::Eof << " Lexer: unmatched bloc comment End." ];
}

/*!
 * @brief Get a std::string copy of the current line from the source pointer
 * @return std::string.
 */
[[maybe_unused]] std::string Lexer::LexCursor::ExtractTextLine() const
{
    std::string Str;
    Str.clear();

    const char *lb, *eb;
    lb = eb = C;

    while ((eb <= E) && (*eb != '\r') && (*eb != '\n'))
        ++eb;
    while ((lb > B) && (*lb != '\r') && (*lb != '\n'))
        --lb;

    for (; lb < eb; lb++)
        Str += *lb;
    return Str;
}

/*!
 * @brief Extract line from the current position and then put "Mark" at the current column number.
 * @return std::string
 * @param  nspc : not used anymore.
 * @note : Must be Sync()'ed before calling Mark();
 * @todo Remove parameter nspc.
 */
std::string Lexer::LexCursor::Mark(int nspc) const
{
    StrAcc Str;
    if (nspc)
        Str.fill(0x20, nspc);
    Str << line << "," << col << ":\n";
    int l = 1;
    const char *cc = B;
    while (l++ <= line)
    {
        while ((cc <= E) && (*cc != '\n')) Str << *cc++;
    }

    Str << '\n';
    if (nspc)
        Str.fill(0x20, nspc);
    for (int x = 1; x < col; x++)
        Str << ' ';
    Str << Utf::Glyph::CArrowUp;
    return Str();
}

/*!
 * @brief Get the string representation of the [internal]cursor Location in the source Text.
 * @return std::string
 */
[[maybe_unused]] std::string Lexer::LexCursor::Location() const
{
    StrAcc Str = "%d,%d";
    Str << line << col;
    return Str();
}

std::string Lexer::LexCursor::ScanString()
{
    //book::codeDebug(__PRETTY_FUNCTION__ +':'+ __LINE__) << '\n';
    const char *be = C;
    char Quote_ = *be;
    std::string Str;
    Str += *be;
    ++be;

    Continue:
    while ((be <= E) && (*be != '\\') && (*be != Quote_))
        Str += *be++;
    if (*be == '\\')
    {
        Str += *be++;
        Str += *be++; // Assume no terminal escape sequence !!!!! PLEASE be a quote!!!
        goto Continue;
    }

    if ((*be != Quote_) && (be > E))
    {
        Sync();
        AppBook::Error() << " Unterminated string constant in internal_cursor::ScanString." << Book::Enums::Fn::Endl << Mark();
        return "eof";
    }
    Str += *be; // Include the rhs Quote.
    return Str;
}

//[[maybe_unused]] std::string Lexer::LexCursor::PrintLocation()
//{
//    return Location();
//}

Lexer::LexCursor::LexCursor(const char *Source_)
{
    C = Source_;
    E = C + std::strlen(C) - 1;
    B = C;
    line = col = 1;
}

#pragma endregion InternalCursor

#pragma region NumScanner

Lexer::NumScanner::NumScanner(const char *_c, const char *_eos)
    : B(_c), E(nullptr), C(_c), Eos(_eos)
{
}

/*!
 * @brief For now a bare minimum digit with some rough floating point Scan.
 * @return true if the source pointer is consumed and advanced
 */
bool Lexer::NumScanner::operator++(int)
{
    if (C > Eos)
        return false;

    if (!isdigit(*C))
    {
        if (*C == '.')
        {
            if (!Real)
            {
                const char *e = C;
                ++e;
                if (!isdigit(*e)) return false;
                St = true;
                Real = true;
            } else
            {
                St = false;
                return false;
            }
        } else
        {
            return false;
        }
    }
    St = true;
    E = C;
    ++C;
    return St;
}

/*!
 * @brief Implements boolean operator
 * @return true if this NumScanner was  a valid numeric sequence, false otherwise.
 */
Lexer::NumScanner::operator bool() const
{
    return St;
}

/*!
 * @brief Computes the numeric scale and 'best guess' base.
 * @return one [combined] of {{u,i}{8,16,32,64}} | fp| oct | hex | bin.
 *
 * @note Numeric Base and sign are omitted as of this version.
 *       Thus it only computes the Scale as unsigned integers.
 *
 * //std::array<Spp::Type::T, 4> Capacity = {Spp::Type::U8, Spp::Type::U16, Spp::Type::U32, Spp::Type::U64};
 * //Spp::Type::T                Token   = Capacity[I];
 */
Spp::Type::T Lexer::NumScanner::operator()() const
{
    if (!Real)
    {
        std::string temp = B;
        StrAcc Str = std::string(temp, 0, E - B);
        uint64_t D;
        Str >> D;
        int scale = 0;
        std::array<uint64_t, 3> R = {0x100, 0x10000, 0x100000000};
        while (D >= R[scale]) ++scale;
        return std::array<Spp::Type::T, 4>{Spp::Type::U8, Spp::Type::U16, Spp::Type::U32, Spp::Type::U64}[scale];
    }

    return Spp::Type::Float;
}

#pragma endregion NumScanner

#pragma region Scanners

// + 4ac(x^2+y+b)

Lexer::ScannerFn Lexer::GetScanner(SppToken &Token)
{
    //AppBook::Debug() << " for " << Color::Yellow << token.details(false);

    for (const auto &scanners_tbl: ScanTable)
    {
        if (Token.S & scanners_tbl.first)
            return scanners_tbl.second;
    }
    return nullptr;
}

Book::Result Lexer::InputBinaryOperator(SppToken &Token)
{
    //logger::debug() << __PRETTY_FUNCTION__ << ":\n";
    if (Token.M == Mnemonic::Sub || Token.M == Mnemonic::Add)
    {
        if (ScanSignPrefix(Token) == Book::Result::Accepted)
            return Book::Result::Accepted;
    }
    Accept(Token);
    _Cursor._F = false;
    return Book::Result::Accepted;
}

/*!
 * @brief Unknown Input Token (Either Number litteral or identifier).
 * @return Expect<>
 */
Book::Result Lexer::InputDefault(SppToken &Token)
{
    //logger::debug() << __PRETTY_FUNCTION__ << ":\n";
    if (ScanNumber(Token) != Book::Result::Accepted)
    {
        //book::codeDebug() << " Not a Number Trying scan_identifier:";
        if (ScanIdentifier(Token) != Book::Result::Accepted)
        {
            //book::codePush() << mCursor.mark() << ":";
            return Book::Result::Rejected;//book::codePush() << //book::codexio::Type::Fatal << ": " << //book::codeInt::UnExpected << " Token Type " << Token.Type_name();
        }
    }

    return Book::Result::Accepted; // return Book::Result::Rejected  --- duh?
}

[[maybe_unused]] Book::Result Lexer::InputUnaryOperator(SppToken &Token)
{

    // Possible prefix and Postfix unary operators:
    if ((Token.M == Mnemonic::BinaryNot) || (Token.M == Mnemonic::Decr) ||
        (Token.M == Mnemonic::Incr))
    {
        if (_Config.Tokens->empty() ||
            (_Config.Tokens->back().S & Spp::Type::Binary))
            return ScanPrefix(Token);
        return ScanPostfix(Token);
    }
    if (Token.T == Spp::Type::Prefix)
        return ScanPrefix(Token);
    return ScanPostfix(Token);
}

Book::Result Lexer::InputPunctuation(SppToken &Token)
{
    //AppBook::Debug() << Token.Mark() << " @ " << _Cursor.Location();
    if (Token.M == Mnemonic::OpenPar)
    {
        AppBook::Debug() << Token.Text() << " - Openpar: check for mul insertion:";
        // Bug : the flag can be set without proper previous condition...let's investigate! :)
        if (_Cursor._F)
        {
            if (!_Config.Tokens->empty() && _Config.Tokens->back().Flags.V)
            {
                Accept(Token);
                InsertMultiply(Token);
                return Book::Result::Accepted;
            }
        }
        AppBook::Debug() << " not...Then checking if previous TokenPtr is number:";
        if (!_Config.Tokens->empty())
        {
            AppBook::Out() << "Previous TokenPtr: " << Book::Enums::Fn::Endl << _Config.Tokens->back().Details(true) << ":";

            // La seule et unique condition est que le token precedant soit une valeur numerique litterale (ex.: '4').
            if (_Config.Tokens->back().IsNumber())
            {
                Accept(Token);
                InsertMultiply(Token);
                return Book::Result::Accepted;
            }
        }
        AppBook::Debug() << "not...proceeding To the Next TokenPtr ... ";
        return Accept(Token);
    }
    // ... A = .0123 :
    if (Token.M == Mnemonic::Dot)
    {
        if (Book::Result r; (r = ScanNumber(Token)) == Book::Result::Accepted)
            return r;
    }

    return Accept(Token);
}

Book::Result Lexer::InputKeyword(SppToken &Token)
{
    return Accept(Token);
}

Book::Result Lexer::InputHex(SppToken &Token)
{
    AppBook::Debug() << ':';
    const char *C_ = _Cursor.C;
    C_ += Token.Text().length();
    const char *E_ = C_;
    if (isspace(*E_))
        return Book::Result::Rejected;

    while (*E_ && !isspace(*E_) &&
           (((*E_ >= '0') && (*E_ <= '9')) || ((*E_ >= 'A') && (*E_ <= 'F')) || ((*E_ >= 'a') && (*E_ <= 'f'))))
        ++E_;

    if (E_ > C_)
        --E_;

    Token.Location.end = E_;
    Token.Flags.V = 1;
    return Accept(Token);
}

/*!
 * @brief Scans const numeric constrtuct
 * @param Token
 * @return Book::Result::Accepted;
 * @todo Scan Scientific Notation!!!
 */
Book::Result Lexer::ScanNumber(SppToken &Token)
{

    //logger::debug() << __PRETTY_FUNCTION__ << ":\n";
    NumScanner Num = NumScanner(_Cursor.C, _Cursor.E);
    while (Num++)
    { ;
    }
    if (!Num)
        return Book::Result::Rejected;

    if (_Cursor._F)
        _Cursor._F = false;

    Token.T = Spp::Type::Number;
    Token.S = Spp::Type::Number | Num() | Spp::Type::Leaf;
    Token.Flags.V = 1;
    Token.Location.begin = Num.B;
    Token.Location.end = Num.E; // And Num.C ?
    // book::codeDebug() << "Lexer::scan_number: Cursor on \n" << mCursor.mark();
    if (!(Token.S & Spp::Type::Float))
    {
        StrAcc str;
        str << Token.Text();
        uint64_t D = 0;
        std::istringstream i(str().c_str());
        switch (Num.Num)
        {
            case NumScanner::Bin:
                //????????? ah!
                break;
            case NumScanner::Oct:
                i >> std::oct >> D;
                break;
            case NumScanner::Dec:
                i >> D;
                break;
            case NumScanner::Hex:
                i >> std::hex >> D;
                break;
            default:
                str >> D;
                break;
        }

        //std::cout << __PRETTY_FUNCTION__ << " Parsed number:" << D << '\n';
        uint64_t n = 0;
        std::array<uint64_t, 3> R = {0x100, 0x10000, 0x100000000};
        while (D >= R[n])
            ++n;

        switch (n)
        {
            case 0:
                Token.S = (Token.S & ~Spp::Type::U64) | Spp::Type::U8;
                break;
            case 1:
                Token.S = (Token.S & ~Spp::Type::U64) | Spp::Type::U16;
                break;
            case 2:
                Token.S = (Token.S & ~Spp::Type::U64) | Spp::Type::U32;
                break;
            default:
                Token.S |= Spp::Type::U64;
                break;
        }
    }
    Token.M = Mnemonic::Noop;
    Token.S |= Type::Const;
    return Accept(Token);
    //return book::codeAccepted;
}

Book::Result Lexer::ScanIdentifier(SppToken &Token)
{

    //book::codeDebug(__PRETTY_FUNCTION__);
    const char *C = _Cursor.C;
    if ((!isalpha(*C)) && (*C != '_')) return Book::Result::Rejected;

    if (!_Cursor._F)
    {
        if (!_Config.Tokens->empty())
        {
            // if((mConfig.Tokens->back().s & Spp::Type::Number) && !mConfig.Tokens->back()._flags.V)
            if (_Config.Tokens->back().Flags.V &&
                !(_Config.Tokens->back().S & Spp::Type::Operator))
                _Cursor._F = true;
            else
            {
                while (*C && (isalnum(*C) || (*C == '_'))) ++C;
                --C;
            }
        } else
        {
            while (*C && (isalnum(*C) || (*C == '_')))
                ++C;
            --C;
        }
    }
    Token.Location.begin = _Cursor.C;
    Token.Location.end = C;
    Token.T = Spp::Type::Id;
    Token.S = Spp::Type::Id | Spp::Type::Leaf;
    Token.M = Mnemonic::Noop;
    Token.Location.Linenum = _Cursor.line;
    Token.Location.Colnum = _Cursor.col;
    Token.D = Distance::Identifier;
    Token.Flags.V = 1;// src_cursor._F ? 1 : 0; //Subject to be modified
    Accept(Token);
    if (_Cursor._F && !_Config.Tokens->back().IsOperator())
        if ((Token.Location.Colnum - _Config.Tokens->back().Location.Colnum) <
            2) // no spaces between multiply two adjacent letters!!! (
            // 2023-07-sept oldlonecoder).
            InsertMultiply(Token);

    return Book::Result::Accepted;
}

void Lexer::InsertMultiply(SppToken &Token) const
{

    auto i = --_Config.Tokens->end();
    --i;
//    AppBook::Debug()
//        << Token.Text() << "::colnum :" << Color::Yellow << Token.loc.colnum
//       ;
//    AppBook::Debug() << i->text() << "::colnum :" << Color::Yellow
//                                << i->loc.colnum;

    if ((Token.Location.Colnum - i->Location.Colnum) >= 2)
    {
        AppBook::Debug() << " Rejected because there must be no spaces between the involved tokens...";
        AppBook::Debug() << Book::Fn::Endl << Token.Details(true) << Book::Fn::Endl << i->Details(true);
        return;
    }
    ++i;
    SppToken Mul;
    Mul = Token; // Push Token properties in the incoming virtual multiply operator
    Mul.T = Spp::Type::Binary;
    Mul.S = Spp::Type::Binary | Spp::Type::Operator;
    Mul.D = Distance::Product;
    Mul.Flags.M = Mul.Flags.V = 1;
    Mul.M = Mnemonic::Mul;

    _Config.Tokens->insert(i, Mul);
    //logger::debug() << __PRETTY_FUNCTION__ << ":\n Details:" << Mul.details() << "\n" << Mul.mark();
}


Book::Result Lexer::ScanSignPrefix(SppToken &Token)
{
    if (!_Config.Tokens->empty() &&
        (_Config.Tokens->back().S & Spp::Type::ClosePair))
    {
        //logger::comment() << "Lexer::scan_sign_prefix:\n" << Token.Mark() << "\n" << " Rejected...\n";
        return Book::Result::Rejected;
    }

    if (_Config.Tokens->empty() || _Config.Tokens->back().IsBinary() ||
        _Config.Tokens->back().IsPunctuation() ||
        !_Config.Tokens->back().Flags.V)
    {
        Token.T = Spp::Type::Prefix;
        Token.S = (Token.S & ~Spp::Type::Binary) | Spp::Type::Sign | Spp::Type::Unary |
                   Spp::Type::Prefix; // Spp::Type::Operator bit already set
        return Accept(Token);
    }
    return Book::Result::Rejected;
}

/*!
 * @brief  Operators such as '--', '++', '!' can be prefix or postfix unary operators.
 *
 * @return
 */
Book::Result Lexer::ScanPrefix(SppToken &Token)
{
    // Possible prefix and Postfix unary operators:
    if ((Token.M == Mnemonic::BinaryNot) || (Token.M == Mnemonic::Decr) ||
        (Token.M == Mnemonic::Incr))
    {
        if (_Config.Tokens->empty() ||
            (_Config.Tokens->back().S & Spp::Type::Binary))
            return Accept(Token);
        return ScanPostfix(Token);
    }
    return Accept(Token);
}

/*!
 * @brief  Operators such as '--', '++', '!' can be prefix or postfix unary operators.
 *
 * @return
 */
Book::Result Lexer::ScanPostfix(SppToken &Token)
{
    if (!((Token.M == Mnemonic::Decr) || (Token.M == Mnemonic::Incr) ||
          (Token.M == Mnemonic::BinaryNot)))
        return Book::Result::Rejected;

    Token.T = Spp::Type::Postfix;
    Token.S = (Token.S & ~Spp::Type::Prefix) | Spp::Type::Postfix; // unary/Operator ...  already set.
    if (Token.M == Mnemonic::BinaryNot)
        Token.M = Mnemonic::Factorial;

    return Accept(Token);
}

#pragma endregion Scanners

Book::Result Lexer::Accept(SppToken &Token_)
{
    if (!Token_)
        return Book::Result::Rejected;

    Token_.Location.Linenum = _Cursor.line;
    Token_.Location.Colnum = _Cursor.col;

    std::size_t sz = Token_.Location.end - Token_.Location.begin + 1;
    Token_.Location.Offset = static_cast<ptrdiff_t>(Token_.Location.begin - _Cursor.B);
    _Cursor.C += sz;
    _Cursor.col += static_cast<int>(sz);

    // First or last inserted token.
    // auto& tok = mConfig.Tokens->empty() ? SppToken::mNull :
    // mConfig.Tokens->back();

    _Config.Tokens->push_back(Token_);
    _Cursor.SkipWS();
    return Book::Result::Accepted;
}


Book::Result Lexer::operator()()
{
    return Lex();
}

void Lexer::DumpTokens(std::function<void(const SppToken &)> callback_) const
{
    if (!callback_)
        return;
    for (const auto &token: *_Config.Tokens)
        callback_(token);
}

Book::Result Lexer::InputText(SppToken &Token)
{
    //logger::debug() << __PRETTY_FUNCTION__ << ":\n";

    std::string R = _Cursor.ScanString();
    //@todo Separate this token into three : {quote, text, quote}

    if (R.empty())
        return Book::Result::Rejected;

    // We separate tokens  ...
    Accept(Token);
    //...
    const char *e = _Cursor.C + (R.length() - 2); // oops!
    Token.Location.begin = _Cursor.C;
    Token.Location.end = e - 1;
    Token.M = Mnemonic::Noop;
    Token.T = Type::Text;
    Token.S = Type::Text | Type::Leaf | Type::Const;
    Accept(Token);

    Token.Location.begin = Token.Location.end = e;
    Token.T = Type::Text;
    Token.S = Type::Text | Type::Leaf | Type::Operator;
    Token.M = *e == '\'' ? Mnemonic::Squote : Mnemonic::Dquote;

    Accept(Token);
    return Book::Result::Accepted;
}

Book::Result Lexer::Lex()
{
    //Book::Result R;
    if (!_Config)
    {
        AppBook::Error() << " None or wrong config";
        return Book::Result::Rejected; // Use logger::push_error to queu error message and code...
    }
    //...
    if (ScanTable.empty())
    {
        ScanTable = {
            {Spp::Type::Null, &Lexer::InputDefault},
            {Spp::Type::Binary, &Lexer::InputBinaryOperator},
            {Spp::Type::Hex, &Lexer::InputHex},
            {Spp::Type::Punc, &Lexer::InputPunctuation},
            {Spp::Type::Prefix, &Lexer::ScanPrefix},
            {Spp::Type::Keyword, &Lexer::InputKeyword},
            {Spp::Type::Text, &Lexer::InputText},
            {Spp::Type::LineComment, &Lexer::ScanCppComment},
            {Spp::Type::BlocComment, &Lexer::ScanCommentBloc}
            //...
        };
    }

    SppToken Token;

    _Cursor = Lexer::LexCursor(_Config.Source);
    _Cursor.SkipWS();
    //book::codeDebug() << "Lexer::Exec(): Scanning '" << mCursor.colnum << "':\n";

    const char *C = nullptr;
    while (!_Cursor.EndOfFile())
    {
        if (C == _Cursor.C)
        {
            AppBook::Error()
                << "Lexer: internal infinite loop! cursor at:" << Book::Enums::Fn::Endl
                << _Cursor.Mark();
            return Book::Result::Rejected;
        }

        C = _Cursor.C;

        Token = SppToken::Scan(_Cursor.C);
        //logger::debug() << __PRETTY_FUNCTION__ << ": " << Token.details();
        ScannerFn S = GetScanner(Token);
        if (S)
        {

            if ((this->*S)(Token) != Book::Result::Accepted)
            {
                AppBook::Aborted()
                    << "Lexer: unexpected Rejected at Position:" << _Cursor.Mark();
                return Book::Result::Rejected;
            }
        }
    }
    AppBook::Status() << Book::Result::Success;
    StrAcc Code = Colorize();
    AppBook::Out() << Code;
    AppBook::Message() << "Dumping tokens stream details:";

    DumpTokens([this](SppToken const& T){
        AppBook::Out() << MarkToken(T,true);
    });

    return Book::Result::Success;//book::codeInt::Ok;
}


Book::Result Lexer::ScanCppComment(SppToken &Token)
{
    //AppBook::Debug(F) << Mnemonic_name(Token.M) << " :@ " << cursor.location();
    Token.Location.begin = _Cursor.C;
    Token.Location.Linenum = _Cursor.line;
    Token.Location.Colnum = _Cursor.col;
    Token.Location.Offset = _Cursor.C - _Cursor.B;
    Token.S |= Spp::Type::LineComment;
    std::string line = _Cursor.ScanToEol();

    Token.Location.end = _Cursor.C;
    Token.Location.Length = Token.Location.end - Token.Location.begin;
    _Cursor++;
    //_Config.Tokens->push_back(Token);
    _Cursor.Sync();

    return Book::Result::Accepted;
}

Book::Result Lexer::ScanCommentBloc(SppToken &Token)
{
    Token.Location.begin = _Cursor.C;
    Token.Location.Linenum = _Cursor.line;
    Token.Location.Colnum = _Cursor.col;
    Token.Location.Offset = _Cursor.C - _Cursor.B;
    Token.S |= Spp::Type::BlocComment;
    _Cursor.BlocComment();
    Token.Location.end = _Cursor.C;
    Token.Location.Length = Token.Location.end - Token.Location.begin;
    _Cursor++;
    //AppBook::Debug() << "cursor: " << cursor.location();
    _Cursor.Sync();
    if (_Cursor.EndOfFile())
        throw AppBook::Exception() [AppBook::Error() << Book::Result::Unexpected << Book::Result::Eof << " unterminated bloc comment." ];

    return Book::Result::Accepted;
}


Lexer::LexicalColours::~LexicalColours()
{
    text.clear();
}


StrAcc Lexer::LexicalColours::Colorize(SppToken::Array *tokens)
{
    size_t offset = 0;
    std::string _color;
    for (auto const &Token: *tokens)
    {

        _color = Token.M == Mnemonic::Noop ? Color::Ansi(PrimitiveTypesColors[Token.T]) :
            _color = Color::Ansi(MnemonicColors[Token.M]);

        if (!_color.empty())
        {
            text.insert(Token.Location.Offset + offset, _color);
            offset += _color.length();
        }
    }
    return text;
}

std::string Lexer::MarkToken(const SppToken &Token, bool c) const
{
    //AppBook::Debug() << " Marking token: " << token.details();
    std::string line = Token.TextLine();

    auto start_token = _Config.Tokens->begin();
    auto end_token = start_token;
    for (; start_token != _Config.Tokens->end(); start_token++)
    {
//        AppBook::Debug() << "this token:" << start_token->text()  << Book::Enums::Fn::endl
//                           << " == " <<  Book::Enums::Fn::endl
//                           << Token.Text() << " ?" << Book::Enums::Fn::endl;

        if (start_token->Location.Linenum == Token.Location.Linenum) break;
    }
    end_token = start_token;


    //AppBook::Debug() << "start_token: '" << start_token->details();

    for (; end_token != _Config.Tokens->end() && (end_token->Location.Linenum == Token.Location.Linenum); end_token++);

    if (end_token == _Config.Tokens->end()) --end_token;

    //AppBook::Debug() << "end_token: '" << end_token->details();

    int offset = 0;
    std::string _color;

    for (; start_token != end_token; start_token++)
    {
        _color = start_token->M == Mnemonic::Noop ? Color::Ansi(PrimitiveTypesColors[start_token->T]) :
            _color = Color::Ansi(MnemonicColors[start_token->M]);

        if (!_color.empty())
        {
            line.insert(start_token->Location.Colnum - 1 + offset, _color);
            offset += _color.length();
        }
    }
    StrAcc txt = line;

    line.clear();
    txt << Color::Reset << " ["
        << Spp::MnemonicName(Token.M) << "; "
        << Token.LocationText() << " ; "
        << Token.TypeName() << " ; "
        << Token.SemanticText() << "]";
    txt << '\n';
    txt.fill(0x20, Token.Location.Colnum - 1);
    txt << Utf::Glyph::ArrowUp;
    return txt();
}


StrAcc Lexer::Colorize() const
{
    LexicalColours lc;
    lc.text = _Config.Source;
    return lc.Colorize(_Config.Tokens);
}
}