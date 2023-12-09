#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义测试数据的范围
#define MIN_NUM 1
#define MAX_NUM 13
#define NUM_COUNT 4

// 定义命令行参数的位置
#define CAL24CHECK_CMD_INDEX 1

// 定义测试数据文件名
#define TEST_DATA_FILE "test_data.txt"
// 定义输出答案文件名
#define ANSWER_FILE "test_answer.txt"

// 函数声明
void generateAndSaveTestData(FILE *file, int current, int *arr, int count);
void runTests(char *cal24checkCmd);

int main(int argc, char *argv[]) {
    // 检查参数个数
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <cal24check_cmd>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // 生成测试数据
    FILE *file = fopen(TEST_DATA_FILE, "w");
    if (file == NULL) {
        perror("Error opening test data file");
        exit(EXIT_FAILURE);
    }

    int arr[NUM_COUNT];
    generateAndSaveTestData(file, MIN_NUM, arr, 0);

    fclose(file);

    // 运行测试
    runTests(argv[CAL24CHECK_CMD_INDEX]);

    return EXIT_SUCCESS;
}

// 生成并保存测试数据
void generateAndSaveTestData(FILE *file, int current, int *arr, int count) {
    if (count == NUM_COUNT) {
        // 输出一组排列
        fprintf(file, "%d", arr[0]);
        for (int i = 1; i < NUM_COUNT; i++) {
            fprintf(file, " %d", arr[i]);
        }
        fprintf(file, "\n");
        return;
    }

    for (int i = current; i <= MAX_NUM; i++) {
        arr[count] = i;
        generateAndSaveTestData(file, i, arr, count + 1);
    }
}

// 运行测试
void runTests(char *cal24checkCmd) {
    FILE *testFile = fopen(TEST_DATA_FILE, "r");
    FILE *answerFile = fopen(ANSWER_FILE, "w");

    if (testFile == NULL || answerFile == NULL) {
        perror("Error opening test or answer file");
        exit(EXIT_FAILURE);
    }

    char line[50];
    while (fgets(line, sizeof(line), testFile) != NULL) {
        // 执行 cal24check 程序，并将结果写入 answer.txt
        char command[100];
        snprintf(command, sizeof(command), "%s %s", cal24checkCmd, line);
        FILE *cal24checkOutput = popen(command, "r");

        if (cal24checkOutput == NULL) {
            perror("Error executing cal24check");
            exit(EXIT_FAILURE);
        }

        int promptflag = 1;
        int answercnt = 0;
        char outputLine[100];
        while (fgets(outputLine, sizeof(outputLine), cal24checkOutput) != NULL) {
            // 判断是否输出“No Answer”
            if (strcmp(outputLine, "No Answer\n") == 0) continue;
            else if (promptflag) {
                promptflag = 0; answercnt++;
                fprintf(answerFile, "# %s", line);
            }
            answercnt++;
            fprintf(answerFile, "%s", outputLine);
        }
        if (!promptflag)
            fprintf(answerFile, "# %d answer(s) in total\n\n", answercnt);

        pclose(cal24checkOutput);
    }

    fclose(testFile);
    fclose(answerFile);
}
