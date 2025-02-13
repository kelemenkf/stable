import numpy as np
import math
import sys
from .stable import Stable 


class StableDensity(Stable):   
    def __init__(self, X , eps = sys.float_info.epsilon):
        super().__init__()
        self.X = X
        self.eps = eps
        self.n = self.determine_series_n()
        self.zeta = self.calculate_zeta()
        self.bound = self.calculate_series_bound()
        self.x_method = (X < self.bound) & (X > -self.bound)


    def get_bound(self):
        return self.bound
    

    def get_x_method(self):
        return self.x_method


    def determine_series_n(self):
        n = 0
        if self.beta == 0: 
            n = 46
        return n


    def calculate_zeta(self):
        zeta = 0
        if self.beta == 0: 
            zeta = 0
        return zeta
            


    def calculate_series_bound(self):
        bound = (self.alpha / (math.pi * self.eps) * (1 + self.zeta**2)**(self.n/2)*(math.gamma(self.alpha*self.n)/math.gamma(self.n)))**(1/(self.alpha*self.n-1))
        return bound
