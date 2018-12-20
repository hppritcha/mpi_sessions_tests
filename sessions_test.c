#include <stdlib.h>
#include <stdio.h>

#include <mpi.h>

void my_session_errhandler (MPI_Session *foo, int *bar, ...)
{
  fprintf (stderr, "my error handler called here with error %d\n", *bar);
}

int main (int argc, char *argv[]) {
  MPI_Flags flags = MPI_FLAG_THREAD_CONCURRENT;
  MPI_Session session;
  MPI_Errhandler test;
  MPI_Group group;
  MPI_Comm comm_world, comm_self;
  MPI_Info info;
  int rc, npsets, one = 1, sum;

  rc = MPI_Session_create_errhandler (my_session_errhandler, &test);
  if (MPI_SUCCESS != rc) {
    fprintf (stderr, "Error handler creation failed with rc = %d\n", rc);
    abort ();
  }

  rc = MPI_Info_create (&info);
  if (MPI_SUCCESS != rc) {
    fprintf (stderr, "Info creation failed with rc = %d\n", rc);
    abort ();
  }

  rc = MPI_Session_init (&flags, info, test, &session);
  if (MPI_SUCCESS != rc) {
    fprintf (stderr, "Session initialization failed with rc = %d\n", rc);
    abort ();
  }

  rc = MPI_Session_get_num_psets (session, &npsets);
  fprintf (stderr, "There are %d psets\n", npsets);
  for (int i = 0 ; i < npsets ; ++i) {
    int psetlen;
    char name[256];
    MPI_Session_get_psetlen (session, i, &psetlen);
    MPI_Session_get_nth_pset (session, i, psetlen, name);
    fprintf (stderr, "  PSET %d: %s (len: %d)\n", i, name, psetlen);
  }

  rc = MPI_Group_from_session_pset (session, "mpi://world", &group);
  if (MPI_SUCCESS != rc) {
    fprintf (stderr, "Could not get a group for mpi://world. rc = %d\n", rc);
    abort ();
  }

  MPI_Comm_create_from_group (group, "my_world", MPI_INFO_NULL, MPI_ERRORS_RETURN, &comm_world);
  MPI_Group_free (&group);

  MPI_Allreduce (&one, &sum, 1, MPI_INT, MPI_SUM, comm_world);

  fprintf (stderr, "World Comm Sum (1): %d\n", sum);


  rc = MPI_Group_from_session_pset (session, "mpi://self", &group);
  if (MPI_SUCCESS != rc) {
    fprintf (stderr, "Could not get a group for mpi://self. rc = %d\n", rc);
    abort ();
  }

  MPI_Comm_create_from_group (group, "myself", MPI_INFO_NULL, MPI_ERRORS_RETURN, &comm_self);
  MPI_Group_free (&group);
  MPI_Allreduce (&one, &sum, 1, MPI_INT, MPI_SUM, comm_self);

  fprintf (stderr, "Self Comm Sum (1): %d\n", sum);

  MPI_Errhandler_free (&test);
  MPI_Info_free (&info);


  MPI_Comm_free (&comm_world);
  MPI_Comm_free (&comm_self);
  MPI_Session_finalize (&session);

  return 0;
}
