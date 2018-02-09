# MapGenerator
A map generation library for game development.

## Download
To download the latest compiled binaries, go to [the project's official website](http://dinf-prj-12.dinf.fsci.usherbrooke.ca/download.html)

NOTE: The website is not accessible outside the University's network.

### Prerequistes
* CMake
* A C++11 compatible compiler

## Documentation
### Coding Guideline
* Use 4 spaces to indent block
* Use snake case for new type
* Each public function, type, macro, constant must be documented with Doxygen's syntax
* CPP headers should use .hpp extension
* C headers should use .h extension
* CPP types should be declared in mapgen namespace

### API Documentation
We use Doxygen to generate our API documentation.
First you must run CMake, it will create a new target called doc.
Running this target will generate all the documentation.

## Versionning
We use [SemVer](http://semver.org/) for versioning.

## Authors
* **Nathan Giraldeau** - *Initial work*
* **Dominic Nault** - *Initial work*
