#pragma once

#include <iostream>
using namespace std;
#include "Compress.h"
#include <time.h>

int main()
{
	double t1 = clock();
	TestCompress();
	double t2 = clock() - t1;
	cout << t2 << endl;
	TestUnCompress();
	double t3 = clock() - t2;
	cout << t3 << endl;
	system("pause");
	return 0;
}