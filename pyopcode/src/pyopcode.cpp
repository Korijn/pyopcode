#pragma once

#include <boost/range.hpp>

#include "numpy_boost/ndarray.cpp"
#include "opcode/Opcode.h"
#include "gil.cpp"


template<typename real_t, typename index_t>
class MeshModel {
    /*
    simple wrapper class for vertices and triangles,
    and corresponding opcode-model, or collision detection acceleration structure
    */
public:
    const ndarray<real_t, 2>    vertices;
    const ndarray<index_t, 2>   triangles;
    const Opcode::MeshInterface interface;
    const Opcode::Model         model;

    explicit MeshModel(const ndarray<real_t, 2> vertices, const ndarray<index_t, 2> triangles) :
        vertices    (vertices),
        triangles   (triangles),
        interface   (init_interface()),
        model       (init_model())
    {}

private:
    Opcode::MeshInterface init_interface() const {
        Opcode::MeshInterface interface;
        interface.SetNbTriangles(triangles.size());
        interface.SetNbVertices(vertices.size());
        // this casting implicitly validates the input shapes and strides
        interface.SetPointers(
            triangles.template view<IceMaths::IndexedTriangle>().data(),
            vertices .template view<IceMaths::Point          >().data()
        );
        return interface;
    }

    Opcode::Model init_model() const {
        //Tree building settings
        Opcode::OPCODECREATE OPCC;
        OPCC.mIMesh = const_cast<Opcode::MeshInterface*>(&interface);
        OPCC.mNoLeaf = true;
        OPCC.mQuantized = false;
        OPCC.mKeepOriginal = false;

        Opcode::Model model;
        {
            releaseGIL GIL;         // release GIL during heavy lifting without python calls
            model.Build(OPCC);
        }
        return model;
    }
};


template<typename real_t, typename index_t>
class MeshCollision {
    typedef MeshModel<real_t, index_t> mesh_t;
    typedef ndarray<real_t, 2> affine_t;

    const mesh_t& mesh0;
    const mesh_t& mesh1;

    const Opcode::BVTCache cache;

public:
    explicit MeshCollision(const mesh_t& mesh0, const mesh_t& mesh1) :
        mesh0(mesh0),
        mesh1(mesh1),
        cache(init_cache())
    {
    }

    Opcode::BVTCache init_cache() const {
        Opcode::BVTCache cache;
        cache.Model0 = &mesh0.model;
        cache.Model1 = &mesh1.model;
        return cache;
    }

    ndarray<index_t, 2> query(const affine_t affine0, const affine_t affine1) const {
        // Collision query
        Opcode::AABBTreeCollider TC;
        {
            releaseGIL GIL;         // release GIL during heavy lifting without python calls
            const bool IsOk(
                TC.Collide(
                    const_cast<Opcode::BVTCache&>(cache),       // const in this context, but not for TC
                    (IceMaths::Matrix4x4*)affine0.data(),
                    (IceMaths::Matrix4x4*)affine1.data()
                )
            );
        }

        // view resulting pairs as range
        boost::iterator_range<const IceCore::Pair*> pairs_range = TC.GetContactStatus() ?
            boost::make_iterator_range(TC.GetPairs()    , TC.GetPairs() + TC.GetNbPairs()):
            boost::make_iterator_range((IceCore::Pair*)0, (IceCore::Pair*)0              );

        // wrap resulting pairs in numpy array
        const boost::array<int, 2> shape = {{boost::distance(pairs_range), 2}};
        ndarray<index_t, 2> pairs(shape);
        boost::copy(pairs_range, pairs.template view<IceCore::Pair>().begin());
        return pairs;
    }

};