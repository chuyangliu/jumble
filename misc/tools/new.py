#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# pylint: disable=C0103,C0111

import argparse
import os
from string import Template


MODULE = "tastylib"

DIR_INC = "include"
DIR_SRC = "src"
DIR_TEST = "test"
DIR_BENCHMARK = "benchmark"

DIR_MODULE_INC = os.path.join(DIR_INC, MODULE)
DIR_MODULE_SRC = os.path.join(DIR_SRC, MODULE)
DIR_BENCHMARK_SRC = os.path.join(DIR_SRC, DIR_BENCHMARK)

EXT_HEADER = "h"
EXT_SRC = "cpp"


# Templates
TMPL_HEADER = \
"""\
#ifndef ${module_upper}_${name_upper}_H_
#define ${module_upper}_${name_upper}_H_

#include "${module}/internal/base.h"

${module_upper}_NS_BEGIN

// TODO

${module_upper}_NS_END

#endif
""" 

TMPL_TEST = \
"""\
#include "gtest/gtest.h"
#include "${module}/${name}.h"

// using ${module}::;

TEST(${name}Test, Basic) {
    // TODO
}
""" 

TMPL_SRC = \
"""\
#include "${module}/${name}.h"

${module_upper}_NS_BEGIN

// TODO

${module_upper}_NS_END
""" 

TMPL_BENCHMARK = \
"""\
#include "${module}/util/io.h"
#include "${module}/util/timer.h"
#include "${module}/${name}.h"

// using ${module}::;

int main() {
    // TODO
    return 0;
}
""" 


# Parse command line args
parser = argparse.ArgumentParser(description="Create new source files.")
parser.add_argument("-s", action='store_const', const=True,
                    help="generate source file (default is header-only)")
parser.add_argument("-b", action='store_const', const=True,
                    help="generate benchmark source file")
parser.add_argument("name",
                    help="name of the header file (without postfix)")
args = parser.parse_args()


# Create header
path = os.path.join(DIR_MODULE_INC, "%s.%s" % (args.name, EXT_HEADER))
print("Creating %s ..." % path)
with open(path, "w") as f:
    f.write(Template(TMPL_HEADER).substitute(
        module=MODULE, module_upper=MODULE.upper(), name_upper=args.name.upper()))

# Create test source
path = os.path.join(DIR_TEST, "%s_%s.%s" % (DIR_TEST, args.name, EXT_SRC))
print("Creating %s ..." % path)
with open(path, "w") as f:
    f.write(Template(TMPL_TEST).substitute(module=MODULE, name=args.name))

# Create source
if args.s:
    path = os.path.join(DIR_MODULE_SRC, "%s.%s" % (args.name, EXT_SRC))
    print("Creating %s ..." % path)
    with open(path, "w") as f:
        f.write(Template(TMPL_SRC).substitute(
            module=MODULE, module_upper=MODULE.upper(), name=args.name))

# Create benchmark source
if args.b:
    path = os.path.join(DIR_BENCHMARK_SRC, "%s_%s.%s" % (DIR_BENCHMARK, args.name, EXT_SRC))
    print("Creating %s ..." % path)
    with open(path, "w") as f:
        f.write(Template(TMPL_BENCHMARK).substitute(module=MODULE, name=args.name))
