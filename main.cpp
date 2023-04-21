// MPITimesTables.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <iomanip>
#include "mpi.h"

int main(int argc, char* argv[])

{
#pragma comment (lib, "msmpi.lib")
    int ierr;
    int rank;
    int commsize;

    ierr = MPI_Init(&argc, &argv);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &commsize);


    if (rank != 0)
    {
        //Will only execute for processes that aren't process 0
        std::vector<int> sendingVec;
        for (int i = 0; i < commsize; i++)
        {
            sendingVec.push_back(rank * i);
        }
        int size = commsize - 1;
        MPI_Send(&sendingVec[0], size, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else {
        //Will only execute for process 0
        for (int i = 1; i < commsize; i++) {

            std::vector<int>receivingVec;
            int size = commsize - 1;
            MPI_Status status;
            receivingVec.resize(size);
            ierr = MPI_Recv(&receivingVec[0], size, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            if (ierr == MPI_SUCCESS)
            {
                //assumes reveive was successful
                for (int j = 1; j < size; j++)
                {
                    std::cout << std::setw(4) << receivingVec[j];
                }
                std::cout << std::endl;
            }
            else
            {
                //receive wasn't successful
                MPI_Abort(MPI_COMM_WORLD, 1);

            }
        }
    }
    ierr = MPI_Finalize();
    return 0;
};

