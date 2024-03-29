/***************************************************************************
*   Copyright (C) 1965/1987/2023 by Serge Lussier                         *
*   serge.lussier@oldlonecoder.club                                       *
*                                                                         *
*                                                                         *
*   Unless otherwise specified, all code in this project is written       *
*   by the author (Serge Lussier)                                         *
*   --------------------------------------------------------------------- *
*   Copyrights from authors other than Serge Lussier also apply here      *
***************************************************************************/

#pragma once

#include <AppBook/Book/BookEnums.h>
#include "Spp/SppToken.h"
#include <map>


namespace Spp {
/*!
 * @brief Lexical Scanner(s)
 *
 * @author &copy; 2020, Serge Lussier(Bretzelus; bretzel; lonesomecoder; LordVorace...),  lussier.serge@gmail.com;
 *
 * @note Re-write of my lexical scanners; dissociate this Lexer from StrAcc and only use std::string.
 */
class SPP_EXPORT Lexer
{

public:
    static std::map<Type::T, Color::Code> PrimitiveTypesColors;
    static std::map<Mnemonic, Color::Code> MnemonicColors;
private:
    
    struct LexCursor {
        const char *B = nullptr; ///> Absolute Beginning of the source Text.
        const char *E = nullptr; ///> Absolute End of the source Text.
        const char *C = nullptr; ///> Index/Position of the cursor int the source Text.

        bool operator++();
        bool operator++(int);
        [[maybe_unused]] bool SkipWS();
        bool EndOfFile(const char *P = nullptr) const;
        [[maybe_unused]] [[nodiscard]] std::ptrdiff_t Index() const;
        int line = -1;
        int col = -1;
        void Sync();
        std::string ScanToEol();

        [[maybe_unused]] [[nodiscard]]std::string ExtractTextLine() const;
        [[nodiscard]]std::string Mark(int nspc = 0) const;
        [[nodiscard]]std::string Location() const;
        bool _F = false; ///< Used as "state machine" for math factor notation syntax style
        [[maybe_unused]] Book::Result ScanTo(const char *SubStr_);
        Book::Result BlocComment();
        std::string ScanString();

        //[[maybe_unused]] std::string PrintLocation();
        LexCursor() = default;
        explicit LexCursor(const char *Source_);
    } _Cursor;


    /*!
     * @brief scan litteral numbers in source atr the current position.
     * @note I make it so complicated...
     */
    struct NumScanner
    {
        const char *B = nullptr;
        const char *E = nullptr;
        const char *C = nullptr;
        const char *Eos = nullptr;
        bool Real = false;

        bool Literal = true; ///< Scan literal digits - do not validate hexadecimal...
        bool St = false;

        enum NumBase
        {
            None,
            Bin,
            Oct,
            Dec,
            Hex,/* ..., */ //  FUCK!!!
        } Num = None;

        NumScanner() =  default;
        NumScanner(const char *_c, const char *_eos);

        ~NumScanner() = default;

        //bool operator++();
        bool operator++(int);

        explicit operator bool() const;
        Type::T operator()() const;
    };

    /*!
     * @brief Colourize the source...
     * @note And this, is so much easier than I thought.
     */
    struct LexicalColours
    {
        std::string text{};

        [[maybe_unused]] Color::Format format{Color::Format::ansi256}; // = Color::Format::ansi256; ///< Not used yet

        LexicalColours() = default;
        ~LexicalColours();

        //Book::Result operator << (const std::string& aSource);
        //Book::Result operator << (const lexer::config_data& cfg);

        //Book::Result Process(SppToken::Array const &tokens);
        StrAcc Colorize(SppToken::Array *tokens);
    };

public:
    struct SPP_EXPORT ConfigData
    {
        const char *Source = nullptr;
        SppToken::Array *Tokens = nullptr;
        explicit  operator bool() const { return Source && Tokens; }
    };


    Lexer() = default;
    Lexer(const Lexer&) = default;
    Lexer(Lexer&&)      = default;

    ~Lexer() = default;

    Lexer::ConfigData &Config() { return _Config; }

    Book::Result Lex();
    Book::Result operator()();

    [[maybe_unused]][[nodiscard]] bool Empty() const
    {
        return _Config.Tokens == nullptr || _Config.Tokens->empty();
    }
    void DumpTokens(std::function<void(const SppToken&)> callback_) const;
    StrAcc Colorize() const;
    private:
    ConfigData _Config;

    Book::Result Accept(SppToken &Token_);

#pragma region Scanners
public:
    using ScanPtr           = Book::Result (Lexer::*)(SppToken &);
    using InputAssociation  = std::pair<Type::T, Lexer::ScanPtr>;
    using ScanTbl           = std::vector<Lexer::InputAssociation>;
    using ScannerFn         = Lexer::ScanPtr;

    static ScannerFn GetScanner(SppToken &token);
    std::string MarkToken(const SppToken& token, bool c) const;
private:

    ///@note I must cleanup and remove unused functions.
    Book::Result InputBinaryOperator(SppToken&);
    Book::Result InputDefault(SppToken&);
    [[maybe_unused]] Book::Result InputUnaryOperator(SppToken&);
    Book::Result InputPunctuation(SppToken&);
    Book::Result InputKeyword(SppToken&);
    Book::Result InputHex(SppToken&);
    Book::Result InputText(SppToken&);
    Book::Result ScanNumber(SppToken&);
    Book::Result ScanIdentifier(SppToken&);
    //Book::Result ScanFactorNotation(SppToken&);
    Book::Result ScanSignPrefix(SppToken&);
    Book::Result ScanPrefix(SppToken&);
    Book::Result ScanPostfix(SppToken&);
    Book::Result ScanCppComment(SppToken& atoken);
    Book::Result ScanCommentBloc(SppToken& atoken);
#pragma endregion Scanners

    void InsertMultiply(SppToken&) const;


};


}