#include "tastylib/util/io.h"
#include "tastylib/util/timer.h"
#include "tastylib/NPuzzle.h"

using tastylib::printLn;
using tastylib::timing;
using tastylib::PuzzleNode;
using tastylib::NPuzzle;

typedef PuzzleNode::Container Container;
typedef PuzzleNode::SizeType SizeType;


void benchmark(PuzzleNode& beg, PuzzleNode& end) {
    printLn("Benchmarking ", beg.getRowCount(), "*", end.getRowCount(), " puzzle...");
    NPuzzle puzzle(beg, end);
    printLn("Beg: ", beg.toString());
    printLn("End: ", end.toString());
    printLn("Searching...");
    auto time = timing([&]() {
        puzzle.solve();
    });
    auto &path = puzzle.getPath();
    auto searchCnt = puzzle.getSearchCount();
    printLn("Searched nodes: ", searchCnt);
    printLn("     Time cost: ", (int)time, " ms");
    printLn("    Efficiency: ", searchCnt / time, " node/ms");
    printLn("   Path length: ", path.size());
    for (const auto &d : path) {
        beg.move(d);
    }
    printLn("Solution check: ", beg == end ? "pass" : "fail");
    printLn("Benchmark of ", beg.getRowCount(), "*", end.getRowCount(), " puzzle finished.\n");
}

Container defVal(const SizeType size) {
    Container val;
    for (SizeType i = 0; i < size * size; ++i) {
        val.push_back((short)i);
    }
    return val;
}

int main() {
    printLn("Benchmark of NPuzzle running...\n");
    {
        {   // Benchmark 3*3 puzzle
            const SizeType SIZE = 3;
            auto val = defVal(SIZE);
            PuzzleNode beg(val, SIZE, SIZE);
            PuzzleNode end(val, SIZE, SIZE);
            beg.shuffle();
            end.shuffle();
            benchmark(beg, end);
        }

        {   // Benchmark 4*4 puzzle
            const SizeType SIZE = 4;
            auto val = defVal(SIZE);
            PuzzleNode beg(val, SIZE, SIZE);
            PuzzleNode end(val, SIZE, SIZE);
            beg.shuffle();
            end.shuffle();
            benchmark(beg, end);
        }

        {   // Benchmark 5*5 puzzle
            const SizeType SIZE = 5;
            auto val = defVal(SIZE);
            PuzzleNode beg(val, SIZE, SIZE);
            PuzzleNode end(val, SIZE, SIZE);
            beg.shuffle();
            end.shuffle();
            benchmark(beg, end);
        }

        {   // Benchmark 6*6 puzzle
            const SizeType SIZE = 6;
            auto val = defVal(SIZE);
            PuzzleNode beg(val, SIZE, SIZE);
            PuzzleNode end(val, SIZE, SIZE);
            beg.shuffle();
            end.shuffle();
            benchmark(beg, end);
        }
    }
    printLn("Benchmark of NPuzzle finished.");
    checkMemoryLeaks();
    return 0;
}
