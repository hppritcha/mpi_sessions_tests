#
# Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
#                         University Research and Technology
#                         Corporation.  All rights reserved.
# Copyright (c) 2004-2018 The University of Tennessee and The University
#                         of Tennessee Research Foundation.  All rights
#                         reserved.
# Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
#                         University of Stuttgart.  All rights reserved.
# Copyright (c) 2004-2005 The Regents of the University of California.
#                         All rights reserved.
# Copyright (c) 2006-2007 Sun Microsystems, Inc.  All rights reserved.
# Copyright (c) 2011-2016 Cisco Systems, Inc.  All rights reserved.
# Copyright (c) 2012      Los Alamos National Security, Inc.  All rights reserved.
# Copyright (c) 2013      Mellanox Technologies, Inc.  All rights reserved.
# Copyright (c) 2017-2018 Research Organization for Information Science
#                         and Technology (RIST). All rights reserved.
# $COPYRIGHT$
#
# Additional copyrights may follow
#
# $HEADER$
#

# Use the Open MPI-provided wrapper compilers.

MPICC = mpicc
MPIFC = mpifort

# Example programs to build

EXAMPLES = \
	 sessions_ex1 \
	 sessions_ex2 \
	 sessions_ex3 \
	 sessions_test


# Default target.  Always build the C MPI examples.  Only build the
# others if we have the appropriate Open MPI / OpenSHMEM language
# bindings.

all: $(EXAMPLES)

# The usual "clean" target

clean:
	rm -f $(EXAMPLES) *~ *.o

# Don't rely on default rules for the Fortran and Java examples

sessions_ex1: sessions_ex1.c
	$(MPICC) $(CFLAGS) $(LDFLAGS) $? $(LDLIBS) -o $@
sessions_ex2: sessions_ex2.c
	$(MPICC) $(CFLAGS) $(LDFLAGS) $? $(LDLIBS) -o $@
sessions_test: sessions_test.c
	$(MPICC) $(CFLAGS) $(LDFLAGS) $? $(LDLIBS) -o $@
sessions_ex3: sessions_ex3.f90
	$(MPIFC) $(FCFLAGS) $(LDFLAGS) $? $(LDLIBS) -o $@

