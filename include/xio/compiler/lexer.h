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

#pragma once

#include <logbook/expect.h>
#include <xio/tokendata.h>




/*!
 * @brief Lexical Scanner(s)
 *
 * @author &copy; 2020, Serge Lussier(Bretzelus; bretzel; lonesomecoder; LordVorace...),  lussier.serge@gmail.com;
 *
 * @note Re-write of my lexical scanners; offset shall dissociate lexer from Lsc::String and only use std::string.
 */
class XIO_PUBLIC lexer
{
    struct lex_cursor {
        const char *B  = nullptr; ///> Absolute Beginning of the source text.
        const char *E  = nullptr; ///> Absolute end of the soure text.
        const char *C = nullptr; ///> Absolute of the index/position of the
                                 /// cursor int the source text.

        bool operator++();
        bool operator++(int);
        [[maybe_unused]] bool skip_ws();
        bool end_of_file(const char *P = nullptr) const;
        [[nodiscard]] std::ptrdiff_t index() const;
        int line = -1;
        int col = -1;
        void sync();
        std::string scan_to_eol();
        [[nodiscard]]std::string line_num() const;
        [[nodiscard]]std::string mark(int nspc = 0) const;
        [[nodiscard]]std::string location() const;
        bool       _F  = false; ///< Used as "state machine" for math factor notation syntax style
        book::code scan_to(const char *SubStr_);
        book::code bloc_comment();
        std::string scan_string();
        std::string print_location();
        lex_cursor() = default;
        explicit lex_cursor(const char *Source_);
    } cursor;

    struct num_scanner
    {
        const char *B   = nullptr;
        const char *E   = nullptr;
        const char *C   = nullptr;
        const char *Eos = nullptr;
        bool Real = false;

        bool Literal = true; ///< scan literal digits - do not validate hexadecimal...
        bool St = false;

        enum NumBase
        {
            None,
            Bin,
            Oct,
            Dec,
            Hex,/* ..., */ //  FUCK!!!
        }Num = None;

        num_scanner() = default;
        num_scanner(const char *_c, const char *_eos);

        ~num_scanner() = default;

        //bool operator++();
        bool operator++(int);

        explicit operator bool() const;
        xio::type::T operator()() const;
    };

    struct LexicalColours
    {
        std::string text;

        chattr::format format; // = chattr::format::ansi256; ///< Not used yet

        LexicalColours(){}
        ~LexicalColours();

        //book::code operator << (const std::string& aSource);
        //book::code operator << (const lexer::config_data& cfg);

        book::code Process (xio::token_data::list const& tokens);
        stracc colorize(xio::token_data::list* tokens);
    };

public:
    struct XIO_PUBLIC config_data
    {
        const char *Source  = nullptr;
        xio::token_data::list *Tokens = nullptr;
        operator bool() const { return Source && Tokens; }
    };


    lexer() = default;
    lexer(const lexer&) = default;
    lexer(lexer&&)      = default;

    ~lexer() = default;

    lexer::config_data &config() { return _config; }

    book::code process();
    book::code operator()();

    [[maybe_unused]] [[nodiscard]] bool Empty() const
    {
        return _config.Tokens == nullptr || _config.Tokens->empty();
    }
    void dump_tokens(std::function<void(const xio::token_data&)> callback_);
    stracc colorize();
private:
    config_data _config;

    book::code accept(xio::token_data &Token_);

#pragma region Scanners
public:
    using scanner_ptr = book::code (lexer::*)(xio::token_data &);
    using input_assoc = std::pair<xio::type::T, lexer::scanner_ptr>;
    using scan_tbl = std::vector<lexer::input_assoc>;
    using scanner_fn = lexer::scanner_ptr;

    static scanner_fn get_scanner(xio::token_data &token);
    std::string mark(const xio::token_data& token, bool c);
private:
    book::code input_binary_operator(xio::token_data&);
    book::code input_default(xio::token_data&);
    book::code input_unary_operator(xio::token_data&);
    book::code input_punctuation(xio::token_data&);
    book::code input_keyword(xio::token_data&);
    book::code input_hex(xio::token_data&);
    book::code input_text(xio::token_data&);
    book::code scan_number(xio::token_data&);
    book::code scan_identifier(xio::token_data&);
    //book::code ScanFactorNotation(xio::token_data&);
    book::code scan_sign_prefix(xio::token_data&);
    book::code scan_prefix(xio::token_data&);
    book::code scan_postfix(xio::token_data&);
    book::code scan_cpp_comment(xio::token_data& atoken);
    book::code scan_comment_bloc(xio::token_data& atoken);
    #pragma endregion Scanners

    void insert_multiply(xio::token_data&);


};




