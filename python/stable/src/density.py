import numpy as np
import math
import sys
from scipy.stats import norm
from .stable import Stable 
from .quadrature_rules import SYMMETRIC_NODES, SYMMETRIC_WEIGHTS, ASYMMETRIC_WEIGHTS_ALPHA_MORE_11, ASYMMETRIC_NODES_ALPHA_MORE_11, ASYMMETRIC_NODES_ALPHA_LESS_09, ASYMMETRIC_WEIGHTS_ALPHA_LESS_09 


class StableDensity(Stable):   
    def __init__(self, X, alpha=2.0, beta=0.0, gamma=1.0, delta=0, negative = False, eps = sys.float_info.epsilon):
        super().__init__(alpha, beta, gamma, delta)
        self.negative = negative
        if self.alpha != 2.0:
            self.zeta = self.calculate_zeta()
            if (self.gamma != 1.0 or self.delta != 0 or self.beta != 0) and self.negative == False:
                self.X = (X - self.delta + self.zeta * self.gamma) / self.gamma
            else: 
                self.X = X
            self.eps = eps
            self.n = self.determine_series_n()
            self.T_alpha = self.calculate_T_alpha()
            self.bound = self.calculate_series_bound()
            self.X_negative = self.determine_x_negative()
            self.x_method = self.determine_x_method() 
            self.pdf = self.calculate_density()
            self.scaled_nodes = []
            self.scaled_weights = []
        else: 
            self.X = X
            self.pdf = self.calculate_density()


    def get_pdf(self):
        if self.negative == False:
            return (1 / self.gamma) * self.pdf
        else: 
            return self.pdf
    

    def get_scaled_nodes(self):
        return self.scaled_nodes
    

    def get_scaled_weights(self):
        return self.scaled_weights
     

    def determine_series_n(self):
        n = 0

        if self.beta == 0: 
            n = 46
        elif self.beta != 0 and self.alpha >= 1.1:
            n = 80
        elif self.beta != 0 and self.alpha >= 0.5 and self.alpha <= 0.9:
            n = 90

        return n


    def calculate_zeta(self):
        zeta = 0

        if self.beta == 0: 
            zeta = 0
        else: 
            zeta = -self.beta * math.tan((math.pi * self.alpha) / 2)

        return zeta
    

    def determine_x_negative(self):
        result = []

        if not self.negative:
            for x in self.X: 
                if (x - self.zeta < 0):
                    result.append(x)
                    i = np.where(self.X == x)
                    self.X = np.delete(self.X, i)

        return np.array(result)
    

    def determine_x_method(self):
        return (self.X - self.zeta < self.bound)
            

    def calculate_series_bound(self):
        bound = (self.alpha / (math.pi * self.eps) * (1 + self.zeta**2)**(self.n/2)*(math.gamma(self.alpha*self.n)/math.gamma(self.n)))**(1/(self.alpha*self.n-1))
        return bound
    

    def calculate_T_alpha(self):
        T_alpha =  (-np.log(self.eps))**(1/self.alpha)
        return T_alpha
    

    def quadrature(self, x):
        if self.beta == 0:
            return np.dot(np.array([self.integrand_symmetric(tau, x) for tau in self.scaled_nodes]), self.scaled_weights)
        else: 
            return np.dot(np.array([self.integrand_asymmetric(tau, x) for tau in self.scaled_nodes]), self.scaled_weights)
    

    def series_representation(self, x):
        f_x = 0

        for k in range(1,self.n+1):
            first_term = (-1)**(k + 1)
            second_term = math.gamma((self.alpha*k)) / math.gamma(k)
            third_term = ((1 + self.zeta**2) ** (k / 2)) 
            fourth_term = math.sin((math.pi * self.alpha / 2 - math.atan(self.zeta)) * k)
            fifth_term = (x - self.zeta)**(-self.alpha * k - 1)
            f_x += first_term * second_term * third_term * fourth_term * fifth_term
            if f_x < 0: 
                print(self.n, x, self.alpha, self.beta, self.gamma, self.delta)

        return (self.alpha / math.pi) * f_x


    def scale_quadrature_rule(self):
        if self.beta == 0: 
            self.scaled_nodes = SYMMETRIC_NODES
            self.scaled_weights = SYMMETRIC_WEIGHTS * (self.T_alpha / math.pi)
        elif self.beta != 0 and self.alpha >= 1.1: 
            self.scaled_nodes = ASYMMETRIC_NODES_ALPHA_MORE_11
            self.scaled_weights = ASYMMETRIC_WEIGHTS_ALPHA_MORE_11 * (self.T_alpha / math.pi)
        elif self.beta != 0 and (self.alpha <= 0.9 and self.alpha >= 0.5):
            self.scaled_nodes = ASYMMETRIC_NODES_ALPHA_LESS_09
            self.scaled_weights = ASYMMETRIC_WEIGHTS_ALPHA_LESS_09 * (self.T_alpha / math.pi)
        

    def integrand_symmetric(self, tau, x):
        return math.cos(x * (tau * self.T_alpha)) * math.exp(-(tau*self.T_alpha)**self.alpha)
    

    def integrand_asymmetric(self, tau, x):
        h = (x - self.zeta) * (tau*self.T_alpha) + self.zeta * (tau * self.T_alpha)**self.alpha
        return math.cos(h) * math.exp(-(tau * self.T_alpha)**self.alpha)
    

    def calculate_density(self):
        pdf = []

        if self.alpha == 2.0:
            pdf = norm.pdf(self.X, self.delta, self.gamma * math.sqrt(2))
        else:
            self.scale_quadrature_rule()

            if not self.negative:
                negative = StableDensity(-self.X_negative, self.alpha, -self.beta, self.gamma, self.delta, True)
                negative_pdf = negative.get_pdf()

                pdf += list(negative_pdf)

            for i in range(self.X.size):
                if self.x_method[i]:
                    f_x = self.quadrature(self.X[i])
                    pdf.append(f_x)
                else: 
                    f_x = self.series_representation(self.X[i]) 
                    pdf.append(f_x)

        return np.array(pdf)