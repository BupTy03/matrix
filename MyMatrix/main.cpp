#include<iostream>
#include <vector>
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

	// Matrix<int> mtx;

	// mtx.add_d1();
	// cout << mtx << endl;
	// mtx.add_d2(3);
	// cout << mtx << endl;
	// mtx.add_d2(4);
	// cout << mtx << endl;
	// mtx.add_d2(5);
	// cout << mtx << endl;
	// mtx.add_d1(6);
	// cout << mtx << endl;

/////////////////////////////////////////////////////

	// int arr[3] = { 1, 2, 3 };

	// Matrix<int> mtx(2, 3);

	// cout << mtx << endl;

	// cout << std::boolalpha;

	// cout << mtx.add_d1(arr) << endl;

	// cout << mtx << endl;

	// cout << mtx.add_d2(arr) << endl;

	// cout << mtx << endl;

	// std::vector<int> v(mtx.size_dim2(), 1);

	// cout << mtx.add_d1(v) << endl;

	// cout << mtx << endl;

	// int arr2[6] = { 1, 2, 3, 4, 5, 6 };

	// Matrix<int> mtx2{2, 3, arr2};

	// cout << mtx2 << endl;

	// mtx2.apply_to_row(1, [](auto& x){ return x*x; });

	// cout << mtx2 << endl;

	// mtx2.fill_row(0, 0);

	// cout << mtx2 << endl;

	/////////////////////////////////////////////////

	Matrix<int> mtx(6, 6);

	//cout << mtx << endl;

	//Matrix<int> mtx2(10, 10);
	Matrix<int> mtx2(0, 7);

	mtx2 = mtx;

	cout << mtx2 << endl;


	return 0;
}