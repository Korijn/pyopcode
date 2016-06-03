
import numpy as np
import pytest
import pyopcode


def triangle_soup(N, range=(0, 0)):
    # create a trianglesoup
    vertices = np.random.uniform(-0.5, 0.5, size=(N * 3, 3)).astype(np.float32)
    view = vertices.reshape(N, 3, 3)
    view += np.random.uniform(*range, size=(N, 1, 3))
    triangles = np.arange(N * 3).reshape(N, 3).astype(np.int32)
    return vertices, triangles


def test_invalid():
    vertices = np.random.rand(10, 3)
    triangles = np.arange(10 * 3).reshape(10, 3).astype(np.int32)

    with pytest.raises(TypeError):
        mesh = pyopcode.Model(vertices, triangles)

    vertices = np.random.rand(10, 2).astype(np.float32)
    with pytest.raises(RuntimeError):
        mesh = pyopcode.Model(vertices, triangles)

    vertices = np.asfortranarray(np.random.rand(10, 3).astype(np.float32))
    with pytest.raises(RuntimeError):
        mesh = pyopcode.Model(vertices, triangles)


def test_basic():
    """no runtime erros plox"""
    vertices, triangles = triangle_soup(10)
    mesh0 = pyopcode.Model(vertices, triangles)
    mesh1 = pyopcode.Model(vertices, triangles)

    col = pyopcode.Collision(mesh0, mesh1)

    identity = np.identity(4).astype(np.float32)
    idx = col.query(identity, identity)
    print(idx)


def test_GIL():
    """tests running of multiple queries in a threadpool"""
    vertices, triangles = triangle_soup(10000, (-5, 5))
    mesh0 = pyopcode.Model(vertices, triangles)
    vertices, triangles = triangle_soup(10000, (-5, 5))
    mesh1 = pyopcode.Model(vertices, triangles)

    col = pyopcode.Collision(mesh0, mesh1)

    identity = np.identity(4).astype(np.float32)
    def transform_generator():
        """generate affine rotation matrices"""
        for i in range(100):
            r = np.random.normal(size=(3, 3))
            u, _, v = np.linalg.svd(r)
            r = u.dot(np.eye(*r.shape)).dot(v)
            a = identity.copy()
            a[:3, :3] = r
            yield a

    from multiprocessing.pool import ThreadPool
    pool = ThreadPool(processes=4)
    results = pool.imap_unordered(lambda affine: col.query(affine, identity), transform_generator())

    import time
    start = time.clock()
    for r in results:
        print(len(r))
    print (time.clock() - start)


test_invalid()