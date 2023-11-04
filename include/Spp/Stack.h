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

#include <Spp/Runtime/Variable.h>
#include <AppBook/Book/AppBook.h>


namespace Spp
{





class SPP_EXPORT Stack : public xio
{
public:
    using Array = std::vector<Stack*>;

protected:
    xio::Array          Instructions;
    Variable::Array     LocalVariables;
    //Function::Array LocalFunctions; // On Hold ...


public:

    Stack();
    Stack(const std::string& _Id);
    Stack(Core::Object* _Parent, const std::string& _Id);
    Stack(Stack* _Parent, SppToken* _Token = nullptr, Alu* A = nullptr);


    ~Stack() override;

    Stack(const Stack&);



// Semantic operators:

//    Stack& operator = (Stack&& SIO) noexcept = default;
      Stack& operator = (const Stack& SIO) ;

    Alu JSR() override;

    Variable* NewVar(SppToken* _Token);
    Variable* GetLocalVariableById(const std::string& _ID);
    Variable* GetVariableById(const std::string& _ID);

    Book::Enums::Code RemoveInstruction(xio* _Obj);
    Book::Enums::Code RemoveVariable   (Variable* _Obj);
    //Book::Enums::Code RemoveFunction(xio* _Obj);
    // Book::Enums::Code RemoveBloc(xio* _Obj); on hold....

    virtual Book::Enums::Code AppendInstruction(xio* _In);


};

}