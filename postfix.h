#include "header.h"

using namespace std;

struct Node
{
    int data;
    Node *down;
};

struct stack
{
public:
    Node *head = new Node();
    stack()
    {
        head->data = 0;
        head->down = NULL;
    }

    void push(int c)
    {
        Node *newNode = new Node();
        newNode->data = c;
        newNode->down = NULL;

        if (!(head->down))
        {
            head->down = newNode;
        }
        else
        {
            newNode->down = head->down;
            head->down = newNode;
        }
    }
    bool isEmpty()
    {
        if (head->down == NULL)
        {
            return true;
        }
        return false;
    }
    void pop() { head->down = head->down->down; }

    Node *top()
    {
        if (this->isEmpty())
        {
            return this->head;
        }
        return head->down;
    }
};

vector<string> subVec(vector<string> s, int l, int r)
{
    vector<string> result;
    for (int i = l; i < r + 1; i++)
    {
        result.push_back(s[i]);
    }
    return result;
}

void printVector(vector<string> s)
{
    for (int i = 0; i < s.size(); i++)
    {
        cout << s[i];
    }
}
vector<string> postFix(vector<string> s)
{
    vector<string> n;
    if (s[0] == "(" && s[s.size() - 1] == ")" && s.size() != 3)
    {
        char sign = s[2][0] - 100;
        n.push_back(s[1]);
        n.push_back(s[3]);
        n.push_back(string(1, sign));
        return n;
    }
    if (s.size() == 1)
    {
        n.push_back(s[0]);
        return n;
    }
    if (s.size() == 2)
    {
        // invalid output
        return n;
    }
    if (s.size() == 3 && s[0] != "(")
    {
        n.push_back(s[0]);
        n.push_back(s[2]);
        n.push_back(s[1]);
        return n;
    }
    if (s.size() == 3 && s[0] == "(")
    {
        n.push_back(s[1]);
        return n;
    }
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == "(")
        {
            if (s[i + 2] == ")")
            {
                vector<string> substr1 = subVec(s, 0, i);

                string a = s[i + 1];
                vector<string> substr2 = subVec(s, i + 3, s.size() - i + 3);
                s.clear();
                s.insert(s.end(), substr1.begin(), substr1.end());
                s.push_back(a);
                s.insert(s.end(), substr2.begin(), substr2.end());
            }
            else
            {
                s[i + 2][0] = s[i + 2][0] + 50;
            }
        }
    }
    char symbols[] = {'+', '-', '*', '/', '^'};

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < s.size(); j++)
        {
            if (s[j][0] == symbols[i])
            {
                vector<string> left = postFix(subVec(s, 0, j - 1));
                vector<string> right = postFix(subVec(s, j + 1, s.size() - 1));
                vector<string> symbol = {s[j]};
                right.insert(right.end(), symbol.begin(), symbol.end());

                left.insert(left.end(), right.begin(), right.end());
                return left;
            }
        }
    }

    return s;
}
bool isDigit(string s)
{
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] < 48 || s[i] > 57)
        {
            return false;
        }
    }
    return true;
}

