# pyopcode
Numpy bindings for the OPCODE collision detection library.

Currently, this only wraps the functionality to perform efficient triangle-triangle collisions between two rigid triangle meshes, using AABB trees.

 * Mesh(vertices, triangles) -> Mesh instance
 * Collision(Mesh_0, Mesh_1) -> Collision instance
 * Collision.query(affine4x4_0, affine4x4_1) -> ndarray, [n_collisions, 2], int

 Support for wrapping the raytracing/picking code is planned as well.

 In general; the hard part in making this package is getting the code to compile. Given that is does that, extending the interface should generally by simple.

 Support is planned for python 2.7, 3.5, windows, and linux