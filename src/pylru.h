#ifndef INCLUDED_PYLRU_PYLRU_H_
#define INCLUDED_PYLRU_PYLRU_H_

#include <lru.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;
namespace pylru {
PYBIND11_MODULE(pylru, m) {
  py::class_<lru<int, int>>(m, "Int2IntLRU")
      .def("get", &pylru::lru<int, int>::get);
};

}  // namespace pylru

#endif
