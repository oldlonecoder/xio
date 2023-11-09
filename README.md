# xio eXecutable Intruction Object
My personal c++ not modern way to experiment coding a machineless scripting interpreter.

# List of main component and classes:
---
- <h5>Spp::Alu</h5> Arithmetic Logical Unit or Accumulator, component of xio.
---
 - <h5>Spp::xio</h5> Instruction Object node.
---
- <h5>Spp::Stack</h5> Scope instructions-list/stack bloc".
---
- <h5>Spp::SppToken</h5> token: type, (script) source-location, operations, and mnemonic informations".
---
- <h5>Spp::SppTypes</h5> Types and enum declarations for the xio env: Spp::Type; Spp::Mnemonic; Spp::Lexem; Spp::Distance.
---
- <h5>Spp::Unit</h5> Sibbling Units; Kind of 'Modules' within the Interpreter's Stack/(global)scope.
---
- <h5>Spp::Variable</h5> Spp variables holding primitive types and reference to runtime data and functions.
---
- <h5>Spp::RTBase</h5> Spp Runtime base objects holding full-duplex/half-duplex data to the runtime resources.
--- 
- <h5>RTFunction</h5>  Spp object to Runtime Functions/Class method.  ( not available yet )
--- 
- <h5>Spp::Lexer</h5>  The Spp lexical analyser and pre-arithmetic syntax parser; SppTokens stream producer.
---
- <h5>Spp::Compiler</h5> The Spp xio intructions/trees producer.
--- 
- <h5>Spp::Lang::Grammar</h5>  put aside-experimentation of Spp (language) Grammar.
---
- <h5>Spp::Interpreter</h5>  The Spp Interpreter ...
- ...
