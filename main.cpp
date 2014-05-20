#include "gost.h"

using namespace std;

int main()
{
    cout << "GOST 28147-89" << endl;
    cout << "Start" << endl;
    char key[] = "58943658563456738695486799257846\0";
    GOST X(key);

    X.Crypt();
    X.Decrypt();
    cout << "Done" << endl;
    return 0;
}

