//
// Created by oldlonecoder on 23-12-17.
//

/******************************************************************************************
 *   Copyright (C) 2023 by Serge Lussier                                                  *
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

/*!
 * @brief Aggregation of centralized, globally callable, colours styles DB, for Token Types/Mnemonics-Colorization,
 * And later also for Syntax/Semantic colors.
 */
struct SPP_EXPORT  ColorScheme
{
    //using MnemonicsElement      = std::vector<Color::Code>;
    using RGBMnemonicsElement   = std::vector<Color::Data>;
    using TypesElements         = std::vector<Color::Code>;
    using RGBTypesElements      = std::vector<Color::Data>;

    using Scheme = std::pair<TypesElements, TypesElements>;
    using Schemes = std::map<std::string, ColorScheme::Scheme>;


    //--------------- Spp::Type -> Color::Code -----------------------------------------
    using TypePair          = std::pair<Type::T, Color::Code>;
    using TypePairArray     = std::vector<ColorScheme::TypePair>;
    static ColorScheme::TypePairArray TypesColorData;
    //----------------------------------------------------------------------------------

    std::string operator[](Mnemonic M);
    std::string operator[](Spp::Type::T T);

// ---------------- Not there yet; ----------------------------------------
//    std::string RGB(Mnemonic M);
//    std::string RGB(Type::T T);
// ------------------------------------------------------------------------

    static TypesElements AffinedMnemonics, TypeElements;

    ColorScheme::Schemes Themes; ///< Database of Schemes;
    ColorScheme::Scheme* CurrentTheme{nullptr};

    ColorScheme();
    ~ColorScheme() = default;




};

} // Spp

//#endif //SPP_COLORSCHEME_H
