<<<<<<< HEAD
/**************************************************************************************************
 * Implementation of the TAD Node
**************************************************************************************************/

#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED
#include "Edge.h" // Include of the Edge class

using namespace std;

// Definition of the Node class
class Node{

    // Attributes
    private:
        Edge* first_edge;
        Edge* last_edge;
        int id;
        unsigned int in_degree;
        unsigned int out_degree;
        int degree;
        float weight;
        Node* next_node;
        bool visited; // Atributo para definir se o nó foi visitado ou não.

    public:
        // Constructor
        Node(int id);
        // Destructor
        ~Node();
        // Getters
        Edge* getFirstEdge();
        Edge* getLastEdge();
        int getId();
        int getInDegree();
        int getOutDegree();
        int getDegree();
        float getWeight();
        Node* getNextNode();
        bool getVisited(); // Retorna o atributo visited;
        // Setters
        void setNextNode(Node* node);
        void setWeight(float weight);
        void setVisited(bool visited); // Define se o nó foi visitado ou não.
        // Other methods
        bool searchEdge(int target_id);
        void insertEdge(int target_id, float weight);
        void removeAllEdges();
        int removeEdge(int id, bool directed, Node* target_node);
        void incrementOutDegree();
        void decrementOutDegree();
        void incrementInDegree();
        void decrementInDegree();
        Edge* hasEdgeBetween(int target_id);
        // Auxiliar methods

};

#endif // NODE_H_INCLUDED
=======
/**************************************************************************************************
 * Implementation of the TAD Node
**************************************************************************************************/

#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED
#include "Edge.h" // Include of the Edge class

using namespace std;

// Definition of the Node class
class Node{

    // Attributes
    private:
        Edge* first_edge;
        Edge* last_edge;
        int id;
        unsigned int in_degree;
        unsigned int out_degree;
        int degree;
        float weight;
        Node* next_node;

    public:
        // Constructor
        Node(int id);
        // Destructor
        ~Node();
        // Getters
        Edge* getFirstEdge();
        Edge* getLastEdge();
        int getId();
        int getInDegree();
        int getOutDegree();
        int getDegree();
        float getWeight();
        Node* getNextNode();
        // Setters
        void setNextNode(Node* node);
        void setWeight(float weight);
        // Other methods
        bool searchEdge(int target_id);
        void insertEdge(int target_id, float weight);
        void removeAllEdges();
        int removeEdge(int id, bool directed, Node* target_node);
        void incrementOutDegree();
        void decrementOutDegree();
        void incrementInDegree();
        void decrementInDegree();
        Edge* hasEdgeBetween(int target_id);
        // Auxiliar methods

};

#endif // NODE_H_INCLUDED
>>>>>>> 9eaf78d17ac558ea14627becffe20b45252229e1
