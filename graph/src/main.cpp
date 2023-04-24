#include "graph.h"
#include <iostream>
#include <cassert>

void test_graph()
{
    
    using GraphS = Graph<std::string>;
    using Edge = GraphS::Edge;
    GraphS g;
    g.addEdge(Edge("1", "2"));
    g.addEdge(Edge("1", "3"));
    g.addEdge(Edge("1", "4"));
    g.addEdge(Edge("3", "5"));
    g.addEdge(Edge("3", "6"));
    std::cout << g.dump();

    struct Figure
    {
        Figure(std::string nm, int ag) : name(nm), age(ag) {}
        std::string name;
        int age;
    };
    using GraphF = Graph<Figure>;
    using EdgeF = GraphF::Edge;
    GraphF f;
    f.addEdge(EdgeF(Figure("1", 1), Figure("2", 2)));
}

int main(int argc, char *argv[])
{
    test_graph();
}

