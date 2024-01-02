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

#include "Spp/Compiler/Compiler.h"


namespace Spp
{

class SPP_EXPORT Amu : public Stack
{
    // Amu Tata; Amu Toto;
    // Amu Tata
    // {
    //      Var = Toto::Var;
    // }


protected:
    std::string      _Source;
    std::string      _Filename;
    SppToken::Array  _Tokens;

    Book::Enums::Code LoadSource();

public:


    Amu() = default;
    Amu(Stack* _Parent, std::string _Id);

    ~Amu() override;

    Book::Enums::Code Compile();

    Alu JSR() override;



};

} // Spp

