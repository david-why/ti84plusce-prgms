#include "TINYSTL/vector.h"
// #include <iostream>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
using namespace tinystl;

template <class ForwardIt, class T>
ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T &value)
{

    ForwardIt it;
    size_t count, step;
    count = last - first;

    while (count > 0)
    {
        it = first;
        step = count / 2;
        for (size_t i = 0; i < step; i++)
            it++;
        if (*it < value)
        {
            first = ++it;
            count -= step + 1;
        }
        else
            count = step;
    }
    return first;
}

template <typename T>
void insert(vector<T> &list, const T &value)
{
    list.insert(lower_bound(list.begin(), list.end(), value), value);
}