# ifndef EXPRESSION_H
# define EXPRESSION_H

# include "rational.h"
# include <stdlib.h>
# include <stdio.h>

// 表达式类型
typedef struct _expression {
    char op; // 操作符
    Rational value; // 当前表达式运算结果
    struct _expression *left; // 左子表达式
    struct _expression *right; // 左子表达式
} Expression;

// 表达式树叶子节点
Expression* getLeafExpression(Rational value);

// 表达式树二元操作节点
Expression* getBinaryExpression(Expression* left, Expression* right, char op);

// 表达式输出
void printExpression(Expression* expression, int bracket);

// 表达式化简至标准型
void simplifyExpression(Expression* expression);

// 判断表达式是否等价
int isExpressionEqual(Expression* e1, Expression* e2);

# endif