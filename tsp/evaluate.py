import time
import random
import matplotlib.pyplot as plt
from utils import City, tour_length

def gen_cities(n, width=900, height=600, seed=100):
    """Generate example cities

    Args:
        n (int): The number of examples to generate.
        width (int, default=900): The range of the x coordinate of the generated cities is [0, width].
        higth (int, default=600): The range of the y coordinate of the generated cities is [0, height].
        seed (int, default=100): The random seed of the generator.

    Returns:
        cities (set): A set of generated city objects.
    """
    random.seed(seed * n)
    cities = set(City(random.randrange(width), random.randrange(height)) for _ in range(n))
    return cities

def plot_tour(tour):
    """Plot the tour you take

    Args:
        tour (list): A list of city objects. It represents the order of visiting cities.

    Returns:
        (None)
    """
    def plot_lines(points, style='bo-'):
        plt.plot([p.x for p in points], [p.y for p in points], style)
        plt.axis('scaled'); plt.axis('off')

    plot_lines(list(tour) + [tour[0]])
    plot_lines([tour[0]], 'rs')

def evaluate(algorithm, cities):
    """Plot the tour of the algorithm and show statistical information

    - Validate the tour obtained by the algorithm.
    - Calculate the length of the tour.
    - Calculate the execution time of the algorithm.
    - Plot the tour of the algorithm you take. 

    Args:
        algorithm (function): The algorithm you designed to solve TSP. Refer to `brute_force_tsp()` function.
        cities (set): A set of citys you generated with function `gen_cities()`.

    Returns:
        (None)
    """
    t0 = time.time()
    tour = algorithm(cities)
    t1 = time.time()

    def valid_tour(tour, cities):
        # print(tour, cities)
        return set(tour) == set(cities) and len(tour) == len(cities)
    assert valid_tour(tour, cities)

    print("{} city tour with length {:.1f} in {:.3f} seconds for {}"
          .format(len(tour), tour_length(tour), t1 - t0, algorithm.__name__))
    
    return (tour, tour_length(tour), t1 - t0)
