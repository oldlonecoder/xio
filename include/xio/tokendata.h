
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

#include "xio/types.h"
//#include "logbook/expect.h"


namespace xio
{

    struct XIO_PUBLIC token_data
    {
        xio::mnemonic    m{mnemonic::Noop};
        xio::type::T     t{xio::type::Null};    ///< Primitive xio::type bit.
        xio::type::T     s{xio::type::Null};    ///< Semantic xio::type bits field
        xio::distance::T d{xio::distance::noop_};
        void* vdata{nullptr};

//        // --------------------- Add link pointers to previous and forward tokens because there situations where giving the std container is so overload and overbloated!
//        token_data* __prev{nullptr};
//        token_data* __next{nullptr};
//        // --------------------------------------------------------------------------------------------------------------------------------------------------------------

        using list     = std::vector<token_data>;
        // ----------------------------------------
        using cache    = std::vector<token_data*>;
        using pointer  = cache::iterator;
        // ----------------------------------------
        using iterator = list::iterator;


        struct XIO_PUBLIC location_data
        {
            const char* begin   = nullptr;
            const char* end     = nullptr;
            int         linenum = -1; ///< line number
            int         colnum  = -1; ///< Column number
            int64_t     offset  = -1; ///< Absolute Offset from the beginning of the Source Text.
            size_t      ln      = 0; ///< Length of this token attribute.
            std::string operator()() const;
            std::string text() const;
            std::string position() const;
        }loc;

        struct XIO_PUBLIC flags
        {
            uint8_t V : 1; ///< Pre-parsed as a value Token;
            uint8_t S : 1; ///< Post parsed as assignable
            uint8_t M : 1; ///< Virtual multiplication operator. (...4ac...)
        }   _flags = { 0, 0, 0 };

        static token_data mNull;

        token_data() = default;
        ~token_data() = default;

        token_data(mnemonic Code_, xio::type::T Type_, xio::type::T Sem_, xio::distance::T Delta_, lexem::T aLexem, uint8_t V);
        token_data(mnemonic Code_, xio::type::T Type_, xio::type::T Sem_, xio::distance::T Delta_, token_data::location_data LocationData_, token_data::flags Flags_, void* Ptr_ = nullptr);
        token_data(const token_data& Token_);
        token_data(token_data&& Token_) noexcept;
        token_data& operator=(token_data&& Token_) noexcept;
        token_data& operator=(const token_data& Token_);
        token_data operator[](mnemonic CC);
        std::string mark(int nspc = 0) const;
        std::string text_line() const;
        //token_data* back_to_startof_line();

        explicit operator bool() const
        {
            return loc.begin != nullptr;
        }
        bool operator||(xio::type::T Sem_) const
        {
            return s || Sem_;
        }

        [[nodiscard]] std::string text() const
        {
            if ((_flags.M) && (m == mnemonic::Mul)) return lexem::Multiply; // Overwrite source location.
            return loc.text();
        }
        [[nodiscard]] std::string location_text() const;
        [[nodiscard]] std::string semantic_text() const;
        [[nodiscard]] std::string type_name() const;
        [[nodiscard]] std::string details(bool mark_it = false) const;

        static token_data scan(const char* C_);
        static std::string dump_token_table();

        [[nodiscard]] bool is_text() const
        {
            return s & xio::type::Text;
        }
        [[nodiscard]] bool is_binary() const
        {
            return s & xio::type::Binary;
        }
        [[nodiscard]] bool is_unary() const
        {
            return s & xio::type::Unary;
        }
        [[nodiscard]] bool is_identifier() const
        {
            return s & xio::type::Id;
        }
        [[nodiscard]] bool is_leaf() const
        {
            return s & xio::type::Leaf;
        }
        [[nodiscard]] bool is_operator() const
        {
            return s & xio::type::Operator;
        }
        [[nodiscard]] bool is_closing_pair() const
        {
            return s & xio::type::ClosePair;
        }
        [[nodiscard]] bool is_punctuation() const
        {
            return s & xio::type::Punc;
        }
        [[nodiscard]] bool is_number() const
        {
            return s & xio::type::Number;
        }
        [[nodiscard]] bool is_hexadecimal() const
        {
            return s & xio::type::Hex;
        }
        [[nodiscard]] bool is_prefix() const
        {
            return s & xio::type::Prefix;
        }
        [[nodiscard]] bool is_postfix() const
        {
            return s & xio::type::Postfix;
        }
        [[nodiscard]] bool is_instruction() const
        {
            return !(s & (xio::type::Operator | xio::type::Leaf));
        }
        [[nodiscard]] bool is_value() const
        {
            return _flags.V;
        }

        static xio::type::T type_of(mnemonic m);
    };
}

