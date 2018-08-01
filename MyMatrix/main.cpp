#include<iostream>
//#include"SMatrix.h"
#include"Matrix.h"

using namespace std;

int main()
{
	// constexpr int arr[10]{ 1,2,3,4,5,6,7,8,9,10 };
	// constexpr SMatrix<int, 3, 4> matrix = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	// constexpr SMatrix<int, 1, 10> matrix1{ arr };
	// SMatrix<int, 3, 4> matrix2 = matrix;

	// matrix2.fill(9);

	// matrix2(1, 2) = 3;

	// cout << matrix2 << endl;

	// int arr2[matrix(1, 2)];
	// int arr3[matrix[1][2]];

	// cout << std::size(arr2) << endl;

	// try{
	// 	cout << matrix(1000, 1000) << endl;	// throws range error
	// }
	// catch(runtime_error e){
	// 	cout << e.what() << endl;
	// }

	Matrix<int> mtx;
	mtx.add_d1();
	cout << mtx << endl;
	mtx.add_d2(3);
	cout << mtx << endl;
	mtx.add_d2(4);
	cout << mtx << endl;
	mtx.add_d2(5);
	cout << mtx << endl;
	mtx.add_d1(6);
	cout << mtx << endl;

	return 0;
}