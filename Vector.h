#pragma once

#include <initializer_list>

template <typename T>
class Vector
{
private:
	T* _begin;
	T* _end;

	unsigned int _size;
	unsigned int _capacity;

	void swap_for_sort(int first, int second);

public:
	Vector();

	Vector(const Vector& other);
	Vector(Vector&& other) noexcept;

	Vector(std::initializer_list<T> list);

	Vector(const int size);

	Vector(const int size, const T& value);

	~Vector();

	void push_back(const T& value);
	void push_back(std::initializer_list<T> list);

	void insert(int index, const T& value);
	void insert(int index, std::initializer_list<T> list);

	void pop_back();

	void erase(const int index);
	void erase(const int first_index, const int second_index);

	void assign(const int size);
	void assign(const int size, const T& value);
	void assign(std::initializer_list<T> list);

	bool search(const T& element);
	bool bin_search(const T& element);

	// return index of element
	int get(const T& element);
	int get(const T& element, const int start_index);

	void sort();
	void sort(const int first, const int last);

	void reserve(const int new_capacity);

	void clear();

	bool is_empty() const;

	T& operator[](const int index);
	const T& operator[](const int index) const;

	void operator=(const Vector& other);
	void operator=(Vector&& other) noexcept;
	void operator=(std::initializer_list<T> list);

	bool operator==(const Vector& other) const;

	unsigned int size() const;
	unsigned int capacity() const;

	T* begin() const;
	T* end() const;
};

template<typename T>
inline Vector<T>::Vector()
	: _begin(nullptr), _end(nullptr), _size(0), _capacity(2)
{ }

template<typename T>
inline Vector<T>::Vector(const Vector& other)
	: _size(other.size()), _capacity(other.capacity())
{
	_begin = new T[_capacity];
	_end = _begin + _size;

	for (unsigned int i = 0; i < _size; i++)
		*(_begin + i) = other[i];
}

template<typename T>
inline Vector<T>::Vector(Vector&& other) noexcept
	: _size(other.size()), _capacity(other.capacity()), _begin(other.begin()), _end(other.end())
{
	other._begin = nullptr;
	other._end = nullptr;

	other._size = 0;
	other._capacity = 0;
}

template<typename T>
inline Vector<T>::Vector(std::initializer_list<T> list)
{
	assign(list);
}

template<typename T>
inline Vector<T>::Vector(const int size)
{
	assign(size);
}

template<typename T>
inline Vector<T>::Vector(const int size, const T& value)
{
	assign(size, value);
}

template<typename T>
inline Vector<T>::~Vector()
{
	clear();
}

template<typename T>
inline void Vector<T>::push_back(const T& value)
{
	if (_size == _capacity)
	{
		_capacity *= 1.5;

		T* new_begin = new T[_capacity] {};

		for (int i = 0; i < _size; i++)
			*(new_begin + i) = *(_begin + i);

		*(new_begin + _size) = value;

		delete[] _begin;
		_begin = nullptr;
		_end = nullptr;

		_begin = new_begin;
		_end = _begin + _size + 1;

		_size++;
	}
	else
	{
		if (_begin == nullptr)
			_begin = new T[_capacity] {};

		*(_begin + _size) = value;
	
		_end = _begin + _size + 1;

		_size++;
	}
}

template<typename T>
inline void Vector<T>::push_back(std::initializer_list<T> list)
{
	if (_size + list.size() > _capacity)
	{
		while(_size + list.size() > _capacity)
			_capacity *= 1.5;

		T* new_begin = new T[_capacity]{};

		for (int i = 0; i < _size; i++)
			*(new_begin + i) = *(_begin + i);

		for (int i = _size; i < list.size() + _size; i++)
			*(new_begin + i) = *(list.begin() - _size + i);

		delete[] _begin;
		_begin = nullptr;
		_end = nullptr;

		_begin = new_begin;
		_end = _begin + _size + list.size();

		_size += list.size();
	}
	else
	{
		if (_begin == nullptr)
			_begin = new T[_capacity] {};

		for (int i = _size; i < list.size() + _size; i++)
			*(_begin + i) = *(list.begin() - _size + i);

		_end = _begin + _size + list.size();

		_size += list.size();
	}
}

template<typename T>
inline void Vector<T>::insert(int index, const T& value)
{
	if (index > _size || index < 0)
		throw std::out_of_range("Out of Range");

	if (_size == _capacity)
		_capacity *= 1.5;

	T* new_begin = new T[_capacity] {};

	for (int i = 0; i < index; i++)
		*(new_begin + i) = *(_begin + i);

	*(new_begin + index) = value;

	for (int i = index; i < _size; i++)
		*(new_begin + i + 1) = *(_begin + i);

	delete[] _begin;
	_begin = nullptr;
	_end = nullptr;

	_begin = new_begin;
	_end = _begin + _size + 1;

	_size++;
}

template<typename T>
inline void Vector<T>::insert(int index, std::initializer_list<T> list)
{
	if (index > _size || index < 0)
		throw std::out_of_range("Out of Range");

	while (_size + list.size() > _capacity)
		_capacity *= 1.5;

	T* new_begin = new T[_capacity]{};

	for (int i = 0; i < index; i++)
		*(new_begin + i) = *(_begin + i);

	for (int i = index; i < list.size() + index; i++)
		*(new_begin + i) = *(list.begin() - index + i);

	for (int i = list.size() + index; i < _size + list.size(); i++)
		*(new_begin + i) = *(_begin - list.size() + i);

	delete[] _begin;
	_begin = nullptr;
	_end = nullptr;

	_begin = new_begin;
	_end = _begin + _size + list.size();

	_size += list.size();
}

template<typename T>
inline void Vector<T>::pop_back()
{
	if (_size == 0)
		return;

	_end = _end - 1;

	_size--;

	if (_size == 0)
		_end = _end + 1;
}

template<typename T>
inline void Vector<T>::erase(const int index)
{
	if (index >= _size || index < 0)
		throw std::out_of_range("Out of Range");

	T* new_begin = new T[_capacity] {};

	for (int i = 0; i < index; i++)
		*(new_begin + i) = *(_begin + i);

	for (int i = index; i < _size; i++)
		*(new_begin + i) = *(_begin + i + 1);

	delete[] _begin;
	_begin = nullptr;
	_end = nullptr;

	_begin = new_begin;
	_end = _begin + _size - 1;

	_size--;
}

template<typename T>
inline void Vector<T>::erase(const int first_index, const int second_index)
{
	if (first_index > _size || first_index < 0 || second_index > _size || second_index < 0)
		throw std::out_of_range("out of range");

	if (second_index < first_index)
		throw std::exception("first index is bigger than second index");

	if (second_index == first_index)
		return;

	T* new_begin = new T[_capacity]{};

	for (int i = 0; i < first_index; i++)
		*(new_begin + i) = *(_begin + i);

	for (int i = second_index - 1; i < _size - 1; i++)
		*(new_begin + (i - (second_index - first_index - 1))) = *(_begin + i + 1);

	delete[] _begin;
	_begin = nullptr;
	_end = nullptr;

	_begin = new_begin;
	_end = _begin + _size - (second_index - first_index);
	T* test = _end - 1;

	_size -= (second_index - first_index);
}

template<typename T>
inline void Vector<T>::assign(const int size)
{
	delete[] _begin;
	_begin = nullptr;
	_end = nullptr;

	_size = size;
	_capacity = _size < 2 ? 2 : _size;

	_begin = new T[_capacity]{};
	_end = _begin + _size;
}

template<typename T>
inline void Vector<T>::assign(const int size, const T& value)
{
	delete[] _begin;
	_begin = nullptr;
	_end = nullptr;

	_size = size;
	_capacity = _size < 2 ? 2 : _size;

	_begin = new T[_capacity];
	_end = _begin + _size;

	for (T* iter = _begin; iter != _end; ++iter)
		*iter = value;
}

template<typename T>
inline void Vector<T>::assign(std::initializer_list<T> list)
{
	delete[] _begin;
	_begin = nullptr;
	_end = nullptr;

	_size = list.size();
	_capacity = _size < 2 ? 2 : _size;

	_begin = new T[_capacity];
	_end = _begin + _size;

	for (unsigned int i = 0; i < _size; i++)
		*(_begin + i) = *(list.begin() + i);
}

template<typename T>
inline bool Vector<T>::search(const T& element)
{
	for (int i = 0; i < _size; i++)
	{
		if (*(_begin + i) == element)
			return true;
	}

	return false;
}

template<typename T>
inline bool Vector<T>::bin_search(const T& element)
{
	int left = 0;
	int right = _size - 1;

	int middle = 0;

	do
	{
		middle = (left + right) / 2;

		if (*(_begin + middle) == element)
			return true;
		else if ((*(_begin + middle)) < element)
			left = middle + 1;
		else if (element < (*(_begin + middle)))
			right = middle - 1;
	} 
	while (left <= right);

	return false;
}

template<typename T>
int Vector<T>::get(const T& element)
{
	for (int i = 0; i < _size; i++)
	{
		if ((*(_begin + i)) == element)
			return i;
	}

	return -1;
}

template<typename T>
int Vector<T>::get(const T& element, const int start_index)
{
	for (int i = start_index; i < _size; i++)
	{
		if ((*(_begin + i)) == element)
			return i;
	}

	return -1;
}

template<typename T>
void Vector<T>::swap_for_sort(int first, int second)
{
	T temp = *(_begin + first);

	*(_begin + first) = *(_begin + second);
	*(_begin + second) = temp;
}

template<typename T>
inline void Vector<T>::sort(const int first, const int last)
{
	T middle = *(_begin + ((first + last) / 2));

	int i = first;
	int j = last;

	do
	{
		while ((*(_begin + i)) < middle)
			i++;
		while (middle < (*(_begin + j)))
			j--;

		if (i <= j)
		{
			swap_for_sort(i, j);

			i++;
			j--;
		}
	} 
	while (i <= j);

	if (i < last)
		sort(i, last);
	if (j > first)
		sort(first, j);
}

template<typename T>
inline void Vector<T>::sort()
{
	sort(0, _size - 1);
}

template<typename T>
inline void Vector<T>::reserve(const int new_capacity)
{
	if (new_capacity < _size)
		_size = new_capacity;
	else if (new_capacity == _size)
		return;

	_capacity = new_capacity;

	T* new_begin = new T[_capacity];

	for (int i = 0; i < _size; i++)
		*(new_begin + i) = *(_begin + i);

	delete[] _begin;
	_begin = nullptr;
	_end = nullptr;

	_begin = new_begin;
	_end = _begin + _size;
}

template<typename T>
inline void Vector<T>::clear()
{
	delete[] _begin;
	_begin = nullptr;
	_end = nullptr;

	_size = 0;
}

template<typename T>
inline bool Vector<T>::is_empty() const
{
	return _begin == nullptr ? true : false;
}

template<typename T>
inline T& Vector<T>::operator[](const int index)
{
	return *(_begin + index);
}

template<typename T>
inline const T& Vector<T>::operator[](const int index) const
{
	return *(_begin + index);
}

template<typename T>
inline void Vector<T>::operator=(const Vector& other)
{
	_size = other.size();
	_capacity = other.capacity();

	delete _begin;
	_begin = nullptr;
	_end = nullptr;

	_begin = new T[_capacity];
	_end = _begin + _size;

	for (unsigned int i = 0; i < _size; i++)
		*(_begin + i) = other[i];
}

template<typename T>
inline void Vector<T>::operator=(Vector&& other) noexcept
{
	_size = other.size();
	_capacity = other.capacity();
	_begin = other.begin();
	_end = other.end();

	other._begin = nullptr;
	other._end = nullptr;

	other._size = 0;
	other._capacity = 0;
}

template<typename T>
inline void Vector<T>::operator=(std::initializer_list<T> list)
{
	assign(list);
}

template<typename T>
bool Vector<T>::operator==(const Vector& other) const
{
	if (_size != other.size())
		return false;

	for (int i = 0; i < _size; i++)
	{
		if (*(_begin + i) != *(other.begin() + i))
			return false;
	}

	return true;
}

template<typename T>
inline unsigned int Vector<T>::size() const { return _size; }

template<typename T>
inline unsigned int Vector<T>::capacity() const { return _capacity; }

template<typename T>
inline T* Vector<T>::begin() const { return _begin; }

template<typename T>
inline T* Vector<T>::end() const { return _end; }