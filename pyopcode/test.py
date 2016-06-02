
import numpy as np
import pyopcode

vertices = np.random.rand(50, 3).astype(np.float32)
triangles = np.arange(30).reshape(10, 3).astype(np.int32)
mesh = pyopcode.Model(vertices, triangles)
mesh1 = pyopcode.Model(vertices, triangles)

col = pyopcode.Collision(mesh, mesh1)
identity = np.identity(4).astype(np.float32)
idx = col.query(identity, identity)
print(idx)