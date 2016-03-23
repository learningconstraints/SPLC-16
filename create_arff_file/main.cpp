#include <iostream>

#include "csv_to_arff.h"

using namespace std;

int main()
{
    cout << "Hello world!" << endl;

    csv_to_arff parser("data_train_test.csv","data_all.arff");

    parser.write();

    parser.close();

    return 0;
}
