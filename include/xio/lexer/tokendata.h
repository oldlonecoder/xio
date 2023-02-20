#pragma once

#include <xio/types.h>
#include <tlux/stracc.h>


namespace xio
{

    class token_data
    {
    public:
        xio::mnemonic       c = mnemonic::Noop;
        xio::type::T  t = xio::type::Null;    ///< Primitive xio::type bit.
        xio::type::T  s = xio::type::Null;    ///< Semantic xio::type bits field
        xio::type::distance::T d = xio::type::distance::noop_;
        void* vdata = nullptr;

        using collection = std::vector<token_data>;
        // ----------------------------------------
        using cache = std::vector<token_data*>;
        using pointer = cache::iterator;
        // ----------------------------------------
        using iterator = collection::iterator;


        struct location_data
        {
            const char* begin = nullptr;
            const char* end = nullptr;
            int     linenum = -1; ///< line number
            int     colnum = -1; ///< Column number
            int64_t offset = -1; ///< Absolute Offset from the beginning of the Source Text.
            int     ln = 0; ///< Length of this token attribute.
            std::string operator()() const;
            std::string text() const;
            std::string position() const;
        }  mLoc;

        struct  Flag
        {
            uint8_t V : 1; ///< Pre-parsed as a value Token;
            uint8_t S : 1; ///< Post parsed as assignable
            uint8_t M : 1; ///< Virtual multiplication operator. (...4ac...)
        }   _flags = { 0, 0, 0 };

        static token_data mNull;

        token_data() = default;
        ~token_data() = default;

        token_data(mnemonic Code_, xio::type::T Type_, xio::type::T Sem_, xio::type::distance::T Delta_, lexem::T aLexem, uint8_t V);
        token_data(mnemonic Code_, xio::type::T Type_, xio::type::T Sem_, xio::type::distance::T Delta_, token_data::location_data LocationData_, token_data::Flag Flags_, void* Ptr_ = nullptr);
        token_data(const token_data& Token_);
        token_data(token_data&& Token_) noexcept;

        token_data& operator=(token_data&& Token_) noexcept;
        token_data& operator=(const token_data& Token_);
        token_data operator[](mnemonic CC);
        //ArgToken* ptr() { return this; }
        std::string mark(int nspc = 0) const;

        explicit operator bool() const
        {
            return mLoc.begin != nullptr;
        }
        bool operator||(xio::type::T Sem_) const
        {
            return s || Sem_;
        }

        [[nodiscard]] std::string text() const
        {
            if ((_flags.M) && (c == mnemonic::Mul)) return lexem::Multiply; // Overwrite source location.
            return mLoc.text();
        }
        [[nodiscard]] std::string location() const;
        [[nodiscard]] std::string semantic_types() const;
        [[nodiscard]] std::string type_name() const;
        [[nodiscard]] std::string details(bool Mark_ = false) const;

        static token_data scan(const char* C_);
        static std::string dump_token_table();

        [[nodiscard]] bool is_text() const
        {
            return t == type::Text;
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


        static xio::type::T ktype(mnemonic m);
    };
}

