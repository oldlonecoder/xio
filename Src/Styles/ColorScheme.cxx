//
// Created by oldlonecoder on 23-12-17.
//

#include "Spp/Styles/ColorScheme.h"

namespace Spp
{

/*!
 * @brief Affined Mnemonic-Color pairs.
 * @note : The "Affined" is a Work in progress ;)
 */

ColorScheme::TypesElements  ColorScheme::AffinedMnemonics =
{
    Color::White,
    Color::SkyBlue1,
    Color::DeepSkyBlue7,
    Color::DeepSkyBlue7,
    Color::SkyBlue1,
    Color::DodgerBlue1,
    Color::DodgerBlue1,
    Color::Salmon1,
    Color::Salmon1,
    Color::Salmon1,
    Color::Salmon1,
    Color::Salmon1,
    Color::Salmon1,
    Color::Salmon1,
    Color::Salmon1,
    Color::Salmon1,
    Color::Salmon1,
    Color::Salmon1,
    Color::White,
    Color::LighcoreateBlue,
    Color::LighcoreateBlue,
    Color::LighcoreateBlue,
    Color::LighcoreateBlue,
    Color::DarkOrange3,
    Color::DarkOrange3,
    Color::LighcoreateBlue,
    Color::White,
    Color::White,
    Color::LighcoreateBlue,
    Color::LighcoreateBlue,
    Color::LighcoreateBlue,
    Color::Salmon1,
    Color::SkyBlue1,
    Color::SkyBlue1,
    Color::SkyBlue1,
    Color::SkyBlue1,
    Color::SkyBlue1,
    Color::SkyBlue1,
    Color::LighcoreateBlue,
    Color::LighcoreateBlue,
    Color::LightSteelBlue,
    Color::LightSteelBlue,
    Color::Gold4,
    Color::Gold4,
    Color::DarkOliveGreen2,
    Color::DarkOliveGreen2,
    Color::Grey69,
    Color::Grey69,
    Color::Grey53,
    Color::Grey54,
    Color::DeepSkyBlue7,
    Color::Violet,
    Color::Violet,
    Color::Violet,
    Color::Violet,
    Color::Violet,
    Color::DeepSkyBlue4,
    Color::White,
    Color::White,
    Color::White,
    Color::White,
    Color::White,
    Color::White,
    Color::White,
    Color::Violet,
    Color::DodgerBlue2,
    Color::DodgerBlue2,
    Color::Gold1,
    Color::Aquamarine2,
    Color::Aquamarine2,
    Color::Aquamarine2,
    Color::Aquamarine2,
    Color::Aquamarine2,
    Color::Aquamarine2,
    Color::Aquamarine2,
    Color::Aquamarine2,
    Color::Aquamarine2,
    Color::Aquamarine2,
    Color::Aquamarine2,
    Color::DodgerBlue2,
    Color::DodgerBlue2,
    Color::White,
    Color::White,
    Color::White,
    Color::White,
    Color::White,
    Color::DodgerBlue2,
    Color::DodgerBlue2,
    Color::DodgerBlue2,
    Color::DodgerBlue2,
    Color::DodgerBlue2,
    Color::DodgerBlue2,
    Color::DodgerBlue2,
    Color::White,
    Color::White,
    Color::White,
    Color::SpringGreen4,
    Color::SpringGreen4,
    Color::SpringGreen4,
    Color::SpringGreen4,
    Color::SpringGreen4,
    Color::SpringGreen4,
    Color::SpringGreen4,
    Color::Grey70,
    Color::SpringGreen4,
    Color::White,
    Color::Yellow5,
    Color::Yellow5,
    Color::Grey66,
    Color::Grey66,
    Color::Grey63
};

ColorScheme::TypesElements ColorScheme::TypeElements =
{

    Color::CadetBlue,
    Color::Cyan2,
    Color::CornflowerBlue,
    Color::LightCoral,
    Color::Cornsilk1,
    Color::DarkSlateGray3,
    Color::White,
    Color::Yellow,
    Color::CadetBlue,
    Color::Grey66,
    Color::Grey63
};


/*
 *
 *

    {Type::Assign,      Color::CadetBlue},
    {Type::Number,      Color::Cyan2},
    {Type::Keyword,     Color::CornflowerBlue},
    {Type::Operator,    Color::LightCoral},
    {Type::Binary,      Color::Cornsilk1},
    {Type::Text,        Color::DarkSlateGray3},
    {Type::Id,          Color::White},
    {Type::Punc,        Color::Yellow},
    {Type::Prefix,      Color::CadetBlue},
    {Type::LineComment, Color::Grey66},
    {Type::BlocComment, Color::Grey63}


ColorScheme::TypePairArray ColorScheme::TypesColorData =
 {
    {Keyword,       Color::Reset},
    {Binary,        Color::Reset},
    {Prefix,        Color::Reset},
    {OpenPair,      Color::Reset},
    {ClosePair,     Color::Reset},
    {Unary,         Color::Reset},
    {Assign,        Color::Reset},
    {LineComment,   Color::Reset},
    {Unary,         Color::Reset},
    {OpenPair,      Color::Reset},
    {ClosePair,     Color::Reset},
    {BlocComment,   Color::Reset},
    {Punc,          Color::Reset},
    {Postfix,       Color::Reset},
    {Text,          Color::Reset},
    {Keyword,       Color::Reset},
    {Number,        Color::Reset},
    {VoidPtr,       Color::Reset},
    {Any,           Color::Reset},
    {Sign,          Color::Reset},
    {I8,            Color::Reset},
    {I16,           Color::Reset},
    {I32,           Color::Reset},
    {I64,           Color::Reset},
    {U8,            Color::Reset},
    {U16,           Color::Reset},
    {U32,           Color::Reset},
    {U64,           Color::Reset},
    {Float,         Color::Reset},
    {Var,           Color::Reset},
    {Bloc,          Color::Reset},
    {Pipe,          Color::Reset},
    {Operator,      Color::Reset},
    {Function,      Color::Reset},
    {FunctionPtr,   Color::Reset},
    {Object,        Color::Reset},
    {Pointer,       Color::Reset},
    {Type,          Color::Reset},
    {Id,            Color::Reset},
    {Ref,           Color::Reset},
    {Bool,          Color::Reset},
    {Hex,           Color::Reset},
    {Const,         Color::Reset},
    {Static,        Color::Reset},
    {F32,           Color::Reset},
    {F64,           Color::Reset},
    {F128,          Color::Reset},
    {OctalFormat,   Color::Reset},
    {BinFormat,     Color::Reset},
    {LineComment,   Color::Reset},
    {BlocComment,   Color::Reset},
    {Integer,       Color::Reset},
    {Unsigned,      Color::Reset}
};
 */




std::string ColorScheme::operator[](Mnemonic M)
{
    auto& [Types,Mnemonics] = *CurrentTheme;
    return Color::Ansi(Mnemonics[static_cast<size_t>(M)]);
}

std::string ColorScheme::operator[](Spp::Type::T T)
{
    // Ansi / [Html: - not yet there... ]
    auto& Types = CurrentTheme->first;
    for(auto I = 0; I < Types.size(); I++)
    {
        if (Types[I] & T)
            return Color::Ansi(Types[I]);
    }
    return Color::Ansi(Types[0]);
}

ColorScheme::ColorScheme()
{
    Themes["Default"] = {ColorScheme::TypeElements,ColorScheme::AffinedMnemonics};
    CurrentTheme = &Themes["Default"];
}


//std::string ColorScheme::RGB(Mnemonic M)
//{
//    return std::string();
//}
//
//std::string ColorScheme::RGB(Type::T T)
//{
//    return std::string();
//}
} // Spp