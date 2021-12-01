#include <bits/stdc++.h>
using namespace std;

class Cfg_rule
{
public:
    char root;
    vector<char> leaf;

    Cfg_rule(char root = 'e')
    {
        this->root = root;
    }

    int is_present(char l)
    {
        for (int i = 0; i < leaf.size(); i++)
        {
            if (l == leaf[i])
                return i;
        }

        return -1;
    }

    void display()
    {
        cout << root << " --> ";

        for (int i = 0; i < leaf.size(); i++)
        {
            cout << leaf[i];
        }
    }
};

set<char> First_single(char f, map<char, list<Cfg_rule> > &rules, set<char> &terminals, set<char> &non_terminals)
{
    set<char> first;

    if (terminals.count(f))
        first.insert(f);

    else
    {
        queue<char> BFS;
        BFS.push(f);

        while (!BFS.empty())
        {

            char d = BFS.front();
            BFS.pop();

            if ((d == 'e' || terminals.count(d) != 0) && first.count(d) == 0)

                else
                {
                    for (auto node_1 : rules[d])
                    {

                        BFS.push(node_1.leaf[0]);
                    }
                }
        }
    }

    return first;
}

map<char, set<char> > First_all(map<char, list<Cfg_rule> > &rules, set<char> &terminals, set<char> &non_terminals)
{

    map<char, set<char> > first;

    for (auto node : terminals)

        first[node].insert(node);

    for (auto node : non_terminals)
    {

        queue<char> BFS;

        BFS.push(node);

        while (!BFS.empty())
        {

            char d = BFS.front();
            BFS.pop();

            if ((d == 'e' || terminals.count(d) != 0) && first[node].count(d) == 0)
                first[node].insert(d);

            else
            {
                for (auto node_1 : rules[d])
                {

                    BFS.push(node_1.leaf[0]);
                }
            }
        }
    }

    return first;
}

void Follow_single(char f, map<char, set<char> > &Follow, map<char, list<Cfg_rule> > &rules, set<char> &terminals, set<char> &non_terminals)
{

    for (auto node : rules)
    {
        for (auto node1 : node.second)
        {
            if (node1.is_present(f) != -1)
            {
                int pos = node1.is_present(f);

                if ((pos + 1) >= node1.leaf.size())
                {

                    if (node1.root != f)
                    {
                        if (Follow.count(node1.root) == 0)
                            Follow_single(node1.root, Follow, rules, terminals, non_terminals);

                        for (auto node2 : Follow[node1.root])
                            Follow[f].insert(node2);
                    }
                }
                else
                {

                    bool only_epsilon = true;

                    for (auto node2 : First_single(node1.leaf[pos + 1], rules, terminals, non_terminals))
                        if (node2 != 'e')

                            only_epsilon = false;

                    if (only_epsilon == true)
                    {

                        if (node1.root != f)
                        {
                            if (Follow.count(node1.root) == 0)
                                Follow_single(node1.root, Follow, rules, terminals, non_terminals);

                            for (auto node2 : Follow[node1.root])
                                Follow[f].insert(node2);
                        }
                    }

                    else
                    {

                        for (auto node2 : First_single(node1.leaf[pos + 1], rules, terminals, non_terminals))
                        {
                            if (node2 != 'e')
                                Follow[f].insert(node2);
                        }
                    }
                }
            }
        }
    }
}

map<char, set<char> > Follow_all(char start_node, map<char, list<Cfg_rule> > &rules, set<char> &terminals, set<char> &non_terminals)
{
    map<char, set<char> > Follow;

    Follow[start_node].insert('$');

    Follow_single(start_node, Follow, rules, terminals, non_terminals);

    for (auto node : non_terminals)
    {
        if (Follow.count(node) == 0)
            Follow_single(node, Follow, rules, terminals, non_terminals);
    }

    return Follow;
}

map<char, map<char, Cfg_rule> > LL1_Parsing_Table_Construct(map<char, list<Cfg_rule> > &rules, map<char, set<char> > &First, map<char, set<char> > &Follow, set<char> &non_terminals, set<char> &terminals)
{

    map<char, map<char, Cfg_rule> > LL1_Parser_Table;

    for (auto rule : rules)
    {
        for (auto rule1 : rule.second)
        {

            char alpha = rule1.leaf[0];

            if (alpha != 'e')
            {

                for (auto node : First[alpha])
                {
                    if (node != 'e')
                    {

                        LL1_Parser_Table[rule1.root][node] = rule1;
                    }
                    else
                    {
                        for (auto node1 : Follow[rule1.root])
                        {

                            LL1_Parser_Table[rule1.root][node1] = rule1;
                        }
                    }
                }
            }

            else
            {
                for (auto node1 : Follow[rule1.root])
                {

                    LL1_Parser_Table[rule1.root][node1] = rule1;
                }
            }
        }
    }

    return LL1_Parser_Table;
}

void Display_Parsing_Table_Construct(map<char, map<char, Cfg_rule> > &LL1_Parsing_Table)
{
    for (auto node : LL1_Parsing_Table)
    {
        for (auto node1 : node.second)
        {
            cout << "M [ " << node.first << " , " << node1.first << " ] : ";
            node1.second.display();
            cout << "\t\t\t,\t\t\t";
        }

        cout << endl;
    }
}

bool LL1_Parse(string test, char start_node, map<char, map<char, Cfg_rule> > LL1_Parser)
{
    stack<char> S;
    S.push('$');
    S.push(start_node);

    int pos = 0;

    test[test.size()] = '$';

    while (S.top() != '$')
    {
        if (S.top() == test[pos])
        {
            cout << "Match for " << test[pos];
            pos++;
            S.pop();
        }

        else
        {
            if (LL1_Parser.count(S.top()))
            {
                if (LL1_Parser[S.top()].count(test[pos]))
                {

                    cout << "Check cell M [ " << S.top() << " , " << test[pos] << " ] and replace ";
                    Cfg_rule rule = LL1_Parser[S.top()][test[pos]];
                    S.pop();

                    rule.display();

                    for (int i = rule.leaf.size() - 1; i >= 0; --i)
                    {
                        if (rule.leaf[i] != 'e')
                            S.push(rule.leaf[i]);
                    }
                }
                else
                {
                    cout << "No cell found for M [ " << S.top() << " , " << test[pos] << " ] and Hence returning Erorr !!";
                    break;
                }
                else
                {
                    cout << "No cell found for M [ " << S.top() << " , " << test[pos] << " ] and Hence returning Erorr !!";
                    break;
                }
            }

            cout << endl;
        }

        if (S.top() == '$' && test[pos] == '$')
            return true;

        return false;
    }

    int main()
    {
        cout << "Enter number of test cases : ";
        int t;
        cin >> t;

        while (t--)
        {

            cout << endl
                 << "Enter the number of grammar rules : ";
            int nr;

            cin >> nr;

            map<char, list<Cfg_rule> > rules;
            set<char> terminals;

            set<char> non_terminals;

            cout << endl
                 <<

                for (int i = 0; i < nr; i++)
            {

                Cfg_rule node;

                cin >> node.root;

                if (non_terminals.count(node.root) == 0)
                    non_terminals.insert(node.root);

                string leafs;
                cin >> leafs;

                for (int i = 0; leafs[i] != '\0'; i++)
                {
                    if (leafs[i] == 'e')
                    {
                    }

                    else if (isupper(leafs[i]))
                    {
                        if (non_terminals.count(leafs[i]) == 0)
                            non_terminals.insert(leafs[i]);
                    }

                    else if (islower(leafs[i]))
                    {
                        if (terminals.count(leafs[i]) == 0)
                            terminals.insert(leafs[i]);
                    }

                    node.leaf.push_back(leafs[i]);
                }

                rules[node.root].push_back(node);
            }

            cout << endl
                 << "Enter the Starting Node : ";
            char start_node;
            cin >> start_node;
            cout << endl;

            map<char, set<char> > First = First_all(rules, terminals, non_terminals);

            map<char, set<char> > Follow = Follow_all(start_node, rules, terminals, non_terminals);

            map<char, map<char, Cfg_rule> > LL1_Parsing_Table = LL1_Parsing_Table_Construct(rules, First, Follow, non_terminals, terminals);

            Display_Parsing_Table_Construct(LL1_Parsing_Table);

            fflush(stdin);
            string to_parse;
            cout << endl
                 << "Enter the Test statement to pass through the LL1 Parser : ";
            cin >> to_parse;

            cout << endl;
            if (LL1_Parse(to_parse, start_node, LL1_Parsing_Table))
                cout << endl
                     << endl
                     << "String Accepted : No Syntax Error" << endl;
            else
                cout << endl
                     << endl
                     << "String Un-accepted : Syntax error is Present" << endl;
        }

        cout << endl
             << endl;
        return 0;
    }