#include <stdio.h>
#include "project3.h"

extern int TraceLevel;

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};
struct distance_table dt2;
struct NeighborCosts   *neighbor2;

/* students to write the following two routines, and maybe some others */

extern float clocktime;

int initCost[MAX_NODES];

void printdt2(int MyNodeNumber, struct NeighborCosts *neighbor, struct distance_table *dtptr);

void rtinit2() {
    struct RoutePacket somePacket;
    
    printf("At time t=%f, rtinit2() called.\n", clocktime);

    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            dt2.costs[i][j] = INFINITY;
        }
        neighbor2 = getNeighborCosts(2);

        for (int i = 0; i < neighbor2->NodesInNetwork; i++) {
            initCost[i] = neighbor2->NodeCosts[i];
            dt2.costs[i][i] = neighbor2->NodeCosts[i];
        }
        printdt2(2, neighbor2, &dt2);

        for (int i = 0; i < neighbor2->NodesInNetwork; i++) {
            if (i == 2) {
                continue;
            }
            if (initCost[i] == INFINITY) {
                continue;
            }
        }

        somePacket.destid = i;
        somePacket.sourceid = 2;

        for (int j = 0; j < MAX_NODES; j++) {
            somePacket.mincost[j] = dt2.costs[j][j];
        }
        printf("At time t=%f, node 2 sends packet to node %d with: ", clocktime, i);

        for (int j = 0; j < MAX_NODES; j++) {
            printf("%d ", j);
        }
        printf("\n");
        toLayer2(somePacket);
    }
}


void rtupdate2( struct RoutePacket *rcvdpkt ) {
    int min;
    int counter = 0;
    int saveData = rcvdpkt->sourceid;

    struct RoutePacket somePacket;

    for (int i = 0; i < MAX_NODES; i++) {
        if (i == 2) {
            continue;
        }
        if (dt2.costs[i][saveData] > (dt2.costs[saveData][saveData] + rcvdpkt->mincost[i])) {
            dt2.costs[i][saveData] = dt2.costs[saveData][saveData] + rcvdpkt->mincost[i];
            counter++;
        }
    }

    if (counter > 0) {
        printdt2(2, neighbor2, &dt2);
        for (int i = 0; i < neighbor2->NodesInNetwork; i++) {
            if (i == 2) {
                continue;
            }
            if (initCost[i] == INFINITY) {
                continue;
            }
            
            somePacket.destid = i;
            somePacket.sourceid = 2;

            printf("At time t=%f, node 2 sends packet to node %d with: ", clocktime, i);

            for (int j = 0; j < MAX_NODES; j++) {
                min = INFINITY;

                for (int k = 0; k < MAX_NODES; k++) {
                    if (dt2.costs[j][k] < min) {
                        min = dt2.costs[j][k];
                    }
                }
                somePacket.mincost[j] = min;

                printf("%d ", min);
            }
            toLayer2(somePacket);
            printf("\n");
        }
    }
}


/////////////////////////////////////////////////////////////////////
//  printdt
//  This routine is being supplied to you.  It is the same code in
//  each node and is tailored based on the input arguments.
//  Required arguments:
//  MyNodeNumber:  This routine assumes that you know your node
//                 number and supply it when making this call.
//  struct NeighborCosts *neighbor:  A pointer to the structure 
//                 that's supplied via a call to getNeighborCosts().
//                 It tells this print routine the configuration
//                 of nodes surrounding the node we're working on.
//  struct distance_table *dtptr: This is the running record of the
//                 current costs as seen by this node.  It is 
//                 constantly updated as the node gets new
//                 messages from other nodes.
/////////////////////////////////////////////////////////////////////
void printdt2( int MyNodeNumber, struct NeighborCosts *neighbor, 
		struct distance_table *dtptr ) {
    int       i, j;
    int       TotalNodes = neighbor->NodesInNetwork;     // Total nodes in network
    int       NumberOfNeighbors = 0;                     // How many neighbors
    int       Neighbors[MAX_NODES];                      // Who are the neighbors

    // Determine our neighbors 
    for ( i = 0; i < TotalNodes; i++ )  {
        if (( neighbor->NodeCosts[i] != INFINITY ) && i != MyNodeNumber )  {
            Neighbors[NumberOfNeighbors] = i;
            NumberOfNeighbors++;
        }
    }
    // Print the header
    printf("                via     \n");
    printf("   D%d |", MyNodeNumber );
    for ( i = 0; i < NumberOfNeighbors; i++ )
        printf("     %d", Neighbors[i]);
    printf("\n");
    printf("  ----|-------------------------------\n");

    // For each node, print the cost by travelling thru each of our neighbors
    for ( i = 0; i < TotalNodes; i++ )   {
        if ( i != MyNodeNumber )  {
            printf("dest %d|", i );
            for ( j = 0; j < NumberOfNeighbors; j++ )  {
                    printf( "  %4d", dtptr->costs[i][Neighbors[j]] );
            }
            printf("\n");
        }
    }
    printf("\n");
}    // End of printdt2

