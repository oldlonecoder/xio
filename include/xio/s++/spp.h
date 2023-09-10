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
#include "xio/xiobloc.h"



namespace xio::cc { class parser; }


namespace xio::spp
{

class XIO_PUBLIC interpretr : public xiobloc
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
    alu process(int argc, char** argv);

    static void error(book::rem::type ertype, book::rem::code ercode, token_data::iterator token, const token_data::collection& line_seq);
    static void warning(book::rem::type wtype, book::rem::code wcode, token_data::iterator token, const token_data::collection& line_seq);

    static void trace_line(token_data::iterator token, const token_data::collection& tokens, bool colour=true, bool mark=true);



};

}


