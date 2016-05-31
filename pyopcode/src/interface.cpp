/*
boost python interface definition
*/

#define BOOST_DISABLE_ASSERTS
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION

#include "exception.cpp"
#include "typedefs.cpp"


using namespace boost;

typedef float32_t   real_t;         // real type of coordinate space



using namespace boost::python;
BOOST_PYTHON_MODULE(Collision)
{
    // init GIL control
    PyEval_InitThreads();

	// initialize numpy support
	init_numpy();
	
	//register array types employed; needed to avoid runtime error
	numpy_boost_python_register_type<int16_t, 1>();
	numpy_boost_python_register_type<int16_t, 2>();
	numpy_boost_python_register_type<int32_t, 1>();
	numpy_boost_python_register_type<int32_t, 2>();
	numpy_boost_python_register_type<int64_t, 1>();
	numpy_boost_python_register_type<int64_t, 2>();

	numpy_boost_python_register_type<real_t, 1>();
	numpy_boost_python_register_type<real_t, 2>();
	numpy_boost_python_register_type<real_t, 3>();


	register_exception_translator<python_exception>(&translate);

}
