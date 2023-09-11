/***************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                         *
 *   serge.lussier@oldlonecoder.club                                       *
 *                                                                         *
 *                                                                         *
 *   Unless otherwise specified, all code in this project is written       *
 *   by the author (Serge Lussier)                                         *
 *   and no one else then not even {copilot, chatgpt, or any other AI}     *
 *   --------------------------------------------------------------------- *
 *   Copyrights from authors other than Serge Lussier also apply here      *
 ***************************************************************************/

#include "xio/lexer/lexer.h"
#include <array>
#include <chrtools/Icons.h>

/*!
 * @brief  Scans for std maths factor notation, RESTRICTED (limited) syntax style:
 *         4ac => 4 x a x m
 *         4(ac...) != 4 x ( a x m ...)
 *         4pi/sin/cos/atan/asin/acos ... != 4 x p x i / 4 x s x i x n ... And NOT 4 x pi or 4 x sin ...
 * *
 *
 * @note   Required that the Left hand side token is a ConstNumber and that the Input token is Constiguous and of unknown type (xio::type::null_t) to be scanned as an identifier.
 *         Input atoken is either scanned in the ref_t Table or not.
 * @return Execp<>
 */


using namespace xio;

lexer::scan_tbl scan_table;

#pragma region InternalCursor

/*!
 * @brief Skips white spaces character, advancing(/consuming) m pointer
 *
 * prefix inc//Rement operator
 * @return true if m is not on EOF, false otherwise.
 */
bool lexer::lex_cursor::operator++()
{
    if (C >= E)
        return false;
    ++C;
    while ((C < E) && (isspace(*C)))
        ++C;
    return true;
}

/*!
 * @brief Skips white spaces character, advancing(/consuming) m pointer
 *
 * Postfix inc//Rement operator, just calls the prefix inc//Rement operator.
 * @return true if m is not on EOF, false otherwise.
 */
bool lexer::lex_cursor::operator++(int)
{
    if(C >= E)
        return false;
    ++C;
    return skip_ws();
}

/*!
* @brief Skips white spaces character, advancing(/consuming) m pointer
*
* Named method, just calls the prefix increment operator.
* @return true if m is not on EOF, false otherwise.
*/
[[maybe_unused]] bool lexer::lex_cursor::skip_ws()
{
    while((C <= E) && (isspace(*C)))
    {
        ++col;
        if(*C == '\n' || *C == '\r')
        {
            ++line;
            col = 1;
        }

        ++C;
    }
    return C <= E;
}

/*!
 * @brief Tests if P is on or past EOF.
 * @param P
 * @return true if P is EOF, false otherwise.
 */
bool lexer::lex_cursor::end_of_file(const char *P) const
{
    if(P)
        return P > E;
    return C > E;
}

/*!
 * @brief Synchronize the Location data from the  pointer.
 *
 * @return none.
 */
void lexer::lex_cursor::sync()
{
    const char *C_;
    line = 1;
    col = 1;
    C_  = B;
    while((C_ >= B) && (C_ <= E) && (C_ < C))
    {
        if((*C_ == '\n') || (*C_ == '\r'))
        {
            col = 1;
            line++;
        }
        else
            ++col;
        ++C_;
    }

    //rem::codeDebug(__PRETTY_FUNCTION__) << ": " << Location();
}

/*!
 * @brief Get the ptrdiff between the m pointer and the beginning of the source text (B pointer).
 * @return int.
 */
std::ptrdiff_t lexer::lex_cursor::index() const
{
    return (std::ptrdiff_t) (C - B);
}

/*!
 * @brief Advances/Consume the m pointer till the next NewLine{'\r'; '\n'}  code in the source text
 * @return distinct std::string of the sequence.
 */
[[maybe_unused]] std::string lexer::lex_cursor::scan_to_eol()
{
    std::string Str;
    while((C <= E) && (*C != '\n') && (*C != '\r'))
        Str += *C++;
    return Str;
}

/*!
 * @brief Advances/Consumes the m pointer to Skip till SubStr_ match.
 * @param SubStr_
 * @return Expect code.
 */
[[maybe_unused]] rem::code lexer::lex_cursor::scan_to(const char *SubStr_)
{
    return book::rem::notimplemented;
}

rem::code lexer::lex_cursor::bloc_comment()
{
    stracc str;
    stracc bloc;
    bloc << *C;
    while (C <= E) {

        if ((*C == '\'') || (*C == '"')) {
            str = scan_string();

            C += str.len();
            bloc << str;
            continue;
        }

        if (*C == '*') {
            ++C;
            bloc << *C;
            if (*C == '/') {
                rem::push_debug() << rem::endl
                                  << color::Yellow << '[' << rem::endl
                                  << color::Reset << bloc << rem::endl
                                  << color::Yellow << ']' << rem::commit;
                sync();
                rem::push_debug() << "Leaving lex_cursor::bloc_comment():" << rem::endl
                                  << rem::commit;
                rem::out() << "where *C = '" << color::Yellow << *C
                           << color::Reset << "', column " << color::Yellow
                           << col << color::Reset << ", Line " << color::Yellow
                           << line << rem::commit;
                return rem::accepted;
            }
            continue;
        }
        ++C;
        bloc << *C;
    }
    throw book::rem::push_except() << rem::eof << " lexer: unmatched bloc comment end seq." << rem::commit;
    return book::rem::eof; // rem::codeInt::Ok;
}

/*!
 * @brief Get a std::string copy of the current line from the m pointer
 * @return string.
 */
std::string lexer::lex_cursor::line_num() const
{
    std::string Str;
    Str.clear();

    const char *lb, *eb;
    lb = eb = C;

    while((eb <= E) && (*eb != '\r') && (*eb != '\n'))
        ++eb;
    while((lb > B) && (*lb != '\r') && (*lb != '\n'))
        --lb;

    for(; lb < eb; lb++)
        Str += *lb;
    return Str;
}

/*!
 * @brief Build a line stracc from the current position then mark at the current columns.
  * @return std::string
  * @note : Must be Sync()'ed before calling mark();

 */

std::string lexer::lex_cursor::mark(int nspc) const
{
    stracc Str;
    if(nspc)
        Str.fill(0x20, nspc);
    Str << line << "," << col << ":\n";
    int l=1;
    const char* cc = B;
    while (l++ <= line) {
        while((cc <=E) && (*cc != '\n')) Str << *cc++;
    }

    Str << '\n';
    if(nspc)
        Str.fill(0x20, nspc);
    for (int x = 1; x < col; x++)
        Str << ' ';
    Str << Icon::CArrowUp;
    return Str();
}

/*!
 * @brief Get the string representation of the [internal]cursor location in the source text.
 * @return std::string
 */
[[maybe_unused]] std::string lexer::lex_cursor::location() const
{
    stracc Str = "%d,%d";
    Str << line << col;
    return Str();
}

std::string lexer::lex_cursor::scan_string()
{
    //rem::codeDebug(__PRETTY_FUNCTION__ +':'+ __LINE__) << '\n';
    const char  *be    = C;
    char        Quote_ = *be;
    std::string Str;
    Str += *be;
    ++be;

    Constinue:
    while((be <= E) && (*be != '\\') && (*be != Quote_))
        Str += *be++;
    if(*be == '\\')
    {
        Str += *be++;
        Str += *be++; // Assume no terminal escape sequence here!!!!! PLEASE be a quote!!!
        goto Constinue;
    }

    if((*be != Quote_) && (be > E))
    {
        sync();
        rem::push_error() << " Unterminated string constant in internal_cursor::scan_string." << rem::endl << mark();
        return "eof";
    }
    Str += *be; // Include the rhs Quote.
    return Str;
    }

    std::string lexer::lex_cursor::print_location()
    {
        return location();
    }

    lexer::lex_cursor::lex_cursor(const char *Source_)
    {
    C = Source_;
    E = C + std::strlen(C) - 1;
    B = C;
    line = col = 1;
    }

#pragma endregion InternalCursor

#pragma region NumScanner

    lexer::num_scanner::num_scanner(const char *_c, const char *_eos)
        : B(_c)
        , E(nullptr)
        , C(_c)
        , Eos(_eos)
    {
    }

/*!
 * @brief For now a bare minimum digit with some rough floating point scan.
 * @return true if the m pointer is consumed and advanced
 */
bool lexer::num_scanner::operator++(int)
{
    if(C > Eos)
        return false;

    if(!isdigit(*C))
    {
        if(*C == '.')
        {
            if(!Real)
            {
                const char *e = C;
                ++e;
                if(!isdigit(*e)) return false;
                St = true;
                Real = true;
            }
            else
            {
                St = false;
                return false;
            }
        }
        else
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
lexer::num_scanner::operator bool() const
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
 * //std::array<xio::type::T, 4> Capacity = {xio::type::U8, xio::type::U16, xio::type::U32, xio::type::U64};
 * //xio::type::T                atoken   = Capacity[I];
 */
xio::type::T lexer::num_scanner::operator()() const
{
    if(!Real)
    {
        std::string temp = B;
        stracc Str = std::string(temp, 0, E-B);
        uint64_t D;
        Str >> D;
        int scale = 0;
        std::array<uint64_t, 3> R = {0x100, 0x10000, 0x100000000};
        while(D >= R[scale]) ++scale;
        return std::array<xio::type::T, 4>{xio::type::U8, xio::type::U16, xio::type::U32, xio::type::U64}[scale];
    }

    return xio::type::Float;
}

#pragma endregion NumScanner

#pragma region Scanners

// + 4ac(x^2+y+b)

lexer::scanner_fn lexer::get_scanner(token_data &token)
{
    //book::rem::push_debug(HERE) << " for " << color::Yellow << token.details(false) << book::rem::commit;

    for(const auto &scanners_tbl: scan_table)
    {
        if(token.s & scanners_tbl.first)
            return scanners_tbl.second;
    }
    return nullptr;
}

rem::code lexer::input_binary_operator(token_data &token)
{
    //logger::debug() << __PRETTY_FUNCTION__ << ":\n";
    if (token.m == mnemonic::Sub || token.m == mnemonic::Add) {
        if(scan_sign_prefix(token) == rem::accepted)
            return rem::accepted;
    }
    accept(token);
    cursor._F = false;
    return rem::accepted;
}

/*!
 * @brief Unknown Input Token (Either Number litteral or identifier).
 * @return Expect<>
 */
rem::code lexer::input_default(token_data &atoken)
{
    //logger::debug() << __PRETTY_FUNCTION__ << ":\n";
    if(scan_number(atoken) != rem::accepted)
    {
        //rem::codeDebug() << " Not a Number Trying scan_identifier:";
        if(scan_identifier(atoken) != rem::accepted)
        {
            //rem::codePush() << mCursor.mark() << ":";
            return rem::rejected;//rem::codePush() << //rem::codexio::type::Fatal << ": " << //rem::codeInt::UnExpected << " Token type " << atoken.type_name();
        }
    }

    return rem::accepted; // return rem::rejected  --- duh?
}

rem::code lexer::input_unary_operator(token_data& atoken)
{

    // Possible prefix and Postfix unary operators:
    if ((atoken.m == mnemonic::BinaryNot) || (atoken.m == mnemonic::Decr) ||
        (atoken.m == mnemonic::Incr)) {
        if (_config.Tokens->empty() ||
            (_config.Tokens->back().s & xio::type::Binary))
            return scan_prefix(atoken);
        return scan_postfix(atoken);
    }
    if(atoken.t == xio::type::Prefix)
        return scan_prefix(atoken);
    return scan_postfix(atoken);
}

rem::code lexer::input_punctuation(token_data &atoken)
{
    rem::push_debug(HEREF) << atoken.mark() << rem::commit;
    if (atoken.m == mnemonic::OpenPar) {
        rem::push_debug(HERE) << atoken.text() << " - Openpar: check for mul insertion:";
        // Bug here: the flag can be set without proper previous condition...let's investigate! :)
        if (cursor._F) {
            if (!_config.Tokens->empty() && _config.Tokens->back()._flags.V) {
                accept(atoken);
                insert_multiply(atoken);
                return rem::accepted;
            }
        }
        rem::push_debug(HERE) << " not...checking if previous token is number:";
        if (!_config.Tokens->empty()) {
            rem::out() << "Previous token: " << rem::endl
                       << _config.Tokens->back().details() << ":";
            // La seule et unique condition est que le token precedant soit une valeur numerique litterale (ex.: '4').
            if (_config.Tokens->back().is_number()) {
                accept(atoken);
                insert_multiply(atoken);
                return rem::accepted;
            }
        }
        rem::push_debug(HERE) << "not...proceeding to the next token ... ";
        return accept(atoken);
    }
    // ... A = .0123 :
    if (atoken.m == mnemonic::Dot) {
        if(rem::code r; (r = scan_number(atoken)) == rem::accepted)
            return r;
    }

    if (atoken.m == mnemonic::CommentCpp)
        return skip_cpp_comment();
    if (atoken.m == mnemonic::BeginComment)
        return skip_comment_bloc();

    return accept(atoken);
}

rem::code lexer::input_keyword(token_data &atoken)
{
    //logger::debug() << __PRETTY_FUNCTION__ << ":\n";
    return accept(atoken);
}

rem::code lexer::input_hex(token_data &atoken)
{
    //logger::debug() << __PRETTY_FUNCTION__ << ":\n";
    //rem::codeDebug(__PRETTY_FUNCTION__) << ":\n";
    const char *C_ = cursor.C;
    C_ += atoken.text().length();
    const char *E_ = C_;
    if(isspace(*E_))
        return rem::rejected;

    while(*E_ && !isspace(*E_) && ( ((*E_ >= '0') && (*E_ <= '9')) || ((*E_ >= 'A') && (*E_ <= 'F')) || ((*E_ >= 'a') && (*E_ <= 'f')) ) )
        ++E_;

    if (E_ > C_)
        --E_;

    atoken.loc.end = E_;
    atoken._flags.V = 1;
    return accept(atoken);
}

/*!
 * @brief Scans const numeric constrtuct
 * @param atoken
 * @return rem::accepted;
 * @todo scan Scientific Notation!!!
 */
rem::code lexer::scan_number(token_data &atoken)
{

    //logger::debug() << __PRETTY_FUNCTION__ << ":\n";
    num_scanner Num = num_scanner(cursor.C, cursor.E);
    while(Num++)
    {
        ;
    }
    if(! Num)
        return rem::rejected;

    if (cursor._F)
        cursor._F = false;

    atoken.t          = xio::type::Number;
    atoken.s          = xio::type::Number | Num() | xio::type::Leaf;
    atoken._flags.V   = 1;
    atoken.loc.begin = Num.B;
    atoken.loc.end = Num.E; // And Num.C ?
    // rem::codeDebug() << "lexer::scan_number: Cursor on \n" << mCursor.mark();
    if(!(atoken.s & xio::type::Float))
    {
        stracc str;
        str << atoken.text();
        uint64_t           D = 0;
        std::istringstream i(str().c_str());
        switch(Num.Num)
        {
            case num_scanner::Bin:
                //????????? ah!
                break;
            case num_scanner::Oct:i >> std::oct >> D;
                break;
            case num_scanner::Dec:i >> D;
                break;
            case num_scanner::Hex:i >> std::hex >> D;
                break;
            default:str >> D;
                break;
        }

        //std::cout << __PRETTY_FUNCTION__ << " Parsed number:" << D << '\n';
        uint64_t                n = 0;
        std::array<uint64_t, 3> R = {0x100, 0x10000, 0x100000000};
        while(D >= R[n])
            ++n;

        switch(n)
        {
            case 0:atoken.s = (atoken.s & ~xio::type::U64) | xio::type::U8;
                break;
            case 1:atoken.s = (atoken.s & ~xio::type::U64) | xio::type::U16;
                break;
            case 2:atoken.s = (atoken.s & ~xio::type::U64) | xio::type::U32;
                break;
            default:atoken.s |= xio::type::U64;
                break;
        }
    }
    atoken.m = mnemonic::Noop;
    atoken.s |= type::Const;
    return accept(atoken);
    //return rem::codeaccepted;
}

rem::code lexer::scan_identifier(token_data &atoken)
{

    //rem::codeDebug(__PRETTY_FUNCTION__);
    const char *C = cursor.C;
    if((!isalpha(*C)) && (*C != '_')) return rem::rejected;

    if (!cursor._F) {
        if (!_config.Tokens->empty()) {
                // if((mConfig.Tokens->back().s & xio::type::Number) && !mConfig.Tokens->back()._flags.V)
                if (_config.Tokens->back()._flags.V &&
                    !(_config.Tokens->back().s & xio::type::Operator))
                cursor._F = true;
                else {
                while (*C && (isalnum(*C) || (*C == '_'))) ++C;
                --C;
                }
        } else {
                while (*C && (isalnum(*C) || (*C == '_')))
                ++C;
                --C;
        }
    }
    atoken.loc.begin = cursor.C;
    atoken.loc.end = C;
    atoken.t            = xio::type::Id;
    atoken.s            = xio::type::Id | xio::type::Leaf;
    atoken.m = mnemonic::Noop;
    atoken.loc.linenum = cursor.line;
    atoken.loc.colnum = cursor.col;
    atoken.d = distance::identifier;
    atoken._flags.V = 1;// src_cursor._F ? 1 : 0; //Subject to be modified
    accept(atoken);
    if (cursor._F && !_config.Tokens->back().is_operator())
        if ((atoken.loc.colnum - _config.Tokens->back().loc.colnum) <
            2) // no spaces between multiply two adjacent letters!!! (
               // 2023-07-sept oldlonecoder).
                insert_multiply(atoken);

    return rem::accepted;
}

void lexer::insert_multiply(token_data &atoken)
{

    auto i = --_config.Tokens->end();
    --i;
    book::rem::push_debug(HERE)
        << atoken.text() << "::colnum :" << color::Yellow << atoken.loc.colnum
        << book::rem::commit;
    book::rem::push_debug(HERE) << i->text() << "::colnum :" << color::Yellow
                                << i->loc.colnum << book::rem::commit;

    if ((atoken.loc.colnum - i->loc.colnum) >= 2) {
        book::rem::push_debug(HERE) << " rejected because there must be no spaces between the involved tokens..." << book::rem::commit;
        return;
    }
    ++i;
    token_data Mul;
    Mul = atoken; // Push atoken properties in the incoming virtual multiply operator
    Mul.t        = xio::type::Binary;
    Mul.s        = xio::type::Binary | xio::type::Operator;
    Mul.d        = distance::product;
    Mul._flags.M = Mul._flags.V = 1;
    Mul.m = mnemonic::Mul;

    _config.Tokens->insert(i, Mul);
    //logger::debug() << __PRETTY_FUNCTION__ << ":\n Details:" << Mul.details() << "\n" << Mul.mark();
}

rem::code lexer::scan_sign_prefix(token_data &atoken)
{
    if (!_config.Tokens->empty() &&
        (_config.Tokens->back().s & xio::type::ClosePair)) {
        //logger::comment() << "lexer::scan_sign_prefix:\n" << atoken.mark() << "\n" << " rejected...\n";
        return rem::rejected;
    }

    if (_config.Tokens->empty() || _config.Tokens->back().is_binary() ||
        _config.Tokens->back().is_punctuation() ||
        !_config.Tokens->back()._flags.V) {
        atoken.t = xio::type::Prefix;
        atoken.s = (atoken.s & ~xio::type::Binary) | xio::type::Sign | xio::type::Unary | xio::type::Prefix; // xio::type::Operator bit already set
        return accept(atoken);
    }
    return rem::rejected;
}

/*!
 * @brief  Operators such as '--', '++', '!' can be prefix or postfix unary operators.
 *
 * @return
 */
rem::code lexer::scan_prefix(token_data &atoken)
{
    // Possible prefix and Postfix unary operators:
    if ((atoken.m == mnemonic::BinaryNot) || (atoken.m == mnemonic::Decr) ||
        (atoken.m == mnemonic::Incr)) {
        if (_config.Tokens->empty() ||
            (_config.Tokens->back().s & xio::type::Binary))
                return accept(atoken);
        return scan_postfix(atoken);
    }
    return accept(atoken);
}

/*!
 * @brief  Operators such as '--', '++', '!' can be prefix or postfix unary operators.
 *
 * @return
 */
rem::code lexer::scan_postfix(token_data &atoken)
{
    if (!((atoken.m == mnemonic::Decr) || (atoken.m == mnemonic::Incr) ||
          (atoken.m == mnemonic::BinaryNot)))
        return rem::rejected;

    atoken.t = xio::type::Postfix;
    atoken.s = (atoken.s & ~xio::type::Prefix) | xio::type::Postfix; // unary/Operator ...  already set.
    if (atoken.m == mnemonic::BinaryNot)
        atoken.m = mnemonic::Factorial;

    return accept(atoken);
}

#pragma endregion Scanners

rem::code lexer::accept(token_data &atoken)
{
    if(!atoken)
        return rem::rejected;

    atoken.loc.linenum = cursor.line;
    atoken.loc.colnum = cursor.col;

    std::size_t sz = atoken.loc.end - atoken.loc.begin + 1;
    atoken.loc.offset = static_cast<ptrdiff_t>(atoken.loc.begin - cursor.B);
    cursor.C += sz;
    cursor.col += static_cast<int>(sz);

    // First or last inserted token.
    // auto& tok = mConfig.Tokens->empty() ? token_data::mNull :
    // mConfig.Tokens->back();

    _config.Tokens->push_back(atoken);
    cursor.skip_ws();
    return rem::accepted;
}


rem::code lexer::operator()()
{
    return process();
}

void lexer::dump_tokens(std::function<void(const token_data &)> callback_)
{
    if(!callback_)
        return;
    for (const auto &token : *_config.Tokens)
        callback_(token);
}

rem::code lexer::input_text(token_data &atoken)
{
    //logger::debug() << __PRETTY_FUNCTION__ << ":\n";

    std::string R = cursor.scan_string();
    //@todo Separate this token into three : {quote, text, quote}

    if(R.empty())
        return rem::rejected;

    // We separate tokens here ...
    accept(atoken);
    //...
    const char *e = cursor.C + (R.length() - 2); // oops!
    atoken.loc.begin = cursor.C;
    atoken.loc.end = e - 1;
    atoken.m = mnemonic::Noop;
    atoken.t = type::Text;
    atoken.s = type::Text|type::Leaf|type::Const;
    accept(atoken);

    atoken.loc.begin = atoken.loc.end = e;
    atoken.t = type::Text;
    atoken.s = type::Text|type::Leaf|type::Operator;
    atoken.m = *e == '\'' ? mnemonic::Squote : mnemonic::Dquote;

    accept(atoken);
    return rem::accepted;
}

rem::code lexer::process()
{
    //rem::code R;
    if (!_config)
        return rem::rejected; // Use logger::push_error to queu error message and code...
    //...
    if(scan_table.empty())
    {
        scan_table = {
            {xio::type::Null, &lexer::input_default},
            {xio::type::Binary,    &lexer::input_binary_operator},
            {xio::type::Hex, &lexer::input_hex},
            {xio::type::Punc, &lexer::input_punctuation},
            {xio::type::Prefix, &lexer::scan_prefix},
            {xio::type::Keyword, &lexer::input_keyword},
            {xio::type::Text, &lexer::input_text}
            //...
        };
    }

    token_data atoken;

    cursor = lexer::lex_cursor(_config.Source);
    cursor.skip_ws();
    //rem::codeDebug() << "lexer::Exec(): Scanning '" << mCursor.colnum << "':\n";

    const char *C = nullptr;
    while (!cursor.end_of_file()) {
        if (C == cursor.C) {
                rem::push_error()
                    << "lexer: internal infinite loop! cursor at:" << rem::endl
                    << cursor.mark() << book::rem::commit;
                return rem::rejected;
        }

        C = cursor.C;

        atoken = token_data::scan(cursor.C);
        //logger::debug() << __PRETTY_FUNCTION__ << ": " << atoken.details();
        scanner_fn S = get_scanner(atoken);
        if(S)
        {

            if((this->*S)(atoken) != rem::accepted)
            {
                rem::push_aborted()
                    << "lexer: unexpected rejected at position:" << cursor.mark()
                    << book::rem::commit;
                return rem::rejected;
            }
        }
// ----------------------- Below bloc clode is not not needed -----------------------------------
//        else
//        {
//            if(atoken.c == mnemonic::CommentCpp)
//            {
//                skip_cpp_comment();
//                continue;
//            }
//            rem::push_aborted() << "lexer loop: there is no scanner for token:" << src_cursor.mark()  << book::rem::commit;
//            return rem::rejected;
//        }
//------------------------------------------------------------------------------------------------
    }
    return rem::accepted;//rem::codeInt::Ok;
}

rem::code lexer::step_begin()
{

    if (!_config)
        return rem::rejected;//rem::codeFatal() << "lexer::Exec(): Config Data is missing crucial informations...";

    if(scan_table.empty())
    {
        scan_table = {
            {xio::type::Null, &lexer::input_default},
            {xio::type::Binary,  &lexer::input_binary_operator},
            {xio::type::Hex, &lexer::input_hex},
            {xio::type::Punc, &lexer::input_punctuation},
            {xio::type::Prefix, &lexer::scan_prefix},
            {xio::type::Keyword, &lexer::input_keyword},
            {xio::type::Text, &lexer::input_text}
        };
    }
    cursor = lexer::lex_cursor(_config.Source);
    cursor.skip_ws();
    return rem::accepted;
}

static const char *C = nullptr;

token_data *lexer::step()
{
    if (cursor.end_of_file())
        return nullptr; // rem::codereturn_status() << rem::codeeof;

    if (C == cursor.C) {

        return nullptr;
    }

    C = cursor.C;

    token_data token;
    token = token_data::scan(cursor.C);
    //rem::codeDebug(__PRETTY_FUNCTION__) << " Details: " << atoken.details();
    scanner_fn S = get_scanner(token);
    if(S)
    {
        if((this->*S)(token) != rem::accepted)
            return nullptr;// status() << rem::coderejected << " -> Aborted: Unexpected token:\n" << src_cursor.mark();
    }
    else
        return nullptr; // status() << rem::rejected << " No scanner for
                        // token:\n" << src_cursor.mark();

    return &_config.Tokens->back(); // Ouch...
}


rem::code lexer::skip_cpp_comment()
{
    cursor.scan_to_eol();
    cursor.skip_ws();
    return rem::accepted;
}

rem::code lexer::skip_comment_bloc()
{
    rem::push_debug(HEREF) << ":" << rem::commit;
    cursor.bloc_comment();
    // src_cursor.C must point the the last '/'.
    cursor++;
    cursor.skip_ws();
    rem::push_debug(HERE) << cursor.location() << rem::commit;
    if (cursor.end_of_file())
        throw rem::push_except() << rem::unexpected << rem::eof << " unterminated bloc comment." << rem::commit;
    sync(); // re-calculate line and col.
    cursor.skip_ws();
    return rem::accepted;
}


