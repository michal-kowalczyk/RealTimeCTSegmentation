#include <vector>
#include <iostream>
#include "tsutil.h"

using namespace std;

int main() {
	vector<double> t1;
	vector<double> t2;

	t1.push_back(4);
	t1.push_back(4);
	t1.push_back(5);
	t1.push_back(5);
	t1.push_back(6);
	t1.push_back(6);
	t1.push_back(7);
	t1.push_back(7);

	t2.push_back(4);
	t2.push_back(5);
	t2.push_back(6);
	t2.push_back(7);

	cout << "lcs=" << TimeSeries::lcs(t1,t2) << endl;
	cout << "lcs=" << TimeSeries::twlcs(t1,t2) << endl;
}
