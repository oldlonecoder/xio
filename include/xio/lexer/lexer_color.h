#pragma once

#include <logbook/expect.h>
#include <chrtools/stracc.h>
#include "lexer/lexer.h"
#include <map>



/*!
    @note important! Make two phases : ph.1: Mnemonic; ph.2: Semantic Context & Primary Type.
*/
class  XIO_PUBLIC lexer_color
{
    xio::token_data::collection tokens;
    chattr::format _formatormat = chattr::format::ansi256; ///< Not used yet

    // Color::Type or Color::Set ...
    //static std::map<type::T, Color::Type> SemanticTypes; // Addresing token_data::s
    static std::map<xio::type::T, color::type> Types;   // Addressing token_data::t
    std::string _product_data;

public:
    //using affine_pair = std::pair<spp::mnemonic, color::type>;
    using affine_set  = std::map<xio::mnemonic, color::type>;

    lexer_color();
    ~lexer_color();

    rem::code init();


    rem::code operator << (const std::string& aSource);
    rem::code operator << (const lexer::config_data& cfg);

    rem::code process (const std::string& src, xio::token_data::collection const& tokens);
    std::string Product() {
        return _product_data;
    }
    std::string mark(xio::token_data& token);


private:
    //...
    static affine_set affine_db;
};




