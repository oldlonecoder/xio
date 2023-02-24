#include <xio/xiobject.h>
#include <tlux/geometry.h>



namespace xio
{




xiobject* xiobject::_global_root_bloc = nullptr;

std::stack<xiobject*> xiobject::pars;

//xiobject* xiobject::get_global_root() { return xiobject::{type::binary,     type::open_pair},  &xiobject::set_right_global_root_bloc; }


#define deg2rad(_xiobject) (((_xiobject)->acc->v.d*3.1415) / 180)
constexpr auto _PI_ = 3.14159265358979323846;
// M_PI ?


// void xiobject::// discard()
// {
//     for (auto x : xiobject::xiobject_gc)
//         if (this == x) return;

//     xiobject::xiobject_gc.push_back(this);
//     lhs = rhs = op = nullptr; // Make sure we do not accidently reuse this;
// }



    // Primary arithmetic supported operators table:
xiobject::xio_opfn_table_t   xiobject::xio_operators_table{
    {mnemonic::LeftShift,           &xiobject::LeftShift},
    {mnemonic::Radical,             &xiobject::Radical},
    {mnemonic::Exponent,            &xiobject::Exponent},
    {mnemonic::RightShift,          &xiobject::RightShift},
    {mnemonic::Decr,                &xiobject::Decr},
    {mnemonic::Incr,                &xiobject::Incr},
    {mnemonic::AssignAdd,           &xiobject::AssignAdd},
    {mnemonic::AssignSub,           &xiobject::AssignSub},
    {mnemonic::AssignMul,           &xiobject::AssignMul},
    {mnemonic::AssignDiv,           &xiobject::AssignDiv},
    {mnemonic::AssignMod,           &xiobject::AssignMod},
    {mnemonic::AssignAnd,           &xiobject::AssignAnd},
    {mnemonic::AssignOr,            &xiobject::AssignOr},
    {mnemonic::AssignXor,           &xiobject::AssignXor},
    {mnemonic::AssignC1,            &xiobject::AssignX1},
    {mnemonic::AssignLeftShift,     &xiobject::AssignLShift},
    {mnemonic::AssignRightShift,    &xiobject::AssignRShift},
    {mnemonic::LessEq,              &xiobject::Leq},
    {mnemonic::GreaterEq,           &xiobject::Geq},
    {mnemonic::Equal,               &xiobject::Eq},
    {mnemonic::NotEqual,            &xiobject::Neq},
    {mnemonic::Add,                 &xiobject::Add},
    {mnemonic::Sub,                 &xiobject::Sub},
    {mnemonic::Mul,                 &xiobject::Mul},
    {mnemonic::Modulo,              &xiobject::Modulo},
    {mnemonic::LessThan,            &xiobject::LessThan},
    {mnemonic::GreaterThan,         &xiobject::GreaterThan},
    {mnemonic::Assign,              &xiobject::Assign},
    {mnemonic::BinaryAnd,           &xiobject::BinAnd},
    {mnemonic::BinaryOr,            &xiobject::BinOr},
    {mnemonic::BinaryXor,           &xiobject::BitXor},
    {mnemonic::C1,                  &xiobject::X1},
    {mnemonic::C2,                  &xiobject::X2},
    {mnemonic::BinaryNot,           &xiobject::BitNot},
    {mnemonic::LogicalAnd,          &xiobject::BoolAnd},
    {mnemonic::LogicalOr,           &xiobject::BoolOr},
    {mnemonic::Div,                 &xiobject::Division},
    {mnemonic::Factorial,           &xiobject::Factorial},
    {mnemonic::Positive,            &xiobject::Positive},
    {mnemonic::Negative,            &xiobject::Negative},
    {mnemonic::Pi,                  &xiobject::KPi},
    {mnemonic::Cosinus,             &xiobject::KCos},
    {mnemonic::ArcCosinus,          &xiobject::KAcos},
    {mnemonic::Tangent,             &xiobject::KTan},
    {mnemonic::ArcTangent,          &xiobject::KAtan},
    {mnemonic::Sinus,               &xiobject::KSin},
    {mnemonic::ArcSinus,            &xiobject::KAsin},
    {mnemonic::Number,              &xiobject::KNumber},
    {mnemonic::U8,                  &xiobject::KU8   },
    {mnemonic::U16,                 &xiobject::KU16  },
    {mnemonic::U32,                 &xiobject::KU32  },
    {mnemonic::U64,                 &xiobject::KU64  },
    {mnemonic::I8,                  &xiobject::KI8   },
    {mnemonic::I16,                 &xiobject::KI16  },
    {mnemonic::I32,                 &xiobject::KI32  },
    {mnemonic::I64,                 &xiobject::KI64  },
    {mnemonic::Float,               &xiobject::KReal },
    {mnemonic::String,              &xiobject::KString}
};


// xiobject::xiobject()
// {
//     //acc = std::make_shared<alu>(0.0);
// }



xiobject::xiobject()
{
    acc = new alu(0.0f); // Shit!
}

xiobject::xiobject(xiobject* parent_bloc)
{
    _parent = parent_bloc;

    //token_info = new token_data;
    acc = new alu;
}


xiobject::~xiobject()
{
    if (!mem.xshared) delete acc;
}

xiobject::xiobject(xiobject* parent_bloc, token_data* aoken, alu* a_alu)
{
    t0 = aoken;
    _parent = parent_bloc;
    if(_parent)
        _parent->append_child(this);

    diagnostic::debug() << "xiobject::xiobject => token details:[" << (t0 ? t0->details() : "nullptr") << "] \n";

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
        *acc = aoken->text();
        return;
    case type::Leaf:
    {
        ///@todo Attention enlever le court-circuit du type-size!!!
        if (t0->s & type::Number)
        {
            double d;
            (stracc(aoken->text())) >> d;
            *acc = d;
            diagnostic::debug() << " acc: " << color::Yellow << (*acc)();
        }
    }
    return;
    case type::Hex:
    {
        uint64_t d;
        stracc(aoken->text()).hexadecimal(d);
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

    auto i = xiobject::xio_operators_table.find(aoken->c);
    xio_fn = i != xiobject::xio_operators_table.end() ? i->second : nullptr;
    diagnostic::debug(sfnl) << " acc: '" << color::Yellow << (*acc)() << color::White << "'";
}

xiobject::list::iterator xiobject::query(xiobject* c)
{
    xiobject::list::iterator i = _children.begin();

    for(; i != _children.end(); i++)
    {
        if(*i == c)
            break;
    }
    return i;
}


// ------------------ ARITHMETIC OPERATORS IMPLEMENTATION ---------------------------------------------

alu xiobject::jsr()
{
    //...

    if (lhs) *acc = lhs->jsr(); // Always catch the lhs value so we return that one if there is no rhs operand.
    if (rhs) *acc = rhs->jsr(); // Always catch the rhs value because it is the value to be returned after being applied to the lhs (if applicable).

    if (xio_fn)
        return (this->*xio_fn)();// All operators assign acc.
    else {
        if (t0->is_operator()) {
            diagnostic::warning() << "operator xiobject [" << color::Yellow << t0->text() << color::Reset << "] has no implementation (yet?).:\n" << t0->mark();
        }
    }
    t0->s |= acc->T;///< todo Oops! Check wtf is that!
    return *acc;
}

code::T xiobject::append_child(xiobject *c)
{
    auto e = query(c);
    if(e != _children.end())
        _children.push_back(c);
    else
        return code::exist;

    return code::accepted;
}


code::T xiobject::detach(xiobject * c)
{
    auto i = query(c);
    if(i != _children.end())
        _children.erase(i);
    else
        return code::notexist;

    return code::accepted;
}

code::T xiobject::detach()
{
    xiobject* p = parent<xiobject>();
    if(p)
    {
        p->detach(this);
    }
    return code::accepted;
}



alu xiobject::LeftShift()

{
    *acc = lhs->acc->number<uint64_t>() << rhs->acc->number<uint64_t>();
    return *acc;
}

alu xiobject::Radical()
{
    *acc = std::pow(lhs->acc->number<double>(), 1 / rhs->acc->number<double>());
    return *acc;
}

alu xiobject::Exponent()
{
    *acc = std::pow(lhs->acc->number<double>(), rhs->acc->number<double>());
    return *acc;
}

alu xiobject::RightShift()
{
    *acc = lhs->acc->number<uint64_t>() >> rhs->acc->number<uint64_t>();
    return *acc;
}


alu xiobject::Decr()
{
    *acc = t0->is_prefix() ? --(*rhs->acc) : (*lhs->acc)--;
    return *acc;
}


alu xiobject::Incr()
{
    *acc = t0->is_prefix() ? ++(*rhs->acc) : (*lhs->acc)++;
    return *acc;
}


alu xiobject::AssignAdd()
{
    *acc = *(lhs->acc) += *(rhs->acc);
    return  *acc;
}


alu xiobject::AssignSub()
{
    *acc = *(lhs->acc) -= *(rhs->acc);
    return  *acc;
}


alu xiobject::AssignMul()
{
    *acc = *(lhs->acc) *= *(rhs->acc);
    return *acc;
}


alu xiobject::AssignDiv()
{
    *acc = *(lhs->acc) /= *(rhs->acc);
    return *acc;
}
alu xiobject::AssignMod()
{
    *acc = *(lhs->acc) %= *(rhs->acc);
    return *acc;

}
alu xiobject::AssignAnd()
{
    *acc = *(lhs->acc) &= *(rhs->acc);
    return *acc;

}
alu xiobject::AssignOr()
{
    *acc = *(lhs->acc) |= *(rhs->acc);
    return *acc;

}
alu xiobject::AssignXor()
{
    *acc = *(lhs->acc) ^= *(rhs->acc);
    return *acc;

}
alu xiobject::AssignX1()
{
    *acc = ~(*rhs->acc);
    return *acc;
}
alu xiobject::AssignLShift()
{
    *acc = *lhs->acc <<= *rhs->acc;
    return *acc;
}


alu xiobject::AssignRShift()
{
    *acc = *lhs->acc >>= *rhs->acc;
    return *acc;
}


alu xiobject::Leq()
{
    *acc = *lhs->acc <= *rhs->acc;
    return *acc;
}


alu xiobject::Geq()
{
    *acc = *lhs->acc >= *rhs->acc;
    return *acc;
}


alu xiobject::Eq()
{
    *acc = (*lhs->acc) == (*rhs->acc);
    return *acc;
}


alu xiobject::Neq()
{
    *acc = *lhs->acc != *rhs->acc;
    return *acc;
}

alu xiobject::Add()
{
    diagnostic::debug(sfnl) << color::Yellow << lhs->acu()() << " " << color::CornflowerBlue << attribute() << " " << color::Yellow << rhs->acu()() << ":";
    *acc = *lhs->acc + *rhs->acc;
    diagnostic::output() << color::CornflowerBlue << " = " << color::Lime << (*acc)();
    return *acc;
}
alu xiobject::Sub()
{
    // hack... en attendant :
    if (t0->s & type::Sign)
        return Negative();
    diagnostic::debug(sfnl) << color::Lime
        << color::Yellow << lhs->acu()() << " " << color::CornflowerBlue << attribute() << " " << color::Yellow << rhs->acu()() << ":";
    *acc = *lhs->acc - *rhs->acc;
    diagnostic::output() << color::CornflowerBlue << " = " << color::Lime << (*acc)();

    return *acc;
}
alu xiobject::Mul()
{
    diagnostic::debug()
        << color::Yellow << lhs->attribute()
        << color::CornflowerBlue << attribute()
        << color::Yellow << rhs->attribute()
        << color::White << "=";
    *acc = *lhs->acc * *rhs->acc;
    diagnostic::output() << (*acc)();
    return *acc;
}
alu xiobject::Modulo()
{
    *acc = *lhs->acc % *rhs->acc;
    return *acc;
}
alu xiobject::LessThan()
{
    *acc = *lhs->acc < *rhs->acc;
    return *acc;
}
alu xiobject::GreaterThan()
{
    *acc = *lhs->acc > *rhs->acc;
    return *acc;
}
alu xiobject::Assign()
{
    diagnostic::debug(sfnl) << color::Lime
        << color::Aquamarine3 << lhs->attribute() << color::Reset << " "
        << " " << color::CornflowerBlue << attribute() << " "
        << color::Yellow
        << rhs->acu()() << ":";

    *acc = *lhs->acc = *rhs->acc;
    diagnostic::output() << color::CornflowerBlue << " => " << color::Lime << (*acc)();
    return *acc;
}
alu xiobject::BinAnd()
{
    *acc = *lhs->acc & *rhs->acc;
    return *acc;
}
alu xiobject::BinOr()
{
    *acc = *lhs->acc | *rhs->acc;
    return *acc;
}
alu xiobject::BitXor()
{
    *acc = *lhs->acc ^ *rhs->acc;
    return *acc;
}
alu xiobject::X1()
{
    *acc = ~(*lhs->acc);
    return *acc;
}
alu xiobject::X2()
{
    *acc = ~(*lhs->acc) + alu(1);
    return *acc;
}


alu xiobject::BitNot()
{
    *acc = !rhs->acc->number<double>();
    return *acc;
}


alu xiobject::BoolAnd()
{
    *acc = *lhs->acc && *rhs->acc;
    return *acc;
}
alu xiobject::BoolOr()
{
    *acc = *lhs->acc || *rhs->acc;
    return *acc;

}

alu xiobject::Division()
{
    diagnostic::debug(sfnl) << color::Lime
        << color::Yellow << lhs->acu()()
        << " " << color::CornflowerBlue << attribute() << " "
        << color::Yellow
        << rhs->acu()() << ":";

    *acc = *lhs->acc / *rhs->acc;
    return *acc;
    //diagnostic::output() << color::CornflowerBlue << " = " << color::Lime << (*acc)();
}


alu xiobject::Factorial()
{
    //*acc = acc->factorial(*lhs->acc);
    *acc = lhs->acc->factorial();
    return *acc;
}



alu xiobject::Positive()
{
    if (rhs->acc->number<double>() < 0)
        *rhs->acc *= -1;

    *acc = *rhs->acc;
    return *acc;
}


alu xiobject::Negative()
{
    diagnostic::debug(sfnl) << color::Lime
        << color::CornflowerBlue << attribute()
        << color::Yellow << rhs->attribute()
        << color::White << "=";

    if (rhs->acc->number<double>() > 0) // ==> a = -1;  -a = ?
        *rhs->acc *= -1;

    *acc = *rhs->acc;
    diagnostic::output() << color::CornflowerBlue << " = " << color::Lime << (*acc)();
    return *acc;
}


alu xiobject::KPi()
{
    *acc = alu(_PI_);
    return *acc;
}


alu xiobject::KCos()
{
    *acc = std::cos(deg2rad(rhs));
    return *acc;
}


alu xiobject::KAcos()
{
    *acc = std::acos(deg2rad(rhs));
    return *acc;
}
alu xiobject::KTan()
{
    *acc = std::tan(deg2rad(rhs));
    return *acc;
}


alu xiobject::KAtan()
{
    *acc = std::atan(deg2rad(rhs));

    return *acc;
}

alu xiobject::KSin()
{
    *acc = std::sin(deg2rad(rhs));
    return *acc;
}

alu xiobject::KAsin()
{
    *acc = std::asin(deg2rad(rhs));
    return *acc;
}

alu xiobject::KNumber()
{

    return *acc;
}

alu xiobject::KU8()
{
    *acc = rhs->acc->number<uint64_t>() & 0xFF;
    return alu();
}

alu xiobject::KU16()
{
    *acc = rhs->acc->number<uint64_t>() & 0xFFFF;
    return *acc;
}

alu xiobject::KU32()
{
    *acc = rhs->acc->number<uint64_t>() & 0xFFFFFFFF;
    return *acc;
}

alu xiobject::KU64()
{
    *acc = (uint64_t)(rhs->acc->number<uint64_t>());// & 0xFFFFFFFFFFFFFFFF);
    return *acc;
}

alu xiobject::KI8()
{
    *acc = (int8_t)(rhs->acc->number<int64_t>() & 0xFF);
    return *acc;
}

alu xiobject::KI16()
{
    diagnostic::debug() << rhs->t0->text();
    *acc = (int16_t)(rhs->acc->number<uint64_t>() & 0xFFFF);
    return *acc;
}

alu xiobject::KI32()
{
    *acc = (int32_t)(rhs->acc->number<uint64_t>() & 0xFFFFFFFF);
    return *acc;
}

alu xiobject::KI64()
{
    *acc = (int64_t)(rhs->acc->number<int64_t>() & 0xFFFFFFFFFFFFFFFF);
    return *acc;
}

alu xiobject::KReal()
{
    *acc = rhs->acc->number<double>();
    return *acc;
}

alu xiobject::KString()
{
    *acc = std::string(*rhs->acc);
    return *acc;
}


std::string xiobject::type_name()
{
    return type::name(t0->t);
}




std::string xiobject::attribute() {
    return t0 ? t0->text() : "''";
}

auto xiobject::query(xiobject* in_lhs, xiobject* in_rhs)
{
    for (auto [lr, fn] : xiobject::move_tbl)
    {
        auto [l, r] = lr;
        if ((in_lhs->t0->t & l) && (in_rhs->t0->t & r)) return fn;
    }
    return static_cast<xiobject * (xiobject::*)(xiobject*)> (nullptr);
}



//---------------------------Arithmetic Expression Tree Building------------------------------------------------
xiobject::move_table_t xiobject::move_tbl =
{
    // to_left:
        {{type::OpenPair, type::Leaf | type::Prefix | type::Binary | type::Id | type::Number | type::Const}, &xiobject::to_left},
        {{type::Binary | type::Prefix | type::Sign | type::Assign,  type::OpenPair | type::Leaf | type::Prefix | type::Number | type::Id}, &xiobject::to_right},
        // rpar_leaf:
        //    {{type::close_pair, type::leaf}, &xiobject::_rpar_leaf},
        //// _assign:
        //    {{type::leaf|type::number|type::id, type::assign}, &xiobject::_assign},
        // _close_par:
        {{type::ClosePair | type::Prefix | type::Postfix | type::Id | type::Number | type::Const, type::ClosePair }, &xiobject::_close_par},
        // close_par_seq:
        {{type::ClosePair, type::Binary | type::Postfix | type::ClosePair}, &xiobject::collapse_par_pair},
        // _binary:
        {{type::Number | type::Binary | type::Prefix | type::Postfix | type::Id, type::Assign | type::Binary}, &xiobject::_binary},
        // opo_right:
        {{type::Number | type::Id, type::Postfix}, &xiobject::op_to_right}
};

xiobject::input_table_t xiobject::input_tbl =
{
    {{type::Id     | type::Assign,    type::Assign}, "&xiobject::_assign"},
    {{type::Number | type::Const | type::Postfix | type::ClosePair | type::Id,    type::Binary}, "&xiobject::_binary"},
    {{type::Number | type::Id    | type::ClosePair | type::Const,    type::Postfix},  "&xiobject::_postfix"},
    {{type::Binary,  type::Prefix},     "&xiobject::_prefix"},
    {{type::Prefix | type::Binary | type::OpenPair | type::Assign,    type::Leaf | type::Number | type::OpenPair | type::Id | type::Prefix},  "&xiobject::_leaf"},
    {{type::Prefix | type::Binary | type::OpenPair,    type::OpenPair},    "&xiobject::_open_pair"},
    {{type::Postfix| type::Const | type::Id | type::OpenPair | type::ClosePair,    type::ClosePair}, "&xiobject::_close_pair"}
};


void xiobject::SyntaxError(xiobject* e)
{
    throw diagnostic::syntax() << "at " << e->t0->location() << code::endl << e->t0->mark();
}


void xiobject::make_error(code::T ErrCode, xiobject* source_node, xiobject* input_node)
{
    throw diagnostic::error() << source_node->attribute()
        << " token input error: "
        << ErrCode
        << input_node->attribute()
        << code::endl
        << input_node->t0->mark();
}

void xiobject::make_error(code::T ErrCode, xiobject* source_node, token_data* inputoken)
{
    throw diagnostic::error() << source_node->attribute()
        << " token input error: "
        << ErrCode
        << inputoken->text()
        << code::endl
        << inputoken->mark();
}

void xiobject::header(xiobject* input_node, source_location&& Loc)
{
    diagnostic::debug(std::move(Loc)) << t0->text() << "<-" << input_node->t0->text() << code::endl << input_node->t0->mark();
}



/*!
 * \brief xiobject::input
 *
 * \note Call to this function is stricly, restricted to the Arithmetic Expression Tree/AST build context.
 * \param parent_bloc
 * \param token
 * \return Insertion vertex xiobject.
 *
 * ex.: if x-1 + 12 ==  0 return 12;
 */
xiobject* xiobject::input(xiobject* parent_bloc, token_data* token, xiobject::maker mk)
{
    for (auto [lr, fn] : xiobject::input_tbl)
    {
        auto [l, r] = lr;
        if ((t0->t & l) && (token->t & r))
        {
            diagnostic::debug(sfnl) << color::Yellow << t0->text() << " <- " << color::Yellow << token->text() << color::Reset << " Input token validated... ";
            ///@todo Check id tokens for function_call and other id-constructs before calling xiobject::input(...).

            xiobject* a;
            if(mk)
                a = mk(token);
            else
                a  = new xiobject(parent_bloc,token);

            auto fn = xiobject::query(this, a);
            if (!fn)
            {
                if(a)
                {
                    a->detach();
                    delete a;
                }
                diagnostic::syntax() << " invalid relational operands at " << token->location() << " - unexpected token:" << code::endl << token->mark();
                return nullptr;
            }
            diagnostic::debug() << t0->text() << "::input(" << token->text() << "):" << code::endl << token->mark();

            return (this->*fn)(a);
        }
    }

    diagnostic::info(sfnl) << t0->text()
        << "::input(" << token->text() << ") invalid relational operands at "
        << token->location() << " - unexpected token : "
        << code::endl << token->mark();
    diagnostic::output() << t0->details() << " || " << token->details() << code::endl << "Returning nullptr";

    return nullptr;
}

xiobject* xiobject::_binary(xiobject* a)
{
    header(a, sfnl);

    if (t0->is_leaf())
    {
        if (a->t0->c == mnemonic::OpenPar)
            SyntaxError(a);
    }

    if (t0->c == mnemonic::OpenPar)
        return to_left(a);

    if (t0->is_binary())
    {
        if (!rhs) SyntaxError(a);

        if (a->t0->d < t0->d)
            return to_right(a);
        if (op)
        {
            auto fn = query(this, a);
            if (!fn)
                xiobject::make_error(code::unexpected, this, a);

            return (op->*fn)(a);
        }

        return a->to_left(this);
    }

    if (op) {
        auto fn = query(this, a);
        if (!fn)
            xiobject::make_error(code::unexpected, this, a);
        return (op->*fn)(a);

    }
    a->to_left(this);
    return a;
}

xiobject* xiobject::_close_pair(xiobject* a)
{
    header(a, sfnl);
    xiobject* x = xiobject::pop_par();
    if (!x)
    {
        diagnostic::error() << "Unmatched left paranthese:" << code::endl << a->t0->mark();
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

    diagnostic::debug()
        << "new input vertex:["
        << color::Yellow
        << a->t0->text()
        << color::Reset
        << "]" << code::endl
        << a->t0->mark();

    return a;
}

xiobject* xiobject::_close_par(xiobject* a)
{
    header(a, sfnl);

    xiobject* x = xiobject::pop_par();
    if (!x)
    {
        diagnostic::error() << "Unmatched left paranthese." << a->t0->mark();
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

    diagnostic::output()
        << "new input vertex:["
        << color::Yellow
        << a->t0->text()
        << color::Reset
        << "]" << code::endl
        << a->t0->mark();

    return a;
}

xiobject* xiobject::collapse_par_pair(xiobject* a)
{
    header(a, sfnl);
    xiobject* v = lhs;

    // Collapse lhs

    v->op = op;
    if (op) op->rhs = v;

    // discard();

    if (v->op) {
        diagnostic::output()
            << color::Yellow << v->op->attribute() << color::CornflowerBlue
            << " <-- "
            << color::Yellow << a->attribute();

        auto p_fn = query(v->op, a);
        if (!p_fn)
            xiobject::SyntaxError(a);

        return (v->op->*p_fn)(a);
    }

    v->op = a;
    a->lhs = v;
    return a;
}

xiobject* xiobject::op_to_right(xiobject* a)
{
    header(a, sfnl);
    if (!op) {
        a->lhs = this;
        op = a;
        return a;
    }
    return op->to_right(a);
}


int xiobject::push_par(xiobject* a)
{
    xiobject::pars.push(a);
    return static_cast<int>(xiobject::pars.size());
}

xiobject* xiobject::pop_par()
{
    if (xiobject::pars.empty()) return nullptr;
    xiobject* x = xiobject::pars.top();
    xiobject::pars.pop(); // Tabarnak!
    return x;
}


xiobject* xiobject::begin(xiobject* parent_, token_data* token)
{
    if (!token->_flags.V)
    {
        return nullptr;
    }
    xiobject* a = new xiobject(parent_, token);
    if (a->t0->c == mnemonic::OpenPar) push_par(a);
    return a;
}

xiobject* xiobject::tree_close()
{
    header(this, sfnl);

    if (t0->c == mnemonic::OpenPar)
    {
        diagnostic::error() << " unexpected end of file.";
        return nullptr;
    }

    if (!xiobject::pars.empty())
    {
        xiobject* x = xiobject::pars.top();
        xiobject::pars.pop();
        diagnostic::error() << " umatched closing parenthese from:" << code::endl << x->t0->mark();
        return nullptr;
    }


    if (t0->c == mnemonic::ClosePar) {
        diagnostic::debug() << "Closing the tree on close parenthese:";
        if (lhs)
        {
            xiobject* x = lhs;
            diagnostic::debug() << "left hand side operand: " << lhs->t0->details() << ":" << code::endl << lhs->t0->mark();

            lhs->op = op;

            if (op)
            {
                op->rhs = lhs;
                xiobject::trace_connect_binary_operands(op);
            }

            // discard();
            return x->tree_root(false);
        }
    }
    return tree_root(false);
}

xiobject* xiobject::tree_root(bool skip_syntax)
{
    diagnostic::debug(sfnl) << "query tree ins from xiobject node:" << code::endl << t0->mark();
    xiobject* x = this;
    xiobject* p = x;
    do {
        x = p;
        if (!skip_syntax)
        {
            switch (x->t0->t) {
            case type::Assign:
            case type::Binary:
                if (!x->lhs)
                {
                    diagnostic::error() << "Syntax error: binary operator is missing its left operand." << code::endl << x->t0->mark();
                    return nullptr;
                }
                if (!x->rhs)
                {
                    diagnostic::error() << "Syntax error: binary operator is missing its right operand." << code::endl << x->t0->mark();
                    return nullptr;
                }
            case type::Prefix:
                if (!x->rhs)
                {
                    diagnostic::error() << "Syntax error: prefix unary operator is missing its (right) operand." << code::endl << x->t0->mark();
                    return nullptr;
                }
                break;
            case type::Postfix:
                if (!x->lhs)
                {
                    diagnostic::error() << "Syntax error: postfix unary operator is missing its (left) operand." << code::endl << x->t0->mark();
                    return nullptr;
                }
                break;
            }
        }
        p = p->op;
    } while (p);

    diagnostic::debug(sfnl) << "query tree ins returning node: " << x->t0->details() << code::endl << x->t0->mark();
    return  x;
}

xiobject* xiobject::to_right(xiobject* in_rhs)
{
    header(in_rhs, sfnl);

    // Temporary hack....
    if (in_rhs->t0->c == mnemonic::OpenPar)
        xiobject::push_par(in_rhs);

    if (rhs) {
        /*
           this
              \
               x <- nextoken
              /
            rhs
        */
        diagnostic::debug() << t0->text() << " -> " << rhs->t0->text()
            << color::Lime << "tree_set_right "
            << color::White << " <- "
            << color::Yellow << in_rhs->t0->text();
        rhs->op = in_rhs;
        in_rhs->lhs = rhs;
    }
    rhs = in_rhs;
    in_rhs->op = this;
    if (t0->is_binary())
    {
        diagnostic::debug(sfnl) << xiobject::trace_connect_binary_operands(this);
    }
    return in_rhs;
}

xiobject* xiobject::to_left(xiobject* in_lhs)
{
    header(in_lhs, sfnl);
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

void xiobject::dot_tree_start(stracc& a_out, const stracc& Title)
{
    a_out << "digraph arithmetic_expressionree {\n";
    a_out << "ratio=compress; ranksep=.55; size = \"6.5,6.5\";\n";
    a_out << "    node [fontname=\"Source Code Pro\", fontsize=12];\n";
    a_out << "    label=\"Arithmetic Expr Tree:[" << Title() << "]\"; fontsize = 10;\n";
}

void xiobject::dot_node(xiobject* A, stracc& a_out)
{
    //static int nullcount = 0;
    if (!A) return;
    if ((!A->lhs) && (!A->rhs)) return;

    if (A->lhs) {
        a_out << "    xiobjectNode" << A << " -> xiobjectNode" << A->lhs << ";\n";
        xiobject::dot_node(A->lhs, a_out);
    }
    //     else
    //         t_node::trace_null_node(A, nullcount++, a_stream);

    if (A->rhs) {
        a_out << "    xiobjectNode" << A << " -> xiobjectNode" << A->rhs << ";\n";
        xiobject::dot_node(A->rhs, a_out);
    }
    //     else
    //         trace_null_node(A, nullcount++, a_stream);
}

void xiobject::dot_null_node(xiobject*, int, stracc&)
{

}

void xiobject::dot_tree(xiobject* a_root, stracc& a_out)
{
    a_root->dot_attr(a_out);
    xiobject::dot_node(a_root, a_out);
}

void xiobject::dot_tree_close(stracc& a_out)
{
    a_out << "}\n";
}

void xiobject::dot_attr(stracc& a_out)
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

    a_out << "    xiobjectNode" << this << " [shape=%s, label=\"%s\"]\n";
    a_out << Shape() << attr();

    if (lhs)
        lhs->dot_attr(a_out);
    if (rhs)
        rhs->dot_attr(a_out);
}

std::string xiobject::trace_connect_binary_operands(xiobject* x)
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



#pragma endregion TRIGRAPH





}


