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



                Keyword,
                Binary,
                Prefix,
                OpenPair,
                ClosePair,
                Unary,
                Assign,
                LineComment
                Unary,
                OpenPair,
                ClosePair,
                BlocComment
                Punc,
                Postfix,
                Text,
                Keyword,
                Number,
                VoidPtr
                Any
                Sign
                I8
                I16
                I32
                I64
                U8
                U16
                U32
                U64
                Float
                Var
                Bloc
                Pipe
                Operator
                Function
                FunctionPtr
                Object
                Pointer
                Type
                Id
                Ref
                Bool
                Hex
                Const
                Static
                F32
                F64
                F128
                OctalFormat
                BinFormat
                LineComment
                BlocComment
                Integer
                Unsigned
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