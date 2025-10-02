#include <mpi.h>
#include <iostream>



void echangeDeDonnees(float* terrain_local,int ligne_tab_local,int nb_cols,int rang,int nb_procs){
    
    MPI_Request requests[4];
    int nb_requests = 0;
    
    if (rang>0) //envoie vers le haut
    {
        //envoie vers le haut
        MPI_Isend(&terrain_local[nb_cols],nb_cols,MPI_FLOAT,rang-1,0,MPI_COMM_WORLD, &requests[nb_requests]);
        nb_requests++;

        //recevoir d'haut
        MPI_Irecv(&terrain_local[0],nb_cols,MPI_FLOAT,rang-1,1,MPI_COMM_WORLD,&requests[nb_requests]);
        nb_requests++;
        
    }
    if (rang<nb_procs-1) //envoie vers le bas
    {
        //envoie vers le bas
        MPI_Isend(&terrain_local[ligne_tab_local * nb_cols],nb_cols,MPI_FLOAT,rang+1,1,MPI_COMM_WORLD, &requests[nb_requests]);
        nb_requests++;

        //recevoir du bas
        MPI_Irecv(&terrain_local[(ligne_tab_local+1)*nb_cols],nb_cols,MPI_FLOAT,rang+1,0,MPI_COMM_WORLD,&requests[nb_requests]);
        nb_requests++;
        
    }
    MPI_Waitall(nb_requests, requests, MPI_STATUSES_IGNORE);
    
}