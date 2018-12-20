# Tests for MPI Sessions extensions to the MPI API

To build these tests you will need the following several packages:

- most recent version of PMIx
- most recent version of prrte
- MPI Sessions prototype

## Installing PMIx

```
git clone git@github.com:pmix/pmix.git
cd pmix
./autogen.pl
./configure --prefix=my_sandbox --with-libevent
make -j install
```

## Installing prrte

```
git clone git@github.com:pmix/prrte.git
cd prrte
./autogen.pl
./configure --prefix=my_sandbox --with-libevent --with-pmix=my_sandbox
make -j install
```

## Installing the prototype

```
git clone git@github.com:hjelmn/ompi.git
cd ompi
git checkout sessions
./autogen.pl
./configure --prefix=my_sandbox --with-libevent --with-pmix=my_sandbox
make -j install
```

## Installing and running these tests

```
export PATH=my_sandbox/bin:$PATH
make
./prte --daemonize
prun -n 4 ./sessions_ex1
prun -n 4 ./sessions_ex2 mpi://world
prun -n 4 ./sessions_test
```
This example assumes your system has at least 4 slots available for MPI processes.
Also, prte is a work in progress.  It may be best to kill the prte daemon after each
run and restart the prte daemon.

