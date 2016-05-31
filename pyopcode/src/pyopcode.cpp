#pragma once

#include "opcode/StdAfx.cpp"
#include "opcode/Opcode.h"
//#include "opcode/OPC_Model.cpp"
//#include "opcode/OPC_MeshInterface.h"

#include "exception.cpp"
#include "numpy_boost/ndarray.cpp"


template<typename real_t, typename index_t>
class MeshModel {
    /* simple wrapper class for vertices and triangles */
public:
    const ndarray<real_t, 2>    vertices;
    const ndarray<index_t, 2>   triangles;
    const Opcode::MeshInterface interface;
	const Opcode::Model         model;

    Mesh(const ndarray<real_t, 2> vertices, const ndarray<index_t, 2> triangles) :
        vertices    (vertices),
        triangles   (triangles),
        interface   (init_interface()),
        model       (init_model())
    {}

    Opcode::MeshInterface init_interface() const {
        Opcode::MeshInterface interface;
        interface.SetNbTriangles(triangles.size());
        interface.SetNbVertices(vertices.size());
        // this casting implicitly validates the input shapes and strides
        interface.SetPointers(
            triangles.view<IceMaths::IndexedTriangle>().data(),
            vertices .view<IceMaths::Point          >().data()
        );
        return interface;
    }

    Opcode::Model init_model() const {
        //Tree building settings
        Opcode::OPCODECREATE OPCC;
        OPCC.mIMesh = interface;
        OPCC.mNoLeaf = true;
        OPCC.mQuantized = false;
        OPCC.mKeepOriginal = false;

        Opcode::Model model;
        model.Build(OPCC);
        return model;
    }
};


template<typename real_t, typename index_t>
class MeshCollision {
    typedef Mesh<real_t, index_t> mesh_t;
    typedef ndarray<real_t, 2> affine_t;

	const mesh_t mesh0;
	const mesh_t mesh1;

public:
    MeshCollision(const mesh_t mesh0, const mesh_t mesh1) :
        mesh0(mesh0), mesh1(mesh1),
    {}

    int query(const affine_t affine0, const affine_t affine1) const {

		Opcode::BVTCache ColCache;
		ColCache.Model0 = &mesh0.model;
		ColCache.Model1 = &mesh1.model;

		// Collision query
    	Opcode::AABBTreeCollider TC;
		const bool IsOk(TC.Collide(ColCache, (IceMaths::Matrix4x4*)affine0.data(), (IceMaths::Matrix4x4*)affine1.data()));

		// Get collision status => if true, objects overlap
		const bool Status (TC.GetContactStatus());

		int NbPairs; //number of colliding pairs

		if (Status > 0) {
			// Number of colliding pairs and list of pairs
			NbPairs = TC.GetNbPairs();
//			p = TC.GetPairs();
		}
		else {
			NbPairs = 0;
			//printf("not colliding\n");
		}
		return NbPairs;
    }

};