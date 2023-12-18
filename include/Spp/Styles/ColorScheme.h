//
// Created by oldlonecoder on 23-12-17.
//

/******************************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                                        *
 *   serge.lussier@oldlonecoder.club                                                      *
 *                                                                                        *
 *                                                                                        *
 *   Unless otherwise specified, all code in this project is written                      *
 *   by the author (Serge Lussier)                                                        *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge Lussier also apply here                     *
 *   Open source FREE licences also apply to the code from the same author (Serge Lussier)*
 *   ----------------------------------------------------------------------------------   *
 ******************************************************************************************/
//#ifndef SPP_COLORSCHEME_H
//#define SPP_COLORSCHEME_H

#pragma once

#include "Spp/Compiler/Lexer.h"
#include <map>



namespace Spp
{

struct SPP_EXPORT  ColorScheme
{
    using Element = std::pair<Spp::Mnemonic, Color::Code>;
    using RGBElement = std::pair<Spp::Mnemonic, Color::Data>;

    using ElementsDictionary = std::map<std::string, ColorScheme::Element>;
    using RGBElementsDictionary = std::map<std::string, ColorScheme::RGBElement>;


};

} // Spp

//#endif //SPP_COLORSCHEME_H
