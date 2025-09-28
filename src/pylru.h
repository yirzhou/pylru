#ifndef INCLUDED_PYLRU_PYLRU_H_
#define INCLUDED_PYLRU_PYLRU_H_

#include <lru.h>
#include <pybind11/pybind11.h>
#include <result.h>

namespace py = pybind11;

PYBIND11_MODULE(pylru, m) {
    m.doc() = "Python bindings for the LRU cache library";

    // Bind the Status enum
    py::enum_<pylru::Status>(m, "Status")
        .value("FOUND", pylru::Status::FOUND)
        .value("NOT_FOUND", pylru::Status::NOT_FOUND);

    // Bind the Result class for int values
    py::class_<pylru::Result<int>>(m, "Result")
        .def("is_found", &pylru::Result<int>::is_found,
             "Check if the result was found")
        .def("is_not_found", &pylru::Result<int>::is_not_found,
             "Check if the result was not found")
        .def("status", &pylru::Result<int>::status,
             "Get the status (FOUND or NOT_FOUND)")
        .def("value", &pylru::Result<int>::value,
             "Get the value (throws if not found)")
        .def("value_or", &pylru::Result<int>::value_or,
             "Get the value or return default if not found")
        .def("__bool__", &pylru::Result<int>::operator bool,
             "Convert to bool (True if found)")
        .def("__repr__", [](const pylru::Result<int> &result) -> std::string {
            if (result.is_found()) {
                return "<Result: FOUND, value=" +
                       std::to_string(result.value()) + ">";
            } else {
                return std::string("<Result: NOT_FOUND>");
            }
        });

    // Bind the LRU cache for int->int
    py::class_<pylru::lru<int, int>>(m, "LRU")
        .def(py::init<const int &>(), "Create an LRU cache with given capacity")
        .def("get", &pylru::lru<int, int>::get, "Get Result by key")
        .def("put", &pylru::lru<int, int>::put, "Put key-value pair into cache")
        .def("__repr__",
             [](const pylru::lru<int, int> &lru) { return "<LRU Cache>"; });
}

#endif
