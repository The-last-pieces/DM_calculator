#include "calculator.h"

int main()
{
    calculator test;
    while (1)
    {
        try
        {
            cin >> test;
        }
        catch (const std::exception & er)
        {
            cout << er.what() << endl;
        }
    }

    return 0;
}
