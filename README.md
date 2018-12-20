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
./prrte --daemonize
```

