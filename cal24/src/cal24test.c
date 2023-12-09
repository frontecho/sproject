# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include "../utils/rational.h"
# include "../utils/expression.h"

# define N 4
# define RES 24

# define MAX_RESULT_COUNT (N*100)
Rational randres[MAX_RESULT_COUNT];
int randrescnt = 0;

int cal24(Expression *exp[], Expression *randexp[], int n);

char op[] = {'+', '-', '*', '/', '\\'}; // 五种运算符，\表示反向除法，即 a\b = b/a

int main(int argc, char *argv[]) {
    Rational num[N];
    Expression *exp[N];

    for (int i = 0; i < N && i+1 < argc; i++) {
        int inputNumber = atoi(argv[i+1]);
        num[i] = getIntRational(inputNumber);
        exp[i] = getLeafExpression(num[i]);
    }

    srand((unsigned)time(NULL));
    Rational randnum = getIntRational(rand() % 100 + 20);
    Expression *randexp[N];
    for (int i = 0; i < N; i++) {
        num[i] = calRational(num[i], randnum, '+');
        randexp[i] = getLeafExpression(num[i]);
    }

    if (!cal24(exp, randexp, N)) printf("No Answer\n");

    return 0;
}

// 计算24点
int cal24(Expression *exp[], Expression *randexp[], int n) {
    if (n == 1) {
        // printExpression(exp[0], 0);printf(" = ");printRational(exp[0]->value);printf("\n");
        if (compareRational(exp[0]->value, getIntRational(RES)) == '=') {
            int printflag = 1;
            for (int i = 0; i < randrescnt; i++) {
                if (compareRational(randres[i], randexp[0]->value) == '=') {
                    printflag = 0;
                    break;
                }
            }
            if (printflag) {
                printExpression(exp[0], 0);
                printf(" = %d", RES);
                printf("\n");
                randres[randrescnt++] = randexp[0]->value;
            }
            return 1;
        }
        else return 0;
    }

    int findflag = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            Expression *a = exp[i], *b = exp[j];  // 选取两个数，保存
            Expression *randa = randexp[i], *randb = randexp[j];
            exp[j] = exp[n-1];  // 从exp表达式数组最后一个位置取数，放到j位置，此时exp数组有效长度减1
            randexp[j] = randexp[n-1];
            // 对a和b做四则运算，保存到i位置
            for (int k = 0; k < 5; k++) {
                // 除数不能为0
                if (op[k] == '/' && b->value.num == 0) continue;
                if (op[k] == '\\' && a->value.num == 0) continue;

                if (op[k] == '-' && compareRational(a->value, b->value) == '<') {  // 减法大数减小数，交换位置
                    exp[i] = getBinaryExpression(b, a, op[k]);
                    randexp[i] = getBinaryExpression(randb, randa, op[k]);
                }else if (op[k] == '\\') {  // 除法 a\b 即 b/a 交换位置
                    exp[i] = getBinaryExpression(b, a, '/');
                    randexp[i] = getBinaryExpression(randb, randa, '/');
                } else {
                    exp[i] = getBinaryExpression(a, b, op[k]);
                    randexp[i] = getBinaryExpression(randa, randb, op[k]);
                }
                if (cal24(exp, randexp, n-1)) {
                    findflag = 1;
                    // return 1; // 有解即返回
                }
            }

            // 恢复到初始状态，此时exp数组有效长度加1，注意exp最后一个元素没有发生变动
            exp[i] = a;
            exp[j] = b;
            randexp[i] = randa;
            randexp[j] = randb;
        }
    }
    return findflag;
}
