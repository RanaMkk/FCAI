#include <cstdio>
#include <cstring>

using namespace std;

// Utility functions and structures from the provided file
// Strings /////////////////////////////////////////////////////////////////////////

bool Equals(const char* a, const char* b)
{
    return strcmp(a, b)==0;
}

bool StartsWith(const char* a, const char* b)
{
    int nb= (int)strlen(b);
    return strncmp(a, b, nb)==0;
}

void Copy(char* a, const char* b, int n=0)
{
    if(n>0) {strncpy(a, b, n); a[n]=0;}
    else strcpy(a, b);
}

////////////////////////////////////////////////////////////////////////////////////
// Input and Output ////////////////////////////////////////////////////////////////

#define MAX_LINE_LENGTH 10000

struct InFile
{
    FILE* file;
    int cur_line_num;

    char line_buf[MAX_LINE_LENGTH];
    int cur_ind, cur_line_size;

    InFile(const char* str) {file=0; if(str) file=fopen(str, "r"); cur_line_size=0; cur_ind=0; cur_line_num=0;}
    ~InFile(){if(file) fclose(file);}

    void SkipSpaces()
    {
        while(cur_ind<cur_line_size)
        {
            char ch=line_buf[cur_ind];
            if(ch!=' ' && ch!='\t' && ch!='\r' && ch!='\n') break;
            cur_ind++;
        }
    }

    bool SkipUpto(const char* str)
    {
        while(true)
        {
            SkipSpaces();
            while(cur_ind>=cur_line_size) {if(!GetNewLine()) return false; SkipSpaces();}

            if(StartsWith(&line_buf[cur_ind], str))
            {
                cur_ind+=strlen(str);
                return true;
            }
            cur_ind++;
        }
        return false;
    }

    bool GetNewLine()
    {
        cur_ind=0; line_buf[0]=0;
        if(!fgets(line_buf, MAX_LINE_LENGTH, file)) return false;
        cur_line_size=strlen(line_buf);
        if(cur_line_size==0) return false; // End of file
        cur_line_num++;
        return true;
    }

    char* GetNextTokenStr()
    {
        SkipSpaces();
        while(cur_ind>=cur_line_size) {if(!GetNewLine()) return 0; SkipSpaces();}
        return &line_buf[cur_ind];
    }

    void Advance(int num)
    {
        cur_ind+=num;
    }
};

struct OutFile
{
    FILE* file;
    OutFile(const char* str) {file=0; if(str) file=fopen(str, "w");}
    ~OutFile(){if(file) fclose(file);}

    void Out(const char* s)
    {
        fprintf(file, "%s\n", s); fflush(file);
    }
};

////////////////////////////////////////////////////////////////////////////////////
// Compiler Parameters /////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////
// Scanner /////////////////////////////////////////////////////////////////////////

#define MAX_TOKEN_LEN 40

enum TokenType{
    IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE,
    ASSIGN, EQUAL, LESS_THAN,
    PLUS, MINUS, TIMES, DIVIDE, POWER,
    SEMI_COLON,
    LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACE, RIGHT_BRACE,
    ID, NUM,
    ENDFILE, ERROR
};

// Used for debugging only /////////////////////////////////////////////////////////
const char* TokenTypeStr[]=
        {
                "If", "Then", "Else", "End", "Repeat", "Until", "Read", "Write",
                "Assign", "Equal", "LessThan",
                "Plus", "Minus", "Times", "Divide", "Power",
                "SemiColon",
                "LeftParen", "RightParen",
                "LeftBrace", "RightBrace",
                "ID", "Num",
                "EndFile", "Error"
        };
// Token structure
struct Token {
    TokenType type;
    char str[MAX_TOKEN_LEN + 1];

    Token() {
        str[0] = 0;
        type = ERROR;
    }
    Token(TokenType _type, const char* _str) {
        type = _type;
        Copy(str, _str);
    }
};



const Token reserved_words[]=
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
const int num_reserved_words=sizeof(reserved_words)/sizeof(reserved_words[0]);

// if there is tokens like < <=, sort them such that sub-tokens come last: <= <
// the closing comment should come immediately after opening comment
const Token symbolic_tokens[]=
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
const int num_symbolic_tokens=sizeof(symbolic_tokens)/sizeof(symbolic_tokens[0]);

inline bool IsDigit(char ch){return (ch>='0' && ch<='9');}
inline bool IsLetter(char ch){return ((ch>='a' && ch<='z') || (ch>='A' && ch<='Z'));}


// Scanner class
class Scanner {
public:
    Scanner(InFile& inFile, OutFile& outFile) : in_file(inFile), out_file(outFile) {}

    void Scan() {
        Token token;
        do {
            token = GetNextToken();
            PrintToken(token);
        } while (token.type != ENDFILE);
    }

private:
    InFile& in_file;
    OutFile& out_file;

    Token GetNextToken() {
        Token token;
        char* token_str = in_file.GetNextTokenStr();

        if (!token_str) {
            token.type = ENDFILE;
            return token;
        }
        // Skip comments
        if (token_str[0] == '{') {
            in_file.SkipUpto("}");
            in_file.Advance(1); // Skip the '}'
            return GetNextToken(); // Get the next token after the comment
        }
        // Check for reserved words
        for (int i = 0; i < num_reserved_words; ++i) {
            if (Equals(token_str, reserved_words[i].str)) {
                token.type = reserved_words[i].type;
                Copy(token.str, token_str);
                return token;
            }
        }

        // Check for symbolic tokens
        for (int i = 0; i < num_symbolic_tokens; ++i) {
            if (StartsWith(token_str, symbolic_tokens[i].str)) {
                token.type = symbolic_tokens[i].type;
                Copy(token.str, token_str, strlen(symbolic_tokens[i].str));
                return token;
            }
        }

        // Check for identifiers or numbers
        if (IsLetter(*token_str)) {
            token.type = ID;
            Copy(token.str, token_str);
        } else if (IsDigit(*token_str)) {
            token.type = NUM;
            Copy(token.str, token_str);
        }

        in_file.Advance(strlen(token_str));
        return token;
    }

    void PrintToken(const Token& token) {
        char output[100];
        sprintf(output, "[%d] %s (%s)", in_file.cur_line_num, token.str, TokenTypeStr[token.type]);
        out_file.Out(output);
    }
};

int main() {
    // Create CompilerInfo object with input and output file names
    CompilerInfo compilerInfo("input.txt", "output.txt", "debug.txt");

    // Create Scanner object
    Scanner scanner(compilerInfo.in_file, compilerInfo.out_file);

    // Perform scanning (Tokenization) and print tokens to the output file
    scanner.Scan();

    return 0;
}
