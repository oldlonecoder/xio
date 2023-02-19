#pragma once

#include <functional>
#include <utility>
#include <stack>
#include <map>


//-- Common includes for this and derived classes...
#include <xio/alu.h>
#include <xio/lexer/tokendata.h>
#include <tlux/diagnostic.h>
using namespace tux;

namespace xio
{
/*!
    @brief Executable Instruction Object.

    @author copy;2022, oldlonecoder (Serge Lussier, lussier.serge@gmail.com)
*/

class  xiobject
{
    friend class compiler;


protected:
    xiobject* op = nullptr; ///< Parent Operator
    xiobject* lhs = nullptr; ///< Left Hand Side Operand.
    xiobject* rhs = nullptr; ///< Right Hand Size Operand

    xiobject* _parent = nullptr;
    token_data* t0 = nullptr; ///< Token informations.

    alu* acc = nullptr; ///< Arithmetic Logical Unit, or Accumulator, or whatever!
    struct storage_attr {
        uint8_t lvalue : 1; // 0 = rvalue non-assignable (such as const,leaf). 1= left value; assignable.
        uint8_t own : 1; // 1 = this xiobject owns its acc storage - so will delete it in the destructor.
        uint8_t leave : 1; // 1 = leave flag because this xiobject is a return statement.
        uint8_t sstatic : 1; // 1 = static storage.
        uint8_t xshared : 1; // 1 = cannot be deleted by the parent's destructor.
        // ...
    }mem = { 0,0,0,0,0 }; ///< Storage types of this xiobject.
    friend class var;
    friend class bloc;
    // friend class compiler;
    // friend class interpreter_t;

public:


    using list = std::vector<xiobject*>;

    xiobject();
    explicit xiobject(xiobject* a_parent);

    xiobject(xiobject* a_parent, token_data* a_token, alu* a_alu = nullptr);

    virtual ~xiobject();

    xiobject& operator =(const xiobject&) = delete;
    xiobject& operator =(xiobject&&) = delete;



    using maker = std::function<xiobject* (token_data*)>;
    alu acu() { return (acc ? *acc : alu(0.0f)); }

    virtual alu jsr(); // "Jump Subroutine" :)


    virtual std::string type_name();

    xiobject* oper() { return op; }
    token_data* token() { return t0; }

    template<class T> T* to() { return dynamic_cast<T*>(this); }
    //static void illegal_rvalue(xiobject* x, token_data* token);
    bool is_leave() { return mem.leave; }

    xiobject::list::iterator query(xiobject* c);

    template<class T> T* parent()
    {
        xiobject* e = this;
        while(e)
        {
            e = e->_parent;
            if(e)
            {
                if(T* p; (p = dynamic_cast<T*>(e))) return p;
            }
        }
        return nullptr;
    }

    code::T append_child(xiobject* c);
    code::T detach(xiobject* c);

protected:


    using input_table_t = std::vector<std::pair<std::pair<type::T, type::T>, std::string>>;
    using move_table_t = std::vector<std::pair<std::pair<type::T, type::T>, xiobject* (xiobject::*)(xiobject*)>>;
    static input_table_t input_tbl;
    static move_table_t move_tbl;

    using xio_opfn_t = alu(xiobject::*)();
    using xio_opfn_table_t = std::map<mnemonic, xio_opfn_t>;


    xio_opfn_t xio_fn = nullptr;

    xiobject::list _children;



    void SyntaxError(xiobject*);
    //xiobject* Warning(xiobject*);
    //xiobject* Fatal(xiobject*);

    static void make_error(code::T ErrCode, xiobject* source_node, xiobject* input_node);
    static void make_error(code::T ErrCode, xiobject* source_node, token_data* input_token);
    void header(xiobject* input_node, source_location&& Loc = {});


    // -------- Arithmetic binary tree INPUT: -----------------------------------
#pragma region INPUT

    xiobject* input(xiobject* parent_bloc, token_data* token);  ///< input nullptr token for telling to finalyse the tree... :)
    xiobject* _binary(xiobject* a);
    //xiobject* _prefix(xiobject* a);
    //xiobject* _postfix(xiobject* a);
    //xiobject* _leaf(xiobject* a);
    //xiobject* _open_pair(xiobject* a);
    xiobject* _close_pair(xiobject* a);
    //xiobject* _assign(xiobject* a);
    //xiobject* _rpar_leaf(xiobject* a);
    xiobject* _close_par(xiobject* a);
    xiobject* collapse_par_pair(xiobject* a);
    xiobject* op_to_right(xiobject* a);
    // (fr) D&egrave;s la phase d'association, on peut d&eacute;j&agrave; conna&icirc;tre le mouvement:
    //  - en validant l'appel de la fonction
    //  - en r&eacute;-associant le nouveau xiobject en se servant de la table move_tbl;
    // -----------------------------------------------------------------------------------------------


#pragma endregion INPUT

    static xiobject* begin(xiobject* parent_, token_data* token);
    xiobject* tree_close();
    xiobject* tree_root(bool skip_syntax);

#pragma region MOVE

    xiobject* to_right(xiobject* in_rhs);
    xiobject* to_left(xiobject* in_lhs);

#pragma endregion MOVE

    //std::string attribute() { return t0->text(); }

    auto query(xiobject* lhs, xiobject* rhs);

    //static void clear_static_pool();

    static std::stack<xiobject*> pars;
    static int push_par(xiobject* a);
    static xiobject* pop_par();


#pragma region TRIGRAPH

    static void dot_tree_start(stracc& a_out, const stracc& Title);
    static void dot_node(xiobject* A, stracc& a_out);
    static void dot_null_node(xiobject*, int, stracc&);
    static void dot_tree(xiobject* a_root, stracc& a_out);
    static void dot_tree_close(stracc& a_out);
    void dot_attr(stracc& a_out);

    //    static std::string trace_connect_binary_operands2(xiobject* x);

#pragma endregion TRIGRAPH
    static std::string trace_connect_binary_operands(xiobject* x);
private:

    static xiobject* _global_root_bloc;

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

    std::string attribute();
};

}
