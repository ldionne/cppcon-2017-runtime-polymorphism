## Runtime Polymorphism: Back to the Basics @ CppCon 2017

This repository contains my [reveal.js][]-based presentation on runtime
polymorphism for [CppCon 2017][]. It is also related to the [Dyno][]
library, which implements the ideas explored in the presentation.

## Usage
Go to https://ldionne.com/cppcon-2017-runtime-polymorphism or open `index.html`
with your browser. A PDF version of the slides is in `slides.pdf`.

## Building the code samples
```sh
# Install the dependencies and get back the CMAKE_PREFIX_PATH to use
CMAKE_PREFIX_PATH="$(./code/dependencies/install.sh)"

# Usual CMake build, with a custom CMAKE_PREFIX_PATH for locally-installed dependencies
(mkdir build && cd build && cmake .. -GNinja -DCMAKE_PREFIX_PATH="${CMAKE_PREFIX_PATH}")
cmake --build build
```

<!-- Links -->
[CppCon 2017]: https://cppcon.org
[reveal.js]: https://github.com/hakimel/reveal.js
[Dyno]: https://github.com/ldionne/dyno
