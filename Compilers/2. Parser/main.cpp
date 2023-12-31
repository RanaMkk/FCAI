#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
using namespace std;
bool Equals(const char* a, const char* b)
{
    return strcmp(a, b) == 0;
}
bool StartsWith(const char* a, const char* b)
{
    int nb = strlen(b);
    return strncmp(a, b, nb) == 0;
}
void Copy(char* a, const char* b, int n = 0)
{
    if (n > 0) { strncpy(a, b, n); a[n] = 0; }
    else strcpy(a, b);
}
void AllocateAndCopy(char** a, const char* b)
{
    if (b == 0) { *a = 0; return; }
    int n = strlen(b);
    *a = new char[n + 1];
    strcpy(*a, b);
}
#define MAX_LINE_LENGTH 10000
struct InFile
{
    FILE* file;
    int cur_line_num;
    char line_buf[MAX_LINE_LENGTH];
    int cur_ind, cur_line_size;
    InFile(const char* str) { file = 0; if (str) file = fopen(str, "r"); cur_line_size = 0; cur_ind = 0; cur_line_num = 0; }
    ~InFile() { if (file) fclose(file); }
    void SkipSpaces()
    {
        while (cur_ind < cur_line_size)
        {
            char ch = line_buf[cur_ind];
            if (ch != ' ' && ch != '\t' && ch != '\r' && ch != '\n') break;
            cur_ind++;
        }
    }
    bool SkipUpto(const char* str)
    {
        while (true)
        {
            SkipSpaces();
            while (cur_ind >= cur_line_size) { if (!GetNewLine()) return false; SkipSpaces(); }

            if (StartsWith(&line_buf[cur_ind], str))
            {
                cur_ind += strlen(str);
                return true;
            }
            cur_ind++;
        }
        return false;
    }
    bool GetNewLine()
    {
        cur_ind = 0; line_buf[0] = 0;
        if (!fgets(line_buf, MAX_LINE_LENGTH, file)) return false;
        cur_line_size = strlen(line_buf);
        if (cur_line_size == 0) return false; // End of file
        cur_line_num++;
        return true;
    }
    char* GetNextTokenStr()
    {
        SkipSpaces();
        while (cur_ind >= cur_line_size) { if (!GetNewLine()) return 0; SkipSpaces(); }
        return &line_buf[cur_ind];
    }
    void Advance(int num)
    {
        cur_ind += num;
    }
};
struct OutFile
{
    FILE* file;
    OutFile(const char* str) { file = 0; if (str) file = fopen(str, "w"); }
    ~OutFile() { if (file) fclose(file); }
    void Out(const char* s)
    {
        fprintf(file, "%s\n", s); fflush(file);
    }
    void Out(string s)
    {
        fprintf(file, "%s\n", s.c_str()); fflush(file);
    }
};
struct CompilerInfo
{
    InFile in_file;
    OutFile out_file;
    OutFile debug_file;
    CompilerInfo(const char* in_str, const char* out_str, const char* debug_str)
            : in_file(in_str), out_file(out_str), debug_file(debug_str)
    {
    }
};
#define MAX_TOKEN_LEN 40
enum TokenType {
    IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE,
    ASSIGN, EQUAL, LESS_THAN,
    PLUS, MINUS, TIMES, DIVIDE, POWER,
    SEMI_COLON,
    LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACE, RIGHT_BRACE,
    ID, NUM,
    ENDFILE, ERROR
};
const char* TokenTypeStr[] ={
        "If", "Then", "Else", "End", "Repeat", "Until", "Read", "Write",
        "Assign", "Equal", "LessThan",
        "Plus", "Minus", "Times", "Divide", "Power",
        "SemiColon",
        "LeftParen", "RightParen",
        "LeftBrace", "RightBrace",
        "ID", "Num",
        "EndFile", "Error"
};
struct Token
{
    TokenType type;
    char str[MAX_TOKEN_LEN + 1];
    Token() { str[0] = 0; type = ERROR; }
    Token(TokenType _type, const char* _str) { type = _type; Copy(str, _str); }
};
const Token reserved_words[] =
        {
                Token(IF, "if"),
                Token(THEN, "then"),
                Token(ELSE, "else"),
                Token(END, "end"),
                Token(REPEAT, "repeat"),
                Token(UNTIL, "until"),
                Token(READ, "read"),
                Token(WRITE, "write")
        };
const int num_reserved_words = sizeof(reserved_words) / sizeof(reserved_words[0]);
const Token symbolic_tokens[] =
        {
                Token(ASSIGN, ":="),
                Token(EQUAL, "="),
                Token(LESS_THAN, "<"),
                Token(PLUS, "+"),
                Token(MINUS, "-"),
                Token(TIMES, "*"),
                Token(DIVIDE, "/"),
                Token(POWER, "^"),
                Token(SEMI_COLON, ";"),
                Token(LEFT_PAREN, "("),
                Token(RIGHT_PAREN, ")"),
                Token(LEFT_BRACE, "{"),
                Token(RIGHT_BRACE, "}")
        };
const int num_symbolic_tokens = sizeof(symbolic_tokens) / sizeof(symbolic_tokens[0]);
inline bool IsDigit(char ch) { return (ch >= '0' && ch <= '9'); }
inline bool IsLetter(char ch) { return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')); }
inline bool IsLetterOrUnderscore(char ch) { return (IsLetter(ch) || ch == '_'); }
void GetNextToken(CompilerInfo* pci, Token* ptoken)
{
    ptoken->type = ERROR;
    ptoken->str[0] = 0;

    int i;
    char* s = pci->in_file.GetNextTokenStr();
    if (!s)
    {
        ptoken->type = ENDFILE;
        ptoken->str[0] = 0;
        return;
    }

    for (i = 0; i < num_symbolic_tokens; i++)
    {
        if (StartsWith(s, symbolic_tokens[i].str))
            break;
    }

    if (i < num_symbolic_tokens)
    {
        if (symbolic_tokens[i].type == LEFT_BRACE)
        {
            pci->in_file.Advance(strlen(symbolic_tokens[i].str));
            if (!pci->in_file.SkipUpto(symbolic_tokens[i + 1].str)) return;
            return GetNextToken(pci, ptoken);
        }
        ptoken->type = symbolic_tokens[i].type;
        Copy(ptoken->str, symbolic_tokens[i].str);
    }
    else if (IsDigit(s[0]))
    {
        int j = 1;
        while (IsDigit(s[j])) j++;

        ptoken->type = NUM;
        Copy(ptoken->str, s, j);
    }
    else if (IsLetterOrUnderscore(s[0]))
    {
        int j = 1;
        while (IsLetterOrUnderscore(s[j])) j++;

        ptoken->type = ID;
        Copy(ptoken->str, s, j);

        for (i = 0; i < num_reserved_words; i++)
        {
            if (Equals(ptoken->str, reserved_words[i].str))
            {
                ptoken->type = reserved_words[i].type;
                break;
            }
        }
    }

    int len = strlen(ptoken->str);
    if (len > 0) pci->in_file.Advance(len);
}
enum NodeKind {
    IF_NODE, REPEAT_NODE, ASSIGN_NODE, READ_NODE, WRITE_NODE,
    OPER_NODE, NUM_NODE, ID_NODE
};
const char* NodeKindStr[] =
        {
                "If", "Repeat", "Assign", "Read", "Write",
                "Oper", "Num", "ID"
        };
enum ExprDataType { VOID, INTEGER, BOOLEAN };
const char* ExprDataTypeStr[] =
        {
                "Void", "Integer", "Boolean"
        };
#define MAX_CHILDREN 3
struct TreeNode
{
    TreeNode* child[MAX_CHILDREN];
    TreeNode* sibling;
    NodeKind node_kind;
    union { TokenType oper; int num; char* id; }; // defined for expression/int/identifier only
    ExprDataType expr_data_type; // defined for expression/int/identifier only
    int line_num;
    TreeNode() { int i; for (i = 0; i < MAX_CHILDREN; i++) child[i] = 0; sibling = 0; expr_data_type = VOID; }
};
struct ParseInfo
{
    Token next_token;
};
void Match(CompilerInfo* pci, ParseInfo* ppi, TokenType expected_token_type)
{
    if (ppi->next_token.type != expected_token_type) throw 0;
    GetNextToken(pci, &ppi->next_token);
}
TreeNode* MathExpr(CompilerInfo*, ParseInfo*);
TreeNode* NewExpr(CompilerInfo* pci, ParseInfo* ppi)
{
    pci->debug_file.Out("Start NewExpr");
    if (ppi->next_token.type == NUM)
    {
        TreeNode* tree = new TreeNode;
        tree->node_kind = NUM_NODE;
        char* num_str = ppi->next_token.str;
        tree->num = 0; while (*num_str) tree->num = tree->num * 10 + ((*num_str++) - '0');
        tree->line_num = pci->in_file.cur_line_num;
        Match(pci, ppi, ppi->next_token.type);
        pci->debug_file.Out("End NewExpr");
        return tree;
    }
    if (ppi->next_token.type == ID)
    {
        TreeNode* tree = new TreeNode;
        tree->node_kind = ID_NODE;
        AllocateAndCopy(&tree->id, ppi->next_token.str);
        tree->line_num = pci->in_file.cur_line_num;
        Match(pci, ppi, ppi->next_token.type);

        pci->debug_file.Out("End NewExpr");
        return tree;
    }
    if (ppi->next_token.type == LEFT_PAREN)
    {
        Match(pci, ppi, LEFT_PAREN);
        TreeNode* tree = MathExpr(pci, ppi);
        Match(pci, ppi, RIGHT_PAREN);

        pci->debug_file.Out("End NewExpr");
        return tree;
    }
    throw 0;
    return 0;
}
TreeNode* Factor(CompilerInfo* pci, ParseInfo* ppi)
{
    pci->debug_file.Out("Start Factor");
    TreeNode* tree = NewExpr(pci, ppi);
    if (ppi->next_token.type == POWER)
    {
        TreeNode* new_tree = new TreeNode;
        new_tree->node_kind = OPER_NODE;
        new_tree->oper = ppi->next_token.type;
        new_tree->line_num = pci->in_file.cur_line_num;
        new_tree->child[0] = tree;
        Match(pci, ppi, ppi->next_token.type);
        new_tree->child[1] = Factor(pci, ppi);
        pci->debug_file.Out("End Factor");
        return new_tree;
    }
    pci->debug_file.Out("End Factor");
    return tree;
}
TreeNode* Term(CompilerInfo* pci, ParseInfo* ppi)
{
    pci->debug_file.Out("Start Term");
    TreeNode* tree = Factor(pci, ppi);
    while (ppi->next_token.type == TIMES || ppi->next_token.type == DIVIDE)
    {
        TreeNode* new_tree = new TreeNode;
        new_tree->node_kind = OPER_NODE;
        new_tree->oper = ppi->next_token.type;
        new_tree->line_num = pci->in_file.cur_line_num;

        new_tree->child[0] = tree;
        Match(pci, ppi, ppi->next_token.type);
        new_tree->child[1] = Factor(pci, ppi);

        tree = new_tree;
    }
    pci->debug_file.Out("End Term");
    return tree;
}
TreeNode* MathExpr(CompilerInfo* pci, ParseInfo* ppi)
{
    pci->debug_file.Out("Start MathExpr");
    TreeNode* tree = Term(pci, ppi);
    while (ppi->next_token.type == PLUS || ppi->next_token.type == MINUS)
    {
        TreeNode* new_tree = new TreeNode;
        new_tree->node_kind = OPER_NODE;
        new_tree->oper = ppi->next_token.type;
        new_tree->line_num = pci->in_file.cur_line_num;

        new_tree->child[0] = tree;
        Match(pci, ppi, ppi->next_token.type);
        new_tree->child[1] = Term(pci, ppi);

        tree = new_tree;
    }
    pci->debug_file.Out("End MathExpr");
    return tree;
}
TreeNode* Expr(CompilerInfo* pci, ParseInfo* ppi)
{
    pci->debug_file.Out("Start Expr");
    TreeNode* tree = MathExpr(pci, ppi);
    if (ppi->next_token.type == EQUAL || ppi->next_token.type == LESS_THAN)
    {
        TreeNode* new_tree = new TreeNode;
        new_tree->node_kind = OPER_NODE;
        new_tree->oper = ppi->next_token.type;
        new_tree->line_num = pci->in_file.cur_line_num;

        new_tree->child[0] = tree;
        Match(pci, ppi, ppi->next_token.type);
        new_tree->child[1] = MathExpr(pci, ppi);

        pci->debug_file.Out("End Expr");
        return new_tree;
    }
    pci->debug_file.Out("End Expr");
    return tree;
}
TreeNode* WriteStmt(CompilerInfo* pci, ParseInfo* ppi)
{
    pci->debug_file.Out("Start WriteStmt");
    TreeNode* tree = new TreeNode;
    tree->node_kind = WRITE_NODE;
    tree->line_num = pci->in_file.cur_line_num;
    Match(pci, ppi, WRITE);
    tree->child[0] = Expr(pci, ppi);
    pci->debug_file.Out("End WriteStmt");
    return tree;
}
TreeNode* ReadStmt(CompilerInfo* pci, ParseInfo* ppi)
{
    pci->debug_file.Out("Start ReadStmt");
    TreeNode* tree = new TreeNode;
    tree->node_kind = READ_NODE;
    tree->line_num = pci->in_file.cur_line_num;

    Match(pci, ppi, READ);
    if (ppi->next_token.type == ID) AllocateAndCopy(&tree->id, ppi->next_token.str);
    Match(pci, ppi, ID);

    pci->debug_file.Out("End ReadStmt");
    return tree;
}
TreeNode* AssignStmt(CompilerInfo* pci, ParseInfo* ppi)
{
    pci->debug_file.Out("Start AssignStmt");
    TreeNode* tree = new TreeNode;
    tree->node_kind = ASSIGN_NODE;
    tree->line_num = pci->in_file.cur_line_num;
    if (ppi->next_token.type == ID) AllocateAndCopy(&tree->id, ppi->next_token.str);
    Match(pci, ppi, ID);
    Match(pci, ppi, ASSIGN); tree->child[0] = Expr(pci, ppi);
    pci->debug_file.Out("End AssignStmt");
    return tree;
}
TreeNode* StmtSeq(CompilerInfo*, ParseInfo*);
TreeNode* RepeatStmt(CompilerInfo* pci, ParseInfo* ppi)
{
    pci->debug_file.Out("Start RepeatStmt");
    TreeNode* tree = new TreeNode;
    tree->node_kind = REPEAT_NODE;
    tree->line_num = pci->in_file.cur_line_num;
    Match(pci, ppi, REPEAT); tree->child[0] = StmtSeq(pci, ppi);
    Match(pci, ppi, UNTIL); tree->child[1] = Expr(pci, ppi);
    pci->debug_file.Out("End RepeatStmt");
    return tree;
}
TreeNode* IfStmt(CompilerInfo* pci, ParseInfo* ppi)
{
    pci->debug_file.Out("Start IfStmt");
    TreeNode* tree = new TreeNode;
    tree->node_kind = IF_NODE;
    tree->line_num = pci->in_file.cur_line_num;

    Match(pci, ppi, IF); tree->child[0] = Expr(pci, ppi);
    Match(pci, ppi, THEN); tree->child[1] = StmtSeq(pci, ppi);
    if (ppi->next_token.type == ELSE) { Match(pci, ppi, ELSE); tree->child[2] = StmtSeq(pci, ppi); }
    Match(pci, ppi, END);

    pci->debug_file.Out("End IfStmt");
    return tree;
}
TreeNode* Stmt(CompilerInfo* pci, ParseInfo* ppi)
{
    pci->debug_file.Out("Start Stmt");
    TreeNode* tree = 0;
    if (ppi->next_token.type == IF) tree = IfStmt(pci, ppi);
    else if (ppi->next_token.type == REPEAT) tree = RepeatStmt(pci, ppi);
    else if (ppi->next_token.type == ID) tree = AssignStmt(pci, ppi);
    else if (ppi->next_token.type == READ) tree = ReadStmt(pci, ppi);
    else if (ppi->next_token.type == WRITE) tree = WriteStmt(pci, ppi);
    else throw 0;

    pci->debug_file.Out("End Stmt");
    return tree;
}
TreeNode* StmtSeq(CompilerInfo* pci, ParseInfo* ppi)
{
    pci->debug_file.Out("Start StmtSeq");

    TreeNode* first_tree = Stmt(pci, ppi);
    TreeNode* last_tree = first_tree;

    // If we did not reach one of the Follow() of StmtSeq(), we are not done yet
    while (ppi->next_token.type != ENDFILE && ppi->next_token.type != END &&
           ppi->next_token.type != ELSE && ppi->next_token.type != UNTIL)
    {
        Match(pci, ppi, SEMI_COLON);
        TreeNode* next_tree = Stmt(pci, ppi);
        last_tree->sibling = next_tree;
        last_tree = next_tree;
    }

    pci->debug_file.Out("End StmtSeq");
    return first_tree;
}
TreeNode* Parse(CompilerInfo* pci)
{
    ParseInfo parse_info;
    GetNextToken(pci, &parse_info.next_token);

    TreeNode* syntax_tree = StmtSeq(pci, &parse_info);

    if (parse_info.next_token.type != ENDFILE)
        pci->debug_file.Out("Error code ends before file ends");

    return syntax_tree;
}
void PrintTree(TreeNode* node, int sh = 0)
{
    int i, NSH = 3;
    for (i = 0; i < sh; i++) printf(" ");

    printf("[%s]", NodeKindStr[node->node_kind]);

    if (node->node_kind == OPER_NODE) printf("[%s]", TokenTypeStr[node->oper]);
    else if (node->node_kind == NUM_NODE) printf("[%d]", node->num);
    else if (node->node_kind == ID_NODE || node->node_kind == READ_NODE || node->node_kind == ASSIGN_NODE) printf("[%s]", node->id);

    if (node->expr_data_type != VOID) printf("[%s]", ExprDataTypeStr[node->expr_data_type]);

    printf("\n");

    for (i = 0; i < MAX_CHILDREN; i++) if (node->child[i]) PrintTree(node->child[i], sh + NSH);
    if (node->sibling) PrintTree(node->sibling, sh);
}
void DestroyTree(TreeNode* node)
{
    int i;

    if (node->node_kind == ID_NODE || node->node_kind == READ_NODE || node->node_kind == ASSIGN_NODE)
        if (node->id) delete[] node->id;

    for (i = 0; i < MAX_CHILDREN; i++) if (node->child[i]) DestroyTree(node->child[i]);
    if (node->sibling) DestroyTree(node->sibling);

    delete node;
}
int main()
{
    // Defining a compiler info object
    CompilerInfo compiler("input.txt", "output.txt", "debug.txt");

    // Defining the root node of the tree
    TreeNode* root = Parse(&compiler);
    cout<<"\nTHE SYNTAX TREE\n";
    PrintTree(root);
    cout<<"\n\n";

    return 0;
}