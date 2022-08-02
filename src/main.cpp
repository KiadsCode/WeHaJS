#include "main.h"

const std::string scriptFormat = ".wh";
const std::string PrintFunction = "outline";
const std::string PrintVarFunction = "outvar";
const std::string InputFunction = "inline";

const char execFunction = '~';
const std::string ifKeyWord = "if";
const std::string funcKeyWord = "function";
const std::string endFuncKeyWord = "end";
const std::string varKeyWord = "var";
const std::string letKeyWord = "let";
const char commentKeySign = '$';
vector<Variable> Variables;
vector<Function> Functions;
vector<string> ScriptCode;
vector<string> OutputCode;
int LineNum = 0;

int main(int argc, char const* argv[])
{
    printf(lx.c_str());
    while (true)
    {
        char inputChar[1024];
        printf("Command: ");
        scanf("%s", inputChar);
        string preCommand = inputChar;
        if (preCommand == ".ex")
            exit(-1);
        if (preCommand == ".load") {
            printf("Script to open: ");
            scanf("%s", inputChar);
            preCommand = inputChar;
            preCommand += scriptFormat;
            ifstream ffrom;
            string lineCode = "";
            ffrom.open(preCommand);
            LineNum = 0;
            if (!ffrom.is_open())
                printf("\nfile error\n\n");
            else {
                while (!ffrom.eof())
                {
                    lineCode = "";
                    std::getline(ffrom, lineCode);
                    ScriptCode.push_back(lineCode);
                }
                while (LineNum != ScriptCode.size())
                {
                    Parser();
                    LineNum++;
                }
                string fileD = preCommand + "-output.js";
                std::ofstream out(fileD.c_str(), std::ios::app);
                if (out.is_open())
                {
                    out << "Auto-Generated file" << std::endl;
                }
                out.close();
            }
            ffrom.close();
            ScriptCode.clear();
            Variables.clear();
            Functions.clear();
            LineNum = 0;
        }
    }
    return 0;
}

Variable FindVar(std::string name){
    int VARNUM = 0;
    for (size_t i = 0; i < Variables.size(); ++i)
    {
        if (strcmp(Variables[i].name.c_str(), name.c_str()) == 0) {
            VARNUM = i;
        }   
    }
    return Variables.at(VARNUM);
}
int FindVarNUM(std::string name){
    int VARNUM = 0;
    for (size_t i = 0; i < Variables.size(); ++i)
    {
        if (strcmp(Variables[i].name.c_str(), name.c_str()) == 0) {
            VARNUM = i;
        }   
    }
    return VARNUM;
}

Function FindFunc(std::string name) {
    int VARNUM = 0;
    for (size_t i = 0; i < Functions.size(); ++i)
    {
        if (strcmp(Functions[i].name.c_str(), name.c_str()) == 0) {
            VARNUM = i;
        }
    }
    return Functions.at(VARNUM);
}

void Parser() {
    string Code = ScriptCode[LineNum];
    ParseMV(Code);
}


#pragma region PARSER
bool ParseFunction(std::string& code) {
    string valid = "";
    string name = "";
    int start = 0, endF = 1;
    int checkL = LineNum;

    for (size_t i = 0; i < code.length(); i++)
    {
        if (valid != funcKeyWord)
            valid += code[i];
    }

    if (valid != funcKeyWord)
        return false;
    if (valid == funcKeyWord)
        start = LineNum;

    for (size_t i = funcKeyWord.length(); code[i] != '('; i++)
    {
        if (code[i] != ' ' && code[i] != '(')
            name += code[i];
    }
    
    printf("%s %s() {\n", jsFunc.c_str(), name.c_str());
    
    for (size_t i = start + 1; i < ScriptCode.size(); i++)
    {
        if (ScriptCode[i] == endFuncKeyWord.c_str()) {
            endF = i;
            LineNum = endF;
            Function func(name, start, endF);
            Functions.push_back(func);
        }
    }
    LineNum =  start + 1;

    while (LineNum != endF)
    {
        string Code = ScriptCode[LineNum];
        printf("    ");
        ParseMV(Code);
        LineNum++;
    }
    printf("}");
    return true;
}
void ParseAssignVar(std::string& code)
{
    string name = "";
    string value = "";
    string validCode = "";
    bool VALID = false;

    for (size_t i = 0; i < code.length(); i++)
    {
        if (validCode != letKeyWord)
            validCode += code[i];
    }

    if (validCode != letKeyWord)
        return;
    
    for (size_t i = letKeyWord.length() + 1; i < code.length() - 1; i++)
    {
        if (code[i] != ' ' || code[i] != '=' || code[i] != ':') 
            name += code[i];
        if (code[i] == ':' || code[i] == ' ')
            i = code.length() - 1;
    }

    if (name == " " || name == "")
        return;
    name = replace(name, ':', '\0');
    name = replace(name, ' ', '\0');
    int totL = letKeyWord.length() + 1 + name.length();
    Variable var = Variables.at(FindVarNUM(name));
    int mxh = totL;

    bool InStringBrackets = false;
    if (var.type == typeString)
    {
        for (size_t i = mxh; code[i] != ';'; i++)
        {
            if (code[i] == '\"')
                InStringBrackets = !InStringBrackets;
            if (code[i] != '\"' && code[i] != '=' && InStringBrackets)
            {
                value += code[i];
            }
        }
        Variables[FindVarNUM(name)].valueS = value;
        printf("%s = \"%s\";\n",Variables[FindVarNUM(name)].name.c_str(), value.c_str());
        return;
    }
    
    if (var.type == typeNumber)
    {
        char msc;
        for (size_t i = mxh - 1; code[i] != ';'; i++)
        {
            if (code[i] == '=' || code[i] == '+' || code[i] == '-')
                msc = code[i];
            if (code[i] != '=' && code[i] != '+' && code[i] != '-' && code[i] != ' ')
            {
                value += code[i];
            }
        }
        int VARNUM = 0;
        for (size_t i = 0; i < Variables.size(); ++i)
        {
            if (strcmp(Variables[i].name.c_str(), name.c_str()) == 0) {
                VARNUM = i;
            }
        }
        if (msc == '=') {
            Variables[VARNUM].valueI = atoi(value.c_str());
            printf("%s = %s;\n",Variables[VARNUM].name.c_str(), value.c_str());
            return;
        }
        if (msc == '+') {
            Variables[VARNUM].valueI += atoi(value.c_str());
            printf("%s += %s;\n",Variables[VARNUM].name.c_str(), value.c_str());
            return;
        }
        if (msc == '-') {
            Variables[VARNUM].valueI -= atoi(value.c_str());
            printf("%s -= %s;\n",Variables[VARNUM].name.c_str(), value.c_str());
            return;
        }
        return;
    }
    if (var.type == typeBool)
    {
        for (size_t i = mxh; code[i] != ';'; i++)
        {
            if (code[i] != '=' && code[i] != ' ')
                value += code[i];
        }
        Variables[FindVarNUM(name)].valueB = value;
        printf("%s = %s;\n",Variables[FindVarNUM(name)].name.c_str(), value.c_str());
        return;
    }
}
void ParseVarOut(std::string& code){
    string msg = "";
    string validCode = "";
    bool VALID = false;
    bool inBracket = false;

    for (int i = 0; i < code.length(); i++)
    {
        if (validCode != PrintVarFunction)
            validCode += code[i];
    }
    if (validCode != PrintVarFunction)
        return;
    for (int i = PrintVarFunction.length(); i < code.length(); i++)
    {
        if (code[i] == '(')
            inBracket = true;
        if (code[i] != '(' && code[i] != ')' && code[i] != '"')
            msg += code[i];
        if (code[i] == ')') {
            inBracket = false;
            VALID = true;
        }
    }
    if (VALID) {
        Variable varMAIN = FindVar(msg);
        printf("%s(%s);", cLog.c_str(), varMAIN.name.c_str());
        printf("\n");
        VALID = false;
    }
}
void ParseMV(std::string& Code){
    if (Code[0] == commentKeySign || Code[0] == '\0')
        return;
    if (ParseFunction(Code))
        return;
    ParseStartFunc(Code);
    ParseNewVariable(Code);
    ParseAssignVar(Code);
    ParseVarOut(Code);
    ParsePrint(Code);
    ParseInput(Code);
}
void ParseStartFunc(std::string& code)
{
    string msg = "";
    int LineBefore = LineNum;
    string validCode = "";
    bool VALID = false;
    bool inBracket = false;

    if (code[0] != execFunction)
        return;
    for (int i = 1; code[i] != '('; i++)
    {
        msg += code[i];
    }
    for (size_t i = msg.length() + 1; i < code.length(); i++)
    {
        if (code[i] == ')' && code[i - 1] == '(')
            VALID = true;
    }
    if (VALID) {
        printf("%s();\n", msg.c_str());
        VALID = false;
    }
}
void ParsePrint(std::string& code) {
    string msg = "";
    string validCode = "";
    bool VALID = false;
    bool inBracket = false;
    bool inStringBrackets = false;

    for (int i = 0; i < code.length(); i++)
    {
        if (validCode != PrintFunction)
            validCode += code[i];
    }
    if (validCode != PrintFunction)
        return;
    for (int i = PrintFunction.length(); i < code.length(); i++)
    {
        if (code[i] == '(')
            inBracket = true;
        if (code[i] == '"' && inBracket)
            inStringBrackets = !inStringBrackets;
        if (code[i] != ')' && code[i] != '"' && inStringBrackets)
            msg += code[i];
        if (code[i] == ')') {
            inBracket = false;
            inStringBrackets = false;
            VALID = true;
        }
    }
    if (VALID) {
        printf("%s(\"%s\");\n", cLog.c_str(), msg.c_str());
        VALID = false;
    }
}
void ParseInput(std::string& code) {
    char Input[1024];
    string msg = "";
    string validCode = "";
    bool VALID = false;
    bool inBracket = false;
    bool inStringBrackets = false;

    for (size_t i = 0; i < code.length(); i++)
    {
        if (validCode != InputFunction)
            validCode += code[i];
    }
    if (validCode != InputFunction)
        return;
    for (size_t i = InputFunction.length(); i < code.length(); i++)
    {
        if (code[i] == '(')
            inBracket = true;
        if (code[i] == '"' && inBracket)
            inStringBrackets = !inStringBrackets;
        if (code[i] != ')' && code[i] != '"' && inStringBrackets)
            msg += code[i];
        if (code[i] == ')') {
            inBracket = false;
            inStringBrackets = false;
            VALID = true;
        }
    }
    if (VALID) {
        printf("%s(\"%s\");\n", jsProm.c_str(), msg.c_str());
        VALID = false;
    }
}
void ParseNewVariable(std::string& code) {
    string name = "";
    string type = "";
    string value = "";
    string validCode = "";
    bool VALID = false;

    for (size_t i = 0; i < code.length(); i++)
    {
        if (validCode != varKeyWord)
            validCode += code[i];
    }

    if (validCode != varKeyWord)
        return;
    
    for (size_t i = varKeyWord.length() + 1; i < code.length() - 1; i++)
    {
        if (code[i] != ' ' || code[i] != '=' || code[i] != ':') 
            name += code[i];
        if (code[i] == ':' || code[i] == ' ')
            i = code.length() - 1;
    }

    if (name == " " || name == "")
        return;
    name = replace(name, ':', '\0');
    int totL = varKeyWord.length() + 1 + name.length();

    for (size_t i = totL; code[i] != ' '; i++)
    {
        type += code[i];
    }

    int mxh = totL + type.length();

    bool InStringBrackets = false;
    if (type == typeString)
    {
        for (size_t i = mxh; code[i] != ';'; i++)
        {
            if (code[i] == '\"')
                InStringBrackets = !InStringBrackets;
            if (code[i] != '\"' && code[i] != '=' && InStringBrackets)
            {
                value += code[i];
            }
        }
        Variable var(name, type, value);
        Variables.push_back(var);
        printf("%s %s = \"%s\";\n", varKeyWord.c_str(), name.c_str(), value.c_str());
        return;
    }
    if (type == typeNumber)
    {
        for (size_t i = mxh; code[i] != ';'; i++)
        {
            if (code[i] != '=' && code[i] != ' ')
            {
                value += code[i];
            }
        }
        Variable var(name, type, value);
        Variables.push_back(var);
        printf("%s %s = %s;\n", varKeyWord.c_str(), name.c_str(), value.c_str());
        return;
    }
    if (type == typeBool)
    {
        for (size_t i = mxh; code[i] != ';'; i++)
        {
            if (code[i] != '=' && code[i] != ' ')
                value += code[i];
        }
        Variable var(name, type, value);
        Variables.push_back(var);
        printf("%s %s = %s;\n", varKeyWord.c_str(), name.c_str(), value.c_str());
        return;
    }
}
#pragma endregion