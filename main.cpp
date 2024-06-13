#include <fstream>
#include "orderedSet.h"
namespace lab_7
{
    static orderedSet PACK;
}
using namespace lab_7;


void fillFromFile(std::ifstream& in) {
    int num1, num2;

    while (in >> num1) {
        in >> num2;
        PACK.insert(num1,num2);
    }
}

int main()
{
    std::ifstream in("input.txt");
    fillFromFile(in);

    PACK.print();

    if (PACK.makeOrderly())
        PACK.print();

    return 0;
}
