#ifndef HAS_PRINTF
typedef unsigned int uint24_t;
typedef int int24_t;
#endif

#include <tice.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

char buffer[256];

template <class BidirIt>
void iter_swap(BidirIt a, BidirIt b)
{
    auto x = *a;
    *a = *b;
    *b = x;
}

template <class BidirIt>
void reverse(BidirIt first, BidirIt last)
{
    while ((first != last) && (first != --last))
        iter_swap(first++, last);
}

template <class BidirIt>
bool next_permutation(BidirIt first, BidirIt last)
{
    if (first == last)
        return false;
    BidirIt i = last;
    if (first == --i)
        return false;

    while (true)
    {
        BidirIt i1, i2;

        i1 = i;
        if (*--i < *i1)
        {
            i2 = last;
            while (!(*i < *--i2))
                ;
            iter_swap(i, i2);
            reverse(i1, last);
            return true;
        }
        if (i == first)
        {
            reverse(first, last);
            return false;
        }
    }
}

int compare(const void *a, const void *b)
{
    float arg1 = *(const float *)a;
    float arg2 = *(const float *)b;

    if (arg1 < arg2)
        return -1;
    if (arg1 > arg2)
        return 1;
    return 0;
}

constexpr float run(float left, uint8_t op, float right)
{
    if (left == INT_MIN || right == INT_MIN)
        return INT_MIN;
    if (op == 0)
        return left + right;
    if (op == 1)
        return left - right;
    if (op == 2)
        return left * right;
    if (right == 0)
        return INT_MIN;
    return left / right;
}

const char ops[] = "+-*/";
#define is24(a) __extension__(  \
    {                           \
        float x = a;            \
        x > 23.99 && x < 24.01; \
    })
#define check(n, fmt)                                                                                            \
    if (is24(n))                                                                                                 \
    {                                                                                                            \
        sprintf(buffer, fmt, (int)nums[0], (int)nums[1], (int)nums[2], (int)nums[3], ops[a1], ops[a2], ops[a3]); \
        while (!os_GetCSC())                                                                                     \
            ;                                                                                                    \
        return 0;                                                                                                \
    }

int main()
{
    os_ClrHome();
    float nums[4];
    real_t tmp;
    os_PutStrFull("{A B C D}");
    os_NewLine();

    os_GetStringInput((char *)"A=?", buffer, 5);
    os_NewLine();
    tmp = os_StrToReal(buffer, NULL);
    nums[0] = os_RealToFloat(&tmp);
    os_GetStringInput((char *)"B=?", buffer, 5);
    os_NewLine();
    tmp = os_StrToReal(buffer, NULL);
    nums[1] = os_RealToFloat(&tmp);
    os_GetStringInput((char *)"C=?", buffer, 5);
    os_NewLine();
    tmp = os_StrToReal(buffer, NULL);
    nums[2] = os_RealToFloat(&tmp);
    os_GetStringInput((char *)"D=?", buffer, 5);
    os_NewLine();
    tmp = os_StrToReal(buffer, NULL);
    nums[3] = os_RealToFloat(&tmp);
    qsort(nums, 4, sizeof(float), compare);

    do
    {
        for (uint8_t a1 = 0; a1 < 4; a1++)
        {
            for (uint8_t a2 = 0; a2 < 4; a2++)
            {
                for (uint8_t a3 = 0; a3 < 4; a3++)
                {
                    if (is24(run(nums[0], a3, run(nums[1], a2, run(nums[2], a1, nums[3])))))
                    {
                        sprintf(buffer, "%d %d %d %d %c%c%c", (int)nums[0], (int)nums[1], (int)nums[2], (int)nums[3], ops[a1], ops[a2], ops[a3]);
                        os_PutStrFull(buffer);
                        while (!os_GetCSC())
                            ;
                        return 0;
                    }
                    if (is24(run(nums[0], a3, run(run(nums[1], a1, nums[2]), a2, nums[3]))))
                    {
                        sprintf(buffer, "%d %d %d %c %d %c%c", (int)nums[0], (int)nums[1], (int)nums[2], ops[a1], (int)nums[3], ops[a2], ops[a3]);
                        os_PutStrFull(buffer);
                        while (!os_GetCSC())
                            ;
                        return 0;
                    }
                    if (is24(run(run(nums[0], a2, run(nums[1], a1, nums[2])), a3, nums[3])))
                    {
                        sprintf(buffer, "%d %d %d %c%c %d %c", (int)nums[0], (int)nums[1], (int)nums[2], ops[a1], ops[a2], (int)nums[3], ops[a3]);
                        os_PutStrFull(buffer);
                        while (!os_GetCSC())
                            ;
                        return 0;
                    }
                    if (is24(run(run(nums[0], a1, nums[1]), a3, run(nums[2], a2, nums[3]))))
                    {
                        sprintf(buffer, "%d %d %c %d %d %c%c", (int)nums[0], (int)nums[1], ops[a1], (int)nums[2], (int)nums[3], ops[a2], ops[a3]);
                        os_PutStrFull(buffer);
                        while (!os_GetCSC())
                            ;
                        return 0;
                    }
                    if (is24(run(run(run(nums[0], a1, nums[1]), a2, nums[2]), a3, nums[3])))
                    {
                        sprintf(buffer, "%d %d %c %d %c %d %c", (int)nums[0], (int)nums[1], ops[a1], (int)nums[2], ops[a2], (int)nums[3], ops[a3]);
                        os_PutStrFull(buffer);
                        while (!os_GetCSC())
                            ;
                        return 0;
                    }
                }
            }
        }
    } while (next_permutation(nums, nums + 4));

    os_PutStrFull("No answers found");
    while (!os_GetCSC())
        ;

    return 0;
}
