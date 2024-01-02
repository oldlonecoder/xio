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
 *   Open source FREE licences also apply To the code from the same author (Serge Lussier)*
 *   ----------------------------------------------------------------------------------   *
 ******************************************************************************************/

#pragma once

#include <functional>
#include <utility>
#include <stack>
#include <map>


//-- Common includes for this and derived classes...
#include "Spp/Alu.h"
#include "Spp/SppToken.h"
//#include "logbook/expect.h"
#include <AppBook/Util/Object.h>
#include <AppBook/Book/BookEnums.h>




namespace Spp
{



/*!
    @brief Script Instruction Object.

    @author copy;2008-2023, oldlonecoder (Serge Lussier, lussier.serge@gmail.com)
*/

class  SPP_EXPORT xio : public Util::Object
{
    //friend class compiler;
//    friend class ::Spp::spp::interpretr;
protected:
    xio* Op = nullptr; ///< Parent Operator
    xio* Lhs = nullptr; ///< Left Hand Side Operand.
    xio* Rhs = nullptr; ///< Right Hand Size Operand

    SppToken* Tk = nullptr; ///< Token informations.

    Alu* A = nullptr; ///< Arithmetic Logical Amu, or Accumulator, or whatever!
    struct SPP_EXPORT storage_attr {
        uint8_t LV : 1; // 0 = rvalue non-assignable (such as const,leaf). 1= left value; assignable.
        uint8_t PRef : 1; // 1 = this xio owns its acc storage - so will delete it in the destructor.
        uint8_t LF : 1; // 1 = leave flag because this xio is a return statement.
        uint8_t Static : 1; // 1 = static storage.
        uint8_t Ref : 1; // 1 = cannot be deleted by the parent's destructor.
        // ...
    }Mem = { 0,0,0,0,0 }; ///< Storage types of this xio.
    //friend class xiovar;
    //friend class xiobloc;
    // friend class compiler;
    // friend class interpreter_t; if A in{0.9,4,5,67,7} {...};

public:

    using Array = std::vector<xio*>;

    xio();
    explicit xio(xio* a_parent);

    xio(Object* a_parent, SppToken* a_token, Alu* a_alu = nullptr);

    ~xio() override;

    xio& operator =(const xio&) = delete;
    xio& operator =(xio&&) = delete;



    using maker = std::function<::Spp::xio * (SppToken *)>;
    Alu Value() { return (A ? *A : Alu(0.0f)); }
    Alu* AluPtr() { return A; }
    virtual Alu JSR(); // "Jump Subroutine" :)


    [[maybe_unused]] virtual std::string TypeName();

    xio* Operator() { return Op; }
    SppToken* TokenPtr() { return Tk; }

//    template<class T> T* To() { return dynamic_cast<T*>(this); }
    //static void illegal_rvalue(xio* x, SppToken* token);
    bool IsLeave() { return Mem.LF; }

//    Object::Iterator GetChildIterator(xio* c);

//    Book::Enums::Code AppendChild(xio* c);
//    virtual Book::Enums::Code Detach(xio* c);
//    virtual Book::Enums::Code detach();

protected:


    using input_table_t = std::vector<std::pair<std::pair<Type::T, Type::T>, std::string>>;
    using move_table_t = std::vector<std::pair<std::pair<Type::T, Type::T>, xio* (xio::*)(xio*)>>;
    static input_table_t input_tbl;
    static move_table_t move_tbl;

    using xio_opfn_t = Alu(xio::*)();
    using SioOperatorFnTable = std::map<Mnemonic, xio_opfn_t>;


    xio_opfn_t xio_fn = nullptr;

    xio::Array _children;



    void syntax_error(xio*);
    xio* Warning(xio*);
    //xio* Fatal(xio*);

    static void make_error(Book::Enums::Code ErrCode, xio* source_node, xio* input_node);
    static void make_error(Book::Enums::Code ErrCode, xio* source_node, SppToken* input_token);
    void Header(xio* input_node, std::source_location&& Loc = std::source_location::current());


    // -------- Arithmetic binary tree INPUT: -----------------------------------
#pragma region INPUT
public:
    xio* TreeInput(xio* parent_bloc, SppToken* token, xio::maker mk= nullptr);  ///< TreeInput nullptr TokenPtr for telling To finalyse the tree... :)
    xio* TreeInputBinary(xio* a);
    //xio* _prefix(xio* a);
    //xio* _postfix(xio* a);
    //xio* _leaf(xio* a);
    //xio* _open_pair(xio* a);
    xio* _close_pair(xio* a);
    //xio* _assign(xio* a);
    //xio* _rpar_leaf(xio* a);
    xio* _close_par(xio* a);
    xio* collapse_par_pair(xio* a);


    xio* CloseTree();
    xio* TreeRoot(bool skip_syntax);
    // (fr) D&egrave;s la phase d'association, on peut d&eacute;j&agrave; conna&icirc;tre le mouvement:
    //  - en validant l'appel de la fonction
    //  - en r&eacute;-associant le nouveau xio en se servant de la table move_tbl;
    // -----------------------------------------------------------------------------------------------

    static xio* TreeBegin(xio* ParentObj, SppToken* Token, const xio::maker& Maker= nullptr);
    xio* ToRight(xio* in_rhs);
    xio* ToLeft(xio* in_lhs);
    xio* OpToRight(xio* a);
#pragma endregion INPUT


#pragma endregion MOVE

    std::string Attribute() { return Tk ? Tk->Text() : "no Attribute"; }

    static auto Match(xio* in_lhs, xio* in_rhs);

    //static void clear_static_pool();

    static std::stack<xio*> pars;
    static int PushPar(xio* a);
    static xio* PopPar();


#pragma region TRIGRAPH

    static void dot_tree_start(StrAcc& a_out, const StrAcc& Title);
    static void dot_node(xio* A, StrAcc& a_out);
    static void dot_null_node(xio*, int, StrAcc&);
    static void dot_tree(xio* a_root, StrAcc& a_out);
    static void dot_tree_close(StrAcc& a_out);
    void dot_attr(StrAcc& a_out);

    //    static std::string trace_connect_binary_operands2(xio* x);

#pragma endregion TRIGRAPH
    static std::string TraceConnectBinaryOperands(xio* x);
    static std::string trace_connect_postfix_operands(xio* x);
private:

    static xio* _global_root_bloc;

    // --------------------------------------------------------------------------
    static SioOperatorFnTable xio_operators_table;


    // ----------- Primary operators ----------------------------
#pragma region ARITMETIC
    virtual Alu  LeftShift();
    virtual Alu  Radical();
    virtual Alu  Exponent();
    virtual Alu  RightShift();
    virtual Alu  Decr();
    virtual Alu  Incr();
    virtual Alu  AssignAdd();
    virtual Alu  AssignSub();
    virtual Alu  AssignMul();
    virtual Alu  AssignDiv();
    virtual Alu  AssignMod();
    virtual Alu  AssignAnd();
    virtual Alu  AssignOr();
    virtual Alu  AssignXor();
    virtual Alu  AssignX1();
    virtual Alu  AssignLShift();
    virtual Alu  AssignRShift();
    virtual Alu  Leq();
    virtual Alu  Geq();
    virtual Alu  Eq();
    virtual Alu  Neq();
    virtual Alu  Add();
    virtual Alu  Sub();
    virtual Alu  Mul();
    virtual Alu  Modulo();
    virtual Alu  LessThan();
    virtual Alu  GreaterThan();
    virtual Alu  Assign();
    virtual Alu  BinAnd();
    virtual Alu  BinOr();
    virtual Alu  BitXor();
    virtual Alu  X1();
    virtual Alu  X2();
    virtual Alu  BitNot();
    virtual Alu  BoolAnd();
    virtual Alu  BoolOr();
    virtual Alu  Division();
    virtual Alu  Factorial();
    virtual Alu  Positive();
    virtual Alu  Negative();
    virtual Alu  KPi();
    virtual Alu  KCos();
    virtual Alu  KAcos();
    virtual Alu  KTan();
    virtual Alu  KAtan();
    virtual Alu  KSin();
    virtual Alu  KAsin();
    virtual Alu  KNumber();
    virtual Alu  KU8();
    virtual Alu  KU16();
    virtual Alu  KU32();
    virtual Alu  KU64();
    virtual Alu  KI8();
    virtual Alu  KI16();
    virtual Alu  KI32();
    virtual Alu  KI64();
    virtual Alu  KReal();
    virtual Alu  KString();

#pragma endregion ARITHMETICS

    //std::string Attribute();
};

}
