#pragma once

#include<iostream>
#include<string>
#include<stdexcept>
#include<algorithm>

using Index = long;

using namespace std;

struct SMatrix_error : std::runtime_error{
	SMatrix_error(const char* q) : std::runtime_error(q){}
	SMatrix_error(string n) : std::runtime_error(n){}
};

template<typename T, const Index d1, const Index d2>
class SMatrix
{
private:
	T elem[d1][d2] = {};
	const Index sz = d1 * d2;

	constexpr void range_check(Index i, Index j) const
	{
		if (i < 0 || i >= d1)
			throw SMatrix_error("range error: dimension 1");
		if (j < 0 || j >= d2)
			throw SMatrix_error("range error: dimension 2");
	}

public:
	constexpr SMatrix() noexcept {}
	constexpr SMatrix(const T& val)
	{
		for (Index i = 0; i < d1; i++)
			for (Index j = 0; j < d2; j++)
				elem[i][j] = val;
	}
	template<const Index n>
	constexpr SMatrix(const T(&arr)[n])
	{
		static_assert(d1*d2 == n, "size of Matrix != size of array");

		Index num = 0;
		for (Index i = 0; i < d1; ++i)
			for (Index j = 0; j < d2; ++j)
				elem[i][j] = arr[num];
				num++;
	}
	constexpr SMatrix(std::initializer_list<T> init_list)
	{
		if (init_list.size() != this->sz)
			throw SMatrix_error("Invalid argument for constructor SMatrix<T>::SMatrix(std::initializer_list<T>)");

		auto initlstIt = init_list.begin();
		for (Index i = 0; i < d1; ++i) 
		{
			for (Index j = 0; j < d2; ++j)
			{
				elem[i][j] = *initlstIt;
				initlstIt++;
			}
		}
	}
	
	constexpr SMatrix(const SMatrix& other)
	{
		for (Index i = 0; i < d1; ++i)
			for (Index j = 0; j < d2; ++j)
				this->elem[i][j] = other.elem[i][j];
	}
	constexpr SMatrix& operator =(const SMatrix& other)
	{
		if(this == &other)
			return *this;

		for (Index i = 0; i < d1; ++i)
			for (Index j = 0; j < d2; ++j)
				this->elem[i][j] = other.elem[i][j];
	}

	SMatrix(SMatrix&&) = delete;
	SMatrix& operator =(SMatrix&&) = delete;

	constexpr inline Index size_dim1() const noexcept { return d1; }
	constexpr inline Index size_dim2() const noexcept { return d2; }
	constexpr inline Index size() const noexcept{ return sz; }

	inline T* data() noexcept { return (T*)elem; }
	inline T const* data() const noexcept { return (T const*)elem; }

	constexpr inline T const& operator ()(const Index i, const Index j) const
	{
		range_check(i, j);
		return elem[i][j];
	}
	inline T& operator ()(const Index i, const Index j)
	{
		range_check(i, j);
		return elem[i][j];
	}


	constexpr inline T const* operator [](Index i) const { return elem[i]; }
	inline T* operator [](Index i) { return elem[i]; }

	void fill(const T& val)
	{
		for (Index i = 0; i < d1; i++)
			for (Index j = 0; j < d2; j++)
				elem[i][j] = val;
	}

	template<typename F, typename... Args>
	void apply(F func, Args&&... args)
	{
		for (Index i = 0; i < d1; i++)
			for (Index j = 0; j < d2; j++)
				data[i][j] = func(data[i][j], std::forward<Args>(args)...);
	}

	friend ostream& operator <<(ostream& os, const SMatrix<T, d1, d2>& m)
	{
		os << "{ ";
		for (Index i = 0; i < d1; i++)
		{
			os << "{ ";
			for (Index j = 0; j < d2; j++)
			{
				os << m[i][j];

				if (j != d2 - 1)
					os << ", ";
			}
			os << " }";

			if (i != d1 - 1)
				os << ", \n  ";
		}
		os << " }";

		return os;
	}

	using iterator = T * ;
	using const_iterator = T const*;

	inline iterator begin() noexcept { return (T*)elem; }
	inline iterator end() noexcept { return (T*)elem + sz; }

	inline const_iterator begin() const noexcept { return this->cbegin(); }
	inline const_iterator end() const noexcept { return this->cend(); }

	inline const_iterator cbegin() const noexcept { return (T const*)elem; }
	inline const_iterator cend() const noexcept { return (T const*)elem + sz; }
};
