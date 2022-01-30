#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#define FILENAME "input.txt"
#define MAX_NUM_NODES 256
#define MAX_LABEL_LENGTH 128
#define START_LABEL "start"
#define END_LABEL "end"

// Node structure type
typedef struct node {
  char label[128];
  struct node* next;
} Node;

// Graph structure type
typedef struct graph {
  int size;
  Node** nodes;
} Graph;

// Create a new graph with provided number of nodes
Graph* createGraph() {
  Graph* graph = malloc(sizeof(Graph));
  graph->size = 0;
  graph->nodes = malloc(sizeof(Node*) * MAX_NUM_NODES);

  for (int i=0; i<MAX_NUM_NODES; i++)
    graph->nodes[i] = NULL;

  return graph;
}

// Get Node by label
Node* getNode(Graph* graph, char* nodeLabel) {
  for (int i=0; i<graph->size; i++) {
    if (!strncmp(graph->nodes[i]->label, nodeLabel, MAX_LABEL_LENGTH))
      return graph->nodes[i]; 
  }
  return NULL;
}

// Create node
Node* createNode(char* nodeLabel) {
  Node* newNode = malloc(sizeof(Node));
  strncpy(newNode->label, nodeLabel, MAX_LABEL_LENGTH);
  newNode->next = NULL;
  return newNode;
}

// Create node with provided label and add it to graph
void addNode(Graph* graph, Node* node) {
  graph->nodes[graph->size++] = node;
}

// Create the nodes if necessary and adds the edge
void addEdge(Graph* graph, char* nodeLabel1, char* nodeLabel2) {
  Node* node1 = getNode(graph, nodeLabel1);
  Node* node2 = getNode(graph, nodeLabel2);
  
  if (!node1) {
    node1 = createNode(nodeLabel1);
    if (strncmp(nodeLabel1, END_LABEL, MAX_LABEL_LENGTH)!=0)
      addNode(graph, node1);
  }
  if (!node2) {
    node2 = createNode(nodeLabel2);
    if (strncmp(nodeLabel2, END_LABEL, MAX_LABEL_LENGTH)!=0)
      addNode(graph, node2);
  }

  Node* tmpAdjNode;
  if (strncmp(nodeLabel1, START_LABEL, MAX_LABEL_LENGTH)!=0) {    
    tmpAdjNode = createNode(nodeLabel1);
    tmpAdjNode->next = node2->next;
    node2->next = tmpAdjNode;
  }

  if (strncmp(nodeLabel2, START_LABEL, MAX_LABEL_LENGTH)!=0) {   
    tmpAdjNode = createNode(nodeLabel2);
    tmpAdjNode->next = node1->next;
    node1->next = tmpAdjNode;
  }
}

// Print the adjacency matrix of the graph
void printGraph(Graph* graph) {
 printf("\n----- Adjacency Matrix ------\n");
 for(int i=0; i<graph->size; i++) {
    printf("%s :",graph->nodes[i]->label);
    Node* node = graph->nodes[i]->next;
    while (node) {
      printf(" %s ", node->label);
      node=node->next;
    }
    printf("\n");
 }
 printf("-----------------------------\n");
}

// Recursive function to count possible paths between two nodes labeled as provided. Small caves can be visited once.
void numberOfPaths1Visit(Graph* graph, Graph* currentPath, char* startLabel, char* endLabel, int* numPaths) {

  // Check if the nodes has already been visited
  if (getNode(currentPath, startLabel)) {
    return;
  } else if (islower(startLabel[0])){
    // Mark the node as visited if starts with a lowercase
    addNode(currentPath, createNode(startLabel));
  }

  Node* currentNode = getNode(graph, startLabel);
  Node* neighbor = currentNode->next;

  while (neighbor != NULL) {
    if (!strncmp(neighbor->label, endLabel, MAX_LABEL_LENGTH)) {
      (*numPaths)++;
    } else {
      numberOfPaths1Visit(graph, currentPath, neighbor->label, endLabel, numPaths);
    }
    neighbor = neighbor->next;
  }

  // Unmark the node if it has been marked as visited
  if (islower(startLabel[0])) {
    free(currentPath->nodes[currentPath->size-1]);
    currentPath->size--;
  }
}

// Return  the number of occurrences in the graph of the provided label
int countOccurrences(Graph* graph, char* nodeLabel) {
  int n = 0;

  for (int i=0; i<graph->size; i++) {
    if (!strncmp(graph->nodes[i]->label, nodeLabel, MAX_LABEL_LENGTH))
      n++;
  }

  return n;
}

// Check if there is any duplicate in the graph
int anyDuplicate(Graph* graph) {

  for (int i=0; i<graph->size; i++) {
    if (countOccurrences(graph, graph->nodes[i]->label) > 1) {
      return 1;
    }
  }

  return 0;
}

// Recursive function to count possible paths between two nodes labeled as provided. Small caves can be visited twice.
void numberOfPaths1Visit2Visits(Graph* graph, Graph* currentPath, char* startLabel, char* endLabel, int* numPaths) {

  int occurrences = countOccurrences(currentPath, startLabel);
  // Check if the node can be visited again
  if ((occurrences > 1) || (occurrences == 1 && anyDuplicate(currentPath)==1 )) {
    return;
  } else if (islower(startLabel[0])){
    // Mark the node as visited if starts with a lowercase
    addNode(currentPath, createNode(startLabel));
  }

  Node* currentNode = getNode(graph, startLabel);
  Node* neighbor = currentNode->next;

  while (neighbor != NULL) {
    if (!strncmp(neighbor->label, endLabel, MAX_LABEL_LENGTH)) {
      (*numPaths)++;
    } else {
      numberOfPaths1Visit2Visits(graph, currentPath, neighbor->label, endLabel, numPaths);
    }
    neighbor = neighbor->next;
  }

  // Unmark the node if it has been marked as visited
  if (islower(startLabel[0])) {
    free(currentPath->nodes[currentPath->size-1]);
    currentPath->size--;
  }
}

// Read graph from input file
void readInput(Graph* graph) {
  FILE* fp = fopen(FILENAME, "r");

  if (!fp) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  struct stat sb;
  if (stat(FILENAME, &sb) == -1) {
    perror("stat");
    exit(EXIT_FAILURE);
  }

  char* file_contents = malloc(sb.st_size);

  printf("\n--------Input Edges----------\n");
  while (fscanf(fp, "%[^\n] ", file_contents) != EOF) {
    printf("%s\n", file_contents);
    char* nodeLabel1 = strtok(file_contents, "-");
    char* nodeLabel2 = strtok(NULL, "-");
    addEdge(graph, nodeLabel1, nodeLabel2);
  }
  printf("-----------------------------\n");

  fclose(fp);
}

// Main function
int main(int argc, char** argv) {
  Graph* graph = createGraph();
  Graph* path = createGraph();
  int numPaths = 0;

  readInput(graph);
  printGraph(graph);
  numberOfPaths1Visit(graph, path, START_LABEL, END_LABEL, &numPaths);
  
  printf("\nNumber of paths with 1 visit per small cave : %d\n", numPaths);

  numPaths = 0;
  numberOfPaths1Visit2Visits(graph, path, START_LABEL, END_LABEL, &numPaths);

  printf("\nNumber of paths with 2 visits per small cave : %d\n", numPaths);

  exit(EXIT_SUCCESS);
}
