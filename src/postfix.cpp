// Copyright 2020 A.SHT
#include "MyStack.h"
#include "postfix.h"

#include <algorithm>
#include <optional>
#include <string>
#include <vector>

bool isOp(const std::string& str)
{
    return str.length() == 1 && (str == "(" || str == ")" || str == "+" || str == "-" || str == "*" || str == "/");
}

int priority(const char& op)
{
    if (op == '(')
        return 0;
    else if (op == ')')
        return 1;
    else if (op == '+' || op == '-')
        return 2;
    else // (op == '*' || op == '/')
        return 3;
}

std::vector<std::string> tokenize(const std::string& str)
{
    std::vector<std::string> tokens;
    std::optional<int> num;
    int decimal_fraction = 0;
    bool is_dev_frac = false;
    const auto push_number = [&num, &tokens, &decimal_fraction, &is_dev_frac]()
    {
        if (num.has_value())
            tokens.push_back(std::to_string(*num) + (is_dev_frac ? (std::string(".") + std::to_string(decimal_fraction)) : std::string()));
        num.reset();
        decimal_fraction = 0;
        is_dev_frac = false;
    };
    std::for_each(std::begin(str), std::end(str), [&num, &tokens, &push_number, &is_dev_frac, &decimal_fraction](const char& ch)
        {
            if (isOp({ ch }))
            {
                push_number();
                tokens.push_back({ ch });
            }
            else if (ch >= '0' && ch <= '9')
            {
                if (!num.has_value())
                    num.emplace(0);
                if (!is_dev_frac)
                    *num = *num * 10 + int(ch - '0');
                else
                    decimal_fraction = decimal_fraction * 10 + int(ch - '0');
            }
            else if (ch == ' ')
                push_number();
            else if (ch == '.')
                is_dev_frac = true;
        });
    push_number();
    return tokens;
}

std::string infix2postfix(std::string infix) {
    std::string postfix;
    const auto tokens = tokenize(infix);
    MyStack<char> opStack(1);
    std::for_each(std::begin(tokens), std::end(tokens), [&opStack, &postfix](const std::string& token)
        {
            if (isOp(token))
            {
                if (token == ")")
                {
                    while (opStack.get() != '(')
                    {
                        postfix += opStack.get();
                        postfix += ' ';
                        opStack.pop();
                    }
                    opStack.pop();
                }
                else
                {
                    const int priorityCurrent = priority(token[0]);
                    if (priorityCurrent == 0 || opStack.isEmpty() || priorityCurrent > priority(opStack.get()))
                        opStack.push(token[0]);
                    else
                    {
                        int priorityPrev;
                        while (!opStack.isEmpty() && (priorityPrev = priority(opStack.get())) >= priorityCurrent)
                        {
                            postfix += opStack.get();
                            postfix += ' ';
                            opStack.pop();
                        }
                        opStack.push(token[0]);
                    }
                }

            }
            else
            {
                postfix += token;
                postfix += ' ';
            }
        });
    while (!opStack.isEmpty())
    {
        postfix += opStack.get();
        postfix += ' ';
        opStack.pop();
    }
    if (postfix.back() == ' ')
        postfix.pop_back();
    return postfix;
}
