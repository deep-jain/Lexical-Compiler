#include "parse.h"
#include "gettoken.h"
#include "tree.h"

static int error_count = 0;

void ParseError(int line, string msg)
{
    ++error_count;
    cout << line + 1 << ": " << msg << endl;
}

ParseTree *Prog(istream &in, int &line)
{
    ParseTree *sl = Slist(in, line);

    if (GetToken::Get(in, line) != DONE)
        ParseError(line, "Unrecognized statement");

    if (sl == 0)
        ParseError(line, "No statements in program");

    if (error_count)
        return 0;

    return sl;
}

ParseTree *Slist(istream &in, int &line)
{

    ParseTree *s = Stmt(in, line);
    if (s == 0)
        return 0;

    Token t;

    if ((t = GetToken::Get(in, line)) != SC && t != NL)
    {

        ParseError(line, "Missing statement separator");
        return 0;
    }

    return new StmtList(s, Slist(in, line));
}

ParseTree *Stmt(istream &in, int &line)
{
    ParseTree *s = 0;

    Token t = GetToken::Get(in, line);
    switch (t.GetTokenType())
    {
    case IF:
        s = IfStmt(in, line);
        break;

    case PRINT:
        s = PrintStmt(in, line);
        break;

    case SET:
        s = SetStmt(in, line);
        break;

    case LOOP:
        s = LoopStmt(in, line);
        break;

    case ERR:
        ParseError(line, "Invalid token");
        break;

    case DONE:
        break;

    case NL:
    case SC:
        s = Stmt(in, line);
        break;

    default:
        GetToken::PushBack(t);
        break;
    }

    return s;
}

ParseTree *IfStmt(istream &in, int &line)
{

    ParseTree *expression = Expr(in, line);
    if (expression == 0)
    {
        ParseError(line, "Missing expression after if");
        return 0;
    }

    Token t2 = GetToken::Get(in, line);
    if (t2.GetTokenType() != BEGIN)
    {
        ParseError(line, "Missing BEGIN after expression");
        return 0;
    }
    ParseTree *statement = Slist(in, line);

    if (statement == 0)
    {
        ParseError(line, "Missing statement after BEGIN");
        return 0;
    }

    Token t3 = GetToken::Get(in, line);
    if (t3.GetTokenType() != END)
    {
        ParseError(line, "Missing END after statement");
        return 0;
    }

    return new IfStatement(line, expression, statement);
}

ParseTree *LoopStmt(istream &in, int &line)
{
    ParseTree *expression = Expr(in, line);
    if (expression == 0)
    {
        ParseError(line, "Missing expression after loop");
        return 0;
    }

    Token t2 = GetToken::Get(in, line);
    if (t2.GetTokenType() != BEGIN)
    {
        ParseError(line, "Missing BEGIN after expression");
        return 0;
    }
    ParseTree *statement = Slist(in, line);

    if (statement == 0)
    {
        ParseError(line, "Missing statement list after BEGIN");
        return 0;
    }

    Token t3 = GetToken::Get(in, line);
    if (t3.GetTokenType() != END)
    {
        ParseError(line, "Missing END after statement THIS ONE");
        return 0;
    }

    return new LoopStatement(line, expression, statement);
}

ParseTree *SetStmt(istream &in, int &line)
{

    Token t = GetToken::Get(in, line);

    if (t.GetTokenType() != ID)
    {
        ParseError(line, "Missing ID after set");
        return 0;
    }

    ParseTree *expression = Expr(in, line);

    if (expression == 0)
    {
        ParseError(line, "Missing expression after id");
        return 0;
    }

    Token t2 = GetToken::Get(in, line);

    if (t2.GetTokenType() == NL || t2.GetTokenType() == SC)
    {

        GetToken::PushBack(t2);
    }

    return new SetStatement(line, t.GetLexeme(), expression);
}

ParseTree *PrintStmt(istream &in, int &line)
{

    ParseTree *expression = Expr(in, line);

    if (expression == 0)
    {
        ParseError(line, "Missing expression after print");
        return 0;
    }

    Token t1 = GetToken::Get(in, line);

    if (t1.GetTokenType() == NL || t1.GetTokenType() != SC)
    {
        GetToken::PushBack(t1);
    }

    else
    {

        GetToken::PushBack(t1);
    }

    return new PrintStatement(line, expression);
}

ParseTree *Expr(istream &in, int &line)
{
    ParseTree *t1 = Prod(in, line);
    if (t1 == 0)
    {
        return 0;
    }

    while (true)
    {
        Token t = GetToken::Get(in, line);

        if (t != PLUS && t != MINUS)
        {
            GetToken::PushBack(t);
            return t1;
        }

        ParseTree *t2 = Prod(in, line);
        if (t2 == 0)
        {
            ParseError(line, "Missing expression after operator");
            return 0;
        }

        if (t == PLUS)
            t1 = new Addition(t.GetLinenum(), t1, t2);
        else
            t1 = new Subtraction(t.GetLinenum(), t1, t2);
    }

    return t1;
}

ParseTree *Prod(istream &in, int &line)
{
    ParseTree *t1 = Primary(in, line);
    if (t1 == 0)
    {
        return 0;
    }

    while (true)
    {
        Token t = GetToken::Get(in, line);

        if (t != STAR && t != SLASH)
        {
            GetToken::PushBack(t);
            return t1;
        }

        ParseTree *t2 = Primary(in, line);
        if (t2 == 0)
        {
            ParseError(line, "Missing expression after operator");
            return 0;
        }

        if (t == STAR)
        {
            t1 = new Multiplication(t.GetLinenum(), t1, t2);
        }

        else if (t == SLASH)
        {
            t1 = new Division(t.GetLinenum(), t1, t2);
        }
    }
    return t1;
}

ParseTree *Primary(istream &in, int &line)
{
    Token t = GetToken::Get(in, line);

    if (t.GetTokenType() == ID)
    {

        return new Ident(t);
    }

    else if (t.GetTokenType() == ICONST)
    {

        return new IConst(line, stoi(t.GetLexeme()));
    }

    else if (t.GetTokenType() == SCONST)
    {

        return new SConst(line, t.GetLexeme());
    }

    else if (t.GetTokenType() == LPAREN)
    {
        ParseTree *expression = Expr(in, line);

        if (expression == 0)
        {
            return 0;
        }

        t = GetToken::Get(in, line);
        if (t.GetTokenType() != RPAREN)
        {
            ParseError(line, "Missing ) after expression");
            return 0;
        }

        return expression;
    }

    ParseError(line, "Primary expected");
    return 0;
}
