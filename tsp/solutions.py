import sys
import random
import itertools
from utils import tour_length, distance

def brute_force_tsp(cities):
    """Brute force method for TSP
    算法穷举所有可能的路径，计算每条路径的长度，最后返回最短路径
    """
    tours = itertools.permutations(cities)
    shortest_tour = min(tours, key=tour_length)
    return shortest_tour

def backtracking_tsp(cities: set):
    """Backtracking method for TSP
    算法从起始城市开始，逐步构建路径，每次选择未访问的下一个城市，直到所有城市都被访问。
    然后，算法考虑回到起始城市的距离，如果找到更短的路径，就更新最短路径和最小距离。以此穷举所有可能路径。
    """
    cities = list(cities)
    n = len(cities)
    if n == 0:
        return []
    
    start = cities[0]
    min_length = sys.maxsize
    best_tour = []
    
    def backtrack(current_city, visited, current_tour, current_length):
        nonlocal min_length, best_tour
        if len(current_tour) == n:
            total_length = current_length + distance(current_city, start)
            if total_length < min_length:
                min_length = total_length
                best_tour = current_tour.copy()
            return
        for city in cities:
            if city not in visited:
                new_length = current_length + distance(current_city, city)
                if new_length < min_length:
                    visited.add(city)
                    current_tour.append(city)
                    backtrack(city, visited, current_tour, new_length)
                    visited.remove(city)
                    current_tour.pop()
    
    backtrack(start, set([start]), [start], 0)
    return best_tour

def greedy_tsp(cities: set):
    """Greedy method for TSP
    从起始城市开始，每次选择距离最近的未访问城市，直到所有城市都被访问过一次
    """
    if not cities:
        return []
    
    cities = list(cities)  # 转换为列表以便索引
    start = cities[0]
    tour = [start]
    unvisited = set(cities)
    unvisited.remove(start)
    
    current = start
    while unvisited:
        next_city = min(unvisited, key=lambda city: distance(current, city))
        tour.append(next_city)
        unvisited.remove(next_city)
        current = next_city
    
    return tour

def dynamic_programming_tsp(cities):
    """Dynamic programming method for TSP
    Held-Karp算法：
    1. 计算距离矩阵
    2. 初始化动态规划表，C[subset][end] = (cost, prev)
    3. 动态规划求解 C[subset][end] = min(C[prev_subset][k] + distance(k, end))
    4. 计算最短回路
    5. 回溯最短路径
    """
    cities_list = list(cities)
    n = len(cities_list)
    # 固定起始城市为索引0
    start = 0

    # 计算距离矩阵
    distance_matrix = [[distance(cities_list[i], cities_list[j]) for j in range(n)] for i in range(n)]

    # 初始化动态规划表，C[subset][end] = (cost, prev)
    C = {}
    for k in range(1, n):
        C[1 << k] = {}
        C[1 << k][k] = (distance_matrix[start][k], start)

    # 动态规划求解
    for subset_size in range(2, n):
        for subset in itertools.combinations(range(1, n), subset_size):
            bits = 0
            for bit in subset:
                bits |= 1 << bit
            C[bits] = {}
            for j in subset:
                prev_bits = bits & ~(1 << j)
                min_cost = float('inf')
                min_prev = None
                for k in subset:
                    if k == j:
                        continue
                    if prev_bits in C and k in C[prev_bits]:
                        cost = C[prev_bits][k][0] + distance_matrix[k][j]
                        if cost < min_cost:
                            min_cost = cost
                            min_prev = k
                C[bits][j] = (min_cost, min_prev)

    # 计算最短回路
    bits = (1 << n) - 2  # 所有城市的位集合，除去起始城市
    min_cost = float('inf')
    min_prev = None
    for k in range(1, n):
        cost = C[bits][k][0] + distance_matrix[k][start]
        if cost < min_cost:
            min_cost = cost
            min_prev = k

    # 回溯最短路径
    path = [start]
    bits = (1 << n) - 2
    current = min_prev
    for _ in range(n - 1):
        path.append(current)
        next_bits = bits & ~(1 << current)
        temp = C[bits][current][1]
        bits = next_bits
        current = temp
    path = path[::-1]

    # 将索引转换为城市对象
    optimal_tour = [cities_list[i] for i in path]

    # 返回最短路线的序列（不包括最后回到的起点）
    return optimal_tour

def genetic_tsp(cities: set, population_size=100, generations=500, mutation_rate=0.01):
    """Genetic algorithm for TSP
    1. 初始化种群
    2. 选择：根据适应度函数选择个体
    3. 交叉：使用PMX交叉
    4. 变异：随机交换两个城市的位置
    5. 迭代直到达到最大迭代次数
    6. 返回最佳路径
    """
    cities = list(cities)
    n = len(cities)
    if n == 0:
        return []
    
    if n >= 10:
        generations = 100*n//10
    
    # 初始化种群
    population = [random.sample(cities, n) for _ in range(population_size)]
    
    def fitness(tour):
        return 1 / tour_length(tour)
    
    def selection(pop):
        sorted_pop = sorted(pop, key=fitness, reverse=True)
        # 选择前50%的个体
        return sorted_pop[:population_size // 2]
    
    def crossover(parent1, parent2):
        # 使用部分映射交叉（PMX）
        start, end = sorted(random.sample(range(n), 2))
        child_p1 = parent1[start:end]
        child = [city for city in parent2 if city not in child_p1]
        return child[:start] + child_p1 + child[start:]
    
    def mutate(tour):
        if random.random() < mutation_rate:
            i, j = random.sample(range(n), 2)
            tour[i], tour[j] = tour[j], tour[i]
        return tour
    
    for gen in range(generations):
        # 选择
        selected = selection(population)
        # 交叉
        children = []
        while len(children) < population_size - len(selected):
            parent1, parent2 = random.sample(selected, 2)
            child = crossover(parent1, parent2)
            children.append(child)
        # 变异
        population = selected + [mutate(child) for child in children]
    
    best_tour = max(population, key=fitness)
    return best_tour

