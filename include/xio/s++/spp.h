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
 *   Open source FREE licences also apply to the code from the same author (Serge Lussier)*
 *   ----------------------------------------------------------------------------------   *
 ******************************************************************************************/

#pragma once
#include "xio/s++/amu.h"



namespace xio::cc { class parser; }


namespace xio::spp
{



/**
 * \brief Augmented amu but it is the interpreter class.
 * 
 * So its scope is The Global scope. Yes, just like any other scripting language, we can compile single intructions at this
 * scope level ... 
 * Or we can explicitly prevent such coding-style and force to create kind of a "main" function as the entry of the program.
 * 
 * \author &copy; 2023, #USERNAME; oldlonecoder (serge.lussier@oldlonecoder.club)
 */
class XIO_PUBLIC interpretr : public amu
{

    std::string name;
    rbase::dictionary_t rt; ///< runtime base object dictionary.
public:

    interpretr() = default;
    interpretr(const interpretr&) = delete;
    interpretr(const std::string& interp_name);

    ~interpretr() override;

    interpretr& operator=(interpretr&&) noexcept = delete;
    interpretr& operator=(const interpretr&) = delete;

    // Quick arithmetic expression evaluator ( small source text ):
    alu operator[](const std::string& expr);
    
    //stracc export_expr_ast(const std::string& expr);
    static void error(book::cat ertype, book::code ercode, token_data::iterator token, const token_data::list& line_seq);
    static void warning(book::cat wtype, book::code wcode, token_data::iterator token, const token_data::list& line_seq);
    

};

}


