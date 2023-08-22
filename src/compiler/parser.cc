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

#include <xio/compiler/parser.h>

namespace xio::cc
{
parser::context_data::context_data(token_data::collection* token_stream)
{
    assign_token_stream(token_stream);
}

parser::context_data::~context_data()
{
}

parser::context_data& parser::context_data::operator=(token_data::collection* tkptr)
{
    assign_token_stream(tkptr);
    return *this;

}

void parser::context_data::assign_token_stream(token_data::collection* tkstream)
{
    begin = cursor = end = tkstream->begin();
    source_end = tkstream->end();
}


book::rem::code parser::parse_expr()
{
    
    return book::rem::notimplemented;
}

book::rem::code parser::parse_rule(const std::string& rule_name)
{
    return book::rem::notimplemented;
}

parser::parser(const char* source_or_filename, token_data::collection* token_stream):
    _filename_or_source(source_or_filename),
    _tokens_stream(token_stream)
{}

parser::parser(const char* source_or_filename, token_data::collection* token_stream, const std::string& use_this_rules_text):
    _filename_or_source(source_or_filename),
    _tokens_stream(token_stream)
{}




}