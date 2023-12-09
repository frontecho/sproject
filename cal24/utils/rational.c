# include <stdio.h>
# include <stdlib.h>
# include "rational.h"

// 有理数约分化简，指针传递
void simplify(Rational *r) {
    int isNegative = 0;
    if (r->den < 0) {
        isNegative = 1 - isNegative;
        r->den = -r->den;
    }
    if (r->num < 0) {
        isNegative = 1 - isNegative;
        r->num = -r->num;
    }

    // gcd
    long long x = r->num, y = r->den;
    while (y != 0) {
        long long r = x % y;
        x = y;
        y = r;
    }
    r->num /= x;
    r->den /= x;

    if (isNegative) r->num = -r->num;
}

// // 分子分母创建有理数
// Rational getRational(int i, int j) {
//     Rational r;
//     r.num = (long long)i;
//     r.den = (long long)j;
//     simplify(&r);
//     return r;
// }

// 整数创建有理数
Rational getIntRational(int i) {
    Rational r;
    r.num = (long long)i;
    r.den = 1LL;
    simplify(&r);
    return r;
}

// 有理数四则运算
Rational calRational(Rational a, Rational b, char op) {
    Rational c;
    switch (op) {
        case '+':
            c.num = a.num * b.den + a.den * b.num;
            c.den = a.den * b.den;
            break;
        case '-':
            c.num = a.num * b.den - a.den * b.num;
            c.den = a.den * b.den;
            break;
        case '*':
            c.num = a.num * b.num;
            c.den = a.den * b.den;
            break;
        case '/':
            //判断除数是否为0
            if (b.num == 0) {
                printf("ERROR: Division by zero!\n");
                exit(0);
            }
            c.num = a.num * b.den;
            c.den = a.den * b.num;
            break;
    }
    simplify(&c);
    return c;
}

// 有理数取负数
Rational negativeRational(Rational r) {
    Rational c;
    c.num = -r.num;
    c.den = r.den;
    return c;
}

// 有理数比较
int compareRational(Rational a, Rational b) {
    Rational c = calRational(a, b, '-');
    if (c.num == 0) return '='; // a = b
    else if (c.num > 0) return '>'; // a > b
    else return '<'; // a < b
}

// 有理数输出
void printRational(Rational r) {
    simplify(&r);
    if (r.den == 1) printf("%lld", r.num);
    else printf("%lld/%lld", r.num, r.den);
}