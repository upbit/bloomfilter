#include <iostream>

#include "m_bloomfilter_expire.h"

using namespace std;

bool Compare(int &a, int &b)
{
	if (a > b)
		return true;
	else
		return false;
}

int main()
{
	BloomFilter<int> bf(Compare, 100000, 2);
	
	int elem1 = 1;
	bf.Add(elem1);
	cout << "Adding " << elem1 << endl;
	int elem2 = 2;
	bf.Add(elem2);
	cout << "Adding " << elem2 << endl;
	int elem3 = 3;
	bf.Add(elem3);
	cout << "Adding " << elem3 << endl;
	int elem4 = 4;
	bf.Add(elem4);
	cout << "Adding " << elem4 << endl;

	cout << elem1 << " is in BloomFilter: " << bf.In(elem1) << endl;
	cout << elem2 << " is in BloomFilter: " << bf.In(elem2) << endl;
	cout << elem3 << " is in BloomFilter: " << bf.In(elem3) << endl;
	cout << elem4 << " is in BloomFilter: " << bf.In(elem4) << endl;

	bf.PrintHeap();
	
	cout << "Do Expire" << endl;
	bf.Expire();
	cout << elem1 << " is in BloomFilter: " << bf.In(elem1) << endl;
	cout << elem2 << " is in BloomFilter: " << bf.In(elem2) << endl;
	cout << elem3 << " is in BloomFilter: " << bf.In(elem3) << endl;
	cout << elem4 << " is in BloomFilter: " << bf.In(elem4) << endl;
	
	bf.PrintHeap();
	
	return 0;
}

