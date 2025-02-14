import numpy as np
import math
import sys
from .stable import Stable 
from .quadrature_rules import symmetric_nodes, symmetric_weights


class StableDensity(Stable):   
    def __init__(self, X , eps = sys.float_info.epsilon):
        super().__init__()
        self.X = X
        self.eps = eps
        self.n = self.determine_series_n()
        self.zeta = self.calculate_zeta()
        self.T_alpha = self.calculate_T_alpha()
        self.bound = self.calculate_series_bound()
        self.x_method = (X < self.bound) & (X > -self.bound)
        self.pdf = self.calculate_density()
        self.scaled_nodes
        self.scaled_weights


    def get_pdf(self):
        return self.pdf
     

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
    

    def calculate_T_alpha(self):
        T_alpha =  (-np.log(self.eps))**(1/self.alpha)
        return T_alpha
    

    def quadrature(self, x):
        np.dot(np.array([self.integrand_symmetric(tau, x) for tau in self.scaled_nodes]), self.scaled_weights)
        return 
    

    def series_representation(self, x):
        pass


    def scale_quadrature_rule(self):
        if self.beta == 0: 
            self.scaled_nodes = symmetric_nodes * self.T_alpha
            self.scaled_weights = symmetric_weights * (self.T_alpha / math.pi)
        

    def integrand_symmetric(self, tau, x):
        return math.cos(x * (tau * self.T_alpha)) * math.exp(-(tau*self.T_alpha)**self.alpha)
    

    def calculate_density(self):
        pdf = []

        self.scale_quadrature_rule()

        for i in range(self.X.size):
            if self.x_method[i]:
                pdf.append(self.quadrature(self.X[i]))
            else: 
                pdf.append(self.series_representation(self.X[i]))

        return np.array(pdf)


    def integrand_asymmetric(self):
        pass
