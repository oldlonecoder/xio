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

#include "xio/xio.h"
#include <chrtools/geometry.h>

#include "xio/s++/spp.h"

namespace xio
{

xio* xio::_global_root_bloc = nullptr;

std::stack<xio*> xio::pars;

//xio* xio::get_global_root() { return xio::{type::binary,     type::open_pair},  &xio::set_right_global_root_bloc; }


#define deg2rad(_xio) (((_xio)->acc->v.d*3.1415) / 180)
constexpr auto _PI_ = 3.14159265358979323846;
// M_PI ?


// void xio::// discard()
// {
//     for (auto x : xio::xio_gc)
//         if (this == x) return;

//     xio::xio_gc.push_back(this);
//     lhs = rhs = op = nullptr; // Make sure we do not accidently reuse this;
// }



    // Primary arithmetic supported operators table:
xio::xio_opfn_table_t   xio::xio_operators_table{
    {mnemonic::LeftShift,           &xio::LeftShift},
    {mnemonic::Radical,             &xio::Radical},
    {mnemonic::Exponent,            &xio::Exponent},
    {mnemonic::RightShift,          &xio::RightShift},
    {mnemonic::Decr,                &xio::Decr},
    {mnemonic::Incr,                &xio::Incr},
    {mnemonic::AssignAdd,           &xio::AssignAdd},
    {mnemonic::AssignSub,           &xio::AssignSub},
    {mnemonic::AssignMul,           &xio::AssignMul},
    {mnemonic::AssignDiv,           &xio::AssignDiv},
    {mnemonic::AssignMod,           &xio::AssignMod},
    {mnemonic::AssignAnd,           &xio::AssignAnd},
    {mnemonic::AssignOr,            &xio::AssignOr},
    {mnemonic::AssignXor,           &xio::AssignXor},
    {mnemonic::AssignC1,            &xio::AssignX1},
    {mnemonic::AssignLeftShift,     &xio::AssignLShift},
    {mnemonic::AssignRightShift,    &xio::AssignRShift},
    {mnemonic::LessEq,              &xio::Leq},
    {mnemonic::GreaterEq,           &xio::Geq},
    {mnemonic::Equal,               &xio::Eq},
    {mnemonic::NotEqual,            &xio::Neq},
    {mnemonic::Add,                 &xio::Add},
    {mnemonic::Sub,                 &xio::Sub},
    {mnemonic::Mul,                 &xio::Mul},
    {mnemonic::Modulo,              &xio::Modulo},
    {mnemonic::LessThan,            &xio::LessThan},
    {mnemonic::GreaterThan,         &xio::GreaterThan},
    {mnemonic::Assign,              &xio::Assign},
    {mnemonic::BinaryAnd,           &xio::BinAnd},
    {mnemonic::BinaryOr,            &xio::BinOr},
    {mnemonic::BinaryXor,           &xio::BitXor},
    {mnemonic::C1,                  &xio::X1},
    {mnemonic::C2,                  &xio::X2},
    {mnemonic::BinaryNot,           &xio::BitNot},
    {mnemonic::LogicalAnd,          &xio::BoolAnd},
    {mnemonic::LogicalOr,           &xio::BoolOr},
    {mnemonic::Div,                 &xio::Division},
    {mnemonic::Factorial,           &xio::Factorial},
    {mnemonic::Positive,            &xio::Positive},
    {mnemonic::Negative,            &xio::Negative},
    {mnemonic::Pi,                  &xio::KPi},
    {mnemonic::Cosinus,             &xio::KCos},
    {mnemonic::ArcCosinus,          &xio::KAcos},
    {mnemonic::Tangent,             &xio::KTan},
    {mnemonic::ArcTangent,          &xio::KAtan},
    {mnemonic::Sinus,               &xio::KSin},
    {mnemonic::ArcSinus,            &xio::KAsin},
    {mnemonic::Number,              &xio::KNumber},
    {mnemonic::U8,                  &xio::KU8   },
    {mnemonic::U16,                 &xio::KU16  },
    {mnemonic::U32,                 &xio::KU32  },
    {mnemonic::U64,                 &xio::KU64  },
    {mnemonic::I8,                  &xio::KI8   },
    {mnemonic::I16,                 &xio::KI16  },
    {mnemonic::I32,                 &xio::KI32  },
    {mnemonic::I64,                 &xio::KI64  },
    {mnemonic::Float,               &xio::KReal },
    {mnemonic::String,              &xio::KString}
};


// xio::xio()
// {
//     //acc = std::make_shared<alu>(0.0);
// }



xio::xio():object()
{
    acc = new alu(0.0f); // Shit!
}

xio::xio(xio* parent_bloc): object(parent_bloc, "xio")
{
    //token_info = new token_data;
    acc = new alu;
}


xio::~xio()
{
    if (!mem.xshared) delete acc;
}

xio::xio(object* parent_bloc, token_data* atoken, alu* a_alu): object(parent_bloc, atoken? atoken->text(): "xio")
{
    t0 = atoken;

    //book::rem::push_debug() << "xio::xio => token details:[" << (t0 ? t0->details() : "nullptr") << "]"<< book::rem::endl << book::rem::commit;

    if (a_alu != nullptr)
    {
        acc = a_alu;
        mem.own = 0;
        return;
    }

    acc = new alu(0.0f);

    if (!t0)
        return;

    switch (t0->t) {
    case type::Text:
        *acc = atoken->text();
        return;
    case type::Number:
    {
        ///@todo Attention enlever le court-circuit du type-size!!!
        if (t0->s & type::Number)
        {
            double d;
            (stracc(atoken->text())) >> d;
            *acc = d;
            book::rem::push_debug() << " acc: " << color::Yellow << (*acc)()<<  book::rem::commit;
        }
    }
    return;
    case type::Hex:
    {
        uint64_t d;
        stracc(atoken->text()).hexadecimal(d);
        *acc = d;
        return;
    }
    return;
    case type::Any:
    case type::VoidPtr:
        *acc = (void*)nullptr;
        return;
    default:

        break;
    }
    
    auto i = xio::xio_operators_table.find(atoken->m);
    xio_fn = i != xio::xio_operators_table.end() ? i->second : nullptr;
    //book::rem::push_debug(HERE) << " acc: '" << color::Yellow << (*acc)() << color::White << "'"<< book::rem::commit;
}

xio::list::iterator xio::query(xio* c)
{
    xio::list::iterator i = _children.begin();

    for(; i != _children.end(); i++)
    {
        if(*i == c)
            break;
    }
    return i;
}


// ------------------ ARITHMETIC OPERATORS IMPLEMENTATION ---------------------------------------------

alu xio::jsr()
{
    //...

    book::rem::push_debug(HERE) << color::White << attribute() << " value:" << color::Yellow << acc->number<uint64_t>() << book::rem::endl << t0->mark();
    if(t0->is_binary())
        book::rem::out() << xio::trace_connect_binary_operands(this)<< book::rem::endl << book::rem::commit;




    if (lhs) *acc = lhs->jsr(); // Always catch the lhs value so we return that one if there is no rhs operand.
    if (rhs) *acc = rhs->jsr(); // Always catch the rhs value because it is the value to be returned after being applied to the lhs (if applicable).

    if (xio_fn)
        return (this->*xio_fn)();// All operators assign acc.
    else {
        if (t0->is_operator()) {
            book::rem::push_warning() << "operator xio [" << color::Yellow << t0->text() << color::Reset << "] has no implementation (yet?).:\n" << t0->mark()<< book::rem::endl << book::rem::commit;
        }
    }
    t0->s |= acc->T; ///< Why is that ?
    // It's because our actual token::type (xio::type::T) has been changed by the type of the resulting operation alu::T ( acc->T is put into t0->s )...
    // Ceci est le changement apporté au champs sémantique du token qui est modifié par le type résultant de l'opération.  ^       ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ...

    return *acc;
}

book::rem::code xio::append_child(xio *c)
{
    auto e = query(c);
    if(e != _children.end())
        _children.push_back(c);
    else
        return book::rem::exist;

    return book::rem::accepted;
}


book::rem::code xio::detach(xio * c)
{
    auto i = query(c);
    if(i != _children.end())
        _children.erase(i);
    else
        return book::rem::notexist;

    return book::rem::accepted;
}

book::rem::code xio::detach()
{
    xio* p = parent<xio>();
    if(p)
    {
        p->detach(this);
    }
    return book::rem::accepted;
}



alu xio::LeftShift()
{
    if((lhs->t0->s & type::Float) || (rhs->t0->s & type::Float))
    {
        *acc = 0.f;
        book::rem::push_warning() << lhs->t0->type_name() << " " << t0->text() << " " << rhs->t0->type_name() << " are incompatible" << book::rem::commit;
    }

    *acc = lhs->acc->number<uint64_t>() << rhs->acc->number<uint64_t>();
    return *acc;
}

alu xio::Radical()
{
    *acc = std::pow(lhs->acc->number<double>(), 1 / rhs->acc->number<double>());
    return *acc;
}

alu xio::Exponent()
{
    *acc = std::pow(lhs->acc->number<double>(), rhs->acc->number<double>());
    return *acc;
}

alu xio::RightShift()
{
    *acc = lhs->acc->number<uint64_t>() >> rhs->acc->number<uint64_t>();
    return *acc;
}


alu xio::Decr()
{
    *acc = t0->is_prefix() ? --(*rhs->acc) : (*lhs->acc)--;
    return *acc;
}


alu xio::Incr()
{
    *acc = t0->is_prefix() ? ++(*rhs->acc) : (*lhs->acc)++;
    return *acc;
}


alu xio::AssignAdd()
{
    *acc = *(lhs->acc) += *(rhs->acc);
    return  *acc;
}


alu xio::AssignSub()
{
    *acc = *(lhs->acc) -= *(rhs->acc);
    return  *acc;
}


alu xio::AssignMul()
{
    *acc = *(lhs->acc) *= *(rhs->acc);
    return *acc;
}


alu xio::AssignDiv()
{
    *acc = *(lhs->acc) /= *(rhs->acc);
    return *acc;
}
alu xio::AssignMod()
{
    *acc = *(lhs->acc) %= *(rhs->acc);
    return *acc;

}
alu xio::AssignAnd()
{
    *acc = *(lhs->acc) &= *(rhs->acc);
    return *acc;

}
alu xio::AssignOr()
{
    *acc = *(lhs->acc) |= *(rhs->acc);
    return *acc;

}
alu xio::AssignXor()
{
    *acc = *(lhs->acc) ^= *(rhs->acc);
    return *acc;

}
alu xio::AssignX1()
{
    *acc = ~(*rhs->acc);
    return *acc;
}
alu xio::AssignLShift()
{
    *acc = *lhs->acc <<= *rhs->acc;
    return *acc;
}


alu xio::AssignRShift()
{
    *acc = *lhs->acc >>= *rhs->acc;
    return *acc;
}


alu xio::Leq()
{
    *acc = *lhs->acc <= *rhs->acc;
    return *acc;
}


alu xio::Geq()
{
    *acc = *lhs->acc >= *rhs->acc;
    return *acc;
}


alu xio::Eq()
{
    *acc = (*lhs->acc) == (*rhs->acc);
    return *acc;
}


alu xio::Neq()
{
    *acc = *lhs->acc != *rhs->acc;
    return *acc;
}

alu xio::Add()
{
    book::rem::push_debug(HERE) << color::Yellow << lhs->value()() << " " << color::CornflowerBlue << attribute() << " " << color::Yellow << rhs->value()() << ":"<< book::rem::commit;
    *acc = *lhs->acc + *rhs->acc;
    book::rem::out() << color::CornflowerBlue << " = " << color::Lime << (*acc)()<< book::rem::commit;
    return *acc;
}
alu xio::Sub()
{
    // hack... en attendant :
    if (t0->s & type::Sign)
        return Negative();
    book::rem::push_debug(HERE) << color::Lime
        << color::Yellow << lhs->value()() << " " << color::CornflowerBlue << attribute() << " " << color::Yellow << rhs->value()() << ":"<< book::rem::commit;
    *acc = *lhs->acc - *rhs->acc;
    book::rem::out() << color::CornflowerBlue << " = " << color::Lime << (*acc)()<< book::rem::commit;

    return *acc;
}
alu xio::Mul()
{
    book::rem::push_debug()
        << color::Yellow << lhs->attribute()
        << color::CornflowerBlue << attribute()
        << color::Yellow << rhs->attribute()
      << color::White<< book::rem::commit;
    *acc = *lhs->acc * *rhs->acc;
    book::rem::out(HEREF) << color::CornflowerBlue << " => " << color::Lime << (*acc)() << book::rem::commit;
    return *acc;
}
alu xio::Modulo()
{
    *acc = *lhs->acc % *rhs->acc;
    return *acc;
}
alu xio::LessThan()
{
    *acc = *lhs->acc < *rhs->acc;
    return *acc;
}
alu xio::GreaterThan()
{
    *acc = *lhs->acc > *rhs->acc;
    return *acc;
}
alu xio::Assign()
{
    book::rem::push_debug(HERE) << color::Lime
        << color::Aquamarine3 << lhs->attribute() << color::Reset << " "
        << " " << color::CornflowerBlue << attribute() << " "
        << color::Yellow
        << rhs->value()() << ":"<< book::rem::commit;

    *lhs->acc = *rhs->acc;
    *acc = *rhs->acc;

    book::rem::out(HEREF) << color::CornflowerBlue << " => " << color::Lime << (*acc)()<< book::rem::commit;
    return *acc;
}


alu xio::BinAnd()
{
    *acc = *lhs->acc & *rhs->acc;
    return *acc;
}


alu xio::BinOr()
{
    *acc = *lhs->acc | *rhs->acc;
    return *acc;
}


alu xio::BitXor()
{
    *acc = *lhs->acc ^ *rhs->acc;
    return *acc;
}

alu xio::X1()
{
    *acc = ~(*lhs->acc);
    return *acc;
}

alu xio::X2()
{
    *acc = ~(*lhs->acc) + alu(1);
    return *acc;
}


alu xio::BitNot()
{
    *acc = !rhs->acc->number<double>();
    return *acc;
}


alu xio::BoolAnd()
{
    *acc = *lhs->acc && *rhs->acc;
    return *acc;
}
alu xio::BoolOr()
{
    *acc = *lhs->acc || *rhs->acc;
    return *acc;

}

alu xio::Division()
{
    book::rem::push_debug(HERE) << color::Lime
        << color::Yellow << lhs->value()()
        << " " << color::CornflowerBlue << attribute() << " "
        << color::Yellow
        << rhs->value()() << ":"<< book::rem::commit;

    *acc = *lhs->acc / *rhs->acc;
    book::rem::out(HEREF) << color::CornflowerBlue << " => " << color::Lime << (*acc)()<< book::rem::commit;
    return *acc;
    //book::rem::out() << color::CornflowerBlue << " = " << color::Lime << (*acc)();
}


alu xio::Factorial()
{
    //*acc = acc->factorial(*lhs->acc);
    book::rem::push_debug(HERE) << color::Lime << lhs->attribute()
                                << color::Yellow << " " << (*lhs->acc)() << " "
                                << color::CornflowerBlue << attribute() << color::White << ":" << book::rem::commit;

    book::rem::out() << xio::trace_connect_postfix_operands(this) << book::rem::commit;

    *acc = lhs->acc->factorial();

    book::rem::out() << color::CornflowerBlue << " => " << color::Yellow << (*acc)()<< book::rem::commit;
    return *acc;
}



alu xio::Positive()
{
    if (rhs->acc->number<double>() < 0)
        *rhs->acc *= -1;

    *acc = *rhs->acc;
    return *acc;
}


alu xio::Negative()
{
    book::rem::push_debug(HERE) << color::Lime
        << color::CornflowerBlue << attribute()
        << color::Yellow << rhs->attribute()
        << color::White << "=" << book::rem::commit;

    if (rhs->acc->number<double>() > 0) // ==> a = -1;  -a = ?
        *rhs->acc *= -1;

    *acc = *rhs->acc;
    book::rem::out() << color::CornflowerBlue << " = " << color::Lime << (*acc)()<< book::rem::commit;
    return *acc;
}


alu xio::KPi()
{
    *acc = alu(_PI_);
    return *acc;
}


alu xio::KCos()
{
    *acc = std::cos(deg2rad(rhs));
    return *acc;
}


alu xio::KAcos()
{
    *acc = std::acos(deg2rad(rhs));
    return *acc;
}
alu xio::KTan()
{
    *acc = std::tan(deg2rad(rhs));
    return *acc;
}


alu xio::KAtan()
{
    *acc = std::atan(deg2rad(rhs));

    return *acc;
}

alu xio::KSin()
{
    *acc = std::sin(deg2rad(rhs));
    return *acc;
}

alu xio::KAsin()
{
    *acc = std::asin(deg2rad(rhs));
    return *acc;
}

alu xio::KNumber()
{

    return *acc;
}

alu xio::KU8()
{
    *acc = rhs->acc->number<uint64_t>() & 0xFF;
    return alu();
}

alu xio::KU16()
{
    *acc = rhs->acc->number<uint64_t>() & 0xFFFF;
    return *acc;
}

alu xio::KU32()
{
    *acc = rhs->acc->number<uint64_t>() & 0xFFFFFFFF;
    return *acc;
}

alu xio::KU64()
{
    *acc = (uint64_t)(rhs->acc->number<uint64_t>());// & 0xFFFFFFFFFFFFFFFF);
    return *acc;
}

alu xio::KI8()
{
    *acc = (int8_t)(rhs->acc->number<int64_t>() & 0xFF);
    return *acc;
}

alu xio::KI16()
{
    book::rem::push_debug() << rhs->t0->text();
    *acc = (int16_t)(rhs->acc->number<uint64_t>() & 0xFFFF);
    return *acc;
}

alu xio::KI32()
{
    *acc = (int32_t)(rhs->acc->number<uint64_t>() & 0xFFFFFFFF);
    return *acc;
}

alu xio::KI64()
{
    //*acc = (int64_t)(rhs->acc->number<int64_t>() & 0xFFFFFFFFFFFFFFFF);
    *acc = rhs->acc->number<int64_t>();
    return *acc;
}

alu xio::KReal()
{
    *acc = rhs->acc->number<double>();
    return *acc;
}

alu xio::KString()
{
    *acc = std::string(*rhs->acc);
    return *acc;
}


std::string xio::type_name()
{
    return type::name(t0->t);
}


auto xio::query(xio* in_lhs, xio* in_rhs)
{
    for (auto [lr, fn] : xio::move_tbl)
    {
        auto [l, r] = lr;
        if ((in_lhs->t0->t & l) && (in_rhs->t0->t & r)) return fn;
    }
    return static_cast<xio * (xio::*)(xio*)> (nullptr);
}



//---------------------------Arithmetic Expression Tree Building------------------------------------------------
xio::move_table_t xio::move_tbl =
{
    // to_left:
        {{type::OpenPair, type::Leaf | type::Prefix | type::Binary | type::Id | type::Number | type::Const}, &xio::to_left},
        {{type::Binary | type::Prefix | type::Sign | type::Assign,  type::OpenPair | type::Leaf | type::Prefix | type::Number | type::Id}, &xio::to_right},
        // rpar_leaf:
        //    {{type::close_pair, type::leaf}, &xio::_rpar_leaf},
        //// _assign:
        //    {{type::leaf|type::number|type::id, type::assign}, &xio::_assign},
        // _close_par:
        {{type::ClosePair | type::Prefix | type::Postfix | type::Id | type::Number | type::Const, type::ClosePair }, &xio::_close_par},
        // close_par_seq:
        {{type::ClosePair, type::Binary | type::Postfix | type::ClosePair}, &xio::collapse_par_pair},
        // _binary:
        {{type::Number | type::Binary | type::Prefix | type::Postfix | type::Id, type::Assign | type::Binary}, &xio::_binary},
        // opo_right:
        {{type::Number | type::Id, type::Postfix}, &xio::op_to_right}
};

xio::input_table_t xio::input_tbl =
{
    {{type::Id     | type::Assign,    type::Assign}, "&xio::_assign"},
    {{type::Number | type::Const | type::Postfix | type::ClosePair | type::Id,    type::Binary}, "&xio::_binary"},
    {{type::Number | type::Id    | type::ClosePair | type::Const,    type::Postfix},  "&xio::_postfix"},
    {{type::Binary,  type::Prefix},     "&xio::_prefix"},
    {{type::Prefix | type::Binary | type::OpenPair | type::Assign,    type::Leaf | type::Number | type::OpenPair | type::Id | type::Prefix},  "&xio::_leaf"},
    {{type::Prefix | type::Binary | type::OpenPair,    type::OpenPair},    "&xio::_open_pair"},
    {{type::Postfix| type::Const | type::Number | type::Id | type::OpenPair | type::ClosePair,    type::ClosePair}, "&xio::_close_pair"}
};


void xio::syntax_error(xio* e)
{
    throw book::rem::push_syntax() << "at " << e->t0->location_text() << book::rem::endl << e->t0->mark() << book::rem::endl << book::rem::commit;
}

xio *xio::warning(xio *)
{
    return nullptr;
}
void xio::make_error(book::rem::code ErrCode, xio* source_node, xio* input_node)
{
    throw book::rem::push_error() << source_node->attribute()
        << " token input error: "
        << ErrCode
        << input_node->attribute()
        << book::rem::endl
        << input_node->t0->mark()
        << book::rem::endl << book::rem::commit;
}

void xio::make_error(book::rem::code ErrCode, xio* source_node, token_data* inputoken)
{
    throw book::rem::push_error() << source_node->attribute()
        << " token input error: "
        << ErrCode
        << inputoken->text()
        << book::rem::endl
        << inputoken->mark()
        << book::rem::endl << book::rem::commit;
}

void xio::header(xio* input_node, book::source_location&& Loc)
{
    book::rem::push_debug(std::move(Loc)) << color::Yellow << t0->text() << color::White << "<-" <<
      color::Yellow << input_node->t0->text() <<
      book::rem::endl << input_node->t0->mark() << book::rem::endl << book::rem::commit;
}



/*!
 * \brief xio::input
 *
 * \note Call to this function is stricly, restricted to the Arithmetic Expression Tree/AST build context.
 * \param parent_bloc
 * \param token
 * \return new Insertion node ( or vertex? ).
 *
 * ex.: if x-1 + 12 ==  0 return 12;
 * * ex.: if a+1+b+c d = 12;
 */
xio* xio::input(xio* parent_bloc, token_data* token, xio::maker mk)
{
    for (auto [lr, fn] : xio::input_tbl)
    {
        auto [l, r] = lr;
        if ((t0->t & l) && (token->t & r))
        {
            book::rem::push_debug(HERE) << color::Yellow << t0->text() << " <- " << color::Yellow << token->text() << color::Reset << " Input token validated... " << book::rem::commit;
            ///@todo Check id tokens for function_call and other id-constructs before calling xio::input(...).

            xio* a;
            if(mk)
                a = mk(token);

            else
                a  = new xio(parent_bloc,token);

            if(!a) return nullptr;
            auto fn = xio::query(this, a);
            if (!fn)
            {
                if(a)
                {
                    a->detach();
                    delete a;
                }
                book::rem::push_syntax() << " invalid relational operands at " << token->location_text() << " - unexpected token:" << book::rem::endl << token->mark() << book::rem::endl << book::rem::commit;
                return nullptr;
            }
            book::rem::push_debug() << t0->text() << "::input(" << token->text() << "):" << book::rem::endl << token->mark()<< book::rem::endl << book::rem::commit;

            return (this->*fn)(a);
        }
    }

    book::rem::push_info(HERE) << color::White << "'" << color::Yellow << t0->text() << color::White << "'" << color::Reset
                               << "::input(" << color::Yellow <<  token->text() << color::Reset << ") => invalid relational operands at "
                               << token->location_text() << " - unexpected token."
        << book::rem::endl << token->mark()<< book::rem::endl << book::rem::commit;
    book::rem::out() << t0->details() << " || " << token->details() << book::rem::endl << "Returning nullptr" << book::rem::commit;

    return nullptr;
}

xio* xio::_binary(xio* a)
{
    header(a, HERE);

    if (t0->is_leaf())
    {
        if (a->t0->m == mnemonic::OpenPar)
            syntax_error(a);
    }
    
    if (t0->m == mnemonic::OpenPar)
        return to_left(a);

    if (t0->is_binary())
    {
        //if (!rhs) syntax_error(a);

        if (a->t0->d < t0->d)
            return to_right(a);
        if (op)
        {
            auto fn = query(this, a);
            if (!fn)
                xio::make_error(book::rem::unexpected, this, a);

            return (op->*fn)(a);
        }

        a->to_left(this); // returns "this" but from here we must return a as the new insertion node into the tree...
        // The only case when it is required to return lhs ( left-hand-side node) is on openning par or left-pair(operator) that isolates the
        // inner expression between open-close pair.
        return a;
    }

    if (op) {
        auto fn = query(this, a);
        if (!fn)
            xio::make_error(book::rem::unexpected, this, a);
        return (op->*fn)(a);

    }
    a->to_left(this);
    return a;
}

xio* xio::_close_pair(xio* a)
{
    header(a, HERE);
    xio* x = xio::pop_par();
    if (!x)
    {
        book::rem::push_error() << "Unmatched left paranthese:" << book::rem::endl << a->t0->mark()<< book::rem::endl << book::rem::commit;
        return nullptr;
    }
    a->op = x->op;
    a->lhs = x->lhs;
    if (a->lhs)
    {
        if (a->lhs->op)
            a->lhs->op = a;
    }
    if (a->op)
        a->op->rhs = a; // oops!!

    book::rem::push_debug()
        << "new input vertex:["
        << color::Yellow
        << a->t0->text()
        << color::Reset
        << "]" << book::rem::endl
        << a->t0->mark()<< book::rem::endl << book::rem::commit;

    return a;
}

xio* xio::_close_par(xio* a)
{
    header(a, HERE);

    xio* x = xio::pop_par();
    if (!x)
    {
        book::rem::push_error() << "Unmatched left paranthese." << a->t0->mark()<< book::rem::endl << book::rem::commit;
        return nullptr;
    }
    a->op = x->op;
    a->lhs = x->lhs;
    if (a->lhs)
    {
        if (a->lhs->op)
            a->lhs->op = a;
    }
    if (a->op)
        a->op->rhs = a; // oops!!

    book::rem::out()
        << "new input vertex:["
        << color::Yellow
        << a->t0->text()
        << color::Reset
        << "]" << book::rem::endl
        << a->t0->mark()<< book::rem::endl << book::rem::commit;

    return a;
}




/*!
 * \brief xio::collapse_par_pair
 * \param a
 * \return
 *
    \code
     [/]                  [/] <- !
    /  \                   \
   42   ) <-- !             -
       /                     \
      -                       4
       \
        4
    \endcode
 */
xio* xio::collapse_par_pair(xio* a)
{
    header(a, HERE);
    //xio* v = lhs;

    // Collapse lhs

    lhs->op = op;
    if (op)
    {
        op->rhs = lhs;
        if(a->t0->d < op->t0->d)
        {
            return op->to_right(a);
        }
    }

    // discard();

    if (lhs->op) {
        book::rem::out()
            << color::Yellow << lhs->op->attribute() << color::CornflowerBlue
            << " <-- "
            << color::Yellow << a->attribute()<< book::rem::commit;

        auto p_fn = query(lhs->op, a);
        if (!p_fn)
            xio::syntax_error(a);

        return (lhs->op->*p_fn)(a);
    }

    lhs->op = a;
    a->lhs = lhs;
    return a;
}

xio* xio::op_to_right(xio* a)
{
    header(a, HERE);
    if (!op) {
        a->lhs = this;
        op = a;
        return a;
    }
    return op->to_right(a);
}


int xio::push_par(xio* a)
{
    xio::pars.push(a);
    return static_cast<int>(xio::pars.size());
}

xio* xio::pop_par()
{
    if (xio::pars.empty()) return nullptr;
    xio* x = xio::pars.top();
    xio::pars.pop(); // Tabarnak!
    return x;
}


xio* xio::begin(xio* parent_, token_data* token, xio::maker xmk)
{
    book::rem::push_debug(HEREF) << color::Yellow << token->text() << book::rem::endl <<  book::rem::commit;

    if (!token->_flags.V)
        return nullptr;

    xio* a{nullptr};
    if(xmk)
        a = xmk(token);
    else
        a = new xio(parent_, token);

    if(!a)
        return nullptr;
    
    if (a->t0->m == mnemonic::OpenPar) push_par(a);
    return a;
}


xio* xio::tree_close()
{
    header(this, HERE);
    
    if (t0->m == mnemonic::OpenPar)
    {
        book::rem::push_error() << " unexpected end of file." << book::rem::commit;
        return nullptr;
    }

    if (!xio::pars.empty())
    {
        xio* x = xio::pars.top();
        xio::pars.pop();
        book::rem::push_error() << " umatched closing parenthese from:" << book::rem::endl << x->t0->mark()<< book::rem::endl << book::rem::commit;
        return nullptr;
    }
    
    
    if (t0->m == mnemonic::ClosePar) {
        book::rem::push_debug() << "Closing the tree on close parenthese:"<< book::rem::commit;
        if (lhs)
        {
            xio* x = lhs;
            book::rem::push_debug() << "left hand side operand: " << lhs->t0->details() << ":" << book::rem::endl << lhs->t0->mark()<< book::rem::endl << book::rem::commit;

            lhs->op = op;

            if (op)
            {
                op->rhs = lhs;
                xio::trace_connect_binary_operands(op);
            }

            // discard();
            return x->tree_root(false);
        }
    }
    return tree_root(false);
}

xio* xio::tree_root(bool skip_syntax)
{
    book::rem::push_debug(HERE) << "query tree ins from xio node:" << book::rem::endl << t0->mark()<< book::rem::endl << book::rem::commit;
    xio* x = this;
    xio* p = x;
    do {
        x = p;
        if (!skip_syntax)
        {
            switch (x->t0->t) {
            case type::Assign:
            case type::Binary:
                if (!x->lhs)
                {
                    book::rem::push_error() << "Syntax error: binary operator has no left operand." << book::rem::endl << x->t0->mark()<< book::rem::endl << book::rem::commit;
                    return nullptr;
                }
                if (!x->rhs)
                {
                    book::rem::push_error() << "Syntax error: binary operator has no right operand." << book::rem::endl << x->t0->mark()<< book::rem::endl << book::rem::commit;
                    return nullptr;
                }
            case type::Prefix:
                if (!x->rhs)
                {
                    book::rem::push_error() << "Syntax error: prefix unary operator has no (right) operand." << book::rem::endl << x->t0->mark()<< book::rem::endl << book::rem::commit;
                    return nullptr;
                }
                break;
            case type::Postfix:
                if (!x->lhs)
                {
                    book::rem::push_error() << "Syntax error: postfix unary operator has no (left) operand." << book::rem::endl << x->t0->mark()<< book::rem::endl << book::rem::commit;
                    return nullptr;
                }
                break;
            }
        }
        p = p->op;
    } while (p);

    book::rem::push_debug(HERE) << "query tree ins returning node: " << x->t0->details() << book::rem::endl << x->t0->mark()<< book::rem::endl << book::rem::commit;
    return  x;
}

xio* xio::to_right(xio* in_rhs)
{
    header(in_rhs, HERE);

    // Temporary hack....
    if (in_rhs->t0->m == mnemonic::OpenPar)
        xio::push_par(in_rhs);

    if (rhs) {
        /*
           this
              \
               x <- nextoken
              /
            rhs
        */
        book::rem::push_debug() << t0->text() << " -> " << rhs->t0->text()
            << color::Lime << "tree_set_right "
            << color::White << " <- "
            << color::Yellow << in_rhs->t0->text()<< book::rem::commit;
        rhs->op = in_rhs;
        in_rhs->lhs = rhs;
    }
    rhs = in_rhs;
    in_rhs->op = this;
    if (t0->is_binary())
    {
        book::rem::push_debug(HERE) << xio::trace_connect_binary_operands(this)<< book::rem::commit;
    }
    return in_rhs;
}

xio* xio::to_left(xio* in_lhs)
{
    header(in_lhs, HERE);
    /*
    (; [; {   // Appliqué sur aucun autre type de token car l'appel de tree_set_left ne se fait qu'� partir de tree_input qui r�soud l'associativit�.
        /
            x < -nextoken
            /
            lhs
    */

    if (lhs) {
        // here we are supposed to be the openning par/index/bracket. so the interior will become right hand side of the parent op of this.
        lhs->op = in_lhs;
        in_lhs->lhs = lhs;
    }
    in_lhs->op = this;

    lhs = in_lhs;
    return in_lhs;
}


#pragma region TRIGRAPH

void xio::dot_tree_start(stracc& a_out, const stracc& Title)
{
    a_out << "digraph arithmetic_expressionree {\n";
    a_out << "ratio=compress; ranksep=.55; size = \"6.5,6.5\";\n";
    //a_out << "    node [fontname=\"Source Code Pro\", fontsize=12];\n";
    a_out << "    node [fontname=\"JetBrainsMono NF\", fontsize=12];\n";
    a_out << "    label=\"Arithmetic Expr Tree:[" << Title() << "]\"; fontsize = 10;\n";
}

void xio::dot_node(xio* A, stracc& a_out)
{
    //static int nullcount = 0;
    if (!A) return;
    if ((!A->lhs) && (!A->rhs)) return;

    if (A->lhs) {
        a_out << "    xioNode" << A << " -> xioNode" << A->lhs << ";\n";
        xio::dot_node(A->lhs, a_out);
    }
    //     else
    //         t_node::trace_null_node(A, nullcount++, a_stream);

    if (A->rhs) {
        a_out << "    xioNode" << A << " -> xioNode" << A->rhs << ";\n";
        xio::dot_node(A->rhs, a_out);
    }
    //     else
    //         trace_null_node(A, nullcount++, a_stream);
}

void xio::dot_null_node(xio*, int, stracc&)
{

}

void xio::dot_tree(xio* a_root, stracc& a_out)
{
    a_root->dot_attr(a_out);
    xio::dot_node(a_root, a_out);
}

void xio::dot_tree_close(stracc& a_out)
{
    a_out << "}\n";
}

void xio::dot_attr(stracc& a_out)
{
    stracc attr;
    attr << t0->text();
    stracc Shape;
    if (t0->t & type::Text)
        Shape << "none";
    else
        if (t0->t & type::Assign)
            Shape << "none";
        else
            Shape << "none";

    a_out << "    xioNode" << this << " [shape=%s, label=\"%s\"]\n";
    a_out << Shape() << attr();

    if (lhs)
        lhs->dot_attr(a_out);
    if (rhs)
        rhs->dot_attr(a_out);
}




std::string xio::trace_connect_binary_operands(xio* x)
{
    // assume this binary operator already has its lhs rhs operands !!
    //stracc str;
    auto lw = x->lhs->attribute().length();
    auto rw = x->rhs->attribute().length();
    auto ow = x->attribute().length();
    auto w = lw + rw + 3; // total width
    w -= lw % 2 == 0;


    auto m_lhs = lw - (lw > 1 ? 1 : 0);

    point oper_xy = point(static_cast<int>(m_lhs) + 1, 0);
    oper_xy.x -= ow % 2 == 0 ? 1 : 0;

    winbuffer area;
    area.set_geometry(static_cast<int>(w), 3); // pour l'instant c'est hardcodé.
    area.gotoxy(oper_xy.x, 0);
    area << x->attribute();
    area << point{ static_cast<int>(m_lhs),1 } << "/ \\";

    area.gotoxy(0, 2);
    area << x->lhs->attribute();

    area.gotoxy(static_cast<int>(m_lhs) + 2 + (rw <= 1 ? 1 : 0), 2);
    area << x->rhs->attribute();
    return area;
}


std::string xio::trace_connect_postfix_operands(xio* x)
{
    // assume this binary operator already has its lhs rhs operands !!
    //stracc str;
    auto lw = x->lhs->attribute().length();
    //auto rw = x->rhs->attribute().length();
    auto ow = x->attribute().length();
    auto w = lw + 3; // total width
    w -= lw % 2 == 0;


    auto m_lhs = lw - (lw > 1 ? 1 : 0);

    point oper_xy = point(static_cast<int>(m_lhs) + 1, 0);
    oper_xy.x -= ow % 2 == 0 ? 1 : 0;

    winbuffer area;
    area.set_geometry(static_cast<int>(w), 3); // pour l'instant c'est hardcodé.
    area.gotoxy(oper_xy.x, 0);
    area << x->attribute();
    area << point{ static_cast<int>(m_lhs),1 } << "/ \\";

    area.gotoxy(0, 2);
    area << x->lhs->attribute();

    //area.gotoxy(static_cast<int>(m_lhs) + 2;
    //area << x->rhs->attribute();
    return area;
}
#pragma endregion TRIGRAPH





}


