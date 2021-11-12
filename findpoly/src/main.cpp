#ifndef HAS_PRINTF
typedef unsigned int uint24_t;
typedef int int24_t;
#endif

#include <tice.h>
#include <fileioc.h>

char buffer[512];

int main()
{
    uint8_t ans_type;
    cplx_list_t *lst;
    real_t zero = os_Int24ToReal(0), two = os_Int24ToReal(2);
    os_GetAnsData(&ans_type);
    if (ans_type == TI_CPLX_LIST_TYPE)
    {
        lst = (cplx_list_t *)os_GetAnsData(&ans_type);
    }
    else if (ans_type == TI_REAL_LIST_TYPE)
    {
        list_t *l = (list_t *)os_GetAnsData(&ans_type);
        lst = ti_AllocCplxList(l->dim, (void (*)(size_t))malloc);
        lst->dim = l->dim;
        for (uint16_t i = 0; i < l->dim; i++)
        {
            lst->items[i].real = l->items[i];
            lst->items[i].imag = zero;
        }
    }
    else
        return 0;

    if (lst->dim == 0)
        return 0;

    uint16_t len = lst->dim + 1;
    for (uint16_t i = 0; i < lst->dim; i++)
        if (os_RealCompare(&zero, &lst->items[i].imag))
            len++;

    real_t tmp1, tmp2;
    list_t *anslist = ti_AllocList(len, (void (*)(size_t))malloc);
    anslist->dim = len;
    for (uint16_t i = 0; i < len - 1; i++)
        anslist->items[i] = zero;
    anslist->items[len - 1] = os_Int24ToReal(1);

    uint16_t index = len - 1;
    for (uint16_t i = 0; i < lst->dim; i++)
    {
        if (os_RealCompare(&zero, &lst->items[i].imag)) // Complex, x^2 + ax + b
        {
            tmp1 = os_RealSub(&zero, &lst->items[i].real);
            real_t a = os_RealMul(&tmp1, &two);
            tmp1 = os_RealMul(&lst->items[i].imag, &lst->items[i].imag);
            tmp2 = os_RealMul(&lst->items[i].real, &lst->items[i].real);
            real_t b = os_RealAdd(&tmp1, &tmp2);
            for (uint16_t j = index; j < len; j++)
            {
                anslist->items[j - 2] = os_RealAdd(anslist->items + j - 2, anslist->items + j);
                tmp1 = os_RealMul(&a, anslist->items + j);
                anslist->items[j - 1] = os_RealAdd(anslist->items + j - 1, &tmp1);
                anslist->items[j] = os_RealMul(&b, anslist->items + j);
            }
            index -= 2;
        }
        else // Real, x - v
        {
            tmp1 = os_RealSub(&zero, &lst->items[i].real);
            for (uint16_t j = index--; j < len; j++)
            {
                anslist->items[j - 1] = os_RealAdd(anslist->items + j, anslist->items + j - 1);
                anslist->items[j] = os_RealMul(anslist->items + j, &tmp1);
            }
        }
    }

    ti_CloseAll();
    ti_SetVar(TI_REAL_LIST_TYPE, ti_Ans, anslist);

    return 0;
}
