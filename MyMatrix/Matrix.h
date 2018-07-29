#pragma once

#include<iostream>
#include<exception>
#include<initializer_list>

using Index = long;

using namespace std;

template<typename T>
class Matrix
{
private:
	T * * data;
	Index dm1;
	Index dm2;
	Index sz;
	Index space_d1;    // ����� ������ ��� dm1
	Index space_d2;    // ����� ������ ��� dm2

public:
	Matrix() noexcept : data(nullptr), dm1(0), dm2(0), sz(0), space_d1(0), space_d2(0) {}
	Matrix(const Index x, const Index y) : dm1(x), dm2(y), sz(dm1 * dm2), space_d1(dm1), space_d2(dm2)
	{
		if (dm1 <= 0 || dm2 <= 0)
			throw invalid_argument("Invalid argument for Matrix<T>::Matrix(const Index, const Index)");

		data = new T*[dm1];

		for (Index i = 0; i < dm1; i++)
			data[i] = new T[dm2]();

	}
	Matrix(const Index x, const Index y, const T& val) : Matrix(x, y)
	{
		for (Index i = 0; i < dm1; i++)
			for (Index j = 0; j < dm2; j++)
				data[i][j] = val;
	}
	Matrix(const Matrix& other) : dm1(other.dm1), dm2(other.dm2), sz(other.sz), space_d1(other.space_d1), space_d2(other.space_d2)
	{
		for (Index i = 0; i < dm1; ++i)
			for (Index j = 0; j < dm2; ++j)
				data[i][j] = other.data[i][j];
	}
	Matrix(Matrix&& other) : Matrix()
	{
		swap(data, other.data);
		swap(dm1, other.dm1);
		swap(dm2, other.dm2);
		swap(sz, other.sz);
		swap(space_d1, other.space_d1);
		swap(space_d2, other.space_d2);
	}

	Matrix& operator=(const Matrix&) = default;
	Matrix& operator =(Matrix&&) = default;

	Index size_dim1() const noexcept { return dm1; }
	Index size_dim2() const noexcept { return dm2; }
	Index size() const noexcept { return sz; }

	//  ���������� � ��������� ���������
	T const& operator ()(const Index i, const Index j) const
	{
		if (i < 0 || i >= dm1 || j < 0 || j >= dm2)
			throw out_of_range("Index is outside of Matrix");

		return data[i][j];
	}
	T& operator ()(Index i, Index j)
	{
		if (i < 0 || i >= dm1 || j < 0 || j >= dm2)
			throw out_of_range("Index is outside of Matrix");

		return data[i][j];
	}

	T const* operator [](Index i) const { return data[i]; }
	T* operator [](Index i) { return data[i]; }

	void fill(const T& val)
	{
		for (Index i = 0; i < dm1; i++)
			for (Index j = 0; j < dm2; j++)
				data[i][j] = val;
	}

	// ��������� ���������� �������/�������������� ������ � ������� M[i][j]
	template<typename F, typename... Args>
	void apply(F func, Args&&... args)
	{
		for (Index i = 0; i < dm1; i++)
			for (Index j = 0; j < dm2; j++)
				data[i][j] = func(data[i][j], std::forward<Args>(args)...);
	}

	void reserve_d1(Index newalloc)
	{
		if (newalloc <= space_d1) return;

		T** p = new T*[newalloc];

		for (Index i = 0; i < dm1; ++i)
			p[i] = data[i];

		delete[] data;
		data = p;
		space_d1 = newalloc;
	}
	Index capacity_d1() const { return space_d1; }
	void resize_d1(Index newsize)
	{
		if (newsize <= dm1) return;

		reserve_d1(newsize);
		for (Index i = dm1; i < newsize; ++i)
			data[i] = new T[space_d2]();

		dm1 = newsize;
		sz = dm1 * dm2;
	}
	void add_d1(const T& val)
	{
		if (space_d1 == 0)
			reserve_d1(8);
		else if (dm1 == space_d1)
			reserve_d1(2 * space_d1);

		data[dm1] = new T[space_d2];

		for (Index i = 0; i < dm2; i++)
			data[dm1][i] = val;

		++dm1;
		sz = dm1 * dm2;
	}
	template<typename Iter>
	void add_d1(Iter first, Iter last)
	{
		if (std::distance(first, last) != dm2)
			return;

		if (space_d1 == 0)
			reserve_d1(8);
		else if (dm1 == space_d1)
			reserve_d1(2 * space_d1);

		data[dm1] = new T[space_d2];

		for (Index i = 0; i < dm2; i++)
		{
			data[dm1][i] = *first;
			first++;
		}

		++dm1;
		sz = dm1 * dm2;
	}
	template<int n>
	void add_d1(const T(&arr)[n])
	{
		if (n != dm2)
			throw invalid_argument{ "size of array is not equal to size of dimention1" };

		if (space_d1 == 0)
			reserve_d1(8);
		else if (dm1 == space_d1)
			reserve_d1(2 * space_d1);

		data[dm1] = new T[space_d2];

		for (Index i = 0; i < n; ++i)
			data[dm1][i] = arr[i];

		++dm1;
		sz = dm1 * dm2;
	}

	void reserve_d2(Index newalloc)
	{
		if (newalloc <= space_d2) return;

		for (Index i = 0; i < dm1; i++)
		{
			T* p = new T[newalloc];

			for (Index j = 0; j < dm2; j++)
				p[j] = data[i][j];

			delete[] data[i];
			data[i] = p;
		}

		space_d2 = newalloc;
	}
	Index capacity_d2() const { return space_d2; }
	void resize_d2(Index newsize)
	{
		if (newsize <= dm2) return;

		reserve_d2(newsize);
		for (Index i = 0; i < dm1; i++)
		{
			for (Index j = dm2; j < newsize; j++)
				data[i][j] = {};
		}

		dm2 = newsize;
		sz = dm1 * dm2;
	}
	void add_d2(const T& val)
	{
		if (space_d2 == 0)
			reserve_d2(8);
		else if (dm2 == space_d2)
			reserve_d2(2 * space_d2);

		for (Index i = 0; i < dm1; i++)
			data[i][dm2] = val;

		++dm2;
		sz = dm1 * dm2;
	}
	template<typename Iter>
	void add_d2(Iter first, Iter last)
	{
		if (std::distance(first, last) != dm1)
			return;

		if (space_d2 == 0)
			reserve_d2(8);
		else if (dm2 == space_d2)
			reserve_d2(2 * space_d2);

		for (Index i = 0; i < dm1; ++i)
		{
			data[i][dm2] = *first;
			first++;
		}

		++dm2;
		sz = dm1 * dm2;
	}
	template<int n>
	void add_d2(const T(&arr)[n])
	{
		if (n != dm1)
			return;

		if (space_d2 == 0)
			reserve_d2(8);
		else if (dm2 == space_d2)
			reserve_d2(2 * space_d2);

		for (Index i = 0; i < n; ++i)	
			data[i][dm2] = arr[i];

		++dm2;
		sz = dm1 * dm2;
	}

	void del_d1()
	{
		del_d1(dm1 - 1);
	}
	void del_d1(Index num)
	{
		if (num < 0 || num >= dm1)
			throw out_of_range("Index is outside of Matrix");

		delete data[num];

		--dm1;

		for (Index i = num; i < dm1; ++i)
			std::swap(data[i], data[i + 1]);

		sz = dm1 * dm2;
	}

	void del_d2()
	{
		del_d2(dm2 - 1);
	}
	void del_d2(Index num)
	{
		if (num < 0 || num >= dm2)
			throw out_of_range("Index is outside of Matrix");

		--dm2;

		for (Index i = 0; i < dm1; ++i)
			for (Index j = num; j < dm2; ++j)
				std::swap(data[i][j], data[i][j + 1]);
		
		sz = dm1 * dm2;
	}

	friend ostream& operator <<(ostream& os, const Matrix& m)
	{
		os << "{" << endl;
		for (Index i = 0; i < m.dm1; i++)
		{
			os << "{ ";
			for (Index j = 0; j < m.dm2; j++)
			{
				os << m[i][j];

				if (j != m.dm2 - 1)
					os << ", ";
			}
			os << " }";

			if (i != m.dm1 - 1)
				os << ", ";

			os << endl;
		}
		os << "}";

		return os;
	}

	class MatrixIterator : public std::iterator<std::input_iterator_tag, T>
	{
		friend class Matrix;
	private:
		MatrixIterator(T** p, Index s) : p(p), step(s - 1), left(0) {}
	public:
		MatrixIterator(const MatrixIterator &it) : p(it.p), step(it.step), left(it.left) {}
		MatrixIterator& operator =(const MatrixIterator&) = default;

		bool operator!=(MatrixIterator const& other) const { return p != other.p || left != other.left || step != other.step; }
		bool operator==(MatrixIterator const& other) const { return p == other.p && left == other.left && step == other.step; }
		T& operator*() const { return *(*p + left); }
		MatrixIterator& operator++()
		{
			if (left >= step)
			{
				left = 0;
				++p;
				return *this;
			}
			left++;
			return *this;
		}
		MatrixIterator& operator++(int)
		{
			if (left >= step)
			{
				left = 0;
				++p;
				return *this;
			}
			left++;
			return *this;
		}
	private:
		T * * p;
		Index step;
		Index left;
	};

	class ConstMatrixIterator : public std::iterator<std::input_iterator_tag, T>
	{
		friend class Matrix;
	private:
		ConstMatrixIterator(T** p, Index s) : p(const_cast<const T**>(p)), step(s - 1), left(0) {}
	public:
		ConstMatrixIterator(const ConstMatrixIterator &it) : p(it.p), step(it.step), left(it.left) {}
		ConstMatrixIterator& operator =(const ConstMatrixIterator&) = default;

		bool operator!=(ConstMatrixIterator const& other) const { return p != other.p || left != other.left || step != other.step; }
		bool operator==(ConstMatrixIterator const& other) const { return p == other.p && left == other.left && step == other.step; }
		T const& operator*() const { return *(*p + left); }
		ConstMatrixIterator& operator++()
		{
			if (left >= step)
			{
				left = 0;
				++p;
				return *this;
			}
			left++;
			return *this;
		}
		ConstMatrixIterator& operator++(int)
		{
			if (left >= step)
			{
				left = 0;
				++p;
				return *this;
			}
			left++;
			return *this;
		}
	private:
		const T** p;
		Index step;
		Index left;
	};

	using iterator = MatrixIterator;
	using const_iterator = ConstMatrixIterator;

	iterator begin() { return iterator(data, dm2); }
	iterator end() { return iterator(data + dm1, dm2); }

	const_iterator cbegin() { return const_iterator(data, dm2); }
	const_iterator cend() { return const_iterator(data + dm1, dm2); }

	~Matrix()
	{
		if (data == nullptr)
			return;

		for (Index i = 0; i < dm1; i++)
			delete[] data[i];

		delete[] data;
	}
};