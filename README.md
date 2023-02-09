
# xio [ eXecutable Instruction Object ]

# --------------------------------------------------- note  --------------------------
 //@note this section (note/note end) will be removed.

# By order of dependancy:
* utf       : Basic glyph support. ( beware: not utf-* standard, using litteraly ...hardcoded const char* )
              For now it is french accents and some glyphes from the awesome (nerd) fonts.
* textattr  : Define and manage everything about text attributes (color/pairs, ansi256 and html enconding)
* stracc    : Holds instance of a std::string as (input) accumulator
* strbrk    : Using string_view and creates array of tokens using provided or default separators
* diagnostic: Diagnostic (and log) messaging using the above tools, used by the tools enumerated below;
* smtl      : Basic simple markup text utility using the stuff provided by this project.

* In parts of the utils above, I try to support ansi256 and html encoding.
# --------------------------------------------------- note end --------------------------

### Exploring, Learning to write a simple C++ framework of libraries
## libteacc_main : main utils support classes in order of dependency:

---
    - [class teacc_signal]
        C++ template header for basic signal-delegates.
          -> No multi-threads yet, but will be later.
    - [class stracc]
        String accumulator for serializing and formatting supported components
    - [diagnopstic]
        Diagnostic : messages accumulator and logs.
        Note:   This class contents is central because from there, it is used in all below classes.
                The implementation also need access to the diagnostic instance through the application instance.
    - [class application]
        Base application class... Holds the global instance of diagnostic, the configuration data of the components.
    - [class strbrk]
        String breaking into tokens from the text stream and the given tokens stream.
    - [geometry components classes]
        {point<T>, dim<T>, rectangle<T>, winbuffer} Basic 2D geometry components management
    - [class object]
        Base Object class on the parent-child relational logistic
    - [class listener, ifd]
        Base IO listeners for stdin, files and sockets. ( for now, Linux operating systems only )
***




***

## libteacc_stml : Simple Text Markup Language.
---
    - [class text]
        Text class embedding attributes list and the compiler.
        Using (hardcoded) UTF-8 strings ( french accents; icon glyphs )

***

## libteacc_ui : Standard Ansi Console UI. ( future )
---
    -[class console]
        Related console/terminal for init, rendering widget UI components, terminate.
    -[class vdc and vdc::cell]
        Backbuffer encoding/decoding
    -[ui classes]
        All derived from widget, they draw and represent their name.
    Note:
    Backbuffers are created and owned only by Toplevel/floating widget class.
    Children widgets are drawn and confined into the top-level parent.

    Commits on console are using ansi256 ESC sequences for rendering the text attributes.
    Management of the Widget/Windows hierarchy is at application level, not at system level.
    teacc.ui is NOT a text processes/desktop/window manager.
    teacc.ui is an Application API using Widgets on the console for its UI.
    Not a fast UI renderer especially on Microsoft Windows Terminal. It is very,very slow.

***

## libteacc_plugin : Basic support for dynamic teacc components (future) library loader.
---
    teacc-related plugins manager. [More to come]

***

##### I am exploring and learning old & new C++, CMake features:

> I have the intention to blog about everythings I code in
* C++
* Php
* TypeScript
* JS
* Html
* CSS
>That implies to make a web site about the subjet and to write git satus reports engine in a full-stack web project!

CopyRights 2023, Serge Lussier (oldlonecoder@gmail.com)


