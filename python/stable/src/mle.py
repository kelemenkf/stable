import numpy as np
import math
from .stable import Stable
from .density import StableDensity


class MLE(Stable):
    def __init__(self, X, alpha, beta, gamma, delta):
        self.X = X 
        self.alpha = alpha 
        self.beta = beta
        self.gamma = gamma 
        self.delta = delta


    def loglikelihood(self):
        logL_initial = 0
        pdf_initial = StableDensity(self.X, self.alpha, self.beta, self.gamma)

