#pragma once
#include <vector>

// Avgör om ett element finn i en vektor
template <class T>
bool contains(const std::vector<T>& vec, const T& element)
{
	int elementCount = vec.size();
	for (int i = 0; i < elementCount; ++i)
	{
		if (vec[i] == element)
		{
			return true;
		}
	}

	return false;
}


// Returnerar det index ett element i en vector har, om det inte finns, returnerar denna funktion -1
template <class T>
int find(const std::vector<T>& vec, const T& element)
{
	int elementCount = vec.size();
	for (int i = 0; i < elementCount; ++i)
	{
		if (vec[i] == element)
		{
			return i;
		}
	}

	return -1;
}