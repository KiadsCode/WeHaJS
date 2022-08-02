#pragma once
#include <string>
#include <iostream>

const std::string typeNumber = "Number";
const std::string typeString = "String";
const std::string typeBool = "Boolean";
const std::string boolVarA = "true";
const std::string boolVarB = "false";

class Variable
{
public:
    int valueI = 0;
    std::string name = "";
    std::string type = "";
    std::string valueS = "";
    std::string valueB = "";
    Variable(std::string NAME, std::string TYPE, std::string VALUE);
    ~Variable();
};

Variable::Variable(std::string NAME, std::string TYPE, std::string VALUE)
{
    name = NAME;
    type = TYPE;
    valueS = VALUE;
    if (type == typeNumber){
        try
        {
            valueI = atoi(valueS.c_str());
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    if (type == typeBool)
    {
        if (valueS == boolVarA || valueS == boolVarB)
        {
            valueB = valueS;
        }
    }
}
Variable::~Variable()
{
}