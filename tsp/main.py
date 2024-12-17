from evaluate import gen_cities, evaluate
from solutions import brute_force_tsp, greedy_tsp, backtracking_tsp, dynamic_programming_tsp, genetic_tsp

if __name__ == "__main__":
    test_cases = [gen_cities(n) for n in [i for i in range(5, 25)] + [25, 30, 35, 40, 45, 50, 60, 70, 80, 90, 100]]
    results = [[] for _ in test_cases]
    for i, cities in enumerate(test_cases):
        if len(cities) <= 11:
            results[i].append(evaluate(brute_force_tsp, cities))
        else:
            results[i].append((None, None, None))
        if len(cities) <= 16:
            results[i].append(evaluate(backtracking_tsp, cities))
        else:
            results[i].append((None, None, None))
        if len(cities) <= 23:
            results[i].append(evaluate(dynamic_programming_tsp, cities))
        else:
            results[i].append((None, None, None))
        results[i].append(evaluate(greedy_tsp, cities))
        results[i].append(evaluate(genetic_tsp, cities))

    # analyze the results
    # extract the times for each test case, in different algorithms
    time_cases = [[algo[2] for algo in r] for r in results]
    time_algos = [[r[algo][2] for r in results] for algo in range(len(results[0]))]
    length_algos = [[r[algo][1] for r in results] for algo in range(len(results[0]))]

    print("Time cases: ", time_cases)
    print("Time algos: ", time_algos)
    print("Length algos: ", length_algos)
