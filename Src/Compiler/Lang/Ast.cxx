#include "Spp/Compiler/Lang/Ast.h"



namespace Spp::Lang
{
Book::Expect<Ast::Node*> MathAST::TreeBegin(SppToken* aToken)
{
    return Book::Expect<Ast::Node*>();
}

Book::Expect<Ast::Node*> MathAST::Input(SppToken* aToken)
{
    return Book::Expect<Ast::Node*>();
}

Book::Expect<Ast::Node*> MathAST::Root()
{
    return Book::Expect<Ast::Node*>();
}

Book::Expect<Ast::Node*> MathAST::Close()
{
    return Book::Expect<Ast::Node*>();
}

Book::Expect<Ast::Node*> Ast::Input(SppToken* aToken)
{
    return Book::Expect<Ast::Node*>();
}



}