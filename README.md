# Lexical-Compiler

##Lexical Compiler Used the following grammar
```
Prog := Slist
Slist := Ssep { Slist } | Stmt Ssep { Slist }
Ssep := NL | SC
Stmt := IfStmt | PrintStmt | SetStmt | LoopStmt
IfStmt := IF Expr BEGIN Slist END
PrintStmt := PRINT Expr
SetStmt := SET IDENT Expr
LoopStmt := LOOP Expr BEGIN Slist END
Expr := Prod { (PLUS | MINUS) Prod }
Prod := Primary { (STAR | SLASH) Primary }
Primary := IDENT | ICONST | SCONST | LPAREN Expr RPAREN
```
