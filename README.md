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
git clone git@github.com:hpc/ompi.git
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
prte --daemonize
prun -n 4 ./sessions_ex1
prun -n 4 ./sessions_ex2 mpi://world
prun -n 4 ./sessions_test
```
This example assumes your system has at least 4 slots available for MPI processes.
Also, prte is a work in progress.  It may be best to kill the prte daemon after each
run and restart the prte daemon.

## Special instructions for Cray XC and SLURM

On Cray XE/XC systems running SLURM, prte has to use a different procedure for launching
processes on the first node of a SLURM allocation.  Namely, prte launches a prte daemon
using the local slurmd daemon.  As a consequence, there are effectively two prte daemons
on the head node, the prte launched on the command line and a second launched via slurmd.

To avoid annoying things like loss of stdout/stdin from the processes launched on the head
node owing to having two prte daemons running on the node, additional arguments most be added to the prte and prun commands:

```
prte --daemonize --system-server
prun -n 4 --system-server-first (additional arguments)
```


