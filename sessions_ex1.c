/*
 * Sessions code example from the proposed Sessions additions
 * to the MPI-Standard.  Chapter 10.4.4
 */

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

static MPI_Session lib_shandle = MPI_SESSION_NULL;
static MPI_Comm lib_comm = MPI_COMM_NULL;

int library_foo_init(void)
{
       int rc;
       const char pset_name[] = "mpi://WORLD";
       MPI_Group wgroup = MPI_GROUP_NULL;

       rc = MPI_Session_init(MPI_INFO_NULL, MPI_ERRORS_RETURN,
                             &lib_shandle);
       if (rc != MPI_SUCCESS) {
           return -1;
       }

       /*
        * create a group from the WORLD process set
        */
       rc = MPI_Group_from_session_pset(lib_shandle,
                                        pset_name,
                                        &wgroup);
       if (rc != MPI_SUCCESS) {
           MPI_Session_finalize(&lib_shandle);
           return -1;
       }

       /*
        * get a communicator
        */
       rc = MPI_Comm_create_from_group(wgroup, "mpi.forum.example",
                                       MPI_INFO_NULL,
                                       MPI_ERRORS_RETURN,
                                       &lib_comm);
       if (rc != MPI_SUCCESS) {
           MPI_Group_free(&wgroup);
           MPI_Session_finalize(&lib_shandle);
           return -1;
       }

       /*
        * free group, library doesn't need it.
        */

       MPI_Group_free(&wgroup);

       return 0;
}

int main(int argc, char **argv)
{
    int rc;

    MPI_Init(&argc, &argv);
    /*
     * initialize some kind of widget library
     */

    rc =library_foo_init();

    MPI_Finalize();

    return 0;
}
