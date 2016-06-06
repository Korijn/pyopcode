/*
boost python interface definition
*/

#define BOOST_DISABLE_ASSERTS
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION


#include <boost/python.hpp>

#include "exception.cpp"
#include "typedefs.cpp"
#include "pyopcode.cpp"

using namespace boost;

typedef float32_t   real_t;         // real type of coordinate space
typedef int32_t     index_t;         // real type of coordinate space

typedef MeshModel<real_t, index_t> model_t;
typedef MeshCollision<real_t, index_t> collision_t;



using namespace boost::python;
BOOST_PYTHON_MODULE(_pyopcode)
{
    // init GIL control
    PyEval_InitThreads();

    // initialize numpy support
    init_numpy();

    //register array types employed; needed to avoid runtime error
    numpy_boost_python_register_type<index_t, 2>();
    numpy_boost_python_register_type<real_t, 2>();

    class_<model_t>("Model", init<ndarray<real_t, 2>, ndarray<index_t, 2>>())
        ;

    class_<collision_t>("Collision", init<model_t&, model_t&>())
        .def("query", &collision_t::query)
        ;

    register_exception_translator<python_exception>(&translate);

}
