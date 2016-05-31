/*
boost python interface definition
*/

#define BOOST_DISABLE_ASSERTS
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#define BAN_OPCODE_AUTOLINK
#define ICE_NO_DLL


#include "exception.h"
#include "typedefs.h"
#include "pyopcode.h"

using namespace boost;

typedef float32_t   real_t;         // real type of coordinate space
typedef int32_t     index_t;         // real type of coordinate space

typedef Mesh<real_t, index_t> mesh_t;
typedef MeshCollision<real_t, index_t> meshcollision_t;



using namespace boost::python;
BOOST_PYTHON_MODULE(pyopcode)
{
    // init GIL control
    PyEval_InitThreads();

	// initialize numpy support
	init_numpy();
	
	//register array types employed; needed to avoid runtime error
	numpy_boost_python_register_type<index_t, 2>();
	numpy_boost_python_register_type<real_t, 2>();

    class_<mesh_t>("Mesh", init<ndarray<real_t, 2>, ndarray<index_t, 2>>())
		;

    class_<meshcollision_t>("MeshCollision", init<mesh_t, mesh_t>())
        .def("query", &meshcollision_t::query)
        ;

	register_exception_translator<python_exception>(&translate);

}
