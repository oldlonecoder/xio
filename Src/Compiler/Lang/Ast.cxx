#include "Spp/Compiler/Lang/Ast.h"



namespace Spp::Lang
{
Book::Expect<Ast::Node*> ArithmeticAST::TreeBegin(SppToken* aToken)
{
    return Book::Expect<Ast::Node*>();
}

Book::Expect<Ast::Node*> ArithmeticAST::Input(SppToken* aToken)
{
    return Book::Expect<Ast::Node*>();
}

Book::Expect<Ast::Node*> ArithmeticAST::Root()
{
    return Book::Expect<Ast::Node*>();
}

Book::Expect<Ast::Node*> ArithmeticAST::Close()
{
    return Book::Expect<Ast::Node*>();
}

Book::Expect<Ast::Node*> Ast::Input(SppToken* aToken)
{
    return Book::Expect<Ast::Node*>();
}



}