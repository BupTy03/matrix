#pragma once

#include<iostream>
#include<stdexcept>
#include<string>

using Index = long;

using namespace std;

struct Matrix_error : std::runtime_error{
	explicit Matrix_error(const char* q) : std::runtime_error(q){}
	explicit Matrix_error(string s) : std::runtime_error(s){}
};

template<typename T>
T Clamp(T& x, const T& lo, const T& hi)
{
	T result = x;
	if (x < lo) result = lo;
	else if (x > hi) result = hi;
	return x = result;
}


template<typename T>
class Matrix
{
private:
	T * * data{nullptr};
	Index dm1{0};
	Index dm2{0};
	Index sz{0};
	Index space_d1{0};
	Index space_d2{0};

	inline void range_check(Index i, Index j)
	{
		if (i < 0 || i >= dm1)
			throw Matrix_error("range error: dimension 1");
		if (j < 0 || j >= dm2)
			throw Matrix_error("range error: dimension 2");
	}

	inline void range_check(Index x, Index min, Index max)
	{
		if (x < min || x > max)
			throw Matrix_error("range error");
	}

public:
	explicit Matrix(Index x = 0, Index y = 0) : dm1(x), dm2(y), sz(dm1 * dm2), space_d1(dm1), space_d2(dm2)
	{
		if (dm1 < 0 || dm2 < 0)
			throw Matrix_error("Invalid argument for Matrix<T>::Matrix(Index, Index)");

		if(dm1 == 0) return;

		//cout << "Выделение памяти при помощи new в конструкторе" << endl;

		data = new T*[dm1];

		if(dm2 == 0) return;

		for (Index i = 0; i < dm1; ++i)
			data[i] = new T[dm2]();
	}
	explicit Matrix(const Index x, const Index y, const T& val) : Matrix(x, y)
	{
		for (Index i = 0; i < dm1; i++)
			for (Index j = 0; j < dm2; j++)
				data[i][j] = val;
	}

	template<class Container>
	explicit Matrix(const Index x, const Index y, const Container& cont) : Matrix(x, y)
	{
		if(std::size(cont) != x*y)
			throw Matrix_error("size of Container is not equal to size of Matrix");

		auto first = std::begin(cont);

		for(Index i = 0; i < dm1; ++i)
		{
			for(Index j = 0; j < dm2; ++j)
			{
				data[i][j] = *first;
				++first;
			}
		}
	}

	Matrix(const Matrix& other) : dm1(other.dm1), dm2(other.dm2), sz(other.sz)
	{
		//cout << "Выделение памяти при помощи new в конструкторе копирования" << endl;

		data = new T*[dm1];

		for (Index i = 0; i < dm1; ++i)
			data[i] = new T[dm2]();

		for (Index i = 0; i < dm1; ++i)
			for (Index j = 0; j < dm2; ++j)
				data[i][j] = other.data[i][j];

		return;
	}

	Matrix& operator=(const Matrix& other)
	{
		if(this == &other)
			return *this;

		if(other.dm1 == 0)
		{
			this->dm2 = other.dm2;
			return *this;
		}

		if(dm1 != 0)
		{
			if(space_d1 >= other.dm1)
			{
				space_d1 -= other.dm1;

				if(dm2 != 0)
				{
					if(space_d2 >= other.dm2){
						space_d2 -= other.dm2;
					}
					else{
						for(Index i = 0; i < dm1; ++i)
							delete[] data[i];

						for(Index i = 0; i < other.dm1; ++i)
							data[i] = new T[other.dm2];

						space_d2 = other.dm2;
					}
				}
				else{
					for(Index i = 0; i < other.dm1; ++i)
						data[i] = new T[other.dm2];
				}

				for(Index i = 0; i < other.dm1; ++i)
					for(Index j = 0; j < other.dm2; ++j)
						data[i][j] = other.data[i][j];

				dm1 = other.dm1;
				dm2 = other.dm2;
				sz = other.sz;
				return *this;
			}
			else{

				if(dm2 != 0){
					for(Index i = 0; i < dm1; ++i)
						delete[] data[i];
				}

				delete[] data;
			}
		}

		//cout << "Выделение памяти при помощи new в операторе присваивания" << endl;

		dm1 = other.dm1;
		dm2 = other.dm2;
		sz = other.sz;
		space_d1 = dm1;
		space_d2 = dm2;

		data = new T*[dm1];

		for (Index i = 0; i < dm1; ++i)
			data[i] = new T[dm2]();

		for (Index i = 0; i < dm1; ++i)
			for (Index j = 0; j < dm2; ++j)
				data[i][j] = other.data[i][j];

		return *this;
	}

	Matrix(Matrix&& other)
	{
		std::swap(data, other.data);
		std::swap(dm1, other.dm1);
		std::swap(dm2, other.dm2);
		std::swap(sz, other.sz);
		std::swap(space_d1, other.space_d1);
		std::swap(space_d2, other.space_d2);
	}

	Matrix& operator =(Matrix&& other)
	{
		if(this == &other)
			return *this;
		
		if(dm1 != 0)
		{
			if(dm2 != 0)
				for(Index i = 0; i < dm1; ++i)
					delete[] data[i];

			delete[] data;
		}

		data = other.data;
		dm1 = other.dm1;
		dm2 = other.dm2;
		sz = other.sz;
		space_d1 = other.space_d1;
		space_d2 = other.space_d2;

		other.data = nullptr;
		other.dm1 = 0;
		other.dm2 = 0;
		other.sz = 0;
		other.space_d1 = 0;
		other.space_d2 = 0;

		return *this;
	}

	inline Index size_dim1() const noexcept { return dm1; }
	inline Index size_dim2() const noexcept { return dm2; }
	inline Index size() const noexcept { return sz; }

	inline T const& operator ()(const Index i, const Index j) const
	{
		range_check(i, j);
		return data[i][j];
	}
	inline T& operator ()(Index i, Index j)
	{
		range_check(i, j);
		return data[i][j];
	}

	inline T const* operator [](Index i) const noexcept { return data[i]; }
	inline T* operator [](Index i) noexcept { return data[i]; }

	// rows [n:dm1)
	Matrix slice(Index n)
	{
		Clamp(n, static_cast<Index>(0), dm1 - 1);

		const Index newsz = dm1 - n;
		Matrix M(newsz, dm2);

		for (Index i = 0, k = n; i < newsz; ++i, ++k)
			for (Index j = 0; j < dm2; ++j)
				M[i][j] = this->data[k][j];

		return M;
	}
	// rows [n:dm1)
	const Matrix slice(Index n) const
	{
		Clamp(n, static_cast<Index>(0), dm1 - 1);

		const Index newsz = dm1 - n;
		Matrix M(newsz, dm2);

		for (Index i = 0, k = n; i < newsz; ++i, ++k)
			for (Index j = 0; j < dm2; ++j)
				M[i][j] = this->data[k][j];

		return M;
	}
	//	rows [n:m)
	Matrix slice(Index n, Index m)
	{
		Clamp(n, static_cast<Index>(0), dm1 - 1);
		Clamp(m, static_cast<Index>(0), dm1 - 1);

		if (m <= n) throw Matrix_error("wrong slice");

		const Index newsz = m - n + 1;
		Matrix M(newsz, dm2);

		for (Index i = 0, k = n; i < newsz; ++i, ++k)
			for (Index j = 0; j < dm2; ++j)
				M[i][j] = this->data[k][j];

		return M;
	}
	//	rows [n:m)
	const Matrix slice(Index n, Index m) const
	{
		Clamp(n, static_cast<Index>(0), dm1 - 1);
		Clamp(m, static_cast<Index>(0), dm1 - 1);

		if (m <= n) throw Matrix_error("wrong slice");

		const Index newsz = m - n + 1;
		Matrix M(newsz, dm2);

		for (Index i = 0, k = n; i < newsz; ++i, ++k)
			for (Index j = 0; j < dm2; ++j)
				M[i][j] = this->data[k][j];

		return M;
	}
	// rows and colls [n1:m1) and [n2:m2)
	Matrix slice(Index n1, Index n2, Index m1, Index m2)
	{
		Clamp(n1, static_cast<Index>(0), dm1 - 1);
		Clamp(m1, static_cast<Index>(0), dm1 - 1);
		Clamp(n2, static_cast<Index>(0), dm2 - 1);
		Clamp(m2, static_cast<Index>(0), dm2 - 1);

		if (m1 <= n1 || m2 <= n2) throw Matrix_error("wrong slice");

		const Index newsz_1 = m1 - n1 + 1;
		const Index newsz_2 = m2 - n2 + 1;
		Matrix M(newsz_1, newsz_2);

		for (Index i = 0, a = n1; i < newsz_1; ++i, ++a)
			for (Index j = 0, b = n2; j < newsz_2; ++j, ++b)
				M[i][j] = this->data[a][b];

		return M;
	}
	// rows and colls [n1:m1) and [n2:m2)
	const Matrix slice(Index n1, Index n2, Index m1, Index m2) const
	{
		Clamp(n1, static_cast<Index>(0), dm1 - 1);
		Clamp(m1, static_cast<Index>(0), dm1 - 1);
		Clamp(n2, static_cast<Index>(0), dm2 - 1);
		Clamp(m2, static_cast<Index>(0), dm2 - 1);

		if (m1 <= n1 || m2 <= n2) throw Matrix_error("wrong slice");

		const Index newsz_1 = m1 - n1 + 1;
		const Index newsz_2 = m2 - n2 + 1;
		Matrix M(newsz_1, newsz_2);

		for (Index i = 0, a = n1; i < newsz_1; ++i, ++a)
			for (Index j = 0, b = n2; j < newsz_2; ++j, ++b)
				M[i][j] = this->data[a][b];

		return M;
	}

	void fill(const T& val)
	{
		for (Index i = 0; i < dm1; ++i)
			for (Index j = 0; j < dm2; ++j)
				data[i][j] = val;
	}

	void fill_row(Index i, const T& val)
	{
		range_check(i, 0, dm1 - 1);

		for (Index j = 0; j < dm2; ++j)
			data[i][j] = val;
	}

	template<class Container>
	void fill_row(Index i, const Container& cont)
	{
		range_check(i, 0, dm1 - 1);

		if(std::size(cont) != dm2)
			throw Matrix_error("size of Container is not equal to size of this row");

		auto first = std::begin(cont);

		for(Index j = 0; j < dm2; ++j)
		{
			data[i][j] = *first;
			first++;
		}
	}

	void fill_col(Index j, const T& val)
	{
		range_check(j, 0, dm2 - 1);

		for (Index i = 0; i < dm1; ++i)
			data[i][j] = val;
	}

	template<class Container>
	void fill_col(Index j, const Container& cont)
	{
		range_check(j, 0, dm2 - 1);

		if(std::size(cont) != dm1)
			throw Matrix_error("size of Container is not equal to size of this col");

		auto first = std::begin(cont);

		for(Index i = 0; i < dm1; ++i)
		{
			data[i][j] = *first;
			first++;
		}
	}

	template<typename F, typename... Args>
	void apply(F func, Args&&... args)
	{
		for (Index i = 0; i < dm1; i++)
			for (Index j = 0; j < dm2; j++)
				data[i][j] = func(data[i][j], std::forward<Args>(args)...);
	}

	template<typename F, typename... Args>
	void apply_to_row(Index i, F func, Args&&... args)
	{
		range_check(i, 0, dm1 - 1);

		for (Index j = 0; j < dm2; ++j)
			data[i][j] = func(data[i][j], std::forward<Args>(args)...);
	}

	template<typename F, typename... Args>
	void apply_to_col(Index j, F func, Args&&... args)
	{
		range_check(j, 0, dm2 - 1);

		for (Index i = 0; i < dm1; ++i)
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

	void add_d1()
	{
		if (space_d1 == 0)
			reserve_d1(8);
		else if (dm1 == space_d1)
			reserve_d1(2 * space_d1);

		data[dm1] = new T[space_d2]();

		++dm1;
		sz = dm1 * dm2;
	}

	bool add_d1(const T& val)
	{
		if(dm2 == 0)
			return false;

		Index old_dm1 = dm1;

		add_d1();

		for (Index i = 0; i < dm2; i++)
			data[old_dm1][i] = val;

		return true;
	}

	template<class Container>
	bool add_d1(const Container& cont)
	{
		if(dm2 == 0 || std::size(cont) != dm2)
			return false;

		auto first 	= std::begin(cont);

		Index old_dm1 = dm1;

		add_d1();

		for (Index i = 0; i < dm2; i++)
		{
			data[old_dm1][i] = *first;
			first++;
		}

		return true;
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

	void add_d2()
	{
		if (space_d2 == 0)
			reserve_d2(8);
		else if (dm2 == space_d2)
			reserve_d2(2 * space_d2);

		for (Index i = 0; i < dm1; i++)
			data[i][dm2] = {};

		++dm2;
		sz = dm1 * dm2;
	}

	bool add_d2(const T& val)
	{
		if(dm1 == 0)
			return false;

		Index old_dm2 = dm2;

		add_d2();

		for (Index i = 0; i < dm1; i++)
			data[i][old_dm2] = val;

		return true;
	}

	template<class Container>
	bool add_d2(const Container& cont)
	{
		if(dm1 == 0 || std::size(cont) != dm1)
			return false;

		auto first = std::begin(cont);

		Index old_dm2 = dm2;

		add_d2();

		for (Index i = 0; i < dm1; ++i)
		{
			data[i][old_dm2] = *first;
			first++;
		}

		return true;
	}

	void del_d1() { del_d1(dm1 - 1); }
	void del_d1(Index num)
	{
		if (num < 0 || num >= dm1)
			throw Matrix_error("Index is outside of Matrix");

		delete data[num];

		--dm1;

		for (Index i = num; i < dm1; ++i)
			std::swap(data[i], data[i + 1]);

		sz = dm1 * dm2;
	}

	void del_d2() { del_d2(dm2 - 1); }
	void del_d2(Index num)
	{
		if (num < 0 || num >= dm2)
			throw Matrix_error("Index is outside of Matrix");

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
		MatrixIterator(T** p, Index s) noexcept : p(p), step(s - 1), left(0) {}
	public:
		MatrixIterator(const MatrixIterator &it) noexcept : p(it.p), step(it.step), left(it.left) {}
		MatrixIterator& operator =(const MatrixIterator&) = default;

		inline bool operator!=(MatrixIterator const& other) const noexcept { return p != other.p || left != other.left || step != other.step; }
		inline bool operator==(MatrixIterator const& other) const noexcept { return p == other.p && left == other.left && step == other.step; }
		inline T& operator*() const noexcept { return *(*p + left); }
		inline T* operator->() noexcept { return (*p + left); }
		inline MatrixIterator& operator++() noexcept
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
		inline MatrixIterator& operator++(int) noexcept
		{
			auto _tmp = *this;
			this->operator++();
			return _tmp;
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
		ConstMatrixIterator(T** p, Index s) noexcept : p(const_cast<const T**>(p)), step(s - 1), left(0) {}
	public:
		ConstMatrixIterator(const ConstMatrixIterator &it) noexcept : p(it.p), step(it.step), left(it.left) {}
		ConstMatrixIterator& operator =(const ConstMatrixIterator&) = default;

		inline bool operator!=(ConstMatrixIterator const& other) const noexcept { return p != other.p || left != other.left || step != other.step; }
		inline bool operator==(ConstMatrixIterator const& other) const noexcept { return p == other.p && left == other.left && step == other.step; }
		inline T const& operator*() const noexcept { return *(*p + left); }
		inline const T* operator->() const noexcept { return (*p + left); }
		inline ConstMatrixIterator& operator++() noexcept
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
		inline ConstMatrixIterator& operator++(int) noexcept
		{
			auto _tmp = *this;
			this->operator++();
			return _tmp;
		}
	private:
		const T** p;
		Index step;
		Index left;
	};

	using iterator = MatrixIterator;
	using const_iterator = ConstMatrixIterator;

	inline iterator begin() noexcept { return iterator(data, dm2); }
	inline iterator end() noexcept { return iterator(data + dm1, dm2); }

	inline const_iterator begin() const noexcept { return this->cbegin(); }
	inline const_iterator end() const noexcept { return this->cend(); }

	inline const_iterator cbegin() const noexcept { return const_iterator(data, dm2); }
	inline const_iterator cend() const noexcept { return const_iterator(data + dm1, dm2); }

	~Matrix()
	{
		//cout << "Удаление" << endl;

		if (dm1 == 0)
			return;

		if(dm2 != 0)
			for (Index i = 0; i < dm1; i++)
					delete[] data[i];

		delete[] data;
	}
};
