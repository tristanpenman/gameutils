# Game Utils

This repo contains some code that I have found useful for game development.

  - **entity.h** - An entity/component system based on the STL

    Entity/component systems are an alternative to traditional OO game objects. For example, instead of creating a Player class, you would define a PlayerComponent struct, create an entity, then attach an instance of PlayerComponent to the
    entity. This allows for dynamic composition of behaviour, and aids in writing of maintainable game logic.

  - **math.h** - Simple implementations of vectors, matrices and quaternions

    Classes provided are `Vec2`, `Vec3`, `Vec4`, `Mat3`, `Mat4`, and `Quat`. These classes support most of the basic operations required for graphics and physics calculations. The interfaces are designed with code clarity as the first priority.

    Templated prototypes for output using `std::ostream` are included in `math.h`, and default implementations for `float` and `double` types are included in `math.cpp`.

Note: Most of this code was written around 2012-13, so it could probably be improved using some techniques from modern C++. Suggestions are welcomed via Pull Requests or GitHub issues.

## Dependencies

This library depends on several features introduced in C++11. Older compilers may work (e.g. those supporting variations of C++0x), but have not been tested.

To support unit testing, googletest has been included in the `libs/gtest-1.6.0` directory. To build the tests, SCons is used, and has been included under `tools/scons`.

## Building

Since this library would typically be integrated into a game's existing build system, only a minimal SCons build script has been included. This script is used for unit testing, but may also be used as a reference when configuring other build systems.

A Makefile has also been included. This is a thin wrapper around the SCons build script, and allows you to simply type `make` or `make test` at the command line.

## License

This code is licensed under the Simplified BSD License.

See the LICENSE file for more information.
