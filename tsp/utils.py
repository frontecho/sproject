import math

class City:
    """A class represent city

    Attributes:
        x (float): The x coordinate of the city.
        y (float): The y coordinate of the city.
    """
    def __init__(self, x, y):
        self.x = x
        self.y = y
    
    def __repr__(self):
        return "(%d,%d)"%(self.x, self.y)

def distance(A, B): 
    """Calculate distance between two cities

    Args:
        A (City): A city object.
        B (City): A city object.
    
    Returns:
        The Euclidean distance between city A and city B. (float)
    """
    return math.sqrt((A.x-B.x)**2 + (A.y-B.y)**2)

def tour_length(tour):
    """Calculate total length of the tour you take 

    Args:
        tour (list): A list of city objects. It represents the order of visiting cities.

    Returns:
        The total length of the tour with the visiting order given. (float)
    """
    return sum(distance(tour[i], tour[i-1]) for i in range(len(tour)))