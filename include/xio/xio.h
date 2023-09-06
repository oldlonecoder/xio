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

#include <functional>
#include <utility>
#include <stack>
#include <map>


//-- Common includes for this and derived classes...
#include "xio/alu.h"
#include "xio/tokendata.h"
//#include "logbook/expect.h"
#include <logbook/object.h>


namespace xio
{
namespace spp
{
    class interpretr;
}
/*!
    @brief Executable Instruction Object.

    @author copy;2022, oldlonecoder (Serge Lussier, lussier.serge@gmail.com)
*/

class  XIO_PUBLIC xio : public book::object
{
    friend class compiler;
    friend class ::xio::spp::interpretr;
protected:
    xio* op = nullptr; ///< Parent Operator
    xio* lhs = nullptr; ///< Left Hand Side Operand.
    xio* rhs = nullptr; ///< Right Hand Size Operand

    token_data* t0 = nullptr; ///< Token informations.

    alu* acc = nullptr; ///< Arithmetic Logical Unit, or Accumulator, or whatever!
    struct XIO_PUBLIC storage_attr {
        uint8_t lvalue : 1; // 0 = rvalue non-assignable (such as const,leaf). 1= left value; assignable.
        uint8_t own : 1; // 1 = this xio owns its acc storage - so will delete it in the destructor.
        uint8_t leave : 1; // 1 = leave flag because this xio is a return statement.
        uint8_t sstatic : 1; // 1 = static storage.
        uint8_t xshared : 1; // 1 = cannot be deleted by the parent's destructor.
        // ...
    }mem = { 0,0,0,0,0 }; ///< Storage types of this xio.
    friend class xiovar;
    friend class xiobloc;
    // friend class compiler;
    // friend class interpreter_t; if A in{0.9,4,5,67,7} {...};

public:


    using list = std::vector<xio*>;

    xio();
    explicit xio(xio* a_parent);

    xio(object* a_parent, token_data* a_token, alu* a_alu = nullptr);

    virtual ~xio();

    xio& operator =(const xio&) = delete;
    xio& operator =(xio&&) = delete;



    using maker = std::function<::xio::xio* (token_data*)>;
    alu acu() { return (acc ? *acc : alu(0.0f)); }
    alu* aluptr() { return acc; }
    virtual alu jsr(); // "Jump Subroutine" :)


    virtual std::string type_name();

    xio* oper() { return op; }
    token_data* token() { return t0; }

    template<class T> T* to() { return dynamic_cast<T*>(this); }
    //static void illegal_rvalue(xio* x, token_data* token);
    bool is_leave() { return mem.leave; }

    xio::list::iterator query(xio* c);

    book::rem::code append_child(xio* c);
    virtual book::rem::code detach(xio* c);
    virtual book::rem::code detach();

protected:


    using input_table_t = std::vector<std::pair<std::pair<type::T, type::T>, std::string>>;
    using move_table_t = std::vector<std::pair<std::pair<type::T, type::T>, xio* (xio::*)(xio*)>>;
    static input_table_t input_tbl;
    static move_table_t move_tbl;

    using xio_opfn_t = alu(xio::*)();
    using xio_opfn_table_t = std::map<mnemonic, xio_opfn_t>;


    xio_opfn_t xio_fn = nullptr;

    xio::list _children;



    void syntax_error(xio*);
    xio* warning(xio*);
    //xio* Fatal(xio*);

    static void make_error(book::rem::code ErrCode, xio* source_node, xio* input_node);
    static void make_error(book::rem::code ErrCode, xio* source_node, token_data* input_token);
    void header(xio* input_node, book::source_location&& Loc = {});


    // -------- Arithmetic binary tree INPUT: -----------------------------------
#pragma region INPUT
public:
    xio* input(xio* parent_bloc, token_data* token, xio::maker mk=nullptr);  ///< input nullptr token for telling to finalyse the tree... :)

    xio* _binary(xio* a);
    //xio* _prefix(xio* a);
    //xio* _postfix(xio* a);
    //xio* _leaf(xio* a);
    //xio* _open_pair(xio* a);
    xio* _close_pair(xio* a);
    //xio* _assign(xio* a);
    //xio* _rpar_leaf(xio* a);
    xio* _close_par(xio* a);
    xio* collapse_par_pair(xio* a);
    xio* op_to_right(xio* a);

    xio* tree_close();
    xio* tree_root(bool skip_syntax);
    // (fr) D&egrave;s la phase d'association, on peut d&eacute;j&agrave; conna&icirc;tre le mouvement:
    //  - en validant l'appel de la fonction
    //  - en r&eacute;-associant le nouveau xio en se servant de la table move_tbl;
    // -----------------------------------------------------------------------------------------------


#pragma endregion INPUT

    static xio* begin(xio* parent_, token_data* token, xio::maker xmk=nullptr);
    xio* to_right(xio* in_rhs);
    xio* to_left(xio* in_lhs);

#pragma endregion MOVE

    std::string attribute() { return t0 ? t0->text(): "no attribute"; }

    auto query(xio* lhs, xio* rhs);

    //static void clear_static_pool();

    static std::stack<xio*> pars;
    static int push_par(xio* a);
    static xio* pop_par();


#pragma region TRIGRAPH

    static void dot_tree_start(stracc& a_out, const stracc& Title);
    static void dot_node(xio* A, stracc& a_out);
    static void dot_null_node(xio*, int, stracc&);
    static void dot_tree(xio* a_root, stracc& a_out);
    static void dot_tree_close(stracc& a_out);
    void dot_attr(stracc& a_out);

    //    static std::string trace_connect_binary_operands2(xio* x);

#pragma endregion TRIGRAPH
    static std::string trace_connect_binary_operands(xio* x);
private:

    static xio* _global_root_bloc;

    // --------------------------------------------------------------------------
    static xio_opfn_table_t xio_operators_table;


    // ----------- Primary operators ----------------------------
#pragma region ARITMETIC
    virtual alu  LeftShift();
    virtual alu  Radical();
    virtual alu  Exponent();
    virtual alu  RightShift();
    virtual alu  Decr();
    virtual alu  Incr();
    virtual alu  AssignAdd();
    virtual alu  AssignSub();
    virtual alu  AssignMul();
    virtual alu  AssignDiv();
    virtual alu  AssignMod();
    virtual alu  AssignAnd();
    virtual alu  AssignOr();
    virtual alu  AssignXor();
    virtual alu  AssignX1();
    virtual alu  AssignLShift();
    virtual alu  AssignRShift();
    virtual alu  Leq();
    virtual alu  Geq();
    virtual alu  Eq();
    virtual alu  Neq();
    virtual alu  Add();
    virtual alu  Sub();
    virtual alu  Mul();
    virtual alu  Modulo();
    virtual alu  LessThan();
    virtual alu  GreaterThan();
    virtual alu  Assign();
    virtual alu  BinAnd();
    virtual alu  BinOr();
    virtual alu  BitXor();
    virtual alu  X1();
    virtual alu  X2();
    virtual alu  BitNot();
    virtual alu  BoolAnd();
    virtual alu  BoolOr();
    virtual alu  Division();
    virtual alu  Factorial();
    virtual alu  Positive();
    virtual alu  Negative();
    virtual alu  KPi();
    virtual alu  KCos();
    virtual alu  KAcos();
    virtual alu  KTan();
    virtual alu  KAtan();
    virtual alu  KSin();
    virtual alu  KAsin();
    virtual alu  KNumber();
    virtual alu  KU8();
    virtual alu  KU16();
    virtual alu  KU32();
    virtual alu  KU64();
    virtual alu  KI8();
    virtual alu  KI16();
    virtual alu  KI32();
    virtual alu  KI64();
    virtual alu  KReal();
    virtual alu  KString();

#pragma endregion ARITHMETICS

    //std::string attribute();
};

}
