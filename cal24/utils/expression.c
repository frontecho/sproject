# include "expression.h"

// 表达式树叶子节点
Expression* getLeafExpression(Rational value) {
    Expression* e = (Expression*)malloc(sizeof(Expression));
    e->op = 0;
    e->value = value;
    e->left = NULL;
    e->right = NULL;
    return e;
}

// 表达式树二元操作节点
Expression* getBinaryExpression(Expression* left, Expression* right, char op) {
    Expression* e = (Expression*)malloc(sizeof(Expression));
    e->op = op;
    e->value = calRational(left->value, right->value, op);

    e->left = left;
    e->right = right;
    return e;
}

// 定义操作符优先级
static int getPrecedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            return 0;
    }
}

// 定义操作符的相反操作符
static char oppositeOp(char op) {
    switch (op) {
        case '+':
            return '-';
        case '-':
            return '+';
        case '*':
            return '/';
        case '/':
            return '*';
        default:
            return 0;
    }
}

// 检查是否需要括号
static int needParentheses(char parentOp, int currentOp, int isRightChild) {
    // 如果当前操作的优先级低于父操作的优先级，或者相同但当前操作在右子节点且父操作是减法或除法，则需要括号
    if (currentOp == 0) return 0; // 叶子节点不需要括号
    int parentPrecedence = getPrecedence(parentOp);
    int currentPrecedence = getPrecedence(currentOp);
    if (currentPrecedence < parentPrecedence || 
        (currentPrecedence == parentPrecedence && isRightChild &&
         ((parentOp == '-') || (parentOp == '/'))
        )
       ) {
        return 1;
    }
    return 0;
}

// 表达式输出
void printExpression(Expression* expression, int bracket) {
    if (expression->op == 0) { // 叶子节点
        printRational(expression->value);
        return;
    }

    // 判断左右表达式是否需要加括号
    // int leftflag = 1;
    // int rightflag = 1;
    int leftflag = needParentheses(expression->op, expression->left->op, 0);
    int rightflag = needParentheses(expression->op, expression->right->op, 1);

    if (bracket) printf("(");
    printExpression(expression->left, leftflag);
    printf(" %c ", expression->op);
    printExpression(expression->right, rightflag);
    if (bracket) printf(")");
}



// 表达式化简至标准型
/*
0.叶子节点个数多的放在左边，根节点操作符为加或乘
1.减号操作符节点调整为加号操作符节点，右子节点取反
2.父节点与右子节点都不是叶子节点，操作符优先级相同，且父节点操作符是为加或乘，调整运算顺序 a+(b+-c)=(a+b)+ -c a*(b* /c)=(a* /b)*c
3.父节点与右子节点都不是叶子节点，操作符为父减子加，或者父除子乘，调整运算顺序 a-(b+c)=(a-b)-c a/(b*c)=(a/b)/c

*/
void simplifyExpression(Expression* expression) {
    return;
}


// 判断表达式是否等价
/* 
算式重复的原因：
  子表达式的交换律；
  子表达式的结合律；
  减法与除法去括号；
*/
int isExpressionEqual(Expression* e1, Expression* e2) {
    if (e1->op == 0 && e2->op == 0) { // 叶子节点
        return compareRational(e1->value, e2->value) == '=';
    }
    // 交换律
    if (e1->op == e2->op && (e1->op == '+' || e1->op == '*')) {
        if (isExpressionEqual(e1->left, e2->left) && isExpressionEqual(e1->right, e2->right)) return 1;
        if (isExpressionEqual(e1->left, e2->right) && isExpressionEqual(e1->right, e2->left)) return 1;
    }
    // 结合律

    /* (a+b)+c=a+(b+c) (a*b)*c=a*(b*c)
     * (a-b)-c=a-(b+c) (a/b)/c=a/(b*c)
     * (a+b)+c=b+(a+c) (a*b)*c=b*(a*c) 交换结合
     */
    if (e1->right) { // 右子树不是叶子节点
        if (getPrecedence(e1->op) == getPrecedence(e1->right->op)) { // 操作符优先级相同
            if (e1->op == '+' || e1->op == '*') { // 操作符是加或乘
                Expression* temp0 = getBinaryExpression(e1->left, e1->right->left, e1->op);
                Expression* temp = getBinaryExpression(temp0, e1->right->right, e1->right->op);
                if (isExpressionEqual(temp, e2)) return 1;
            } else if (e1->op == '-' || e1->op == '/') { // 操作符是减或除
                Expression* temp0 = getBinaryExpression(e1->left, e1->right->left, e1->op);
                Expression* temp = getBinaryExpression(temp0, e1->right->right, oppositeOp(e1->right->op));
                if (isExpressionEqual(temp, e2)) return 1;
            }
        }
    }

    return 0;
}
