//
// Created by oldlonecoder on 21-12-12.
//

#include <xio/compiler/grammar/grammar.h>



namespace xio
{
//
// Created by slussier on 19-08-25.
//


std::map<grammar::term::type, color::type> colours = 
{
    {grammar::term::type::m, color::LightSteelBlue3},
    {grammar::term::type::r, color::LightSalmon4},
    {grammar::term::type::s, color::Lime},
    {grammar::term::type::s, color::White}
};





grammar::Dictionary grammar::grammar_dictionnary = {
    {':', &grammar::enter_rule_def},
    {'|', &grammar::new_sequence},
    {'.', &grammar::end_rule},
    {'+', &grammar::set_repeat},
    {'*', &grammar::set_optional},
    {'?', &grammar::set_oneof}, // One of
    {'\'', &grammar::enter_litteral},
    {'"', &grammar::enter_litteral},
    {'#', &grammar::set_directive},
};

grammar::rule::collection grammar::rules;

std::string grammar_text = R"(
module             : 'module' Id ';'.
stmts              : +statement.
statement          : ';' | instruction | assignstmt ';'| declvar ';'| #expression ';'| #var_id ';'.
assignstmt         : declvar Assign #expression | #var_id Assign #expression.
declvar            : *typename #newvar.
funcsig            : *typename function_id '(' *params ')'.
declfunc           : funcsig ';'| funcsig statement ';' | funcsig bloc.
paramseq           : ',' param.
param              : *typename Id.
function_call      : function_id '(' args ')' | objcfncall.
params             : param *+paramseq.
objcarg            : Id ':' #expression.
arg                : objcarg | #expression.
argseq             : ',' arg.
args               : arg *+argseq.
typename           : *'static' ?'i8' ?'u8' ?'i16' ?'u16' ?'i32' ?'u32' ?'i64' ?'u64' ?'real' ?'number' ?'string' ?#objectid.
instruction        : ?'if' ?'then'  ?'switch' ?'case' ?'for' ?'while' ?'repeat' ?'until' ?'do' ?'return'.
if                 : 'if' condexpr ifbody | 'if' '(' condexpr ')' ifbody.
bloc               :  '{' stmts '}'.
truebloc           : *'then' bloc | *'then' statement.
elsebloc           : 'else' bloc | 'else' statement.
ifbody             : truebloc *elsebloc.
condexpr           : assignstmt | #expression.
function_id        : *'::' #functionid | #objectid '::' #functionid | #obj_instance '.' #functionid.
objcfncall         : '[' function_id  *args ']'.

)";

grammar::grammar()
{

}

grammar::~grammar()
{


}

book::rem::code grammar::build()
{
    book::rem::push_info() << __PRETTY_FUNCTION__ << ":";
    if(built())
    {
        book::rem::push_debug() << "grammar is already built - ok";
        return book::rem::ok;
    }
    if(_text().empty())
        _text = grammar_text;

    book::rem::push_debug()
        << color::White << ":Building rules :\n----------------------------\n"
        << color::Yellow << _text
        << color::White << "\n----------------------------------\n";

    _words = _text();
    std::size_t   count = _words(tokens, ":.+*?#", true);
    book::rem::push_debug() << "building words list...";
    auto crs=tokens.begin();
    // for(; crs != tokens.end(); crs++)
    //     words.push_back(s());

    if(!count)
    {
        book::rem::push_fatal() << ": grammar. (internal error)";
        return book::rem::empty;
    }

    book::rem::push_debug() << tokens.size() << " words.";
    _state = st_begin;
    do
    {
        book::rem::code r;
        auto      p = grammar::grammar_dictionnary.find((*crs)().c_str()[0]);
        if(p != grammar::grammar_dictionnary.end())
        {
            r = (this->*(p->second))(crs);
        }
        else
        {
            r = parse_identifier(crs);
        }
        if(r != book::rem::accepted)
            return r;
    }
    while(crs != tokens.end());
    book::rem::push_debug() << ":";
    return book::rem::accepted;
}

std::string grammar::dump_sequence(const term_seq& seq)
{
    stracc Out;
    Out << color::White << " | ";// << Ends;
    for(auto t: seq.terms) Out << t() << ' ';// << Ends;
    Out << color::Reset;
    return Out();
}


void grammar::dump()
{

    book::rem::out() << color::DarkRed2 << "%-15s" << "rule" <<  color::Black << " | terms sequence:";
    book::rem::out() << "-----------------------------------------------------------------------------------";
    stracc Out;
    for(const auto &rule: rules)
    {
        Out << color::DarkRed2 << "%-15s" << rule.second->_id;
        for(auto seq: rule.second->sequences)
        {
            stracc SeqStr;
            SeqStr <<  dump_sequence(seq);
            Out << SeqStr;
        }
        Out << '\n';
    }
    book::rem::out() << Out();
}

void grammar::init_rules()
{

}

book::rem::code grammar::parse_identifier(strbrk::token_t::iterator  &crs)
{
    rule *r = query_rule((*crs)());
    switch(_state)
    {
        case st_begin:
            if(r)
            {
                if(!r->empty())
                {
                    book::rem::push_fatal() << " rule, named: " << (*crs)() << " already exists in the context of a new rule definition.";
                    return book::rem::rejected;
                }
                _rule = r;
            }
            else
            {
                std::string rule_name = (*crs)();
                _rule = new rule(rule_name);
                rules[rule_name] = _rule;
            }
            a.Reset();
            _state = st_init_rule; //  expect ':' as next token in main loop.
            break;
        case st_init_rule:_state = st_seq;
            break;
        default: break;
        case st_option:
        case st_seq:_state = st_seq;
            // lexem::T ?
            /*lexer::lexem::lexer::lexem::mnemonic m = lexem::m(crs->c_str());
            if( m != lexer::lexem::lexer::lexem::mnemonic::knull ) {
                _rule->a = a;
                (*_rule) | m;
                a.reset();
                break;
            }*/

            type::T t = type::from_str((*crs)());
            if(t)// & teacc::type::bloc_t) // Quick and dirty hack about bypassing the lexer::teacc::type::bloc type:
            {
                _rule->a = a;
                (*_rule) | t;
                a.Reset();
                break;
            }

            if(r)
            {
                _rule->a = a;
                (*_rule) | r;
                a.Reset();
                break;
            }
            else
            {
                r = new rule((*crs)());
                rules[(*crs)()] = r;
                _rule->a = a;
                _state = st_seq; //  expect ':' as next token in main loop.
                (*_rule) | r;
                a.Reset();
            }
            break;
    }
    ++crs;
    return book::rem::accepted;
}

book::rem::code grammar::enter_rule_def(strbrk::token_t::iterator &crs)
{
    // ':' :
    if(_state == st_init_rule)
    {
        _state = st_seq;
        a.Reset();
        ++crs;
        return book::rem::accepted;
    }
    book::rem::push_fatal(HEREF) << " misplaced ':' ";
    return book::rem::rejected;
}

book::rem::code grammar::new_sequence(strbrk::token_t::iterator &crs)
{
    // logdebug
    //     << color::HCyan << __FUNCTION__
    //     << color::White << ": ["
    //     << color::Yellow << *crs
    //     << color::White << ']'
    //     << Ends;

    if(_state == st_option)
    {
        book::rem::push_fatal() << " syntax error: " << (*crs)() << "' is invalid in this context";
        return book::rem::rejected;
    }

    _rule->new_sequence();
    _state = st_seq;
    a.Reset();
    ++crs;
    return book::rem::accepted;
}

book::rem::code grammar::end_rule(strbrk::token_t::iterator &crs)
{
    // logdebug
    //     << color::HCyan << __FUNCTION__
    //     << color::White << ": ["
    //     << color::Yellow << *crs
    //     << color::White << ']'
    //     << Ends;
    _state = st_begin;
    ++crs;
    return book::rem::accepted;
}

book::rem::code grammar::set_repeat(strbrk::token_t::iterator &crs)
{
    //logdebug
    //    << color::HCyan << __FUNCTION__
    //    << color::White << ": ["
    //    << color::Yellow << *crs
    //    << color::White << ']'
    //    << Ends;
    _state = st_option;
    +a;
    ++crs;
    return book::rem::accepted;
}

book::rem::code grammar::set_directive(strbrk::token_t::iterator &crs)
{
    !a;
    _state = st_option;
    /*
               book::rem::push_debug()
               << color::Yellow << a()
               << color::White << ": ["
               << color::Yellow << *crs
               << color::White << ']';
   */

    ++crs;
    return book::rem::accepted;
}

book::rem::code grammar::set_optional(strbrk::token_t::iterator &crs)
{
    //logdebug
    //    << color::HCyan << __FUNCTION__
    //    << color::White << ": ["
    //    << color::Yellow << *crs
    //    << color::White << ']'
    //    << Ends;
    *a;
    ++crs;
    _state = st_option;
    return book::rem::accepted;
}

book::rem::code grammar::enter_litteral(strbrk::token_t::iterator &crs)
{

    //    book::rem::push_debug()
    //    << color::White << ": ["
    //    << color::Yellow << *crs
    //    << color::White << ']';

    if((_state != st_seq) && (_state != st_option))
    {
        book::rem::push_fatal() << "syntax error '" << (*crs)() << "' is not a valid s++ grammar token in context" << "(state machine:" << (int) _state << ")";
        return book::rem::rejected;
    }

    strbrk::token_t::iterator i, quote;
    i = quote = crs;
    ++i;
    if((*i)() == (*quote)())
    {
        book::rem::push_fatal() << "error: litteral grammar element cannot be empty";
        return book::rem::rejected;
    }

    token_data token = token_data::scan((*i)().c_str());
    if(token)
    {
        _rule->a = a;
        (*_rule) | token.c;
        a.Reset();
    }
    else
    {
        book::rem::push_fatal() << "syntax error '" << (*i)() << "' is not a valid s++ grammar token";
        return book::rem::rejected;
    }
    crs = i;
    ++crs;
    if((*crs)() == (*quote)())
        ++crs;
    //++crs; // will be on the next token.

    return book::rem::accepted;
}

book::rem::code grammar::set_oneof(strbrk::token_t::iterator &crs)
{
    //     logdebug
    //         << color::HCyan << __FUNCTION__
    //         << color::White << ": ["
    //         << color::Yellow << *crs
    //         << color::White << ']'
    //         << Ends;
    ~a;
    ++crs;
    return book::rem::accepted;
}

[[maybe_unused]] int grammar::init()
{
    _text = "";
    return 0;
}

grammar::rule *grammar::query_rule(const std::string &a_id)
{
    auto i = rules.find(a_id);
    return i == rules.end() ? nullptr : i->second;
}


const grammar::rule *grammar::operator[](const std::string &r_id) const
{
    book::rem::push_debug(HEREF) << " query: '" << r_id << "':\n";
    auto i = grammar::rules.find(r_id);
    if(i == grammar::rules.end())
    {
        book::rem::push_error() << " no such rule: '" << r_id << "'";
        return nullptr;
    }
    book::rem::push_debug() << " rule '" << r_id << "' found.";
    return i->second;
}
void grammar::destroy_rules()
{
     for(auto rit: grammar::rules)
     {
         rule *r = rit.second;
         delete r;
     }
    rules.clear();
}

grammar::term::term(rule *r, term_properties a_)
{
    a = a_;
    object.r = r;
    _type = term::type::r;
}

grammar::term::term(xio::type::T a_sem, term_properties a_)
{
    a = a_;
    object.sem = a_sem;
    _type = term::type::s;
}

grammar::term::term(mnemonic a_code, term_properties a_)
{
    a = a_;
    object.m = a_code;
    _type = term::type::m;
}

grammar::term::term(const std::string &a_lexem)
{
    _type = term::type::m;
    object.m = lexem::from_str(a_lexem);
}

grammar::term::term(term &&_t)
noexcept
{
    //     logdebugfn << ":" << Ends;
    using std::swap;
    swap(object, _t.object);
    _type = _t._type;
    swap(a, _t.a);
}

grammar::term::term(const grammar::term &_t)
{
    object = _t.object;
    _type  = _t._type;
    a      = _t.a;
}

grammar::term & grammar::term::operator=(term &&_t) noexcept
{
    using std::swap;
    swap(object, _t.object);
    _type = _t._type;
    swap(a, _t.a);
    return *this;
}

grammar::term & grammar::term::operator=(const term &_t)
{
    object = _t.object;
    _type  = _t._type;
    a      = _t.a;
    return *this;
}

bool grammar::term::operator==(const term &t) const
{
    if(_type != t._type)
        return false;

    switch(_type)
    {
        case type::m:return object.m == t.object.m;
        case type::r:return object.r == t.object.r;
        case type::s:return (object.sem & t.object.sem) != 0;
        case type::nil:return false;
    }
    return false;
}

bool grammar::term::operator==(const token_data &t) const
{
    switch(_type)
    {
        case type::m:return object.m == t.c;
        case type::s:return (object.sem & t.s) != 0;
        case type::nil:
        default:return false;
    }
    return false;
}

bool grammar::term::operator!=(const token_data &t) const
{
    switch(_type)
    {
        case type::m:return object.m != t.c;
        case type::s:return (object.sem & t.s) == 0;
        case type::nil:
        default:return true;
    }
    return true;
}

grammar::term::~term()
{
}

std::string grammar::term::operator()() const
{
    stracc str;
    str << a() << colours[_type];

    //switch(_type)
    //{
    //    case term::type::m:
    //    {
    //        token_data tok = token_data()[object.m];
    //        str << color::DarkOrange << tok.text(); // String (TEXT)
    //    }
    //        break;
    //    case term::type::r:
    //        // Can't happen but we never know: (nullptr)
    //        if(object.r)
    //            str << color::Black << object.r->_id;
    //        break;
    //    case term::type::s:
    //        str << color::DarkMagenta <<  xio::type::name(object.sem);
    //        break;
    //    default:str << "nil";
    //        break;
    //}

    return str();
}

grammar::rule::rule(const std::string &a_id)
{
    _id = a_id;
    sequences.push_back({});
    seq = sequences.begin();
}

grammar::rule::~rule()
{
    sequences.clear();
    _id.clear();
}

grammar::rule & grammar::rule::new_sequence()
{
    sequences.push_back(term_seq());
    seq = --sequences.end();
    a.Reset();
    return *this;
}

grammar::rule & grammar::rule::operator|(rule *_r)
{
    term t = term(_r);
    t.a = a;
    a.Reset();
    *seq << t;
    return *this;
}

grammar::rule & grammar::rule::operator|(xio::type::T _t)
{
    term t = term(_t);
    t.a = a;
    a.Reset();
    *seq << t;
    return *this;
}

grammar::rule & grammar::rule::operator|(mnemonic _t)
{
    term t = term(_t);
    t.a = a;
    a.Reset();
    *seq << t;
    return *this;
}

grammar::term grammar::term_seq::next(term::const_iterator &it) const
{
    if(it != terms.end())
        ++it;
    return *it;
}

grammar::term_seq & grammar::term_seq::operator<<(term a_t)
{
    terms.push_back(a_t);
    return *this;
}

grammar::term_seq & grammar::term_seq::operator<<(xio::type::T a_t)
{
    terms.emplace_back(a_t);
    return *this;
}

grammar::term_seq & grammar::term_seq::operator<<(mnemonic a_t)
{
    terms.emplace_back(a_t);
    return *this;
}

grammar::term_seq & grammar::term_seq::operator<<(rule *a_t)
{
    terms.emplace_back(a_t);
    return *this;
}

std::string grammar::term_properties::operator()()
{
    stracc str;
    if(Z)
        str << color::LightCyan3 << '*';
    if(R)
        str << color::White << '+';
    if(L)
        str << color::CadetBlue << '?';
    if(X)
        str << color::LighcoreateBlue << '#';
    return str();
}

}

