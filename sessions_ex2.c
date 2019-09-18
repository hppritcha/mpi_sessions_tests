/*
 * Sessions code example from the proposed Sessions additions
 * to the MPI-Standard.  Chapter 10.4.4
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
       int i, n_psets, psetlen, rc, psetsize, comm_size, pcomm_rank;
       int ar_out, flag = 0;
       char *pset_name;
       char info_val[MPI_MAX_INFO_VAL];
       MPI_Session shandle = MPI_SESSION_NULL;
       MPI_Info sinfo = MPI_INFO_NULL;
       MPI_Info info = MPI_INFO_NULL;
       MPI_Group pgroup = MPI_GROUP_NULL;
       MPI_Comm pcomm;
       const char ts_level_req[] = "MPI_THREAD_MULTIPLE";

       if (argc < 2) {
           fprintf(stderr, "A process set name is required\n");
           exit -1;
       }

       rc = MPI_Info_create (&info);
       if (MPI_SUCCESS != rc) {
           fprintf (stderr, "Info creation failed with rc = %d\n", rc);
           abort ();
       }

       MPI_Info_set(info, "thread_support_level", ts_level_req);

       MPI_Session_init(info, MPI_ERRORS_RETURN, &shandle);

       rc = MPI_Session_get_num_psets(shandle, &n_psets);
       if (rc != MPI_SUCCESS) goto exit_w_err;

       assert(n_psets >= 2);  /* WORLD and SELF process
                                sets are suppose to be available */

       for (i = 0, pset_name = NULL; i < n_psets; i++) {
            rc = MPI_Session_get_psetlen(shandle, i, &psetlen);
            if (rc != MPI_SUCCESS) goto exit_w_err;
            pset_name = (char *)malloc(sizeof(char) * (psetlen + 1));
            rc = MPI_Session_get_nth_pset(shandle, i, psetlen + 1, pset_name);
            if (rc != MPI_SUCCESS) goto exit_w_err;

            if (strcmp(pset_name, argv[1]) == 0) break;

            free(pset_name);
            pset_name = NULL;
       }

       if (pset_name == NULL) {

           fprintf(stderr, "Well didn't find requested process set,"
                           " bailing out\n");
           goto exit_w_err;

       } else {

           /*
            * get instance of an info object for this Session
            */
           rc = MPI_Session_get_pset_info(shandle, pset_name, &sinfo);
           if (rc != MPI_SUCCESS) goto exit_w_err;

           rc = MPI_Info_get(sinfo, "size", MPI_MAX_INFO_VAL, info_val, &flag);
           if (rc != MPI_SUCCESS) goto exit_w_err;
           assert(flag != 0);
           psetsize = atoi(info_val);

           /*
            * create a group from the process set
            */
           rc = MPI_Group_from_session_pset(shandle, pset_name,
                                            &pgroup);
           if (rc != MPI_SUCCESS) goto exit_w_err;

           /*
            * got here, let's go for the communicator, we'll do a simple flat
            * communicator here.
            */
           rc = MPI_Comm_create_from_group(pgroup, "session_example",
                                           MPI_INFO_NULL,
                                           MPI_ERRORS_RETURN,
                                           &pcomm);
           if (rc != MPI_SUCCESS) goto exit_w_err;

           rc = MPI_Comm_size(pcomm, &comm_size);
           if (rc != MPI_SUCCESS) goto exit_w_err;

           if (comm_size != psetsize) {
               fprintf(stderr, "Process set size and the number"
                               " of ranks in communicator differ, bailing\n");
               goto exit_w_err;
           }

           /*
            * check if we can actually communicate using this communicator
            */

           rc = MPI_Comm_rank(pcomm, &pcomm_rank);
           if (rc != MPI_SUCCESS) goto exit_w_err;

           fprintf(stderr, "trying to do an allreduce\n");
           rc = MPI_Allreduce(&pcomm_rank, &ar_out, 1, MPI_INT, MPI_SUM,
                              pcomm);
           if (rc != MPI_SUCCESS) goto exit_w_err;

           if (ar_out != (((psetsize - 1) * psetsize)/2)) {
               fprintf(stderr, "Incorrect reduction value, bailing\n");
               goto exit_w_err;
           }

       }

       fprintf(stderr, "Test passed0\n");

       free(pset_name);
       MPI_Comm_free(&pcomm);
       MPI_Group_free(&pgroup);
       MPI_Info_free(&sinfo);
       MPI_Info_free(&info);
       MPI_Session_finalize(&shandle);
       fprintf(stderr, "Test passed\n");
       exit(0);

exit_w_err:
       if (pset_name != NULL) free(pset_name);
       if (sinfo != MPI_INFO_NULL) MPI_Info_free(&sinfo);
       if (info != MPI_INFO_NULL) MPI_Info_free(&info);
       if (pgroup != MPI_GROUP_NULL) MPI_Group_free(&pgroup);
       if (pcomm != MPI_COMM_NULL) MPI_Comm_free(&pcomm);
       MPI_Session_finalize(&shandle);
       exit(-1);
}



