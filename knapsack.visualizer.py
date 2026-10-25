
def solve_knapsack(weights, values, capacity):
    n = len(weights)

    # Create DP table
    dp = [[0] * (capacity + 1) for _ in range(n + 1)]

    print("\nInitial DP Table:")
    print_table(dp, 0, capacity)

    # Fill the DP table one item at a time
    for i in range(1, n + 1):
        weight = weights[i - 1]
        value = values[i - 1]

        for w in range(capacity + 1):
            # Do not include the current item
            dp[i][w] = dp[i - 1][w]

            # Include the current item if it fits
            if weight <= w:
                dp[i][w] = max(
                    dp[i][w],
                    value + dp[i - 1][w - weight]
                )

        print(f"\nAfter processing item {i}:")
        print_table(dp, i, capacity)

    return dp[n][capacity]


def print_table(dp, rows, capacity):
    print("Items |", end=" ")

    for w in range(capacity + 1):
        print(f"{w:3}", end=" ")

    print()

    print("-" * (8 + 4 * (capacity + 1)))

    for i in range(rows + 1):
        print(f"{i:5} |", end=" ")

        for value in dp[i]:
            print(f"{value:3}", end=" ")

        print()


def main():
    try:
        with open("input.txt", "r") as file:
            content = file.read().splitlines()
    except FileNotFoundError:
        print("Error: input.txt file not found!")
        return

    test_cases = []
    current_case = []

    for line in content:
        line = line.strip()

        if not line:
            continue

        if line.startswith("#Test"):
            if current_case:
                test_cases.append(current_case)
            current_case = []
        else:
            current_case.append(line)

    if current_case:
        test_cases.append(current_case)

    results = []

    for case_number, case in enumerate(test_cases, start=1):
        n, capacity = map(int, case[0].split())

        weights = []
        values = []

        for line in case[1:n + 1]:
            weight, value = map(int, line.split())
            weights.append(weight)
            values.append(value)

        print(f"\n========== TEST CASE {case_number} ==========")
        print(f"Number of items: {n}")
        print(f"Capacity: {capacity}")
        print(f"Weights: {weights}")
        print(f"Values: {values}")

        answer = solve_knapsack(weights, values, capacity)

        print(f"\nMaximum total value = {answer}")

        results.append(
            f"#Test case {case_number} input ({n} {capacity}) "
            f"test result {answer}"
        )

    with open("result.txt", "w") as file:
        file.write("\n".join(results) + "\n")

    print("\nAll results saved in result.txt")


if __name__ == "__main__":
    main()