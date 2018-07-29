#include<iostream>
#include"SMatrix.h"
#include"Matrix.h"

using namespace std;

int main()
{
	constexpr int arr[10]{ 1,2,3,4,5,6,7,8,9,10 };

	Matrix<int> M(2, 5);
	cout << M << endl;

	constexpr int arr2[5] = { 1,2,3,4,5 };
	constexpr int arr3[3] = { 1,2,3 };
	M.add_d1(arr2);
	M.add_d2(arr3);
	cout << M << endl;
	M.del_d1(1);
	cout << M << endl;

	system("pause");
	return 0;
}