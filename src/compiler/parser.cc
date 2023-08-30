/***************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                         *
 *   serge.lussier@oldlonecoder.club                                       *
 *                                                                         *
 *                                                                         *
 *   Unless otherwise specified, all code in this project is written       *
 *   by the author (Serge Lussier)                                         *
 *   and no one else then not even {copilot, chatgpt, or any other AI}     *
 *   --------------------------------------------------------------------- *
 *   Copyrights from authors other than Serge Lussier also apply here      *
 ***************************************************************************/




/**
 * \file parser.cc
 * \defgroup compiler
 * \author &copy; 2023, #USERNAME; oldlonecoder (serge.lussier@oldlonecoder.club)
 */


#include <xio/compiler/parser.h>

namespace xio

{



std::map<std::string, book::rem::code(parser::*)()> extern_parsers =
{
    {"expression", &parser::parse_expr},
    {"functionid", nullptr},
    {"newvar", nullptr},
    {"obj_instance",nullptr},
    {"objectid", nullptr},
    {"var_id", nullptr}
};





parser::parser(xiobloc* bloc, const char* source_or_filename): _filename_or_source(source_or_filename),_bloc(bloc)
{}

parser::parser(xiobloc* bloc, const char* source_or_filename, const std::string& use_this_rules_text):
    _filename_or_source(source_or_filename),
    _rules_src(use_this_rules_text),
    _bloc(bloc)
{}






/**
 *  \brief parse_expr  Explicitely parses rule 'expression'.
 *
 * \return book::rem::accepted, or book::rem::error if failed.
 * \author &copy; August 23, 2023; oldlonecoder, (serge.lussier@oldlonecoder.club)

 */
book::rem::code parser::parse_expr()
{

    return book::rem::notimplemented;
}


/**
 * \brief parse_rule
 *
 * \param rule_name
 * \return book::rem::accepted, or book::rem::rejected if <i>rule name</i> does not matches the code.
 * \author &copy; August 23, 2023; oldlonecoder, (serge.lussier@oldlonecoder.club)

 */
book::rem::code parser::parse_rule(const std::string& rule_name)
{
    grammar::rule* rule = grammar::rules[rule_name];
    if (!rule)
    {
        book::rem::push_error() << " the rule identified by '" << rule_name << "' doe not exist.";
        return book::rem::notexist;
    }

    if (rule->a.is_parserctrl())
    {
        for (auto& [rid, fn] : extern_parsers)
        {
            if (rid == rule_name)
            {
                if (!fn)
                {
                    book::rem::push_error() << "[parser rule]: The external rule identified by '" << rule->a() << color::Reset << '\'' << color::Yellow << rule_name << color::Reset << "' is " << book::rem::notimplemented;
                    return book::rem::notimplemented;
                }
                return (this->*fn)();
            }
        }
        book::rem::push_error() << "[parser rule]: The external rule identified by '" << color::Yellow << rule_name << color::Reset << "' is out of bounds : " << book::rem::notexist;
    }

    //...
    book::rem::push_message() << "[parser rule]: The sequential rule identified by " << rule->a() << color::Reset << '\''
        << color::Yellow
        << rule_name
        << color::Reset << "' is"
        << book::rem::notimplemented;

    return book::rem::notimplemented;
}



/**
 * \brief make_instruction callback function invoked from xio::input.
 *
 * Will create the proper instance of the instruction from the token_data infos.
 * \param token  pointer to the current token.
 * \return pointer to newly created xio;
 * \note As of 2023-08-28, only xio's POD variable types are created on identifier token restricted to arithmetic expressions.
 * \author &copy; August 28, 2023; oldlonecoder, (serge.lussier@oldlonecoder.club)
 */
xio* parser::make_instruction(token_data* token)
{

    return nullptr;
}


}