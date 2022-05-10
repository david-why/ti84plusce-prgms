#ifndef __INT24_TYPE__
#define __INT24_TYPE__ int
#define __INT24_MAX__ 0x7fffff
#define __INT24_MIN__ (~__INT24_MAX__)
typedef __INT24_TYPE__ int24_t;
#define __UINT24_TYPE__ unsigned int
#define __UINT24_MAX__ 0xffffff
typedef __UINT24_TYPE__ uint24_t;
#endif

#include <tice.h>
#include <stdio.h>
#include <string.h>
#include <fileioc.h>
#include <tireal.hpp>

int isdeg()
{
    return os_TestFlagBits(1 << 2);
}
real_t torad(const real_t &r)
{
    if (isdeg())
        return os_RealDegToRad(&r);
    return r;
}
real_t todeg(const real_t &r)
{
    if (isdeg())
        return os_RealRadToDeg(&r);
    return r;
}

using namespace ti;
using namespace ti::literals;

char buf[128];

#define fa(A, B, C, a, b, c) A, B, C, a, b, c, A##2, B##2, C##2, a##2, b##2, c##2
#define ARGS real &A, real &B, real &C, real &a, real &b, real &c, real &A2 __attribute__((unused)), real &B2 __attribute__((unused)), real &C2 __attribute__((unused)), real &a2 __attribute__((unused)), real &b2 __attribute__((unused)), real &c2 __attribute__((unused))
#define defa fa(A, B, C, a, b, c)

void print(const real &r)
{
    static char buf[16];
    r.toCString(buf);
    os_PutStrFull(buf);
    os_NewLine();
}

// KNOWN: a, B, C
int asa(ARGS)
{
    A = real::pi() - B - C;
    if (A <= 0)
        return 0;
    b = a * B.sin() / A.sin();
    c = a * C.sin() / A.sin();
    return 1;
}
// KNOWN: a, A, B
int aas(ARGS)
{
    // os_ClrHome();
    // print(A);
    // print(B);
    // print(C);
    // print(a);
    // print(b);
    // print(c);
    // os_GetKey();
    C = real::pi() - A - B;
    if (C <= 0)
        return 0;
    return asa(defa);
}
// KNOWN: a, b, c
int sss(ARGS)
{
    C = ((a * a + b * b - c * c) / (a * b * 2)).asin();
    B = ((a * a + c * c - b * b) / (a * c * 2)).asin();
    A = real::pi() - B - C;
    return 1;
}
// KNOWN: a, b, C
int sas(ARGS)
{
    c = (a * a + b * b - a * b * 2 * C.cos()).sqrt();
    return sss(defa);
}
// KNOWN: a, b, A
int ssa(ARGS)
{
    real h = b * A.sin();
    if (h > a)
        return 0;
    if (h == a)
    {
        B = real::pi_2();
        C = B - A;
        c = (b * b - a * a).sqrt();
        return 1;
    }
    B = (b * A.sin() / a).acos();
    C = real::pi() - A - B;
    c = a * C.sin() / A.sin();
    if (a >= b)
        return 1;
    a2 = a;
    b2 = b;
    A2 = A;
    B2 = real::pi() - B;
    C2 = B - A;
    c2 = a2 * C2.sin() / A2.sin();
    return 2;
}

#define call(F, A, B, C, a, b, c) sol = F(fa(A, B, C, a, b, c))

int solve(ARGS)
{
    int sol = 0;

    if (false)
    {
    }
    else if (a > 0 && b > 0 && c > 0)
        call(sss, A, B, C, a, b, c);

    else if (a > 0 && b > 0 && C > 0)
        call(sas, A, B, C, a, b, c);
    else if (a > 0 && c > 0 && B > 0)
        call(sas, A, C, B, a, c, b);
    else if (b > 0 && c > 0 && A > 0)
        call(sas, C, B, A, c, b, a);

    else if (a > 0 && B > 0 && C > 0)
        call(asa, A, B, C, a, b, c);
    else if (b > 0 && A > 0 && C > 0)
        call(asa, B, A, C, b, a, c);
    else if (c > 0 && A > 0 && B > 0)
        call(asa, C, B, A, c, b, a);

    else if (a > 0 && A > 0 && B > 0)
        call(aas, A, B, C, a, b, c);
    else if (a > 0 && A > 0 && C > 0)
        call(aas, A, C, B, a, c, b);
    else if (b > 0 && A > 0 && B > 0)
        call(aas, B, A, C, b, a, c);
    else if (b > 0 && B > 0 && C > 0)
        call(aas, B, C, A, b, c, a);
    else if (c > 0 && A > 0 && C > 0)
        call(aas, C, A, B, c, a, b);
    else if (c > 0 && B > 0 && C > 0)
        call(aas, C, B, A, c, b, a);

    else if (a > 0 && b > 0 && A > 0)
        call(ssa, A, B, C, a, b, c);
    else if (a > 0 && b > 0 && B > 0)
        call(ssa, B, A, C, b, a, c);
    else if (a > 0 && c > 0 && A > 0)
        call(ssa, A, C, B, a, c, b);
    else if (a > 0 && c > 0 && C > 0)
        call(ssa, C, A, B, c, a, b);
    else if (b > 0 && c > 0 && B > 0)
        call(ssa, B, C, A, b, c, a);
    else if (b > 0 && c > 0 && C > 0)
        call(ssa, C, B, A, c, b, a);

    return sol;
}
real_t input_real(const char *text)
{
    static char buf[16];
    os_GetStringInput((char *)text, buf, 16);
    os_NewLine();
    return os_StrToReal(buf, NULL);
}

void runtests()
{
    real defa, A_, B_, C_, a_, b_, c_;

    A_ = real::pi() / 6;
    B_ = real::pi() / 3;
    C_ = real::pi() / 2;
    a_ = 1;
    b_ = (3_r).sqrt();
    c_ = 2;
    os_ClrHome();
    puts("Correct answer:");
    print(A_);
    print(B_);
    print(C_);
    print(a_);
    print(b_);
    print(c_);
    os_GetKey();
    for (int i = 0; i < 64; i++)
    {
        A = A_, B = B_, C = C_, a = a_, b = b_, c = c_;
        int v = 0;
        if ((i & 1) == 0)
            A = 0, ++v;
        if ((i & 2) == 0)
            B = 0, ++v;
        if ((i & 4) == 0)
            C = 0, ++v;
        if ((i & 8) == 0)
            a = 0, ++v;
        if ((i & 16) == 0)
            b = 0, ++v;
        if ((i & 32) == 0)
            c = 0, ++v;
        if (v != 3 || i == 7)
            continue;
        int s = solve(defa);
        if (s == 0 || !((A == A_ && B == B_ && C == C_ && a == a_ && b == b_ && c == c_) || (A2 == A_ && B2 == B_ && C2 == C_ && a2 == a_ && b2 == b_ && c2 == c_)))
        {
            os_ClrHome();
            os_PutStrFull("ERROR:");
            print(i);
            os_PutStrFull("S:");
            print(s);
            os_PutStrFull("A:");
            print(A);
            os_PutStrFull("B:");
            print(B);
            os_PutStrFull("C:");
            print(C);
            os_PutStrFull("a:");
            print(a);
            os_PutStrFull("b:");
            print(b);
            os_PutStrFull("c:");
            print(c);
            os_GetKey();
        }
    }
}

int main()
{
    os_ClrHome();
    uint8_t ans_type;
    list_t *list = (list_t *)os_GetAnsData(&ans_type);
    real A, B, C, a, b, c;
    if (ans_type != TI_REAL_LIST_TYPE || list->dim != 6)
    {
        A = torad(input_real("A?"));
        B = torad(input_real("B?"));
        C = torad(input_real("C?"));
        a = input_real("a?");
        b = input_real("b?");
        c = input_real("c?");
    }
    else
    {
        A = torad(list->items[0]), B = torad(list->items[1]), C = torad(list->items[2]);
        a = list->items[3], b = list->items[4], c = list->items[5];
    }
    real A2, B2, C2, a2, b2, c2;

    int sol = solve(defa);
    A = todeg(A);
    B = todeg(B);
    C = todeg(C);
    A2 = todeg(A2);
    B2 = todeg(B2);
    C2 = todeg(C2);
    os_ClrHome();
    os_PutStrFull("Answers:");
    print(sol);
    os_PutStrFull("A:");
    print(A);
    os_PutStrFull("B:");
    print(B);
    os_PutStrFull("C:");
    print(C);
    os_PutStrFull("a:");
    print(a);
    os_PutStrFull("b:");
    print(b);
    os_PutStrFull("c:");
    print(c);
    os_GetKey();
    if (sol == 2)
    {
        os_ClrHome();
        puts("Answer #2:");
        os_PutStrFull("A:");
        print(A);
        os_PutStrFull("B:");
        print(B);
        os_PutStrFull("C:");
        print(C);
        os_PutStrFull("a:");
        print(a);
        os_PutStrFull("b:");
        print(b);
        os_PutStrFull("c:");
        print(c);
        os_GetKey();
        list = ti_MallocList(13);
        list->items[0] = os_Int24ToReal(2);
        list->items[7] = A2;
        list->items[8] = B2;
        list->items[9] = C2;
        list->items[10] = a2;
        list->items[11] = b2;
        list->items[12] = c2;
    }
    else
        list = ti_MallocList(7);

    list->items[0] = os_Int24ToReal(sol);
    list->items[1] = A;
    list->items[2] = B;
    list->items[3] = C;
    list->items[4] = a;
    list->items[5] = b;
    list->items[6] = c;

    ti_SetVar(TI_REAL_LIST_TYPE, ti_Ans, list);

    return 0;
}
