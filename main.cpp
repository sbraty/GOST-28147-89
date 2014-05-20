#include "gost.h"

using namespace std;

int main()
{
    cout << "GOST 28147-89" << endl;
    cout << "Start" << endl;
    char key[] = "30091993300919933009199330091993\0";
    GOST X(key);

    X.Crypt();
    X.Decrypt();
    cout << "Done" << endl;
    return 0;
}

