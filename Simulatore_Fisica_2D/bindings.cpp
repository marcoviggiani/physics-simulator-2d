#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <vector>

#include "Vec2.h"
#include "Body.h"
#include "World.h"

namespace py = pybind11;

// Dice a pybind11: "std::vector<Body> non convertirlo in una lista copiata,
// lo gestisco io come tipo a sé". Va messo PRIMA di PYBIND11_MODULE.
PYBIND11_MAKE_OPAQUE(std::vector<Body>)

PYBIND11_MODULE(fisica, m) {
    m.doc() = "Motore fisico 2D in C++";

    // ---------- Vec2 ----------
    py::class_<Vec2>(m, "Vec2")
        .def(py::init([](double x, double y) { return Vec2{ x, y }; }),
            py::arg("x") = 0.0, py::arg("y") = 0.0)
        .def_readwrite("x", &Vec2::x)
        .def_readwrite("y", &Vec2::y)
        .def("length", &Vec2::length)
        .def("__repr__", [](const Vec2& v) {
        return py::str("Vec2({}, {})").format(v.x, v.y);
            });

    m.def("dot", &dot);

    // ---------- Body ----------
    py::class_<Body>(m, "Body")
        .def(py::init<>())
        .def_readwrite("position", &Body::position)
        .def_readwrite("velocity", &Body::velocity)
        .def_readwrite("mass", &Body::mass)
        .def_readwrite("radius", &Body::radius)
        .def_readwrite("restitution", &Body::restitution);

    // ---------- Lista di Body (accesso reale, non copie) ----------
    // Dopo Body (che deve essere già registrato) e prima di World.
    py::bind_vector<std::vector<Body>>(m, "BodyList");

    // ---------- World ----------
    py::class_<World>(m, "World")
        .def(py::init<>())
        .def_readwrite("gravity", &World::gravity)
        .def_readwrite("width", &World::width)
        .def_readwrite("height", &World::height)
        .def_readwrite("bodies", &World::bodies)
        .def("step", &World::step, py::arg("dt"))
        // addBody lancia std::invalid_argument: in Python diventa ValueError
        .def("add_body", &World::addBody, py::arg("body"));
}