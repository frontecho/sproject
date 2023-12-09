# include <stdio.h>
# include <stdlib.h>
# include "../utils/rational.h"
# include "../utils/expression.h"

# define N 4
# define RES 24

# define REMOVE_DUPLICATES 0
# define MAX_RESULT_COUNT (N*100)
int resultcnt = 0;
Expression result[MAX_RESULT_COUNT];

int cal24(Expression *exp[], int n);

char op[] = {'+', '-', '*', '/', '\\'}; // 五种运算符，\表示反向除法，即 a\b = b/a

int main(void) {
    Rational num[N];
    Expression *exp[N];

    int inputNumber;
    printf("请输入%d个要计算%d点的数字：", N, RES);
    for (int i = 0; i < N; i++) {
        scanf("%d", &inputNumber);
        num[i] = getIntRational(inputNumber);
        exp[i] = getLeafExpression(num[i]);
    }

    if (!cal24(exp, N)) printf("No Answer\n");

    return 0;
}

// 计算24点
int cal24(Expression *exp[], int n) {
    if (n == 1) {
        // printExpression(exp[0], 0);printf(" = ");printRational(exp[0]->value);printf("\n");
        if (compareRational(exp[0]->value, getIntRational(RES)) == '=') {
            int isExisted = 0;
            // 判断是否重复
            if (REMOVE_DUPLICATES) {
                for (int i = 0; i < resultcnt; i++) {
                    if (isExpressionEqual(exp[0], &result[i])) isExisted = 1;
                }
            }
            if (!isExisted) {
                printExpression(exp[0], 0);
                printf(" = %d\n", RES);
                if (REMOVE_DUPLICATES) result[resultcnt++] = *exp[0];
            }
            return 1;
        }
        else return 0;
    }

    int findflag = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            Expression *a = exp[i], *b = exp[j];  // 选取两个数，保存
            exp[j] = exp[n-1];  // 从exp表达式数组最后一个位置取数，放到j位置，此时exp数组有效长度减1
            
            // 对a和b做四则运算，保存到i位置
            for (int k = 0; k < 5; k++) {
                // 除数不能为0
                if (op[k] == '/' && b->value.num == 0) continue;
                if (op[k] == '\\' && a->value.num == 0) continue;

                if (op[k] == '-' && compareRational(a->value, b->value) == '<')  // 减法大数减小数，交换位置
                    exp[i] = getBinaryExpression(b, a, op[k]);
                else if (op[k] == '\\')  // 除法 a\b 即 b/a 交换位置
                    exp[i] = getBinaryExpression(b, a, '/');
                else
                    exp[i] = getBinaryExpression(a, b, op[k]);

                if (cal24(exp, n-1)) {
                    findflag = 1;
                    // return 1; // 有解即返回
                }
            }

            // 恢复到初始状态，此时exp数组有效长度加1，注意exp最后一个元素没有发生变动
            exp[i] = a;
            exp[j] = b;
        }
    }
    return findflag;
}
