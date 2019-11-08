#include <iostream>
#include <map>
#include <stack>
#include <assert.h>
#include <cstdlib>
#include <ctime>
#include <vector>

static std::string to_postfix(const std::string& infix)
{
    static const std::map<char, short> prec = {
        { '^', 4 },
        { '*', 3 },
        { '/', 3 },
        { '+', 2 },
        { '-', 2 },
        { '(', 1 },
    };

    std::string postfix;
    std::stack<char> ops;

    for (auto token : infix) {
        if (isdigit(token)) {
            postfix += token;
        }
        else if (token == '(') {
            ops.push(token);
        }
        else if (token == ')') {
            while (ops.top() != '(') {
                postfix += ops.top();
                ops.pop();
            }
            ops.pop();
        }
        else {
            while (not ops.empty() and (not token or (prec.at(ops.top()) >= prec.at(token)))) {
                postfix += ops.top();
                ops.pop();
            }
            ops.push(token);
        }
    }

    while (not ops.empty()) {
        postfix += ops.top();
        ops.pop();
    }

    return postfix;
}

static std::vector<int> gen_random(int n)
{
    std::srand(unsigned(std::time(0)));
    std::vector<int> numbers;
    for (int i=0; i<n; ++i)
        numbers.push_back(1 + std::rand() % 9);
    return numbers;
}

template <class ...Args>
static std::string gen_infix(const std::string& fmt, Args&& ...args)
{
    std::vector<char> buf(fmt.size());
    snprintf(&buf[0], buf.size(), fmt.c_str(), args...);
    return std::string(buf.begin(), buf.end());
}

int main()
{
    assert(to_postfix("2+7*5") == "275*+");
    {
        auto nums = gen_random(3);
        auto infix = gen_infix("%d*%d+%d",
                               nums[0], nums[1], nums[2]);
        auto postfix = gen_infix("%d%d*%d+",
                                 nums[0], nums[1], nums[2]);
        assert(to_postfix(infix) == postfix);
    }
    {
        auto nums = gen_random(6);
        auto infix = gen_infix("(%d+%d)*%d-%d^(%d+%d)",
                               nums[0], nums[1], nums[2],
                               nums[3], nums[4], nums[5]);
        auto postfix = gen_infix("%d%d+%d*%d%d%d+^-",
                                 nums[0], nums[1], nums[2],
                                 nums[3], nums[4], nums[5]);
        assert(to_postfix(infix) == postfix);
    }
    {
        auto nums = gen_random(5);
        auto infix = gen_infix("%d^%d+%d-%d*%d",
                               nums[0], nums[1], nums[2],
                               nums[3], nums[4]);
        auto postfix = gen_infix("%d%d^%d+%d%d*-",
                                 nums[0], nums[1], nums[2],
                                 nums[3], nums[4]);
        assert(to_postfix(infix) == postfix);
    }
    {
        auto nums = gen_random(5);
        auto infix = gen_infix("%d^%d*(%d+%d)/%d",
                               nums[0], nums[1], nums[2],
                               nums[3], nums[4]);
        auto postfix = gen_infix("%d%d^%d%d+*%d/",
                                 nums[0], nums[1], nums[2],
                                 nums[3], nums[4]);
        assert(to_postfix(infix) == postfix);
    }
    {
        auto nums = gen_random(9);
        auto infix = gen_infix("(%d-%d-%d)+%d/%d/%d-%d/%d/%d",
                               nums[0], nums[1], nums[2],
                               nums[3], nums[4], nums[5],
                               nums[6], nums[7], nums[8]);
        auto postfix = gen_infix("%d%d-%d-%d%d/%d/+%d%d/%d/-",
                                 nums[0], nums[1], nums[2],
                                 nums[3], nums[4], nums[5],
                                 nums[6], nums[7], nums[8]);
        assert(to_postfix(infix) == postfix);
    }
}
