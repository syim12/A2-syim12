#include <bits/stdc++.h>
using namespace std;

class identifier
{
public:
    int id;
    string name;

    identifier()
    {
        id = -1;
        name = "\0";
    }

    identifier(int id, string name)
    {
        this->id = id;
        this->name = name;
    }
};

class Symbol_Table
{
public:
    int current_size;
    map<int, identifier> identifiers;

    Symbol_Table()
    {
        current_size = 0;
    }

    int insert(string var)
    {
        bool allocate = true;

        for (auto node : identifiers)
        {

            if (var.compare(node.second.name) == 0)
            {
                allocate = false;
                break;
            }
        }

        if (allocate == false)
            return -1;

        int id = ++current_size;

        identifiers[id] = identifier(id, var);

        return id;
    }

    void del(int id)
    {
        identifiers.erase(id);
    }

    void pop()
    {
        identifiers.erase(current_size);
        current_size--;
    }

} SymbolTable;

string vector_to_string(vector<char> str)
{
    string to_string(str.begin(), str.end());

    return to_string;
}

class Token
{
public:
    string t;

    Token()
    {
        t = "Not an identifiable Token";
    }

    Token(string token_type)
    {
        t = "< " + token_type + " >";
    }

    Token(string token_type, string token_value)
    {
        t = "< " + token_type + " , " + token_value + " >";
    }

    Token(string token_type, int token_value)
    {
        t = "< " + token_type + " , " + to_string(token_value) + " >";
    }

    Token(string token_type, char token_value)
    {
        t = "< " + token_type + " , " + token_value + " >";
    }
};

class keyword_node
{
public:
    char a;
    map<char, keyword_node *> dict;
    bool isTerminal;

    keyword_node(char a = '\0')
    {
        this->a = a;
        isTerminal = false;
    }
};

class keyword_Trie
{
public:
    keyword_node *root;

    keyword_Trie()
    {
        root = new keyword_node();
    }

    void insert(string str)
    {
        keyword_node *temp = root;
        int i = 0;

        while (str[i] != '\0')
        {
            char data = str[i];

            if (temp->dict.count(data) == 0)
                temp->dict[data] = new keyword_node(data);

            temp = temp->dict[data];
            i++;
        }

        temp->isTerminal = true;
    }

    bool nfa(string file_input, vector<Token> &tokens, int &i)
    {
        int pos = i;
        bool valid = false;
        bool loop = true;
        char data;

        vector<char> word;

        keyword_node *temp = root;

        while (loop)
        {

            if (pos <= file_input.length())
                data = file_input[pos];
            else
            {
                loop = false;
                data = '\0';
            }

            if (temp->isTerminal && !isalnum(data))
            {
                valid = true;
                Token T;
                T = Token(vector_to_string(word));
                tokens.push_back(T);
                loop = false;
            }

            if (temp->dict.count(data))
            {
                temp = temp->dict[data];
                pos++;
                word.push_back(data);
            }
            else
            {
                loop = false;
            }
        }

        if (valid == true)
        {
            i = pos;
        }

        return valid;
    }

} keywordTrie;

void initialize_keywords()
{

    int number_of_keywords = 2;

    string keyword[number_of_keywords] = {"if",
                                          "else"};

    for (int i = 0; i < number_of_keywords; i++)
    {
        keywordTrie.insert(keyword[i]);
    }
}

bool getKeyword(string file_input, vector<Token> &tokens, int &i)
{
    int pos = i;

    bool valid = keywordTrie.nfa(file_input, tokens, i);

    return valid;
}

bool getVariable(string file_input, vector<Token> &tokens, int &i)
{

    int pos = i;

    int state = 0;

    bool loop = true;
    bool valid = false;

    vector<char> var;

    char c;

    while (loop)
    {

        if (pos <= file_input.length())
            c = file_input[pos];
        else
        {
            loop = false;
            c = '\0';
        }

        switch (state)
        {

        case 0:
            if (isalpha(c))
            {
                state = 1;
                var.push_back(c);
                pos++;
            }

            else
            {
                loop = false;
            }

            break;

        case 1:
            if (isalnum(c))
            {
                state = 1;
                var.push_back(c);
                pos++;
            }

            else
            {
                state = 2;
                pos++;
            }

            break;

        case 2:
            valid = true;
            loop = false;
            pos--;

            int id = SymbolTable.insert(vector_to_string(var));

            if (id != -1)
            {
                Token T("ID", id);
                tokens.push_back(T);
            }
            else
            {
                Token T;
                tokens.push_back(T);
            }

            break;
        }
    }

    if (valid == true)
        i = pos;

    return valid;
}

bool getArithmetic(string file_input, vector<Token> &tokens, int &i)
{
    bool valid = false;

    char c = file_input[i];

    Token T;

    switch (c)
    {
    case '+':
        valid = true;
        T = Token("ARITH", c);
        tokens.push_back(T);

        break;

    case '-':
        valid = true;
        T = Token("ARITH", c);
        tokens.push_back(T);

        break;

    case '*':
        valid = true;
        T = Token("ARITH", c);
        tokens.push_back(T);

        break;

    case '/':
        valid = true;
        T = Token("ARITH", c);
        tokens.push_back(T);

        break;
    }

    if (valid == true)
        i++;

    return valid;
}

bool getParenthesis(string file_input, vector<Token> &tokens, int &i)
{
    bool valid = false;

    char c = file_input[i];

    Token T;

    switch (c)
    {
    case '(':
        valid = true;
        T = Token("PAREN_OPEN", c);
        tokens.push_back(T);

        break;

    case ')':
        valid = true;
        T = Token("PAREN_CLOSE", c);
        tokens.push_back(T);

        break;

    case '{':
        valid = true;
        T = Token("PAREN_OPEN", c);
        tokens.push_back(T);

        break;

    case '}':
        valid = true;
        T = Token("PAREN_CLOSE", c);
        tokens.push_back(T);

        break;

    case '[':
        valid = true;
        T = Token("PAREN_OPEN", c);
        tokens.push_back(T);

        break;

    case ']':
        valid = true;
        T = Token("PAREN_CLOSE", c);
        tokens.push_back(T);

        break;
    }

    if (valid == true)
        i++;

    return valid;
}

bool getPunctuation(string file_input, vector<Token> &tokens, int &i)
{
    bool valid = false;

    char c = file_input[i];

    Token T;

    switch (c)
    {
    case ';':
        valid = true;
        T = Token("PUNC", c);
        tokens.push_back(T);

        break;

    case '"':
        valid = true;
        T = Token("PUNC", c);
        tokens.push_back(T);

        break;

    case '\'':
        valid = true;
        T = Token("PUNC", c);
        tokens.push_back(T);

        break;
    }

    if (valid == true)
        i++;

    return valid;
}

bool getWhitespace(string file_input, vector<Token> &tokens, int &i)
{
    int pos = i;

    int state = 0;

    bool loop = true;
    bool valid = false;

    char c;

    while (loop)
    {
        if (pos <= file_input.length())
            c = file_input[pos];
        else
        {
            loop = false;
            c = '\0';
        }

        switch (state)
        {

        case 0:
            if (isspace(c))
            {
                state = 1;
                pos++;
            }

            else
            {
                loop = valid = false;
            }

            break;

        case 1:
            if (isspace(c))
            {
                pos++;
            }

            else
            {
                state = 2;
                pos++;
            }

            break;

        case 2:
            valid = true;
            loop = false;
            pos--;
            break;
        }
    }

    if (valid == true)
        i = pos;

    return valid;
}

bool getRelop(string file_input, vector<Token> &tokens, int &i)
{
    int pos = i;

    int state = 0;

    bool loop = true;
    bool valid = false;

    char c;

    while (loop)
    {

        if (pos <= file_input.length())
            c = file_input[pos];
        else
        {
            loop = false;
            c = '\0';
        }

        switch (state)
        {

        case 0:
            if (c == '<')
            {
                pos++;
                state = 1;
            }

            else if (c == '=')
            {
                pos++;
                state = 5;
            }

            else if (c == '>')
            {
                pos++;
                state = 6;
            }

            else
            {
                valid = false;
                loop = false;
            }

            break;

        case 1:
            if (c == '=')
            {
                pos++;
                state = 2;
            }

            else if (c == '>')
            {
                pos++;
                state = 3;
            }

            else
            {
                pos++;
                state = 4;
            }

            break;

        case 2:
        {
            Token T("RELOP", "LE");
            tokens.push_back(T);

            valid = true;
            loop = false;

            break;
        }

        case 3:
        {
            Token T("RELOP", "NE");
            tokens.push_back(T);

            valid = true;
            loop = false;

            break;
        }

        case 4:
        {
            Token T("RELOP", "LT");
            tokens.push_back(T);

            valid = true;
            loop = false;

            break;
        }

        case 5:
        {
            Token T("RELOP", "EQ");
            tokens.push_back(T);

            valid = true;
            loop = false;

            break;
        }

        case 6:
            if (c == '=')
            {
                pos++;
                state = 7;
            }

            else
            {
                pos++;
                state = 8;
            }

            break;

        case 7:
        {
            Token T("RELOP", "GE");
            tokens.push_back(T);

            valid = true;
            loop = false;

            break;
        }

        case 8:
        {
            Token T("RELOP", "GT");
            tokens.push_back(T);

            pos--;
            valid = true;
            loop = false;

            break;
        }
        }
    }

    if (valid == true)
        i = pos;
    return valid;

    bool getNumber(string file_input, vector<Token> & tokens, int &i)
    {
        int pos = i;

        int state = 0;

        bool loop = true;
        bool valid = false;

        vector<char> number_string;

        char c;

        while (loop)
        {
            if (pos <= file_input.length())
                c = file_input[pos];
            else
            {
                loop = false;
                c = '\0';
            }

            switch (state)
            {

            case 0:
                if (isdigit(c))
                {
                    pos++;
                    state = 1;
                    number_string.push_back(c);
                }

                else
                {
                    valid = false;
                    loop = false;
                }

                break;

            case 1:
                if (isdigit(c))
                {
                    pos++;
                    state = 1;
                    number_string.push_back(c);
                }

                else if (c == '.')
                {
                    pos++;
                    state = 3;
                    number_string.push_back(c);
                }

                else if (c == 'e')
                {
                    pos++;
                    state = 6;
                    number_string.push_back(c);
                }

                else
                {
                    pos++;
                    state = 2;
                }

                break;

            case 2:
            {
                Token T;
                T = Token("NUM", vector_to_string(number_string));

                tokens.push_back(T);

                pos--;
                valid = true;
                loop = false;

                break;
            }

            case 3:
                if (isdigit(c))
                {
                    pos++;
                    state = 4;
                    number_string.push_back(c);
                }

                else
                {
                    valid = false;
                    loop = false;
                }

                break;

            case 4:
                if (isdigit(c))
                {
                    pos++;
                    state = 4;
                    number_string.push_back(c);
                }

                else if (c == 'e')
                {
                    pos++;
                    state = 6;
                    number_string.push_back(c);
                }

                else
                {
                    pos++;
                    state = 5;
                }

                break;

            case 5:
            {
                Token T;

                T = Token("NUM", vector_to_string(number_string));

                tokens.push_back(T);

                pos--;
                valid = true;
                loop = false;

                break;
            }

            case 6:
                if (isdigit(c))
                {
                    pos++;
                    state = 8;
                    number_string.push_back(c);
                }

                else if (c == '+' || c == '-')
                {
                    pos++;
                    state = 7;
                    number_string.push_back(c);
                }

                else
                {
                    valid = false;
                    loop = false;
                }

                break;

            case 7:
                if (isdigit(c))
                {
                    pos++;
                    state = 8;
                    number_string.push_back(c);
                }

                else
                {
                    valid = false;
                    loop = false;
                }

                break;

            case 8:
                if (isdigit(c))
                {
                    pos++;
                    state = 8;
                    number_string.push_back(c);
                }

                else
                {
                    pos++;
                    state = 9;
                }

                break;

            case 9:
            {
                Token T;

                T = Token("NUM", vector_to_string(number_string));

                tokens.push_back(T);

                pos--;
                valid = true;
                loop = false;

                break;
            }
            }
        }

        if (valid == true)
            i = pos;

        return valid;
    }

    void serial_lexical_analysis(string file_input, vector<Token> & tokens)
    {
        int i = 0;

        while (file_input[i] != '\0' && i <= file_input.length())
        {

            if (getKeyword(file_input, tokens, i))
                ;

            else if (getVariable(file_input, tokens, i))
                ;

            else if (getNumber(file_input, tokens, i))
                ;

            else if (getRelop(file_input, tokens, i))
                ;

            else if (getArithmetic(file_input, tokens, i))
                ;

            else if (getParenthesis(file_input, tokens, i))
                ;

            else if (getPunctuation(file_input, tokens, i))
                ;

            else if (getWhitespace(file_input, tokens, i))
                ;

            else
            {
                Token T;
                tokens.push_back(T);
                i++;
            }
        }
    }

    void parallel_lexical_analysis(string file_input, vector<Token> & tokens)
    {
        int i = 0;
        while (file_input[i] != '\0' && i <= file_input.length())
        {
            int final_pos[8] = {0, 0, 0, 0, 0, 0, 0, 0};

            vector<Token> temp_tokens;
            for (int k = 0; k < 8; k++)
            {
                final_pos[k] = i;

                switch (k)
                {
                case 0:
                    getKeyword(file_input, temp_tokens, final_pos[0]);
                    break;

                case 1:
                    getVariable(file_input, temp_tokens, final_pos[1]);
                    break;

                case 2:
                    getNumber(file_input, temp_tokens, final_pos[2]);
                    break;

                case 3:
                    getRelop(file_input, temp_tokens, final_pos[3]);
                    break;

                case 4:
                    getArithmetic(file_input, temp_tokens, final_pos[4]);
                    break;

                case 5:
                    getParenthesis(file_input, temp_tokens, final_pos[5]);
                    break;

                case 6:
                    getPunctuation(file_input, temp_tokens, final_pos[6]);
                    break;

                case 7:
                    getWhitespace(file_input, temp_tokens, final_pos[7]);
                    break;
                }
            }

            int max_index = -1;
            int max_pos = i;

            for (int k = 0; k < 8; k++)
            {
                if (final_pos[k] > max_pos)
                {
                    max_index = k;
                    max_pos = final_pos[k];
                }
            }

            if (max_index == -1)
            {
                Token T;
                tokens.push_back(T);
                i++;
            }
            else
            {

                if (final_pos[1] > i)
                    SymbolTable.pop();

                switch (max_index)
                {
                case 0:
                    getKeyword(file_input, tokens, i);
                    break;

                case 1:
                    getVariable(file_input, tokens, i);
                    break;

                case 2:
                    getNumber(file_input, tokens, i);
                    break;

                case 3:
                    getRelop(file_input, tokens, i);
                    break;

                case 4:
                    getArithmetic(file_input, tokens, i);
                    break;

                case 5:
                    getParenthesis(file_input, tokens, i);
                    break;

                case 6:
                    getPunctuation(file_input, tokens, i);
                    break;

                case 7:
                    getWhitespace(file_input, tokens, i);
                    break;
                }
            }
        }
    }

    int main()
    {

        initialize_keywords();

        cout << "Enter number of test cases : ";
        int t;
        cin >> t;
        while (t--)
        {
            cout << "Enter the Input Stream : ";
            string input_stream;
            fflush(stdin);
            getline(cin, input_stream);
            cout << "\n"
                 << "Lexical Analysis for Input Stream : " << input_stream;

            vector<Token> tokens;

            parallel_lexical_analysis(input_stream, tokens);

            cout << endl;
            for (int i = 0; i < tokens.size(); i++)
                cout << tokens[i].t << endl;

            cout << "\n";
            cout << endl;
        }
        return 0;
    }