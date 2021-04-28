#include<stdio.h>
#include<stdlib.h>
#ifndef _GRAPH_H_
#define _GRAPH_H_
#include<stdio.h>

typedef struct stGraph * Graph;
typedef struct stNode * Node;
typedef struct stSPTable * SPTable;
typedef int Vertex;
typedef int Key;

#define WHITE  1
#define GRAY   0
#define BLACK -1
#define INFTY  10000
#define UNKNOWN -1
#define NOTVERTEX -1

Graph   CreteateGraph(int iNumber_of_vertices);
void    InsertEdge(Graph G, Vertex u, Vertex v);
void    BFS(Graph G, Vertex s);
void    DFS(Graph G, Vertex s);
SPTable Dijstra(Graph G, float **c);
SPTable BellmanFord(Graph G, float **c);
SPTable Prim(Graph G, float **c);

struct stGraph{
    int iN;
    Node *pVertex;
};

struct stNode{
    Vertex iVertexID;
    Node pNext;
};

struct stSPTable
{
    int *pKnown;
    float *pdistance_to_source;
    Vertex *pPrevious_vertex;
};

#endif
#include<string.h>
#include<math.h>
#include<time.h>

/////////////////////////////////////////////
// Function CreateEmptyGraph
// Inputs   number of vertex
// Outputs  Pointer to the the graph
/////////////////////////////////////////////
Graph   CreateGraph(int iNumber_of_vertices)
{
    int i=0;
    Graph G;

    // First allocate memory 
    G = malloc(sizeof(struct stGraph));
    if( G == NULL)
    {
        printf("Memory Full!!\n");
        exit(0);
    }
    G->iN       = iNumber_of_vertices;
    G->pVertex  = malloc(iNumber_of_vertices*sizeof(struct stNode));
    if( G->pVertex == NULL)
    {
        printf("Memory Full!!\n");
        exit(0);
    }

    // Create Empty Graph
    for (i=0;i<iNumber_of_vertices;i++)
    {
        G->pVertex[i]->iVertexID    = i;
        G->pVertex[i]->pNext        = NULL;
    }
    return G;
}
////////////////////////////////////////////////////
//// Insert  Edge (u,v) in Graph                  //
//   Inputs: Graph, vertex u, vertex v            //
//   Note that, one can also store edge cost in each
//   node along with the vertex id. In current 
//   implementation, it is assumed that cost function
//   will be provided seperately.                 //
////////////////////////////////////////////////////
void InsertEdge(Graph G, Vertex u, Vertex v)
{
    int i,found=0;
    Node temp;
    Vertex w;

    // First check if the edge already exists.
    temp    = G->pVertex[u];
    while(temp->pNext != NULL)
    {
        w = temp->pNext->iVertexID;
        if(w==v)
            found=1;
        temp = temp->pNext;
    }

    // If it is not already there.
    if(!found)
    {
        temp                    = (Node) malloc(sizeof(struct stNode));
        temp->iVertexID         = v;
        temp->pNext             = G->pVertex[u]->pNext;
        G->pVertex[u]->pNext    = temp;

    }
    return;
}
/////////////////////// Dijstra Algorithm for SPP 
//// SPTable Dijstra(Graph G, float **c)
//   Inputs: Graph G and cost function c
//   Output: Table containing information
//           about SP from Vertex 0. Function also prints 
//           SP for all nodes. can be commented if not required
//   Assumptions c[][] >=0
//////////////////////////////////////
SPTable Dijstra(Graph G, float **c)
{
    SPTable myTable;
    int i,j,N;
    PQueue Q;
    Vertex u,v;
    Node temp;

    // Allocate the memory to hold the Dijsktra table
    N= G->iN;
    myTable = (SPTable)malloc(sizeof(struct stSPTable));
    if(myTable == NULL)
    {
        exit(0);
    }
    myTable->pKnown                 = (int *)malloc(N*sizeof(int));
    myTable->pdistance_to_source    = (float *)malloc(N*sizeof(float));
    myTable->pPrevious_vertex       = (Vertex *)malloc(N*sizeof(Vertex));

    ////
    // Initialize the table
    for(i=0;i<N;i++)
    {
        myTable->pKnown[i]                = 0;
        myTable->pdistance_to_source[i]   = c[i][0];
        myTable->pPrevious_vertex[i]      = UNKNOWN;              
    }

    // Create Priority Queue and Put all the element 
    Q = CreateEmptyPriorityQueue();
    for(i=0;i<N;i++)
        Priority_Enqueue(Q,(Element)i,(Key)myTable->pdistance_to_source[i]); // Some implementation of queuues with extract min support

    // Whlie there is a node for which shortest path is not known
    while(!IsPQueueEmpty(Q))
    {
        // u will be next node for which we now know the shortest path.
        u = (Vertex)ExtractMin(Q);

        myTable->pKnown[u] = 1;
        temp    = G->pVertex[u];
        // for all neighbours of u for whom SP is not known, 
        // check if shorter path is available through u.
        while(temp->pNext != NULL)
        {
            v           = temp->pNext->iVertexID;
            if( (!myTable->pKnown[v]) && (myTable->pdistance_to_source[u]+c[u][v] < myTable->pdistance_to_source[v] ) )
            {
                // RELAX operation from pseudo code.
                myTable->pdistance_to_source[v] = myTable->pdistance_to_source[u]+c[u][v]; // Update dv
                myTable->pPrevious_vertex[v]    = u; // pv
                DecreaseKey(Q,(Element)v, (Key)myTable->pdistance_to_source[v]);
            }
            temp = temp->pNext;
        }
    }

    // Print SP for all the nodes
    for(i=1;i<N;i++)
    {
        printf("Shortest Path from Vertex 0 to %d \n",i);
        PrintPath_Dijsktra(myTable, i);
    }
    return myTable;
}

// Recussion to get shortest path to Vertex v
void PrintPath_Dijsktra(SPTable myTable, Vertex v)
{
    if(myTable->pPrevious_vertex[v] != NOTVERTEX)
    {
       PrintPath_Dijsktra(myTable,myTable->pPrevious_vertex[v]);
       printf(" -> ");
    }
    printf("%d",v);
    return;
}
int main()
{
   int N,
   scanf("%d",&N);
}