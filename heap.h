#include "header.h"

// returns true if strings are the same
bool strcmpr(string s1, string s2)
{
    if (s1.length() != s2.length())
    {
        return false;
    }
    for (int i = 0; i < s1.length(); i++)
    {
        if (s1[i] != s2[i])
        {
            return false;
        }
    }

    return true;
}

struct variable
{
    string name;
    int value;
    variable(string n, int v)
    {
        this->name = n;
        this->value = v;
    }
};

string noSpacestring(string s)
{
    stringstream ss;

    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] != ' ')
        {
            ss << s[i];
        }
    }

    return ss.str();
}

struct heap
{
public:
    vector<variable> heap;

    void addVariable(string id, int value)
    {

        if (id[0] == ' ' || id[id.length() - 1] == ' ')
        {
            id = noSpacestring(id);
        }
        for (int i = 0; i < heap.size(); i++)
        {
            if (strcmpr(heap[i].name, id))
            {
                heap[i].value = value;
                return;
            }
        }
        heap.push_back(variable(id, value));
    }

    int searchVal(string name)
    {
        for (int i = 0; i < heap.size(); i++)
        {
            if (strcmpr(heap[i].name, name))
            {
                return heap[i].value;
            }
        }

        return 1882203893;
    }
};

struct program
{
    vector<string> lines;
    vector<string> id;
    vector<int> pri;

    void deleteLine()
    {
        lines.erase(lines.begin());
        id.erase(id.begin());
        pri.erase(pri.begin());
        return;
    }
};

vector<string> divideList(string s)
{
    vector<string> result;
    stringstream ss(s);

    stringstream temp;

    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] != ' ')
        {
            if (s[i] != ',')
            {
                temp << s[i];
            }
            else
            {
                result.push_back(temp.str());
                temp.str("");
            }
        }
    }
    result.push_back(temp.str());
    temp.clear();

    return result;
}

program setFunc(program pro)
{
    program nuevo = program();
    for (int i = 0; i < pro.id.size(); i++)
    {
        if (pro.id[i] == "definition")
        {
            int space = pro.pri[i];
            nuevo.id.push_back(pro.id[i]);
            nuevo.lines.push_back(pro.lines[i]);
            nuevo.pri.push_back(pro.pri[i]);
            pro.id[i] = "";
            pro.lines[i] = "";
            for (int j = i + 1; j < pro.id.size(); j++)
            {
                if (space < pro.pri[j])
                {
                    nuevo.id.push_back(pro.id[j]);
                    nuevo.lines.push_back(pro.lines[j]);
                    nuevo.pri.push_back(pro.pri[j]);
                    pro.id[j] = "";
                    pro.lines[j] = "";
                }
                else
                {
                    return nuevo;
                }
            }
        }
    }
    return nuevo;
}

bool isFunction(string s)
{
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == '(')
        {
            return true;
        }
    }
    return false;
}

string getFuncName(string s)
{
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == '(')
        {

            return s.substr(0, i);
        }
    }
    cout << "not a function" << endl;
    return "";
}

bool isAlpha(string s)
{
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] < 48 || s[i] > 57)
        {
            return true;
        }
    }
    return false;
}

const char operatores[] = {'+', '-', '*', '/', '%'};

bool isOp(char c)
{
    for (int i = 0; i < 5; i++)
    {
        if (c == operatores[i])
        {
            return true;
        }
    }
    return false;
}

bool isOperation(string s)
{
    for (int i = 0; i < s.length(); i++)
    {
        if (isOp(s[i]))
        {
            return true;
        }
    }
    return false;
}



string getReturnVar(string s)
{
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == ' ' && s[i + 1] != ' ' && s[i - 1] != ' ')
        {
            for (int j = i; j < s.length(); j++)
            {

                return s.substr(i + 1, s.length());
            }
        }
    }
    return "";
}
