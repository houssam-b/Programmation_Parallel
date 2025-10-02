#include <iostream>
#include <mpi.h>
#include "mnt.h"
#include "mnt_parallel.h"

int main(int argc, char **argv) {
    int pid, nprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    char* filename = argv[1];
    int root = 0;

    int nb_lignes=0;
    int nb_cols=0;
    float no_value= 0.0f;

    float* terrain=nullptr;

    if (pid==root) {
        lecture(filename, &nb_lignes, &nb_cols, &no_value, &terrain);
        // std::cout << "le terrain" << std::endl;
        // affichageTerrain(nb_lignes, nb_cols, terrain);
    }

    MPI_Bcast(&nb_lignes, 1, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Bcast(&nb_cols, 1, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Bcast(&no_value, 1, MPI_FLOAT, root, MPI_COMM_WORLD);

    if (nb_lignes % nprocs != 0) {
        if (pid == root) {
            std::cerr << "ERREUR: " << nb_lignes << " lignes non divisible par " 
                      << nprocs << " processus!" << std::endl;
        }
        MPI_Finalize();
        return 1;
    }
    
    int ligne_procs = nb_lignes / nprocs ;

    int taille_tab_local =(ligne_procs +2 )*nb_cols;


    float* terrain_local = new float[taille_tab_local];
    for (int i = 0; i < taille_tab_local; i++) {
        terrain_local[i] = no_value;  // Tout rempli avec -9999
    }

    //para pour scatter
    int elements_par_proc = ligne_procs * nb_cols;
    float* buffer_reception = &terrain_local[nb_cols]; //nb_cols 7itach 16 lwala dyal lghost

    
    int offset =nb_cols;
    MPI_Scatter(
        &terrain[offset],
        elements_par_proc,
        MPI_FLOAT,
        buffer_reception,
        elements_par_proc,
        MPI_FLOAT,
        root,
        MPI_COMM_WORLD
    );
    
    
    MPI_Barrier(MPI_COMM_WORLD); 

    for (int i = 0; i < nprocs; i++) {
        if (pid == i) {
            std::cout << "\n=================================================================================" << std::endl;
            std::cout << "PROCESSUS " << pid<<"\n";
            affichageTerrainLocal(ligne_procs+2, nb_cols, &terrain_local[0]);
            
            std::cout << "=====================================================================================" << std::endl;
        }
        MPI_Barrier(MPI_COMM_WORLD);  
    }
    echangeDeDonnees(terrain_local, ligne_procs, nb_cols, pid, nprocs);
   
    

    MPI_Barrier(MPI_COMM_WORLD);

   
    for (int i = 0; i < nprocs; i++) {
        if (pid == i) {
            std::cout << "\n================================================================================" << std::endl;
            std::cout << "PROCESSUS " << pid<<"\n";
            affichageTerrainLocal(ligne_procs+2, nb_cols, &terrain_local[0]);
            
            std::cout << "=====================================================================================" << std::endl;
        }
        MPI_Barrier(MPI_COMM_WORLD);  
    }
    
    if (pid==root)
        delete[] terrain;

    MPI_Finalize();
    return 0;
}