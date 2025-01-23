#include "header.h"
#include "heap.h"
#include "postfix.h"
using namespace std;

heap memory;

void executer(program pro, int spaces);
int returnValue(string funcname);

struct function;

vector<function> definitions;

int solveString(vector<string> s, heap mem)
{
  stack prob;

  for (int i = 0; i < s.size(); i++)
  {
    if (isalnum(s[i][0]))
    {
      if (!isDigit(s[i]))
      {
        if (isFunction(s[i]))
        {
          mem.addVariable(getFuncName(s[i]), returnValue(s[i]));
          prob.push(mem.searchVal(getFuncName(s[i])));
        }
        else
        {
          prob.push(mem.searchVal(s[i]));
        }
      }
      else
      {
        prob.push(stoi(s[i]));
      }
    }
    else
    {
      int a = prob.top()->data;
      prob.pop();

      if (prob.isEmpty())
      {
        return 0;
      }
      int b = prob.top()->data;
      prob.pop();

      switch (s[i][0])
      {
      case '+':
        prob.push(b + a);
        break;
      case '-':
        prob.push(b - a);
        break;
      case '*':
        prob.push(b * a);
        break;
      case '/':
        prob.push(b / a);
        break;
      case '^':
        prob.push(pow(b, a));
        break;
      }
    }
  }

  return prob.top()->data;
}

const string keywords[] = {"int", "float", "double", "char",
                           "string", "bool", "if", "else"};

stringstream deleteComments(stringstream &input)
{
  stringstream final;
  string line;
  while (getline(input, line))
  {
    for (int i = 0; i < line.length(); i++)
    {
      if (line[i] == '#')
      {
        final << line.substr(0, i) << endl;
        break;
      }
      if (i == line.length() - 1)
      {
        final << line << endl;
      }
    }
  }
  return final;
}

string eraseSpaces(string s)
{
  vector<string> result;
  stringstream ss(s);
  stringstream nospace;
  string nuevo;
  while (getline(ss, nuevo, ' '))
  {
    if (nuevo != " " && nuevo != "\n" && nuevo != "" && nuevo != "\t")
    {
      nospace << nuevo;
    }
  }
  return nospace.str();
}

bool isInsideFunction(string s, int actual)
{
  for (int i = actual; i >= 0; i--)
  {
    if (s[i] == '(')
    {
      if (i - 1 >= 0 && isalpha(s[i - 1]))
      {
        return true;
      }
    }
    if (s[i] == ')')
    {
      return false;
    }
  }
  return false;
}

vector<string> prePost(string s)
{
  vector<string> result;
  stringstream ss(s);

  stringstream temp;

  for (int i = 0; i < s.length(); i++)
  {
    if (s[i] != ' ')
    {
      if (!isOp(s[i]) && s[i] != '=')
      {
        temp << s[i];
      }
      else
      {
        if (isInsideFunction(s, i))
        {
          temp << s[i];
        }
        else
        {
          result.push_back(temp.str());
          temp.str("");
          temp << s[i];
          result.push_back(temp.str());
          temp.str("");
        }
      }
    }
  }
  result.push_back(temp.str());
  temp.clear();

  return result;
}

string detectInst(string s)
{

  for (int i = 0; i < s.length(); i++)
  {
    if (s[i] == '=')
    {
      for (int j = i; j < s.length(); j++)
      {
        if (isOp(s[j]))
        {
          return "operation";
        }
      }
      return "declaration";
    }
    else
    {
      if (s.substr(0, i) == "print")
      {
        return "print";
      }
      if (s[i] == 'i')
      {
        if (s[i + 1] == 'f')
        {
          return "if";
        }
      }
      if (s[i] == 'e')
      {
        if (s[i + 1] == 'l')
        {
          if (s[i + 2] == 's')
          {
            if (s[i + 3] == 'e')
            {
              return "else";
            }
          }
        }
      }
      if (s[i] == 'r')
      {
        if (s[i + 1] == 'e')
        {
          if (s[i + 2] == 't')
          {
            if (s[i + 3] == 'u')
            {
              if (s[i + 4] == 'r')
              {
                if (s[i + 5] == 'n')
                {
                  return "return";
                }
              }
            }
          }
        }
      }
      if (s.substr(0, i) == "def")
      {
        return "definition";
      }
    }
  }
  return "";
}

// the functions solves an operation and stores the variable name
void solveOp(string command)
{
  vector<string> name;
  vector<string> op;
  vector<string> newcommand = prePost(command);
  for (int i = 0; i < newcommand.size(); i++)
  {
    if (newcommand[i] == "=")
    {
      name = subVec(newcommand, 0, i - 1);
      op = subVec(newcommand, i + 1, newcommand.size() - 1);
    }
  }

  vector<string> op2 = postFix(op);

  int result = solveString(op2, memory);

  memory.addVariable(name[0], result);
};

vector<int> getFuncInputvar(string s, heap memory)
{
  vector<int> tempint;

  for (int i = 0; i < s.length(); i++)
  {
    if (s[i] == '(')
    {
      for (int j = i; j < s.length(); j++)
      {
        if (s[j] == ')')
        {
          vector<string> cool = divideList(s.substr(i + 1, j - i - 1));

          for (int k = 0; k < cool.size(); k++)
          {
            if (isOperation(cool[k]))
            {
              vector<string> op = prePost(cool[k]);
              vector<string> op2 = postFix(op);
              int result = solveString(op2, memory);
              tempint.push_back(result);
            }
            else if (isAlpha(cool[k]))
            {
              tempint.push_back(memory.searchVal(cool[k]));
            }
            else
            {
              tempint.push_back(stoi(cool[k]));
            }
          }
          return tempint;
        }
      }
    }
  }
  cout << "not a function" << endl;
  return tempint;
}

// similar to solveOp but it only detects equal sign and assigns variable and
// its name !!!!!!!!!!!!!!!
void declareVar(string command)
{
  string name;
  string val;
  string newcommand = eraseSpaces(command);
  for (int i = 0; i < newcommand.length(); i++)
  {
    if (newcommand[i] == '=')
    {
      name = newcommand.substr(0, i);
      val = newcommand.substr(i + 1, newcommand.size() - 1);
    }
  }

  int n;
  if (!isDigit(val))
  {
    if (isFunction(val))
    {

      memory.addVariable(name, returnValue(val));
      return;
    }
    n = memory.searchVal(val);
    memory.addVariable(name, n);
    return;
  }

  memory.addVariable(name, stoi(val));
}

// detects if the string want to be printed directly or is the name of of a
// variable
string createOutput(string s)
{
  int l;
  int r;
  stringstream varname;
  for (int i = 0; i < s.length(); i++)
  {
    if (s[i] == '\"')
    {
      for (int j = i + 1; j < s.length(); j++)
      {
        if (s[j] == '\"')
        {
          l = i;
          r = j;
          return s.substr(i + 1, j - 1);
        }
      }
    }
    else if (s[i] != ' ')
    {
      varname << s[i];
    }
  }
  int var = memory.searchVal(varname.str());
  return to_string(var);
}

vector<string> getPrintCommands(string s)
{
  vector<string> final;
  stringstream ss(s);
  string nuevo;
  while (getline(ss, nuevo, ','))
  {
    final.push_back(createOutput(nuevo));
    final.push_back(" ");
  }
  final.pop_back();
  return final;
}

void printPython(string command)
{
  vector<string> output;
  int l;
  int r;
  // print("Sum = ", resultSum)
  for (int i = 0; i < command.length(); i++)
  {
    if (command[i] == '(')
    {
      l = i;
    }
    if (command[i] == ')')
    {
      r = i;
    }
  }

  vector<string> toPrint = getPrintCommands(command.substr(l + 1, r - l - 1));
  printVector(toPrint);
  cout << endl;
}

string eraseTab(string s)
{
  stringstream out;
  for (int i = 0; i < s.length(); i++)
  {
    if (s[i] != '\t')
    {
      out << s[i];
    }
  }
  return out.str();
}

bool checkif(string statement, heap mem)
{
  int val1;
  int val2;
  stringstream ss(statement);
  string getter;
  vector<string> news;
  while (getline(ss, getter, ' '))
  {
    if (getter != "\t" && getter != "if" && getter != "")
    {
      if (getter[getter.length() - 1] == ':')
      {
        getter = getter.substr(0, getter.length() - 1);
      }
      news.push_back(getter);
    }
  }

  if (!isDigit(news[0]))
  {
    val1 = mem.searchVal(news[0]);
  }
  else
  {
    val1 = stoi(news[0]);
  }

  if (!isDigit(news[2]))
  {
    val2 = mem.searchVal(news[2]);
  }
  else
  {
    val2 = stoi(news[2]);
  }

  if (news[1] == "==")
  {
    if (val1 == val2)
    {
      return true;
    }
    return false;
  }
  if (news[1] == "<=")
  {
    if (val1 <= val2)
    {
      return true;
    }
    return false;
  }

  if (news[1] == ">=")
  {
    if (val1 >= val2)
    {
      return true;
    }
    return false;
  }
  if (news[1] == "<")
  {
    if (val1 < val2)
    {
      return true;
    }

    return false;
  }
  if (news[1] == ">")
  {
    if (val1 > val2)
    {
      return true;
    }
    return false;
  }
  return false;
}

program deleteElse(program pro, int space)
{
  pro.id[0] = "";
  pro.lines[0] = "";
  for (int i = 0; i < pro.id.size(); i++)
  {
    if (pro.id[i] == "else" && pro.pri[i] == space)
    {
      pro.id[i] = "";
      pro.lines[i] = "";
      for (int j = i + 1; j < pro.id.size(); j++)
      {
        if (space < pro.pri[j])
        {
          pro.id[j] = "";
          pro.lines[j] = "";
        }
        else
        {
          return pro;
        }
      }
    }
  }
  return pro;
}

program deleteIf(program pro, int space)
{
  for (int i = 0; i < pro.id.size(); i++)
  {
    if (pro.id[i] == "if" && pro.pri[i] == space)
    {
      pro.id[i] = "";
      pro.lines[i] = "";
      for (int j = i + 1; j < pro.id.size(); j++)
      {
        if (space < pro.pri[j])
        {
          pro.id[j] = "";
          pro.lines[j] = "";
        }
        else
        {

          return pro;
        }
      }
    }
  }
  return pro;
}

program deleteDef(program pro, int space)
{
  for (int i = 0; i < pro.id.size(); i++)
  {
    if (pro.id[i] == "definition" && pro.pri[i] == space)
    {
      pro.id[i] = "";
      pro.lines[i] = "";
      for (int j = i + 1; j < pro.id.size(); j++)
      {
        if (space < pro.pri[j])
        {
          pro.id[j] = "";
          pro.lines[j] = "";
        }
        else
        {
          return pro;
        }
      }
    }
  }
  return pro;
}

void executer(program pro, int spaces)
{
  if (pro.lines.size() > 0)
  {
    if (strcmpr(pro.id[0], "definition"))
    {
      pro = deleteDef(pro, pro.pri[0]);
      executer(pro, pro.pri[0]);
      return;
    }
    if (strcmpr(pro.id[0], "declaration"))
    {
      declareVar(pro.lines[0]);
    }
    if (strcmpr(pro.id[0], "operation"))
    {
      solveOp(pro.lines[0]);
    }
    if (strcmpr(pro.id[0], "print"))
    {
      printPython(pro.lines[0]);
    }
    if (strcmpr(pro.id[0], "if"))
    {
      if (checkif(pro.lines[0], memory))
      {
        pro = deleteElse(pro, pro.pri[0]);
        executer(pro, pro.pri[0]);
        return;
      }
      else
      {
        pro = deleteIf(pro, pro.pri[0]);
        executer(pro, spaces);
        return;
      }
    }

    pro.lines.erase(pro.lines.begin());
    pro.id.erase(pro.id.begin());
    pro.pri.erase(pro.pri.begin());
    executer(pro, spaces);
  }
}

int getHierarchy(string line)
{
  int result = 0;
  for (int i = 0; i < line.length(); i++)
  {
    if (line[i] == ' ')
    {
      ++result;
    }
    else
    {
      return result;
    }
  }
  return result;
}

struct function
{
  program ins;
  string name;
  vector<string> input;
  heap temp;
  heap globalvariables;
  string vartoReturn;

  function(program i, heap global)
  {
    this->ins = i;
    this->globalvariables = global;
    detectName(ins.lines[0]);
    detectInputvar(ins.lines[0]);
  }
  vector<int> funcgetFuncInputvar(string s)
  {
    vector<int> tempint;

    for (int i = 0; i < s.length(); i++)
    {
      if (s[i] == '(')
      {
        for (int j = i; j < s.length(); j++)
        {
          if (s[j] == ')')
          {
            vector<string> cool = divideList(s.substr(i + 1, j - i - 1));

            for (int k = 0; k < cool.size(); k++)
            {
              if (isOperation(cool[k]))
              {
                vector<string> op = prePost(cool[k]);
                vector<string> op2 = postFix(op);
                int result = funcSolveString(op2);
                tempint.push_back(result);
              }
              else if (isAlpha(cool[k]))
              {
                if (temp.searchVal(cool[k]) == 1882203893)
                {
                  tempint.push_back(globalvariables.searchVal(cool[k]));
                }
                else
                {
                  tempint.push_back(temp.searchVal(cool[k]));
                }
              }
              else
              {
                tempint.push_back(stoi(cool[k]));
              }
            }
            return tempint;
          }
        }
      }
    }
    cout << "not a function" << endl;
    return tempint;
  }

  int funcreturnValue(string instruction)
  {
    string funcname = getFuncName(instruction);
    vector<int> funcinval = funcgetFuncInputvar(instruction);
    for (int i = 0; i < definitions.size(); i++)
    {
      if (definitions[i].name == funcname)
      {
        definitions[i].updateGlobalVariables(memory);
        int temp = definitions[i].executeCode(funcinval);
        return temp;
      }
    }

    cout << "value not found !!!!!";
    return -1;
  }
  void funcdeclareVar(string command)
  {
    string name;
    string val;
    string newcommand = eraseSpaces(command);
    for (int i = 0; i < newcommand.length(); i++)
    {
      if (newcommand[i] == '=')
      {
        name = newcommand.substr(0, i);
        val = newcommand.substr(i + 1, newcommand.size() - 1);
      }
    }

    int n;
    if (!isDigit(val))
    {
      if (isFunction(val))
      {
        temp.addVariable(name, funcreturnValue(val));
        return;
      }
      n = temp.searchVal(val);
      temp.addVariable(name, n);
      return;
    }

    temp.addVariable(name, stoi(val));
  }

  bool funccheckif(string statement)
  {
    int val1;
    int val2;
    stringstream ss(statement);
    string getter;
    vector<string> news;

    while (getline(ss, getter, ' '))
    {
      if (getter != "\t" && getter != "if" && getter != "")
      {
        if (getter[getter.length() - 1] == ':')
        {
          getter = getter.substr(0, getter.length() - 1);
        }
        news.push_back(getter);
      }
    }

    if (!isDigit(news[0]))
    {
      val1 = temp.searchVal(news[0]);
    }
    else
    {
      val1 = stoi(news[0]);
    }

    if (!isDigit(news[2]))
    {
      val2 = temp.searchVal(news[2]);
    }
    else
    {
      val2 = stoi(news[2]);
    }

    if (news[1] == "==")
    {
      if (val1 == val2)
      {
        return true;
      }
      return false;
    }
    if (news[1] == "<=")
    {
      if (val1 <= val2)
      {
        return true;
      }
      return false;
    }

    if (news[1] == ">=")
    {
      if (val1 >= val2)
      {
        return true;
      }
      return false;
    }
    if (news[1] == "<")
    {
      if (val1 < val2)
      {
        return true;
      }

      return false;
    }
    if (news[1] == ">")
    {
      if (val1 > val2)
      {
        return true;
      }
      return false;
    }
    return false;
  }

  int funcSolveString(vector<string> s)
  {
    stack prob;

    for (int i = 0; i < s.size(); i++)
    {
      if (isalnum(s[i][0]))
      {
        if (!isDigit(s[i]))
        {
          if (isFunction(s[i]))
          {
            temp.addVariable(getFuncName(s[i]), funcreturnValue(s[i]));
            prob.push(temp.searchVal(getFuncName(s[i])));
          }
          else
          {
            if (temp.searchVal(s[i]) == 1882203893)
            {
              prob.push(globalvariables.searchVal(s[i]));
            }
            else
            {
              prob.push(temp.searchVal(s[i]));
            }
          }
        }
        else
        {
          prob.push(stoi(s[i]));
        }
      }
      else
      {
        int a = prob.top()->data;
        prob.pop();

        if (prob.isEmpty())
        {
          return 0;
        }
        int b = prob.top()->data;
        prob.pop();

        switch (s[i][0])
        {
        case '+':
          prob.push(b + a);
          break;
        case '-':
          prob.push(b - a);
          break;
        case '*':
          prob.push(b * a);
          break;
        case '/':
          prob.push(b / a);
          break;
        case '^':
          prob.push(pow(b, a));
          break;
        }
      }
    }

    return prob.top()->data;
  }

  void funcsolveOp(string command)
  {
    vector<string> name;
    vector<string> op;
    vector<string> newcommand = prePost(command);

    for (int i = 0; i < newcommand.size(); i++)
    {
      if (newcommand[i] == "=")
      {
        name = subVec(newcommand, 0, i - 1);
        op = subVec(newcommand, i + 1, newcommand.size() - 1);
      }
    }

    vector<string> op2 = postFix(op);

    int result = funcSolveString(op2);

    temp.addVariable(name[0], result);
  };

  void executeCode2(program pro, int spaces)
  {
    if (pro.lines.size() > 0)
    {

      if (strcmpr(pro.id[0], "return"))
      {
        vartoReturn = getReturnVar(pro.lines[0]);
        return;
      }
      if (strcmpr(pro.id[0], "declaration"))
      {
        funcdeclareVar(pro.lines[0]);
      }
      if (strcmpr(pro.id[0], "operation"))
      {
        funcsolveOp(pro.lines[0]);
      }
      if (strcmpr(pro.id[0], "print"))
      {
        printPython(pro.lines[0]);
      }
      if (strcmpr(pro.id[0], "if"))
      {
        if (funccheckif(pro.lines[0]))
        {
          pro = deleteElse(pro, pro.pri[0]);
          executeCode2(pro, pro.pri[0]);
          return;
        }
        else
        {
          pro = deleteIf(pro, pro.pri[0]);
          executeCode2(pro, spaces);
          return;
        }
      }

      pro.lines.erase(pro.lines.begin());
      pro.id.erase(pro.id.begin());
      pro.pri.erase(pro.pri.begin());
      executeCode2(pro, spaces);
    }
  }

  int executeCode(vector<int> inputval)
  {
    for (int i = 0; i < input.size(); i++)
    {
      temp.addVariable(input[i], inputval[i]);
    }

    executeCode2(ins, 0);

    int nuvo = temp.searchVal(vartoReturn);

    heap temp = heap();
    return nuvo;
  }

  void updateGlobalVariables(heap global)
  {

    this->globalvariables = global;
  }

private:
  void detectName(string s)
  {

    for (int i = 0; i < s.length(); i++)
    {
      if (s[i] == ' ')
      {
        for (int j = i; j < s.length(); j++)
        {
          if (s[j] == '(')
          {
            this->name = s.substr(i + 1, j - i - 1);
            return;
          }
        }
      }
    }
    cout << "not a function" << endl;
    return;
  }

  void detectInputvar(string s)
  {
    for (int i = 0; i < s.length(); i++)
    {
      if (s[i] == '(')
      {
        for (int j = i; j < s.length(); j++)
        {
          if (s[j] == ')')
          {
            this->input = divideList(s.substr(i + 1, j - i - 1));
            return;
          }
        }
      }
    }
  }
};

int returnValue(string instruction)
{
  string funcname = getFuncName(instruction);
  vector<int> funcinval = getFuncInputvar(instruction, memory);
  for (int i = 0; i < definitions.size(); i++)
  {
    if (definitions[i].name == funcname)
    {
      definitions[i].updateGlobalVariables(memory);
      int temp = definitions[i].executeCode(funcinval);
      return temp;
    }
  }

  cout << "value not found !!!!!";
  return -1;
}

int main(int argc, char* argv[])
{

  string filename = argv[1];

  // Create a text string, which is used to output the text file
  string myText;
  stringstream input;

  // Read from the text file
  ifstream MyReadFile(filename);

  // Use a while loop together with the getline() function to read the file
  // line by line
  while (getline(MyReadFile, myText))
  {
    // Output the text from the file
    input << myText << endl;
  }
  stringstream nocomments = deleteComments(input);

  vector<variable> heap;

  program pro = program();

  while (getline(nocomments, myText))
  {
    if (myText != "")
    {
      pro.pri.push_back(getHierarchy(myText));
      pro.lines.push_back(myText);
      pro.id.push_back(detectInst(myText));
    }
  }

  for (int i = 0; i < pro.id.size(); i++)
  {
    if (strcmpr(pro.id[i], "definition"))
    {

      definitions.push_back(function(setFunc(pro), memory));
      pro = deleteDef(pro, 0);
    }
  }

  executer(pro, 0);


  // Close the file
  MyReadFile.close();
}
