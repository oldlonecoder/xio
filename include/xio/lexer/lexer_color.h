#pragma once

#include <tlux/diagnostic.h>
#include <tlux/stracc.h>
#include <xio/lexer/lexer.h>
#include <map>


namespace xio
{

/*!
    @note important! Make two phases : ph.1: Mnemonic; ph.2: Semantic Context & Primary Type.
*/
class  lexer_color
{
    token_data::collection tokens;
    textattr::format _formatormat = textattr::format::ansi256; ///< Not used yet

    // Color::Type or Color::Set ...
    //static std::map<type::T, Color::Type> SemanticTypes; // Addresing token_data::s
    static std::map<type::T, color::type> Types;   // Addressing token_data::t
    std::string _product_data;

public:
    //using affine_pair = std::pair<xio::mnemonic, color::type>;
    using affine_set  = std::map<xio::mnemonic, color::type>;

    lexer_color();
    ~lexer_color();

    code::T init();


    code::T operator << (const std::string& aSource);
    std::string Product() {
        return _product_data;
    }
    std::string mark(token_data& token);

private:
    //...
    static affine_set affine_db;
};



}
