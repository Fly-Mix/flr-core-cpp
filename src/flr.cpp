//
// Created by York on 2021/4/18.
//

#include <iostream>
#include "flr-core/flr.h"
#include <ryml.hpp>
#include <ryml_std.hpp>

// convenience functions to print a node
void show_keyval(ryml::NodeRef n)
{
    assert(n.has_key());
    std::cout << n.key() << ": " << n.val() << "\n";
}
void show_val(ryml::NodeRef n)
{
    assert(n.has_val());
    std::cout << n.val() << "\n";
}

void flr::hello() {
    std::cout << "Hello, World! Hello C++! " << std::endl;

    // ryml can parse in situ (and read-only buffers too):
    char src[] = "{foo: 1, bar: [2, 3]}";
    c4::substr srcview = src; // a mutable view to the source buffer
    // there are also overloads for reusing the tree and parser
    ryml::Tree tree = ryml::parse(srcview);

    // get a reference to the "foo" node
    ryml::NodeRef node = tree["foo"];
    show_keyval(node);  // "foo: 1"
    show_val(tree["bar"][0]);  // "2"
    show_val(tree["bar"][1]);  // "3"

    // deserializing:
    int foo;
    std::cout << "foo(deserializing before): " << foo << std::endl;
    node >> foo; // now foo == 1
    std::cout << "foo(deserializing after): " << foo << std::endl;
}