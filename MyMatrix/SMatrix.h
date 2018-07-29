#pragma once

#include<iostream>
#include<exception>

using Index = long;

using namespace std;

template<typename T, const Index d1, const Index d2>
class SMatrix
{
private:
	T elem[d1][d2] = {};
	const Index sz = d1 * d2;

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
		{
			for (Index j = 0; j < d2; ++j)
			{
				elem[i][j] = arr[num];
				num++;
			}
		}
	}
	SMatrix(const std::initializer_list<T> init_list)
	{
		if (init_list.size() != this->sz)
			throw invalid_argument{ "Invalid argument for constructor SMatrix<T>::SMatrix(std::initializer_list<T>)" };

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
	
	SMatrix(const SMatrix& other)
	{
		std::copy(other.cbegin(), other.cend(), this->begin());
	}
	SMatrix& operator =(const SMatrix&) = default;

	SMatrix(SMatrix&&) = delete;
	SMatrix& operator =(SMatrix&&) = delete;

	constexpr Index size_dim1() const { return d1; }
	constexpr Index size_dim2() const { return d2; }
	constexpr Index size() const { return sz; }

	T* data() { return (T*)elem; }
	const T* data() const { return (const T*)elem; }

	//  доступ с проверкой диапазона
	T const& operator ()(const Index i, const Index j) const
	{
		if (i < 0 || i >= d1 || j < 0 || j >= d2)
			throw out_of_range("Index is outside of SMatrix");

		return elem[i][j];
	}
	T& operator ()(const Index i, const Index j)
	{
		if (i < 0 || i >= d1 || j < 0 || j >= d2)
			throw out_of_range("Index is outside of SMatrix");

		return elem[i][j];
	}

	//  доступ без проверки диапазона
	T const* operator [](Index i) const { return elem[i]; }
	T* operator [](Index i) { return elem[i]; }

	void fill(const T& val)
	{
		for (Index i = 0; i < d1; i++)
			for (Index j = 0; j < d2; j++)
				elem[i][j] = val;
	}

	// применяет переданную функцию/функциональный объект к каждому M[i][j]
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

	iterator begin() { return (T*)elem; }
	iterator end() { return (T*)elem + sz; }

	const_iterator cbegin() const { return (T const*)elem; }
	const_iterator cend() const { return (T const*)elem + sz; }
};