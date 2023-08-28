/***************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                         *
 *   serge.lussier@oldlonecoder.club                                       *
 *                                                                         *
 *                                                                         *
 *   Unless otherwise specified, all code in this project is written       *
 *   by the author (Serge Lussier)                                         *
 *   and no one else then not even {copilot, chatgpt, or any other AI}     *
 *   --------------------------------------------------------------------- *
 *   Copyrights from authors other than Serge Lussier also apply here      *
 ***************************************************************************/


#pragma once
#include <logbook/expect.h>
#include <xio/tokendata.h>
#include <map>
#include <chrtools/strbrk.h>

#include <stack>

namespace xio
{
namespace cc 
{
class parser;
}


class XIO_PUBLIC grammar
{
public:
    struct XIO_PUBLIC rule;
private:
    friend class ::xio::cc::parser;
    friend class xio_interpreter;
public:
    /*!
        * @brief
        * @todo Holly shit!! Confused on X flag: accept state and Implicit-Rule Flag!!!! ^^ ?
        *
        */
    struct XIO_PUBLIC term_properties
    {
        uint8_t Z : 1; ///< Zero or one (optional * )
        uint8_t R : 1; ///< Repeat      (        + )
        uint8_t L : 1; ///< List        (one of  ~ ?)
        uint8_t X : 1; ///< parserctrl    ( Parser to takes control of this rule: ast Parser term)
        uint8_t S : 1; ///< State!! 0 = unset/rejected; 1 = set/accepted !!!

        term_properties& operator|(term_properties const& _)
        {
            Z |= _.Z;
            R |= _.R;
            L |= _.L;
            X |= _.X;
            //s = A.s;
            return *this;
        }

        term_properties& operator+()
        {
            R = 1;
            return *this;
        }
        term_properties& operator*()
        {
            Z = 1;
            return *this;
        }
        term_properties& operator!()
        {
            X = 1;
            return *this;
        }
        term_properties& operator~()
        {
            L = 1;
            return *this;
        }
        void Reset()
        {
            Z = R = L = X = S = 0;
        }
        std::string operator()();
        [[nodiscard]] bool is_optional() const
        {
            return Z != 0;
        }
        [[nodiscard]] bool is_oneof() const
        {
            return L != 0;
        }
        [[nodiscard]] bool is_strict() const
        {
            return Z == 0 && L == 0 && R == 0;
        }
        [[nodiscard]] bool is_repeat() const
        {
            return R != 0;
        }
        [[nodiscard]] bool accepted() const
        {
            return S != 0;
        }
        [[nodiscard]] bool is_parserctrl()
        {
            return X == 1;
        }

        void accept()
        {
            S = 1;
        }
        void reject()
        {
            S = 0;
        }
    };

    struct XIO_PUBLIC term
    {
        mutable term_properties a = { 0, 0, 0, 0, 0 }; ///< default : punctual, strict match
    
        enum class type : uint8_t
        {
            //term,
            r = 0,
            s,
            m,
            nil
        };

        void accept()
        {
            a.accept();
        }
        void Reject()
        {
            a.reject();
        }

        term::type _type = term::type::nil;

        union info
        {
            rule* r;
            ::xio::type::T sem;
            mnemonic      m;
        }object = { nullptr };

        using collection = std::vector<term>;
        using iterator = collection::iterator;
        using const_iterator = collection::const_iterator;

        // pas le choix... 'y faut un parser....

        term() = default;
        term(rule* r, term_properties a_ = { 0, 0, 0, 0, 0 });
        term(::xio::type::T a_sem, term_properties a_ = { 0, 0, 0, 0, 0 });
        term(mnemonic a_code, term_properties a_ = { 0, 0, 0, 0, 0 });

        term(const std::string& a_lexem);

        term(term&& _t) noexcept;
        term(const term& _t);

        term& operator=(term&& _t) noexcept;
        term& operator=(const term& _t);

        bool operator==(const term& t) const;
        bool operator==(const token_data& t) const;
        bool operator!=(const token_data& t) const;

        operator bool()
        {
            return _type != term::type::nil;
        }
        ~term();

        term& operator*()
        {
            *a;
            return *this;
        }
        term& operator+()
        {
            +a;
            return *this;
        }
        term& operator!()
        {
            !a;
            return *this;
        }
        term& operator~()
        {
            ~a;
            return *this;
        }

        std::string operator()() const;

        bool is_rule() const
        {
            return _type == type::r;
        }
        bool is_semantic() const
        {
            return _type == type::s;
        }
        bool is_mnemonic() const
        {
            return _type == type::m;
        }
        bool is_null() const
        {
            return _type == type::nil;
        }

        //static term query(const char *C_);
        //static term query(xio::type::T T_);
        //static term query(mnemonic M_);
    };

    struct XIO_PUBLIC term_seq
    {
        term_properties a = { 0, 0, 0, 0, 0 }; ///< default : punctual, strict match

        //using data = std::vector<term_list_t>;

        term::collection terms;

        using collection = std::vector<term_seq>;
        using const_iterator = collection::const_iterator;
        using iterator = collection::iterator;
        using stack = std::stack<iterator>;

        term_seq() = default;

        term::const_iterator begin() const
        {
            return terms.cbegin();
        }

        bool end(term::const_iterator t) const
        {
            return terms.cend() == t;
        }

        term next(term::const_iterator& it) const;

        ~term_seq()
        {
            terms.clear();
        }
        term_seq& operator<<(term a_t);

        // Emplace_back:
        term_seq& operator<<(::xio::type::T a_t);
        term_seq& operator<<(mnemonic a_t);
        term_seq& operator<<(rule* a_t);
    };

    struct XIO_PUBLIC rule
    {
        term_seq::collection sequences;
        // ---------------------------------------
        term_seq::iterator   seq; // Temporary held for building this rule.
        // ---------------------------------------
        //term_seq::stack seq_stack;

        using collection = std::map<std::string, rule*>;
        using iterator = collection::const_iterator;

        term_properties a = { 0, 0, 0, 0, 0 }; /// Volatile attribute infos. ( Copied into the rule's recursion context  )
        term_properties inject = { 0, 0, 0, 0, 0 };

        std::string _id;

        rule() = default;
        rule(const std::string& a_id);

        rule(int)
        {}
        ~rule();
        bool empty() const
        {
            return sequences.empty() ? true : sequences.begin()->terms.empty();
        }
        void inject_attr(term_properties a_a)
        {
            inject = a_a;
        }
        rule& new_sequence();
        rule& operator|(rule* _r);
        //rule& operator |(const char*   _t);
        rule& operator|(::xio::type::T _t);
        rule& operator|(mnemonic _t);
        std::string name()
        {
            return _id;
        }
        term_seq::const_iterator begin() const
        {
            return sequences.cbegin();
        }
        bool end(term_seq::const_iterator s) const
        {
            return s == sequences.cend();
        }
    };

    /*!
        @brief the tea gammar class .

        @author &copy;2021, oldlonecoder (lussier.serge@gmail.com).

        @note offset really don't know why offset need to do a complete syntax grammar (generator) for the tea++ interpreter.
        My first intention was to create a specific arithmetic expression parser and then create a hand made parser for the the rest of the syntax.
    */

    public:
        grammar();

        ~grammar();
        stracc& text() { return _text; }

        book::rem::code build();

        void dump();
        std::string dump_sequence(const term_seq& seq);

    private:
        enum state_mac
        {
            st_begin = 0, ///< about to define new rule: cursor on rule id.
            st_init_rule, ///< initialize sequence of terms: cursor on ':'.
            st_seq,       ///< back and forth between st_seq<->st_init_term.
            st_option,    ///< one of ' *+? ' option state.
            st_init_term, ///< query {rule | type semantic | e_code | litteral} : cursor on identifier.
            st_add_seq,   ///< terminate current sequence and start a new sequence on the rule: cursor on ','.
            st_end_rule   ///< terminate rule : cursor on '.'.
        };

        term_properties a = { 0, 0, 0, 0, 0 }; ///< default : punctual, strict match

        state_mac               _state = grammar::st_begin;

        [[maybe_unused]] int init();
        static rule::collection rules;
        rule* _rule = nullptr;
        static rule* query_rule(const std::string& a_id);

        using RuleScanner = book::rem::code(grammar::*)(strbrk::token_t::iterator&);
        strbrk::token_t::list tokens;
        stracc               _text;
        strbrk              _words;

        using Dictionary = std::map<char, grammar::RuleScanner>;
        static Dictionary grammar_dictionnary;

    public:
        const rule* operator[](const std::string& r_id) const;

        static bool built()
        {
            return rules.size() != 0;
        }

        static void init_rules();
        static void destroy_rules();
    private:
        grammar::Dictionary::iterator RuleIt;

        //--------------- Rules builders -------------------

        book::rem::code parse_identifier(strbrk::token_t::iterator& crs);
        book::rem::code enter_rule_def(strbrk::token_t::iterator& crs);
        book::rem::code new_sequence(strbrk::token_t::iterator& crs);
        book::rem::code end_rule(strbrk::token_t::iterator& crs);
        book::rem::code set_repeat(strbrk::token_t::iterator& crs);
        book::rem::code set_optional(strbrk::token_t::iterator& crs);
        book::rem::code enter_litteral(strbrk::token_t::iterator& crs);
        book::rem::code set_oneof(strbrk::token_t::iterator& crs);
        book::rem::code set_parserctrl(strbrk::token_t::iterator& crs);
};

}
//#endif //VXIO_GRAMMAR_H
