#include "Graph.h"
#include "Node.h"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <math.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

struct pontos {
  float x;
  float y;
};

struct instancia {
  int nodeSource;
  int nodeTarget;
  int edWeight;
};

Graph *leitura(ifstream &input_file, int directed, int weightedEdge, int weightedNode) {

  //Variáveis para auxiliar na criação dos nós no Grafo
  int idNodeSource;
  int idNodeTarget;
  int order;
  int numEdges;
  float edgeWeight;

  //Pegando a ordem do grafo
  input_file >> order;

  cout << "\nLendo o arquivo input.txt..." << endl;
  //cout << "Ordem: " << order << endl;

  //Criando objeto grafo: Ordem - direcionado - peso Aresta - peso Nó
  Graph *graph = new Graph(order, directed, weightedEdge, weightedNode);

  //Leitura de arquivo
  if (!graph->getWeightedEdge() && !graph->getWeightedNode()) // Sem peso nas arestas e sem peso nos nós
  {

    while (input_file >> idNodeSource >> idNodeTarget) {
      //cout << "Edge: " << idNodeSource << " -- " << idNodeTarget << endl;
      graph->insertEdge(idNodeSource, idNodeTarget, 0);
    }
  } else if (graph->getWeightedEdge() && !graph->getWeightedNode()) // Com peso nas arestas e sem peso nos nós
  {
    while (input_file >> idNodeSource >> idNodeTarget >> edgeWeight) {
      //cout << "Edge: " << idNodeSource << " - " << idNodeTarget << " - " << edgeWeight << endl;
      graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);
    }
  } else if (graph->getWeightedNode() && !graph->getWeightedEdge()) // Com peso no nó e sem peso na aresta
  {
    float nodeSourceWeight, nodeTargetWeight;

    while (input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight) {
      graph->insertEdge(idNodeSource, idNodeTarget, 0);
      graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
      graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);
    }
  } else if (graph->getWeightedNode() && graph->getWeightedEdge()) // Com peso no nó e com peso na aresta
  {

    float nodeSourceWeight, nodeTargetWeight, edgeWeight;

    while (input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight) {
      graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);
      graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
      graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);
    }
  }

  return graph;
}

void calcPeso(vector<pontos> coord) {
  vector<instancia> instanc;
  float valF = 0.0, var = 0.0, valI = 0.0;
  int dist = 0;
  int size = coord.size();
  stringstream ss;
  ss << "inst" << size << ".txt";
  ofstream output;
  output.open(ss.str(), ios::out | ios::trunc);
  output << coord.size() << endl;

  for (int i = 1; i <= coord.size(); i++) {
    for (int j = i + 1; j <= coord.size(); j++) {

      valF = sqrt(pow((coord[i].x - coord[j].x), 2) + pow((coord[i].y - coord[j].y), 2));

      valI = (int)valF;
      var = fabs(valF - valI);
      if (var >= 0.5)
        dist = valI + 1;
      else
        dist = valI;

      instancia ins = {i, j, dist};
      instanc.push_back(ins);
      output << i << " " << j << " " << dist << endl;
    }
  }

  output.close();
}

void leInstancia(string path) {
  string x, y, temp;
  vector<pontos> coord;

  fstream inst(path, ios::in);

  int i = 0;
  while (inst.good()) {
    getline(inst, temp, ' ');
    getline(inst, x, ' ');
    getline(inst, y, '\n');

    pontos p = {stof(x.c_str()), stof(y.c_str())};
    coord.push_back(p);
  }

  inst.close();

  calcPeso(coord);
}

int menu() {

  int selecao;

  cout << "\nMENU" << endl;
  cout << "----" << endl;
  cout << "[1]  Fecho Transitivo Direto" << endl;
  cout << "[2]  Fecho Transitivo Indireto" << endl;
  cout << "[3]  Caminho Mínimo entre dois vértices - Dijkstra" << endl;
  cout << "[4]  Caminho Mínimo entre dois vértices - Floyd" << endl;
  cout << "[5]  Árvore Geradora Mínima sobre subgrafo vertice induzido por X usando algoritmo de Prim" << endl;
  cout << "[6]  Árvore Geradora Mínima sobre subgrafo vertice induzido por X usando algoritmo de Kruskal" << endl;
  cout << "[7]  Caminhamento Profundidade destacando as Arestas de retorno" << endl;
  cout << "[8]  Ordenação topologica em D ou a informação de que não é um grafo acíclico direcionado" << endl;
  cout << "[9]  Algoritmo Guloso" << endl;
  cout << "[10] Algoritmo Guloso Randomizado " << endl;
  cout << "[11] Algoritmo Guloso Randomizado Reativo" << endl;
  cout << "[12] Algoritmo Guloso 2" << endl;
  cout << "[13] Algoritmo Guloso 2 Randomizado " << endl;
  cout << "[14] Algoritmo Guloso 2 Randomizado Reativo" << endl;
  cout << "[0]  Sair" << endl;

  cout << "\nSua opção: ";
  cin >> selecao;

  return selecao;
}

void selecionar(int selecao, Graph *graph, ofstream &output_file) {

  switch (selecao) {
    //Sair;
  case 0:
    exit(0);
    break;
  case 1: // Fecho Transitivo Direto;
    int v;
    cout << "Digite o id do nó: ";
    cin >> v;

    graph->directTransitiveClosing(v, output_file);
    cout << endl;
    break;

  case 2: // Fecho Transitivo Indireto;
    int h;
    cout << "Digite o id do nó: ";
    cin >> h;
    graph->indirectTransitiveClosing(h, output_file);
    cout << endl;
    break;

  case 3: //Caminho Mínimo entre dois vértices - Dijkstra
    graph->dijkstra(output_file);
    break;

  case 4: // Caminho Mínimo entre dois vértices - Floyd
    graph->floydWarshall(output_file);
    break;

  case 5: // Árvore Geradora Mínima sobre subgrafo vertice induzido por X usando algoritmo de Prim
    cout << "\nPara rodar o algoritmo de Prim, é preciso um subgrafo vértice induzido" << endl;
    graph->agmPrim(graph->getVertInduz(), output_file);
    break;

  case 6: // Árvore Geradora Mínima sobre subgrafo vertice induzido por X usando algoritmo de Kruskal
    cout << "\nPara rodar o algoritmo de Kruskal, é preciso um subgrafo vértice induzido" << endl;
    graph->agmKruskal(graph->getVertInduz(), output_file);
    break;

  case 7: // Caminhamento Profundidade destacando as Arestas de retorno
    graph->deepSearch(output_file);
    break;

  case 8: // Ordenação topologica em DAG ou a informação de que não é um grafo acíclico direcionado
    graph->topologicalSorting();
    break;
  case 9: //Algoritmo Guloso
    graph->greed();

    break;
  case 10: //Algoritmo Guloso Randomizado
    graph->greedRandom();

    break;
  case 11: //Algoritmo Guloso Randomizado Reativo
    graph->greedRactiveRandom();

    break;
  case 12: //Algoritmo Guloso
    graph->greed2();

    break;
  case 13: //Algoritmo Guloso Randomizado
    graph->greedRandom2();

    break;
  case 14: //Algoritmo Guloso Randomizado Reativo
    graph->greedRactiveRandom2();

    break;

  default:
    system("clear");
    cout << " Erro!!! Opção invalida." << endl;
  }
}

int mainMenu(ofstream &output_file, Graph *graph) {

  int selecao = -1;

  while (selecao != 0) {
    selecao = menu();

    if (output_file.is_open())
      selecionar(selecao, graph, output_file);
    else
      cout << "Arquivo não encontrado!" << endl;
    output_file << endl;
  }

  return 0;
}

int main(int argc, char const *argv[]) {
  //Verificação se todos os parâmetros do programa foram entrados
  if (argc != 6) {
    cout << "ERRO: Esperado: ./<program_name> <input_file> <output_file> <directed> <weighted_edge> <weighted_node> " << endl;
    return 1;
  }

  string program_name(argv[0]);
  string input_file_name(argv[1]);

  string instance;
  if (input_file_name.find("v") <= input_file_name.size()) {
    string instance = input_file_name.substr(input_file_name.find("v"));
    cout << "Running " << program_name << " with instance " << instance << " ... " << endl;
  }

  //Abrindo arquivo de entrada
  ifstream input_file;
  ofstream output_file;
  input_file.open(argv[1], ios::in);
  output_file.open(argv[2], ios::out | ios::trunc);

  Graph *graph;

  if (input_file.is_open()) {
    graph = leitura(input_file, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
  } else
    cout << "Não foi possível abrir o arquivo! " << argv[1];

  string d, a, v;
  d = a = v = "Sim";

  int o = graph->getDirected();
  int e = graph->getWeightedEdge();
  int n = graph->getWeightedNode();

  if (!o)
    d = "Não";
  if (!e)
    a = "Não";
  if (!n)
    v = "Não";

  cout << "\nCriando grafo..." << endl;
  cout << "Ordem: " << graph->getOrder() << endl;
  cout << "Nº Arestas: " << graph->getNumberEdges() << endl;
  cout << "Direcionado? " << d << endl;
  cout << "Arestas com peso? " << a << endl;
  cout << "Vertices com peso? " << v << endl;

  //leInstancia("pr136.txt");

  mainMenu(output_file, graph);

  //Fechando arquivo de entrada
  input_file.close();

  //Fechando arquivo de saída
  output_file.close();

  return 0;
}
