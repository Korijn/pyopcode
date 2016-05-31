#pragma once

//#include "opcode/Opcode.h"

#include "exception.cpp"
#include "numpy_boost/ndarray.cpp"


template<typename real_t, typename index_t>
class Mesh {
    /* simple wrapper class for vertices and triangles */
public:
    const ndarray<real_t, 2> vertices;
    const ndarray<index_t, 2> triangles;
//	const Opcode::Model model;

    Mesh(const ndarray<real_t, 2> vertices, const ndarray<index_t, 2> triangles) :
        vertices(vertices),
        triangles(triangles)
        //model(init_model())
    {
        if (vertices.shape()[1] != 3)
            throw python_exception("vertices needs to be an [n, 3] array");
        if (triangles.shape()[1] != 3)
            throw python_exception("triangles needs to be an [n, 3] array");
    }

//    Opcode::Model build() const {
//        // FIXME: no idea who is responsible for the lifetime of this object...
//        Opcode::MeshInterface* opcMeshAccess = new Opcode::MeshInterface();
//        opcMeshAccess->SetNbTriangles(triangles.size());
//        opcMeshAccess->SetNbVertices(vertices.size());
//        opcMeshAccess->SetPointers(
//            triangles.view<IceMaths::IndexedTriangle>().data(),
//            vertices .view<IceMaths::Point          >().data()
//        );
//
//        //Tree building settings
//        Opcode::OPCODECREATE OPCC;
//        OPCC.mIMesh = opcMeshAccess;
//        OPCC.mNoLeaf = true;
//        OPCC.mQuantized = false;
//        OPCC.mKeepOriginal = false;
//
//        Opcode::Model model;
//        model.Build(OPCC);
//        return model;
//    }
};

//
//template<typename real_t, typename index_t>
//class MeshCollision {
//    typedef Mesh<real_t, index_t> mesh_t;
//    typedef ndarray<real_t, 2> affine_t;
//
//	const mesh_t mesh1;
//	const mesh_t mesh2;
//
//	const Opcode::Model opcodemodel1;
//	const Opcode::Model opcodemodel2;
//
//
//public:
//
//    MeshCollision(const mesh_t mesh1, const mesh_t mesh2) :
//        mesh1(mesh1),
//        mesh2(mesh2),
//        opcodemodel1(mesh1.build()),
//        opcodemodel2(mesh2.build())
//    {}
//
//    int query(const affine_t affine1, const affine_t affine2) const {
//
//		Opcode::BVTCache ColCache;
//		ColCache.Model0 = &opcodemodel1;
//		ColCache.Model1 = &opcodemodel2;
//
//		// Collision query
//    	Opcode::AABBTreeCollider TC;
//		const bool IsOk (TC.Collide(ColCache, (IceMaths::Matrix4x4*)affine1.data(), (IceMaths::Matrix4x4*)affine2.data()));
//
//		// Get collision status => if true, objects overlap
//		const bool Status (TC.GetContactStatus());
//
//		int NbPairs; //number of colliding pairs
//
//		if (Status > 0) {
//			// Number of colliding pairs and list of pairs
//			NbPairs = TC.GetNbPairs();
//			//printf("colliding: %d\n", NbPairs);
////			p = TC.GetPairs();
//		}
//		else {
//			NbPairs = 0;
//			//printf("not colliding\n");
//		}
//		return NbPairs;
//    }
//
//};