# htmlpp

You probably want a template engine... [inja](https://github.com/pantor/inja) looks nice.  
but if you're sure you don't, htmlpp will at least save you typing a few angle brackets.

[![GitHub Workflow Status](https://img.shields.io/github/workflow/status/cardillo/htmlpp/build)](https://github.com/cardillo/htmlpp/actions/workflows/build.yml)

## quick start

```cpp
#include <html.hpp>
```

that's it.  now you have access to the `html::` namespace will all the tags and attributes
you expect.

## example

a more complete example, see [example.cpp](test/example.cpp) for the full source.

```cpp
    server.run([](auto&& stream) {
        stream << html::indent;
        stream << html::html
            << html::head
            << html::script[html::language("javascript")]
            << "function sayHello() {" << std::endl
            << "  document.getElementById('message').innerHTML = 'Hello, world';" << std::endl
            << "}" << std::endl
            << html::head::end
            << html::body[html::attr("onload", "sayHello()")]
            << html::div[html::id("message")]
            << html::end;
    });
}
```
