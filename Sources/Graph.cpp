#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <list>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <set>
#include <vector>
#include <climits>
#include <math.h>
#include <numeric>
#include <float.h>
#include <string>
#include <sstream>
#include <time.h>
#include <ctime>
#include <chrono>

#include "Graph.h"
#include "Node.h"
#include "Edge.h"

int INF = 99999999;
int D = 3; // restrição de grau

using namespace std;

//**************************************************************************************************
//! * Defining the Graph's methods
//**************************************************************************************************/

//! Constructor
Graph::Graph(int order, bool directed, bool weighted_edge, bool weighted_node)
{
    this->order = order;
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;
    this->position = 0;
}

//! Destructor
Graph::~Graph()
{
    Node *next_node = this->first_node;

    while (next_node != nullptr)
    {
        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
}

//! Getters --------------------------------------------------------------------------------------------

int Graph::getOrder()
{
    return this->order;
}
int Graph::getNumberEdges()
{
    return this->number_edges;
}

//Function that verifies if the graph is directed
bool Graph::getDirected()
{
    return this->directed;
}

//Function that verifies if the graph is weighted at the edges
bool Graph::getWeightedEdge()
{
    return this->weighted_edge;
}

//Function that verifies if the graph is weighted at the nodes
bool Graph::getWeightedNode()
{
    return this->weighted_node;
}

Node *Graph::getFirstNode()
{
    return this->first_node;
}

Node *Graph::getLastNode()
{
    return this->last_node;
}

Node *Graph::getNode(int id)
{
    //cria ponteiro para percorrer a lista de nodes
    Node *node = first_node;

    //encontra o node com o id desejado
    while (node != nullptr)
    {
        if (node->getId() == id)
            return node;
        node = node->getNextNode();
    }

    //retorna o node ou null caso nao encontre
    return nullptr;
}

Node *Graph::getNodePosition(int position)
{
    //cria ponteiro para percorrer a lista de nodes
    Node *node = first_node;

    //encontra o node com o id desejado
    while (node != nullptr)
    {
        if (node->getPosition() == position)
            return node;

        node = node->getNextNode();
    }

    //retorna o node ou null caso nao encontre
    return nullptr;
}

// Função para gerar um Subgrafo Vértice Induzido
Graph *Graph::getVertInduz()
{
    cout << "\nVocê deseja rodar para o Grafo inteiro ou para um subgrafo?" << endl;
    cout << "1 - Grafo inteiro\n2 - Subgrafo\nSua opção: ";
    int opcao;
    cin >> opcao;

    if (opcao == 2)
    {
        cout << "\nDigite os IDs dos vértices que irão compor esse subgrafo separados por ponto-vírgula (Exemplo: 5;6;1;8):" << endl;

        // Lendo os vértices do subgrafo
        string aux;
        cout << "Vértices: ";
        cin >> aux;

        // Vector para armazenar os ids dos vértices do subgrafo
        vector<int> idvertices;
        idvertices.clear();

        // Separando a string
        stringstream ss(aux);
        while (getline(ss, aux, ';'))
        {
            if (this->searchNode(stoi(aux)))
                idvertices.push_back(stoi(aux));
            else
                cout << "O vértice " << aux << " é inválido, pois não está no Grafo" << endl;
        }

        // Criar o subgrafo vértice induzido
        Graph *subgrafo = new Graph(idvertices.size(), this->getDirected(), this->getWeightedEdge(), this->getWeightedNode());

        // Inserindo as arestas correspondentes no subgrafo
        this->cleanVisited();
        for (int i = 0; i < idvertices.size(); i++)
        {
            for (int j = i + 1; j < idvertices.size(); j++)

                // Verificar se a aresta realmente existe no grafo original
                if ((!this->getNode(idvertices[j])->getVisited()) && this->getNode(idvertices[i])->searchEdge(idvertices[j]))
                {
                    Edge *aux = this->getNode(idvertices[i])->getEdge(idvertices[j]);
                    subgrafo->insertEdge(idvertices[i], idvertices[j], aux->getWeight());
                }
                else
                    subgrafo->insertNode(idvertices[j]);

            this->getNode(idvertices[i])->setVisited(true);
        }

        cout << "\nO Subgrafo X foi gerado com sucesso! ";
        cout << "(Ordem = " << subgrafo->getOrder() << " e Num de Arestas = " << subgrafo->getNumberEdges() << ")" << endl;

        return subgrafo;
    }
    else
        return this;
}

//! Other methods --------------------------------------------------------------------------------------------
/*
    The outdegree attribute of nodes is used as a counter for the number of edges in the graph.
    This allows the correct updating of the numbers of edges in the graph being directed or not.
*/
void Graph::insertNode(int id)
{
    //confere se o grafo tem nodes
    if (first_node != nullptr)
    {
        //caso tenha, cria um novo node, aponta o ultimo pro novo e o novo de torna o ultimo
        Node *novo_node = new Node(id);
        novo_node->setPosition(this->position);
        last_node->setNextNode(novo_node);
        last_node = novo_node;
    }
    else
    {
        //caso nao tenha, cria um novo node e ele se torna o ultimo e o primeiro
        Node *novo_node = new Node(id);
        novo_node->setPosition(this->position);
        first_node = novo_node;
        last_node = novo_node;
    }
    this->position = this->position + 1;
}

void Graph::insertEdge(int id, int target_id, float weight)
{
    //cria um ponteiro para o node desejado e o um auxiliar para o node alvo da aresta
    Node *node = getNode(id);
    Node *aux = getNode(target_id);

    if (node == nullptr)
    {
        this->insertNode(id);
        node = last_node;
    }
    if (aux == nullptr)
    {
        this->insertNode(target_id);
        aux = last_node;
    }
    //confere se os nodes existem
    if (node != nullptr && aux != nullptr)
    {

        //confere se a aresta já existe
        if (!node->searchEdge(target_id))
        {
            //caso o node exista mas a aresta nao, insere a aresta
            node->insertEdge(target_id, aux->getPosition(), weight);

            this->number_edges++;

            // se o grafo for nao-direcionado e nao houver aresta de volta
            if (this->directed == 0 && !aux->searchEdge(id))
            {
                //insere a aresta de volta
                aux->insertEdge(id, node->getPosition(), weight);
            }
        }
    }
}

void Graph::removeNode(int id)
{
    //cria um ponteiro para o node desejado
    Node *node = getNode(id);

    //retorna caso nao exista o node desejado
    if (node == nullptr)
        return;
    else if (node == first_node) //se o node eh o primeiro, apenas faz o proximo ser o primeiro
        first_node = node->getNextNode();
    else
    {
        //caso seja um node do meio ou o ultimo, cria um ponteiro auxiliar
        Node *aux = first_node;

        //encontra o node anterior ao node desejado
        while (aux->getNextNode() != node)
            aux = aux->getNextNode();

        //se o no desejado for o ultimo, atualiza o ultimo para o anterior
        if (node == last_node)
            last_node = aux;

        //seta o proximo de anterior para o proximo do desejado
        aux->setNextNode(node->getNextNode());
    }

    //deleta o node desejado
    delete node;
}

bool Graph::searchNode(int id)
{
    //cria um ponteiro para o node desejado
    Node *node = getNode(id);

    //retorna falso caso nao encontre e verdadeiro caso encontre
    if (node == nullptr)
        return false;
    else
        return true;
}

void Graph::cleanVisited()
{
    /**
     * @brief Função para definir todos os nós do grafo como não visitados.
     * 
     */

    Node *node = this->getFirstNode(); // Ponteiro que armazena o endereço de memória do primeiro nó do grafo.

    // Realiza a operação para todos os nós do grafo.
    while (node != nullptr)
    {
        node->setVisited(false);    // Define o nó como não visitado.
        node = node->getNextNode(); // Ponteiro passa a apontar para o próximo nó do grafo.
    }
}

//! FUNÇÕES AUXILIARES --------------------------------------------------------------------------------------------

// Estrutura e funções auxiliares para o algoritmo de Kruskal
struct SubArvore
{
    int pai;
    int ordem;
};

// Função para encontrar em qual subárvore está o nó de id n. Usada no Kruskal
int qualSubArvore(SubArvore subarvores[], int n)
{
    if (subarvores[n].pai != n)
        subarvores[n].pai = qualSubArvore(subarvores, subarvores[n].pai);

    return subarvores[n].pai;
}

// Função para unir duas subárvores de dois nós u e v. Usada no Kruskal
void unirSubArvores(SubArvore subarvores[], int u, int v)
{
    // Encontrando os índices das subárvores
    int subU = qualSubArvore(subarvores, u);
    int subV = qualSubArvore(subarvores, v);

    // Unindo a menor com a maior
    if (subarvores[subU].ordem < subarvores[subV].ordem)
        subarvores[subU].pai = subV;
    else if (subarvores[subU].ordem > subarvores[subV].ordem)
        subarvores[subV].pai = subU;

    else
    {
        subarvores[subV].pai = subU;
        subarvores[subU].ordem += subarvores[subV].ordem;
    }
}

// Função para auxiliar o algoritmo de Prim. Retorna a posição do nó com menor custo de vizinhança que não esteja na agm
int posicaoMenor(vector<int> &custoViz, vector<bool> &naAGM)
{
    int min = INF;
    int pos;
    bool tem_pos = false;
    for (int i = 0; i < custoViz.size(); i++)
    {
        if (custoViz[i] < min && naAGM[i] == false)
        {
            min = custoViz[i];
            pos = i;
            tem_pos = true;
        }
    }
    if (tem_pos)
        return pos;
    else
    {
        for (int i = 0; i < custoViz.size(); i++)
        {
            if (custoViz[i] == min && naAGM[i] == false)
            {
                min = custoViz[i];
                pos = i;
                tem_pos = true;
                return pos;
            }
        }
    }
    return pos;
}

// Função para imprimir a AGM via Prim
void imprimirPrim(Graph *subgrafo, vector<int> &agm, ofstream &outFile)
{
    int peso = 0;
    cout << "\nÁRVORE GERADORA MÍNIMA via Prim\n"
         << endl;
    cout << "graph {" << endl;
    for (int i = 0; i < subgrafo->getOrder(); i++)
    {
        if (agm[i] != INF)
        {
            int id_destino = subgrafo->getNodePosition(i)->getId();
            if (agm[i] == id_destino)
                cout << "  " << agm[i] << endl;
            else
            {
                cout << "  " << agm[i] << " -- " << id_destino;
                cout << " [label = " << subgrafo->getNode(agm[i])->getEdge(id_destino)->getWeight() << "]" << endl;
                peso += subgrafo->getNode(agm[i])->getEdge(id_destino)->getWeight();
            }
        }
    }
    cout << "}" << endl;
    cout << "\nPeso da AGM: " << peso << endl;
    cout << "\nPrim concluído com sucesso!" << endl;

    cout << "\nDeseja imprimir essa AGM no arquivo de saída? (1 - Sim ou 0 - Não)" << endl;
    int op;
    do
    {
        cout << "Sua opção: ";
        cin >> op;
    } while (op != 1 && op != 0);

    if (op == 1)
    {
        outFile << "graph {" << endl;
        for (int i = 0; i < subgrafo->getOrder(); i++)
        {
            if (agm[i] != INF)
            {
                int id_destino = subgrafo->getNodePosition(i)->getId();
                if (agm[i] == id_destino)
                    outFile << "  " << agm[i] << endl;
                else
                {
                    outFile << "  " << agm[i] << " -- " << id_destino;
                    outFile << " [label = " << subgrafo->getNode(agm[i])->getEdge(id_destino)->getWeight() << "]" << endl;
                }
            }
        }
        outFile << "}" << endl;
        cout << "\nImpressão concluída! Cheque o arquivo de saída" << endl;
    }
}

// Função para imprimir a AGM via Kruskal
void imprimirKruskal(vector<pair<int, pair<int, int>>> &arestas, vector<int> &agm, ofstream &outFile)
{
    int peso = 0;
    cout << "\nÁRVORE GERADORA MÍNIMA via Kruskal\n"
         << endl;
    cout << "graph {" << endl;
    for (int i = 0; i < agm.size(); i++)
    {
        if (arestas[agm[i]].second.first == arestas[agm[i]].second.second)
            cout << "  " << arestas[agm[i]].second.first << endl;
        else
        {
            cout << "  " << arestas[agm[i]].second.first << " -- " << arestas[agm[i]].second.second;
            cout << " [label = " << arestas[agm[i]].first << "]" << endl;
            peso += arestas[agm[i]].first;
        }
    }
    cout << "}" << endl;
    cout << "\nPeso da AGM: " << peso << endl;
    cout << "\nKruskal concluído com sucesso!" << endl;

    cout << "\nDeseja imprimir essa AGM no arquivo de saída? (1 - Sim ou 0 - Não)" << endl;
    int op;
    do
    {
        cout << "Sua opção: ";
        cin >> op;
    } while (op != 1 && op != 0);

    if (op == 1)
    {
        outFile << "graph {" << endl;
        for (int i = 0; i < agm.size(); i++)
        {
            if (arestas[agm[i]].second.first == arestas[agm[i]].second.second)
                outFile << "  " << arestas[agm[i]].second.first << endl;
            else
            {
                outFile << "  " << arestas[agm[i]].second.first << " -- " << arestas[agm[i]].second.second;
                outFile << " [label = " << arestas[agm[i]].first << "]" << endl;
            }
        }
        outFile << "}" << endl;
        cout << "\nImpressão concluída! Cheque o arquivo de saída" << endl;
    }
}

// FUNCIONALIDADES --------------------------------------------------------------------------------------------

/**
     * @brief          Função para identificar e exibir o Fecho Transitivo Direto do grafo direcionado.
     * @param id       Valor que representa o id do nó para o qual será calculado o fecho transitivo direto.
     * @param outFile  Arquivo de saída em .dot.
     * 
     */
void Graph::directTransitiveClosing(int id, ofstream &outFile)
{

    Node *node = this->getNode(id); // Nó para o qual será calculado o fecho transitivo direto.

    this->cleanVisited(); // Chama a função para setar todos os nós do grafo como não visitados.

    // Verifica se o nó node existe.
    if (node != nullptr)
    {

        int entrada;

        cout << "Deseja salvar o resultado em um arquivo de saída? " << endl;
        cout << "Digite: \n1 - Sim\n2 - Não" << endl;
        cout << "Entrada: ";
        cin >> entrada;

        if (entrada == 1)
        {
            outFile << "digraph{\n";
        }

        cout << "Fecho transitivo direto: ";

        deepPath(node); // Realiza o caminho em profundidade no grafo a partir do nó node.

        // Imprime o id de todos os nós visitados.
        for (node = this->first_node; node != nullptr; node = node->getNextNode())
        {
            Edge *edge = node->getFirstEdge();

            if (node->getVisited())
            {
                cout << node->getId() << " | ";

                // Imprime no arquivo .dot a árvore do fecho transitivo direto.
                if (entrada == 1)
                {
                    while (edge != nullptr)
                    {

                        int id = edge->getTargetId();
                        outFile << node->getId() << "->" << edge->getTargetId() << ";\n";
                        edge = edge->getNextEdge();
                    }
                }
            }
        }

        if (entrada == 1)
        {
            outFile << "}\n";
        }
    }
    // Se node não existe, exibe uma mensagem de erro.
    else
    {
        cout << "Erro! Não existe um nó com o id proposto.";
    }
}

/**
     * @brief          Função para identificar e exibir o Fecho Transitivo Indireto do grafo direcionado.
     * @param id       Valor que representa o id do nó para o qual será calculado o fecho transitivo indireto.
     * @param outFile  Arquivo de saída em .dot.
     * 
     */
void Graph::indirectTransitiveClosing(int id, ofstream &outFile)
{

    Node *target = this->getNode(id);    // Nó alvo que recebe o id passado como parâmetro.
    Node *source = this->getFirstNode(); // Nó através do qual será feita a verificação se target é acessível.

    // Verifica se o nó target existe.
    if (target != nullptr)
    {

        int entrada;

        cout << "Deseja salvar o resultado em um arquivo de saída? " << endl;
        cout << "Digite: \n1 - Sim\n2 - Não" << endl;
        cout << "Entrada: ";
        cin >> entrada;

        if (entrada == 1)
        {
            outFile << "digraph{\n";
        }

        cout << "Fecho transitivo indireto: ";

        // Realiza a busca em profundidade para todos os nós do grafo.
        while (source != nullptr)
        {

            this->cleanVisited(); // Chama a função para setar todos os nós do grafo como não visitados.

            deepPath(source); // Realiza o caminho em profundidade no grafo a partir do nó source.

            // Se target foi visitado no caminho em profundidade, imprime o id de source.
            if (target->getVisited())
            {

                cout << source->getId() << " | ";

                if (entrada == 1)
                {
                    outFile << source->getId() << ";\n"; // Imprime no arquivo .dot o id do nó source.
                }
            }
            source = source->getNextNode();
        }

        if (entrada == 1)
        {
            outFile << "}\n";
        }
    }
    // Se target não existe, imprime uma mensagem de erro.
    else
    {
        cout << "Erro! Não existe um nó com o id proposto.";
    }
}

/**
 * @brief    Função para buscar o caminho minimo usando o algoritmo de dijkstra
 */
float Graph::dijkstra(ofstream &outFile)
{

    int idSource, idTarget;
    Node *noSource, *noTarget;
    string idS, idT;
    try
    { //Try para verificar se os parâmetros passados são IDs inteiros
        cout << "Digite o node Source" << endl;
        cin >> idS;
        cout << "Digite o node Target" << endl;
        cin >> idT;
        idSource = stoi(idS);
        idTarget = stoi(idT);
    }
    catch (const exception &e)
    {
        cout << "Parâmetros inválidos." << endl;
        return 0;
    }

    if (idSource == idTarget)
    {
        cout << "\n\nA distância é: " << 0 << endl;
        return 0;
    } //Encerá caso seja o mesmo no

    noSource = getNode(idSource); //Busca o no
    noTarget = getNode(idTarget); //Busca o no

    if (noSource != nullptr && noTarget != nullptr)
    {

        int pSource = noSource->getPosition(), pTarget = noTarget->getPosition(), distancia = INF, V = getOrder();
        int ver = 0, c_edge = 0, u;

        int *distance = new int[V];  //Vetor para os distâncias entre a posição do noSorce e os demais
        int *antec = new int[V];     //Vetor para os antecessores
        bool *visited = new bool[V]; //Vetor para as posições já visitadas
        for (int i = 0; i < V; i++)
        {
            distance[i] = INF;
            visited[i] = false;
        }                      //Inicializador dos vetores visitados e distância
        distance[pSource] = 0; //Distância do vertice para ele mesmo

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> fp; //Fila de prioridade para os pares distancia e vertice

        fp.push(make_pair(distance[pSource], pSource)); //Adiciona o par vetor distância e

        pair<int, int> p = fp.top(); //Adiciona o p na fila de prioridade

        Node *node = nullptr;
        Edge *edge = nullptr;

        while (!fp.empty())
        {

            pair<int, int> p = fp.top(); //Pega o do topo
            u = p.second;                //Obtem o vértice
            fp.pop();                    //Remove da lista de prioridade
            if (visited[u] == false)
            {
                visited[u] = true; //Marca o vertice como visitado
                node = getNodePosition(u);
                if (node != nullptr) //Busca o no pela posição
                    edge = node->getFirstEdge();
                else
                    edge = nullptr; //Pega a primeira aresta do no

                while (edge != nullptr)
                { //Passa por todas as arestas do vertice u

                    if (!getWeightedEdge())
                        c_edge = 1; //Para caso não haja pesso a distância será 1 por salto
                    else
                        c_edge = edge->getWeight();

                    ver = edge->getTargetPosition(); //Pega a posição do no Target dessa aresta

                    if (distance[ver] > (distance[u] + c_edge))
                    {                                           //Verifica se a distância é menor
                        antec[ver] = u;                         //Atualiza o antecessor
                        distance[ver] = (distance[u] + c_edge); //Atualiza a distância
                        fp.push(make_pair(distance[ver], ver)); //Adiciona o vertice na fila de prioridade
                    }
                    edge = edge->getNextEdge(); //Avança para o a proxima aresta do vertice
                }
            }
        }

        distancia = distance[pTarget];

        delete[] distance; //Desalocando o vetore usado
        delete[] visited;  //Desalocando o vetore usado

        if (distancia < INF)
            saidaDijkstra(antec, pSource, pTarget, outFile); //Imprime todo a lista na ordem de acesso

        delete[] antec;
        cout << "\n\nA distância é: " << distancia << endl;
        return distancia;
    }
    else
    {

        if (noSource == nullptr)
            cout << "Source node não existe nesse grafo" << endl;
        if (noTarget == nullptr)
            cout << "Target node não existe nesse grafo" << endl;
        return -1;
    }
}

/**
 * @brief Função de busca de caminho minimo usando o algoritmo de Floyd-Warshall
 * 
 * @param outFile 
 * @return float 
 */
float Graph::floydWarshall(ofstream &outFile)
{
    int idSource, idTarget;
    Node *noSource, *noTarget;
    string idS, idT;
    try
    { //Try para verificar se os parâmetros passados são IDs inteiros
        cout << "Digite o node Source" << endl;
        cin >> idS;
        cout << "Digite o node Target" << endl;
        cin >> idT;
        idSource = stoi(idS);
        idTarget = stoi(idT);
    }
    catch (const exception &e)
    {
        cout << "Parâmetros inválidos" << endl;
        return 0;
    }

    if (idSource == idTarget)
    {
        cout << "\n\nA distância é: " << 0 << endl;
        return 0;
    }

    noSource = getNode(idSource); //Busca o no Sorce
    noTarget = getNode(idTarget); //Busca o no Target

    if (noSource != nullptr && noTarget != nullptr)
    {

        int V = getOrder(), i, j, k, distancia;
        int **dist, **pred;

        dist = iniciaDistanciaFloyd(dist, V);  //Inicia a matriz de distância
        pred = iniciaAnterioresFloyd(pred, V); //Inicia a matriz de anteriores

        for (k = 0; k < V; k++) //Calculando a distância de todas as posições
            for (i = 0; i < V; i++)
                for (j = 0; j < V; j++)
                    if (dist[i][j] > (dist[i][k] + dist[k][j]))
                    {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        pred[i][j] = pred[k][j]; //Atualiza a posição do no antercessor dos nos na posição i e j
                    }
        distancia = dist[noSource->getPosition()][noTarget->getPosition()]; //Armazena a distância minima entre os dois nós

        for (i = 0; i < V; i++)
        {
            delete[] dist[i];
        }
        delete[] dist; //Desalocando a matriz distancia usada

        if (distancia < INF)
            saidaFloyd(pred, noSource, noTarget, outFile);

        for (i = 0; i < V; i++)
        {
            delete[] pred[i];
        } //Desalocando a matriz de antecessores
        delete[] pred;

        cout << "\n\nDistância é: " << distancia << endl;
        return distancia;
    }
    else
    {

        if (noSource == nullptr)
            cout << "Source node não existe nesse grafo" << endl;
        if (noTarget == nullptr)
            cout << "Target node não existe nesse grafo" << endl;
        return -1;
    }
}

/**
 * @brief ALGORITMO DE PRIM para encontrar a Árvore Geradora Mínima
 * 
 * @param subgrafo 
 * @param outFile 
 */
void Graph::agmPrim(Graph *subgrafo, ofstream &outFile)
{
    cout << "\nIniciando a execução do algoritmo de Prim..." << endl;

    // 1º PASSO: Organizar os custos das vizinhanças dos nós em um vector

    // Vector para armazenar os custoViz dos nós do subgrafo. O índice no vector é compatível com a posição do nó no subgrafo
    vector<int> custoViz;
    custoViz.clear();

    // Vector para checar se o nó já foi inserido na agm
    vector<bool> naAGM(subgrafo->getOrder(), false);

    // O primeiro nó do vector será inicializado com custoViz = 0
    custoViz.push_back(0);

    // Os demais nós serão inicializados com custoViz = INFINITO
    for (int i = 1; i < subgrafo->getOrder(); i++)
        custoViz.push_back(INF);

    cout << "1º passo concluído com sucesso" << endl;

    // 2º PASSO: Criar Arvore Geradora Minima -> vetor com os pais de cada nó da agm ou INF caso nao tenha pai

    // Os índices da agm corresponderão à posição do nó no subgrafo
    // A raiz da agm, indice 0, será o primeiro nó do subgrafo, portanto não terá pai
    vector<int> agm(subgrafo->getOrder(), INF);

    cout << "2º passo concluído com sucesso" << endl;

    // 3º PASSO: Iterar pelos vértices verificando o custoViz e inserindo na agm

    int cont = 0;
    while (cont < subgrafo->getOrder())
    {
        // Pega o nó com menor custoViz que ainda não está na agm
        int pos_menor = posicaoMenor(custoViz, naAGM);         // Posição do nó
        int u = subgrafo->getNodePosition(pos_menor)->getId(); // ID do nó
        // Atualiza naAGM, pois, nessa iteração, u será colocado na agm
        naAGM[pos_menor] = true;

        // Iterar pelos nós v adjacentes a u e verificar se o peso da aresta entre eles é menor que o seu custoViz
        Edge *aux = subgrafo->getNode(u)->getFirstEdge();
        if (aux == nullptr) // nó não tem arestas
            agm[pos_menor] = u;
        else
        {
            while (aux != nullptr)
            {
                int v = aux->getTargetId();                      // ID de v
                int pos_v = subgrafo->getNode(v)->getPosition(); // posição de v
                if (!naAGM[pos_v])                               // executa caso o nó v ainda não esteja na agm
                {
                    // Se o peso da aresta (u, v) for menor que o custoViz de v, atualiza o custoViz com o valor do peso
                    if (aux->getWeight() < custoViz[pos_v])
                    {
                        custoViz[pos_v] = aux->getWeight();
                        // Atualiza o pai de v na agm
                        agm[pos_v] = u;
                    }
                }
                aux = aux->getNextEdge();
            }
        }
        cont++;
    }

    cout << "3º passo concluído com sucesso" << endl;

    // 4º PASSO: Imprimir a Árvore Geradora Mínima e seu peso

    imprimirPrim(subgrafo, agm, outFile);

    return;
}

// ALGORITMO DE KRUSKAL
// para encontrar a Árvore Geradora Mínima
void Graph::agmKruskal(Graph *subgrafo, ofstream &outFile)
{
    cout << "\nIniciando a execução do algoritmo de Kruskal..." << endl;

    // 1º PASSO: Vector para armazenar as arestas do grafo

    vector<pair<int, pair<int, int>>> arestas; //vector<peso, noOrigem, noDestino>
    arestas.clear();

    subgrafo->cleanVisited();
    Node *noAux = subgrafo->getFirstNode();
    Edge *arestaAux = noAux->getFirstEdge();

    int u = noAux->getId(); // id do nó de origem
    int v;

    if (arestaAux != nullptr)
        v = arestaAux->getTargetId(); //id do nó destino

    // Percorrer todas as arestas do Grafo
    for (int i = 1; i < subgrafo->getOrder(); i++)
    {
        if (arestaAux == nullptr)
            arestas.push_back({INF, {u, u}});

        while (arestaAux != nullptr)
        {
            // Coloca a aresta no vetor de arestas
            if (!subgrafo->getNode(v)->getVisited())
                arestas.push_back({arestaAux->getWeight(), {u, v}});

            // Atualiza os auxiliares se a aresta não for null
            arestaAux = arestaAux->getNextEdge();
            if (arestaAux != nullptr)
            {
                v = arestaAux->getTargetId();
            }
        }

        noAux->setVisited(true);
        noAux = subgrafo->getNodePosition(i);
        arestaAux = noAux->getFirstEdge();
        u = noAux->getId();
        if (arestaAux != nullptr)
            v = arestaAux->getTargetId();
    }

    cout << "1º passo concluído com sucesso" << endl;

    // 2º PASSO: Ordenar as arestas por peso do menor para o maior

    sort(arestas.begin(), arestas.end());
    cout << "2º passo concluído com sucesso" << endl;

    // 3º PASSO: Criar subávores cada uma contendo um nó isolado

    int V = subgrafo->getOrder();
    SubArvore *subarvores = new SubArvore[(V * sizeof(SubArvore))]; // vetor para armazenar todas as subárvores

    for (int i = 0; i < V; i++)
    {
        subarvores[i].pai = i;
        subarvores[i].ordem = 1;
    }
    cout << "3º passo concluído com sucesso" << endl;

    // 4º PASSO: Montar a Árvore Geradora Mínima

    vector<int> agm; // vetor com o índice associado à posição de cada aresta da árvore geradora mínima no vector 'arestas' do subgrafo
    agm.clear();

    // Iterar até atingir condição de parada
    int cont = 0;
    while (agm.size() < V - 1 && cont < arestas.size())
    {
        pair<int, int> proxAresta = arestas[cont].second;
        int u = proxAresta.first;
        int v = proxAresta.second;

        if (u == v)
            agm.push_back(cont);

        // Se u e v não estão na mesma subárvore
        if (qualSubArvore(subarvores, subgrafo->getNode(u)->getPosition()) != qualSubArvore(subarvores, subgrafo->getNode(v)->getPosition()))
        {
            agm.push_back(cont);
            unirSubArvores(subarvores, subgrafo->getNode(u)->getPosition(), subgrafo->getNode(v)->getPosition());
        }
        cont++;
    }
    cout << "4º passo concluído com sucesso" << endl;

    // 5º PASSO: Imprimir a Árvore Geradora Mínima e seu peso

    imprimirKruskal(arestas, agm, outFile);

    delete[] subarvores;
    return;
}

/**
 * @brief Caminhamento Profundidade destacando as Arestas de retorno
 * 
 * @param outFile Arquivo paraa getação de saida gráfica
 */
void Graph::deepSearch(ofstream &outFile)
{
    // variáveis
    vector<int> retorno;
    vector<int> findG;
    vector<string> graph;
    int id;
    char r;

    cout << "\nCaminhamento Profundidade destacando as Arestas de retorno\n\n";
    do
    {
        cout << "Informe o numero no nó: ";
        cin >> id;
    } while (!this->searchNode(id));

    Node *node = this->getNode(id);
    auxDeepSearch(node, &findG, &retorno, outFile, &graph); //chama a função auxiliar

    cout << "\n -- Árvore em Profundidade -- \n";
    for (int i = 0; i < findG.size(); i++)
        cout << findG[i] << " | ";

    cout << "\n\n -- Arestas de Retorno -- \n";
    for (int i = 0; i < retorno.size(); i++)
        cout << retorno[i] << " | ";
    cout << endl;

    do
    {
        cout << "Gerar Grafo e arquivo de saída(s/n)?\n";
        cin >> r;
    } while (r != 's' && r != 'S' && r != 'n' && r != 'n');

    if (r == 's' || r == 'S')
        printDeepSearch(&graph, outFile);
    retorno.clear();
    findG.clear();
    graph.clear();
}

void Graph::printDeepSearch(vector<string> *graph, ofstream &outFile)
{
    ofstream output;
    output.open("output.dot", ios::out | ios::trunc);
    output << "graph{\n";
    outFile << "graph{\n";
    for (int i = 0; i < graph->size(); i++)
    {
        output << graph->at(i) << endl;
        outFile << graph->at(i) << endl;
    }
    output << "}";
    outFile << "}";
    output.close();
    system("dot -Tpng output.dot -o output.png");
    //outFile.close();
}

/**
 * @brief function that prints a topological sorting 
 * A função para fazer a classificação topológica.
 * Ele usa topologicalSortUtil () recursivamente.
 */
void Graph::topologicalSorting()
{
    cout << "\nOrdenação topologica em Grafo Acíclico Direcionado - DAG\n"
         << endl;

    if (!this->getDirected())
    {
        cout << "Ordenacao Topologica impossivel: Grafo nao-direcionado." << endl;
        return;
    }
    else if (this->thisIsCyclic())
    {
        cout << "Ordenacao Topologica impossivel: Grafo não acíclico." << endl;
        return;
    }
    else
    {
        stack<int> Stack;
        Node *node = this->first_node;
        Edge *edge = node->getFirstEdge();
        this->cleanVisited(); // Marca todos os vértices como não visitados

        while (node != nullptr)
        {
            if (!node->getVisited())
                topologicalSortUtil(node, edge, Stack);

            node = node->getNextNode();
        }

        cout << "Ordenação: ";
        while (!Stack.empty())
        {
            cout << Stack.top() << "  ";
            Stack.pop();
        }
        cout << "\n\n";
    }
}

//! AUXILIAR METHODS --------------------------------------------------------------------------------------------

/**
 *  @brief         Função genérica para realizar o caminho em profundidade a partir de um nó.
 *  @param node    Nó através do qual será feito o caminho em profundidade.
 *  
 */
void Graph::deepPath(Node *node)
{

    node->setVisited(true); // Define o nó node como visitado.

    // Operação recursiva para percorrer todos os nós acessíveis a partir de node.
    // Percorre todas as arestas do nó.
    for (Edge *edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
    {

        // Verifica se o nó para o qual a aresta aponta foi visitado.
        // Caso não tenha sido, deepPath é chamado novamente passando como parâmetro o nó alvo da aresta.
        if (!getNode(edge->getTargetId())->getVisited())
        {

            deepPath(getNode(edge->getTargetId()));
        }
    }
}

/**
 * @brief Auxiliar da busca em profundidade de um Nó dado
 * 
 * @param node      Ponteiro para o nó
 * @param findG     vetor para os vértices do caminho
 * @param retorno   vetor para as arestas de retorno
 * @param outFile   arquivo texto de saída
 * @param graf      vetor para gravação dos dados impressos no arquivo de saída
 */
void Graph::auxDeepSearch(Node *node, vector<int> *findG, vector<int> *retorno, ofstream &outFile, vector<string> *graf)
{

    findG->push_back(node->getId());
    node->setVisited(true);
    for (Edge *edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
    {
        if (!getNode(edge->getTargetId())->getVisited())
        {
            graf->push_back(to_string(node->getId()) + "--" + to_string(edge->getTargetId()));
            auxDeepSearch(getNode(edge->getTargetId()), findG, retorno, outFile, graf);
        }
    }
    retorno->push_back(node->getId());
}

/**
 * @brief               Função auxiliar para as saidas dos algoritmos de caminho minimo, imprimindo e salvando no arquivo
 * @param   antecessor  Lista contendo os antecessores do caminho minimo, representa a ordem de acesso
 * @param   outFile     Arquivo de saída
 */
void Graph::caminhoMinimo(list<int> &antecessor, ofstream &outFile)
{
    string arco, entrada;                            //Para a escrita no outFile, se for arco '->' se for aresta '--'
    int primeiro = antecessor.front(), tNode, sNode; //Usado para armazenar o primeiro vertice, e auxiliar na escrita no arquivo dot

    cout << "\nDeseja salvar a saída?\n1 para sim.\nQualquer outra opção para não" << endl;
    cout << "Sua opção: ";
    cin >> entrada;
    if (getDirected())
    {
        if (entrada == "1")
            outFile << "\ndigraph{ \n";
        arco = " -> ";
    }
    else
    {
        if (entrada == "1")
            outFile << "\ngraph{ \n";
        arco = " -- ";
    }
    Node *no = getNodePosition(primeiro);
    sNode = no->getId();
    Node *noAux = nullptr; //No auxiliar
    Edge *edge = nullptr;  //Edge auxiliar para pegar o peso
    cout << "\nCAMINHO MINIMO\n"
         << endl;
    while (!antecessor.empty())
    {                                   //Passa por toda a lista de ordem de acesso em buscando o ID
        no = getNodePosition(primeiro); //no recebe o node que é o primeiro no caminho minimo
        tNode = no->getId();

        if (tNode != sNode && entrada == "1") //Para escrever no arquivo dot o caminho, caso possua peso busca esse peso
            if (getWeightedEdge())
            {
                edge = noAux->getEdge(tNode); //Busca a aresta para pegar o seu peso
                outFile << sNode << arco << tNode << "[label=" << edge->getWeight() << "]\n";
            }
            else
                outFile << sNode << arco << tNode << "\n";

        sNode = tNode; //Atualiza o valor do sNode

        cout << no->getId() << arco;   //Imprime para o usuário
        antecessor.pop_front();        //Remove o primeiro elemento da lista dos antecessor
        primeiro = antecessor.front(); //Atualiza o valor do primeiro
        noAux = no;                    //Atualiza o valor do noAux
    }
    if (entrada == "1")
        outFile << "}";
}

/**
 * @brief               Função auxiliar de Floy, para iniciar a matriz dos anteriores
 * @param anteriores    Matriz dos anteriores que será inicializada
 * @param tam           Tamanho da matriz quadratica, recebe um valor i para cada posição
 * 
 */
int **Graph::iniciaAnterioresFloyd(int **anteriores, int tam)
{
    anteriores = new int *[tam]; //Aloca a matriz dos anteriores
    for (int i = 0; i < tam; i++)
        anteriores[i] = new int[tam]; //Aloca cada posição da matriz anteriores
    for (int i = 0; i < tam; i++)
        for (int j = 0; j < tam; j++)
            anteriores[i][j] = i; //Coloca um valor em cada posição da matriz anteriores

    return anteriores; //Retorna a matriz anteriores
}

/**
 * @brief               Função auxiliar de Floyd, para iniciar a posição da matriz de distância contendo os pesos
 * @param   distancia   Vetor de distância a ser inicializado
 * @param   tam         Tamanho da matriz quadratica, recebe a ordem do grafo
 * 
 */
int **Graph::iniciaDistanciaFloyd(int **distancia, int tam)
{
    distancia = new int *[tam]; //Alocando a matriz distância
    for (int i = 0; i < tam; i++)
        distancia[i] = new int[tam]; //Aloca cada posição da matriz distância
    for (int i = 0; i < tam; i++)
        for (int j = 0; j < tam; j++)
            distancia[i][j] = INF; //Atribuindo a distância de infinito para todas a posições
    for (int i = 0; i < tam; i++)
        distancia[i][i] = 0; //Atribuindo a distância de 0 do vertice para ele mesmo

    Node *node = nullptr;
    Edge *edge = nullptr;

    for (int i = 0; i < tam; i++)
    { //Colocando o peso das arestas na matriz, caso o grafo não seja ponderado o valor 1 será atribuido para cada salto
        node = getNodePosition(i);
        edge = node->getFirstEdge();

        while (edge != nullptr)
        {

            if (!getWeightedEdge())
                distancia[i][edge->getTargetPosition()] = 1; //Adiciona 1 para cada salto caso o grafo não seja ponderado
            else
                distancia[i][edge->getTargetPosition()] = edge->getWeight(); //Adiciona o peso nessa posição da matriz
            edge = edge->getNextEdge();                                      //Avança a aresta
        }
    }
    return distancia; //Retorna a matriz distância
}

/**
 * @brief               Função auxiliar de floyd para imprimir e salvar em arquivo dot o caminho mínimo
 * @param   antecessor  Lista contendo os antecessores do caminho minimo, representa a ordem de acesso
 */
void Graph::saidaFloyd(int **pred, Node *noSource, Node *noTarget, ofstream &outFile)
{

    list<int> ordemAcesso; //Lista para conteer a ordem de acesso dos vertices, de trás para frente
    int ant;

    ordemAcesso.push_front(noTarget->getPosition());              //Adiciona a posição do no Target na filha
    ant = pred[noSource->getPosition()][noTarget->getPosition()]; //Pega a possição do antercessor ao no Source e Target

    while (ant != noSource->getPosition())
    { //Loop que adciona na lista todas as posições dos nos do menor caminho
        ordemAcesso.push_front(ant);
        ant = pred[noSource->getPosition()][ant];
    }
    ordemAcesso.push_front(noSource->getPosition());

    caminhoMinimo(ordemAcesso, outFile);
}

/**
 * @brief               Função auxiliar de disjkstra para imprimir o caminho
 * @param   antecessor  Vetor contendo o antecessor de cada posição
 * @param   idSource    ID do nó de origem
 * @param   idTarget    ID do nó de destino   
 */
void Graph::saidaDijkstra(int antecessor[], int idSource, int idTarget, ofstream &outFile)
{

    string arco;
    int noAnterior, primeiro, tNode, sNode; //Usado para armazenar o vertice anterior, e auxiliar na escrita no arquivo dot

    list<int> ordemAcesso; //Lista contendo a ordem de acesso dos vertices

    ordemAcesso.push_front(idTarget);  //Armazena na lista na ordem de acesso dos vertices,
    noAnterior = antecessor[idTarget]; //apartir de seus anteriores, começando pelo nó target

    while (noAnterior != idSource)
    {
        ordemAcesso.push_front(noAnterior);
        noAnterior = antecessor[noAnterior];
    }
    ordemAcesso.push_front(idSource); //Insere o nó Source como o primeiro a ser acessado
    primeiro = ordemAcesso.front();

    caminhoMinimo(ordemAcesso, outFile);
}

/**
 * @brief Verifica se o grafo é ciclico ou se é cíclico e direcionado
 * 
 * @return true 
 * @return false 
 */
bool Graph::thisIsCyclic()
{
    if (this->directed)
        return this->isCyclicDirected();
    else
        return this->isCyclic();
}

/**
 * @brief Função auxiliar para verificação se o grafo é cíclico
 * 
 * @param nodeId 
 * @param isVisited 
 * @param parentId 
 * @return true 
 * @return false 
 */
bool Graph::auxIsCyclic(int nodeId, bool isVisited[], int parentId)
{
    isVisited[nodeId] = true;
    Node *node = getNode(nodeId);
    Edge *edge = node->getFirstEdge();

    while (edge != nullptr)
    {
        if (!isVisited[edge->getTargetId()])
        {
            if (auxIsCyclic(edge->getTargetId(), isVisited, nodeId))
            {
                cout << edge->getTargetId() << " - ";
                return true;
            }
        }
        else if (edge->getTargetId() != parentId)
        {
            return true;
        }

        edge = edge->getNextEdge();
    }

    return false;
}
/**
 * @brief Função auxiliar para verificação se o graph é cíclico 
 * 
 * @return true 
 * @return false 
 */
bool Graph::isCyclic()
{
    int order = this->getOrder();

    bool *isVisited = new bool[order];

    for (int i = 0; i < order; i++)
    {
        isVisited[i] = false;
    }

    Node *node = first_node;
    while (node != nullptr)
    {
        if (!isVisited[node->getId()])
        {
            if (auxIsCyclic(node->getId(), isVisited, -1))
            {
                return true;
            }
        }

        node = node->getNextNode();
    }

    return false;
}
/**
 * @brief Função auxiliar para verificação se o graph é cíclico e direcionado
 * 
 * @param nodeId 
 * @param isVisited 
 * @param isContainedRecusirve 
 * @return true 
 * @return false 
 */
bool Graph::auxIsCyclicDirected(int nodeId, bool isVisited[], bool *isContainedRecusirve)
{
    Node *node = this->getNode(nodeId);
    Edge *edge = node->getFirstEdge();

    if (!isVisited[nodeId])
    {
        isVisited[nodeId] = true;
        isContainedRecusirve[nodeId] = true;

        while (edge != nullptr)
        {
            if (!isVisited[edge->getTargetId()] && auxIsCyclicDirected(edge->getTargetId(), isVisited, isContainedRecusirve))
            {
                return true;
            }
            else if (isContainedRecusirve[edge->getTargetId()])
            {
                return true;
            }

            edge = edge->getNextEdge();
        }
    }

    isContainedRecusirve[nodeId] = false;
    return false;
}
/** 
 * @brief Função boleana que verifica se o graph é ciclíco
 * 
 * @return true 
 * @return false 
 */
bool Graph::isCyclicDirected()
{
    bool *isVisited = new bool[this->getOrder()];
    bool *isContainedRecusirve = new bool[this->getOrder()];
    Node *node = first_node;

    for (int i = 0; i < this->getOrder(); i++)
    {
        isVisited[i] = false; // Verifica se o nó atual foi visitado.
        isContainedRecusirve[i] = false;
    }

    while (node != nullptr)
    {
        if (auxIsCyclicDirected(node->getId(), isVisited, isContainedRecusirve))
        {
            return true;
        }

        node = node->getNextNode();
    }

    return false;
}

/**
 * @brief Função recursiva usada por topologicalSort
 * 
 * @param node      ponteiro para o No
 * @param edge      ponterio para a Aresta
 * @param Stack     Pilha de Nós
 */
void Graph::topologicalSortUtil(Node *node, Edge *edge, stack<int> &Stack)
{
    if (node != nullptr)
    {
        node->setVisited(true); // Marca o nó atual como visitado.
        Node *auxNode;          // cria nó axiliar
        edge = node->getFirstEdge();

        while (edge != nullptr)
        {
            auxNode = this->getNode(edge->getTargetId()); // Aponta o no auxiliar para edge
            if (!auxNode->getVisited())
                topologicalSortUtil(auxNode, edge, Stack); // recursivo para todos o vértices adjacentes a ele

            edge = edge->getNextEdge();
        }

        if (node != nullptr) //Empilha o vértice atual
            Stack.push(node->getId());
    }
}

// 2a PARTE DO TRABALHO  ------------------------------------------------------------------------------------------------

bool maior(pair<int, int> i, pair<int, int> j)
{
    return (i.first > j.first);
}

int acha(vector<pair<int, int>> &vet, pair<int, int> val)
{
    int i = 0;
    for (i = 0; i < vet.size(); i++)
    {
        if (vet[i].first == val.first && vet[i].second == val.second)
            return i;
    }
    return INF;
}

/* A função ordenaArestas recebe como parâmetro uma matriz e uma opção. Na matriz,
    o vector de fora representa cada nó e, para cada nó, há um vector de pares, em que
    cada par representa uma aresta daquele nó. O par tem o formato {peso, posicao_notarget}.
    A função ordena cada uma dessas arestas de cada nó. Se op = 0, ordem crescente. Se op = 1, descrescente*/
void ordenaArestas(vector<vector<pair<int, int>>> &matriz, int op)
{
    for (int i = 0; i < matriz.size(); i++)
    {
        if (op == 0)
            sort(matriz[i].begin(), matriz[i].end());
        else if (op == 1)
            sort(matriz[i].begin(), matriz[i].end(), maior);
    }
}

/* A função ordenaArestas recebe como parâmetro uma matriz em que o vector de fora
    representa cada nó e, para cada nó, há um vector de pares, em que
    cada par representa uma aresta daquele nó. O par tem o formato {peso, posicao_notarget}.
    A função retorna essa matriz preenchida*/
void preencheMatriz(Graph *grafo, vector<vector<pair<int, int>>> &matriz)
{
    grafo->cleanVisited();
    Node *noAux = grafo->getFirstNode();
    Edge *arestaAux = noAux->getFirstEdge();

    int pos_u = noAux->getPosition(); // id do nó de origem
    int v;

    if (arestaAux != nullptr)
        v = arestaAux->getTargetId(); //id do nó destino

    // Percorrer todas as arestas do Grafo
    for (int i = 1; i < grafo->getOrder(); i++)
    {
        if (arestaAux == nullptr)
            matriz[pos_u].push_back({INF, pos_u});

        while (arestaAux != nullptr)
        {
            // Coloca a aresta na matriz de acordo com o nó de origem correspondente
            if (!grafo->getNode(v)->getVisited())
            {
                int pos_v = grafo->getNode(v)->getPosition();
                matriz[pos_u].push_back({arestaAux->getWeight(), pos_v});
            }

            // Atualiza os auxiliares se a aresta não for null
            arestaAux = arestaAux->getNextEdge();
            if (arestaAux != nullptr)
                v = arestaAux->getTargetId();
        }

        noAux->setVisited(true);
        noAux = grafo->getNodePosition(i);
        arestaAux = noAux->getFirstEdge();
        pos_u = noAux->getPosition();
        if (arestaAux != nullptr)
            v = arestaAux->getTargetId();
    }
}

void primAdaptado(Graph *grafo, vector<vector<pair<int, int>>> &matriz, vector<vector<pair<int, int>>> &matrizAGM, vector<pair<int, int>> &pesoTotalNo, vector<bool> &dentroRestricao)
{
    // Vector para armazenar os custoViz dos nós do subgrafo. O índice no vector é compatível com a posição do nó no subgrafo
    vector<int> custoViz;
    custoViz.clear();

    // Vector para checar se o nó já foi inserido na agm
    vector<bool> naAGM(grafo->getOrder(), false);

    // O primeiro nó do vector será inicializado com custoViz = 0
    custoViz.push_back(0);

    // Os demais nós serão inicializados com custoViz = INFINITO
    for (int i = 1; i < grafo->getOrder(); i++)
        custoViz.push_back(INF);

    vector<int> agm(grafo->getOrder(), INF);

    int cont = 0;
    while (cont < grafo->getOrder())
    {
        // Pega o nó com menor custoViz que ainda não está na agm
        int pos_u = posicaoMenor(custoViz, naAGM);      // Posição do nó
        int u = grafo->getNodePosition(pos_u)->getId(); // ID do nó
        // Atualiza naAGM, pois, nessa iteração, u será colocado na agm
        naAGM[pos_u] = true;

        // Iterar pelos nós v adjacentes a u e verificar se o peso da aresta entre eles é menor que o seu custoViz
        Edge *aux = grafo->getNode(u)->getFirstEdge();

        while (aux != nullptr)
        {
            int v = aux->getTargetId();                   // ID de v
            int pos_v = grafo->getNode(v)->getPosition(); // posição de v
            if (!naAGM[pos_v])                            // executa caso o nó v ainda não esteja na agm
            {
                // Se o peso da aresta (u, v) for menor que o custoViz de v, atualiza o custoViz com o valor do peso
                if (aux->getWeight() < custoViz[pos_v])
                {
                    custoViz[pos_v] = aux->getWeight();

                    // Atualiza o pai de v na agm
                    agm[pos_v] = u;
                }
            }
            aux = aux->getNextEdge();
        }
        cont++;
    }

    // Atualiza os parametros matriz, matrizAGM, pesoTotalNo, dentroRestricao
    for (int i = 0; i < agm.size(); i++)
    {
        //cout << "i = " << i << " tem-se: {" << matriz[i][0].first << ", " << matriz[i][0].second << "}" << endl;
        if (agm[i] != INF)
        {
            int pos_u = grafo->getNode(agm[i])->getPosition();
            int aux = grafo->getNodePosition(i)->getEdge(agm[i])->getWeight();

            int grau;

            pair<int, int> aresta(aux, pos_u);
            //cout << "aresta: {" << aresta.first << ", " << aresta.second << endl;
            int it = acha(matriz[i], aresta);

            if (it != INF)
            {
                //cout << " . ";
                matrizAGM[i].push_back(aresta);          // insere a aresta na matrizAGM
                matriz[i].erase(matriz[i].begin() + it); // apaga da matriz
            }
            else
            {
                aresta = {aux, i};
                it = acha(matriz[pos_u], aresta);
                if (it != INF)
                {
                    //cout << " ! ";
                    matrizAGM[pos_u].push_back(aresta);              // insere a aresta na matrizAGM
                    matriz[pos_u].erase(matriz[pos_u].begin() + it); // apaga da matriz
                }
            }

            pesoTotalNo[i].second ++;
            pesoTotalNo[pos_u].second ++;
            if (pesoTotalNo[i].second > D)
            {
                dentroRestricao[i] = false;
                cout << "\nID fora: " << grafo->getNodePosition(i)->getId() << endl;
                cout << "index fora: " << i << endl;
            }
            if(pesoTotalNo[pos_u].second > D)
            {
                dentroRestricao[pos_u] = false;
                cout << "\nID fora: " << grafo->getNodePosition(pos_u)->getId() << endl;
                cout << "index fora: " << pos_u << endl;
            }    
            pesoTotalNo[i].first += aux;
            pesoTotalNo[pos_u].first += aux;
        }
    }
    return;
}

int pesoTotalAGM(vector<pair<int, int>> &pesoNoTotal)
{
    int peso = 0;
    for (int i = 0; i < pesoNoTotal.size(); i++)
        peso += pesoNoTotal[i].first;
    return peso / 2;
}

/**
 * @brief Algoritmo Guloso
 * 
 * @return float 
 */
void Graph::greed()
{
    /* matriz em que o vector de fora representa cada nó e,
    para cada nó, há um vector de pares, em que cada par representa
    uma aresta daquele nó. O par tem o formato {peso, posicao_notarget} */
    vector<vector<pair<int, int>>> matriz(this->getOrder());

    // Preenchendo e ordenando a matriz
    preencheMatriz(this, matriz);

    ordenaArestas(matriz, 1);

    // controles
    vector<pair<int, int>> pesoTotalNo(this->getOrder(), {0, 0}); // par {peso, grau}
    vector<bool> dentroRestricao(this->getOrder(), true);         // vector para controlar os nós que estão dentro da restrição

    // matriz que vai ter as arestas da agm
    vector<vector<pair<int, int>>> matrizAGM(this->getOrder());

    primAdaptado(this, matriz, matrizAGM, pesoTotalNo, dentroRestricao);

    int pesoAGM = pesoTotalAGM(pesoTotalNo);

    ordenaArestas(matrizAGM, 0);

    // Enquanto não estiver tudo dentro da restrição
    while (find(dentroRestricao.begin(), dentroRestricao.end(), false) != dentroRestricao.end())
    {
        cout << "\nNós ainda fora da restrição: " << count(dentroRestricao.begin(), dentroRestricao.end(), false) << endl;

        // index do nó que está fora da restrição de grau
        int index = (int)(find(dentroRestricao.begin(), dentroRestricao.end(), false) - dentroRestricao.begin());
        cout << "index fora da restrição: " << index << endl;
        cout << "ID do fora da restrição: " << this->getNodePosition(index)->getId() << endl;

        // SUBSTITUIÇÃO DE ARESTA

        // 1o descarta as arestas que nao poderão ser inseridas caso o nó já esteja no limite
        pair<int, int> aresta1 = matrizAGM[index].back(); // par {peso, posição_notarget}
        while (pesoTotalNo[aresta1.second].second >= D)
        {
            matriz[aresta1.second].clear();
            matrizAGM[index].pop_back();
            aresta1 = matrizAGM[index].back();
            while (matriz[aresta1.second].size() == 0)
            {
                matrizAGM[index].pop_back();
                aresta1 = matrizAGM[index].back();
            }
        }

        // 2o pega na matriz a primeira aresta mais leve do nó
        pair<int, int> aresta2 = matriz[aresta1.second].back();

        // se a entrada da aresta violar a restrição de grau, pegar a próxima menor
        while (pesoTotalNo[aresta2.second].second >= D)
        {
            matriz[aresta2.second].clear();
            matriz[aresta2.second].pop_back();
            aresta2 = matriz[aresta2.second].back();
            while (matriz[aresta2.second].size() == 0)
            {
                matriz[aresta2.second].pop_back();
                aresta2 = matriz[aresta2.second].back();
            }
        }

        // 3o com a aresta pronta, retira-la da matriz e atualizar os valores dos controles
        matriz[aresta2.second].pop_back();

        // atualiza peso e grau dos nós
        pesoTotalNo[index].first -= aresta1.first;
        pesoTotalNo[index].second--;
        pesoTotalNo[aresta1.second].first -= aresta1.first;
        pesoTotalNo[aresta1.second].second--;

        // atualiza o status das restrições
        if (pesoTotalNo[index].second <= D)
            dentroRestricao[index] = true;
        if (pesoTotalNo[aresta1.second].second <= D)
            dentroRestricao[aresta1.second] = true;

        // atualiza peso agm
        pesoAGM = pesoAGM - aresta1.first + aresta2.first;
    }

    cout << "\nPeso da Arvore Geradora com Restrição de Grau = " << pesoAGM << endl;
    return;
}

/**
 * @brief Algoritmo Guloso Randomizado
 * 
 * @return float 
 */
void Graph::greedRandom()
{
}

/**
 * @brief Algoritmo Guloso Randomizado Reativo
 * 
 * @return float 
 */
void Graph::greedRactiveRandom()
{
}