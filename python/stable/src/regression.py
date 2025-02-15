from .stable import Stable 
from .density import StableDensity 
import statsmodels.api as sm
import statsmodels.formula.api as smf


class StableRegression(Stable):
    def __init__(self, X, y):
        super().__init__()
        self.X = X
        self.y = y