
import numpy as np
import pyopcode

vertices = np.random.rand(50, 3).astype(np.float32)
triangles = np.arange(30).reshape(10, 3)
mesh = pyopcode.Model(vertices, triangles)