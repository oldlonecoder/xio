/***************************************************************************
 *   Copyright (C) 2005/2023 by Serge Lussier                              *
 *   2023: lussier.serge@gmail.com, oldlonecoder@gmail.com                 *
 *   2023: oldlonecoder@arknowledge.page                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <tlux/application.h>
#include <xio/lexer/lexer_color.h>
#include <xio/xiobloc.h>


using namespace tux;

namespace xio
{

class Test : public application
{
    lexer lex;
    lexer_color lc;
    token_data::collection tokens;
    stracc source;

public:
    Test();
    ~Test() override;

    code::T operator ()();
    code::T init(int argc, char** argv) override;

};

Test::Test():application(){}

Test::~Test()
{
    tokens.clear();
    source.clear();
}

code::T Test::operator ()()
{

    source =
R"(if !collection.empty()
{
    u8 A = 4ac * c/3 - .33;
    return A;
}
)";

    lexer::config_data d;
    d.Source = source.str().c_str();
    d.Tokens = &tokens;
    lex.config() = d;


    auto r = lex();
    if(r != code::accepted)
    {
        diagnostic::info() << code::failed << " lexer tests";
        //...
    }
    lc << lex.config().Source;
    diagnostic::message() << code::begin << " dumping tokens:";
    for(auto& token : tokens)
    {
        diagnostic::output() << token.details(true);
    }
    diagnostic::test() << code::end << " lexer_color: " << code::endl << lc.Product() << color::Reset;

    diagnostic::test() << " xio::alu:";
    alu A = 42;
    diagnostic::output() << code::begin << " xio::alu A(=42) := " << color::Yellow << A();
    alu B = 0;
    diagnostic::test() << "test alu B = 0; alu C = A/B:";
    alu C = A / B;
    alu D = "infinity";
    diagnostic::output() << "C:" << D();
    diagnostic::test() << code::end << code::success;
    return code::accepted;
}

code::T Test::init(int argc, char **argv)
{
    // Setup the diagnostic data  and attr db;
    application::init(argc,argv);
    return code::ok;
}

}

auto main(int argc, char **argv) -> int
{
    stracc str = "Hello, xio !\n";
    std::cout << str();
    xio::Test Test;
    if(code::T r; (r = Test.init(argc, argv)) != code::ok)
    {
        diagnostic::message(sfnl) << r << " tests stop here.";
        goto out;
    }
    diagnostic::message() << " Now launching the tests:";
    Test();
out:
    diagnostic::clear([](diagnostic::log_entry& e) {
        std::cout << diagnostic::cc(e);// << '\n'; // ?
        //...
    });
    std::cout << "\n";
    return 0;
}


