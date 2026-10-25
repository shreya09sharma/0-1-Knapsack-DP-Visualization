
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 4096

void displayTable(int n, int W, int *dp, int *weights, int *values) {
    int i, w;

    printf("\n      DP TABLE\n");
    printf("Item  ");

    for (w = 0; w <= W; w++)
        printf("%4d", w);

    printf("\n");

    for (i = 0; i <= n; i++) {
        if (i == 0)
            printf("%-6s", "Base");
        else {
            char label[20];
            sprintf(label, "Item %d", i);
            printf("%-6s", label);
        }

        for (w = 0; w <= W; w++)
            printf("%4d", dp[i * (W + 1) + w]);

        printf("\n");
    }

    printf("\n");

    if (n > 0) {
        printf("Current item: Weight = %d, Value = %d\n",
               weights[n - 1], values[n - 1]);
    }
}

int main(void) {
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("result.txt", "w");

    char line[MAX_LINE];
    int testCase = 0;

    if (input == NULL || output == NULL) {
        printf("Error opening input.txt or result.txt!\n");

        if (input != NULL)
            fclose(input);

        if (output != NULL)
            fclose(output);

        return 1;
    }

    while (fgets(line, sizeof(line), input)) {
        if (strncmp(line, "#Test", 5) != 0)
            continue;

        int n, W;
        int found = 0;

        while (fgets(line, sizeof(line), input)) {
            if (sscanf(line, "%d %d", &n, &W) == 2) {
                found = 1;
                break;
            }
        }

        if (!found)
            break;

        if (n < 0 || W < 0 || n > 1000 || W > 10000) {
            printf("Invalid number of items or capacity!\n");
            break;
        }

        int *weights = malloc((size_t)(n ? n : 1) * sizeof(int));
        int *values = malloc((size_t)(n ? n : 1) * sizeof(int));

        if (!weights || !values) {
            printf("Memory allocation failed!\n");
            free(weights);
            free(values);
            break;
        }

        int valid = 1;

        for (int i = 0; i < n; i++) {
            if (!fgets(line, sizeof(line), input) ||
                sscanf(line, "%d %d", &weights[i], &values[i]) != 2 ||
                weights[i] <= 0 || values[i] < 0) {
                valid = 0;
                break;
            }
        }

        if (!valid) {
            printf("Invalid or incomplete item data!\n");
            free(weights);
            free(values);
            break;
        }

        size_t rows = (size_t)n + 1;
        size_t cols = (size_t)W + 1;

        if (rows > (size_t)-1 / cols ||
            rows * cols > (size_t)-1 / sizeof(int)) {
            printf("DP table size is too large!\n");
            free(weights);
            free(values);
            break;
        }

        int *dp = calloc(rows * cols, sizeof(int));

        if (!dp) {
            printf("Memory allocation failed for DP table!\n");
            free(weights);
            free(values);
            break;
        }

        testCase++;

        printf("\n========== TEST CASE %d ==========\n", testCase);
        printf("Number of items: %d\nCapacity: %d\n", n, W);

        printf("\nStep 0: Initial DP table\n");
        displayTable(0, W, dp, weights, values);

        for (int i = 1; i <= n; i++) {
            printf("\nStep %d: Processing item %d\n", i, i);

            for (int w = 0; w <= W; w++) {
                int exclude = dp[(i - 1) * cols + w];
                int include = exclude;

                if (weights[i - 1] <= w) {
                    include = values[i - 1] +
                        dp[(i - 1) * cols + w - weights[i - 1]];
                }

                dp[i * cols + w] =
                    (include > exclude) ? include : exclude;
            }

            displayTable(i, W, dp, weights, values);
        }

        int answer = dp[n * cols + W];

        printf("Maximum total value = %d\n", answer);

        fprintf(output,
                "#Test %d (%d %d) Maximum total value = %d\n",
                testCase, n, W, answer);

        free(dp);
        free(weights);
        free(values);
    }

    fclose(input);
    fclose(output);

    printf("\nAll results saved in result.txt\n");

    return 0;
}
