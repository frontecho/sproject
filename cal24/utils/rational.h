#ifndef Rational_H
#define Rational_H

// 有理数类型
typedef struct {
    long long num; // 分子
    long long den; // 分母
} Rational;

void simplify(Rational *r);
// Rational getRational(int i, int j);
Rational getIntRational(int i);
Rational calRational(Rational a, Rational b, char op);
int compareRational(Rational a, Rational b);
void printRational(Rational r);

#endif