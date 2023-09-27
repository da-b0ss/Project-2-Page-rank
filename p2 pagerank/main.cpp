#include <iostream>
#include <iomanip>
#include<map>
#include<vector>
#include<iterator>
#include <utility>
using namespace std;      

class AdjacencyList {
  private:
    map<string, vector<pair<string,double>> > graph; //Adjacency list
    map<string,double> rankM; //RANK Map that holds the rank of each url
    map<string, double> outDeg; //OUTDEGREE Map that holds the outdegree of each url
    int pIter; //Power Iterations
    
  public:
    void pageRank(int pIter);
    void addPage(string from, string to);
    void printList();
    void createRankMap();
};

//Adds pages to the adjacency list
void AdjacencyList::addPage(string to, string from){
  //Creates an adjacency list
  graph[to].push_back(make_pair(from, 0));        
  
  /*Checks if the from page is already in the graph map. 
  If it is not, it adds an empty vector to the map for the from page.
  This ensures that all pages in the graph are included in the graph map, 
  even if they have no outgoing edges.*/
  if (graph.find(from)==graph.end())
  {
    graph[from] = {};
  }

  //Sets up the outdegree vector
  if (outDeg.count(from)) 
  {
    outDeg[from]++;
  } 
  else {
    outDeg[from] = 1.0;
  }  
}

//Creates the rank map
void AdjacencyList::createRankMap(){ 
  map<string, vector<pair<string,double>>>::iterator it;
    
  //Iterates through the graph's keys
  for(it = graph.begin(); it != graph.end(); ++it)
  {
    //Iterates through the graph's value, which is a vector of pairs, 
    //so it iterates through all the pairs in that value vector 
    for(int j = 0; j < it->second.size(); j++){
            
      //Checks for the outdegree of the "to" node and sets it as the weight
      //in the second part of the pair
      if(outDeg.find(it->second[j].first) != outDeg.end()){
        it->second[j].second = 1 / outDeg[it->second[j].first];
      }
    }
  }

  //Initializes the weights in the rank matrix 
  for(it = graph.begin(); it != graph.end(); ++it) 
  {
    double temp = 1.0 / graph.size();
    rankM[it->first] = temp;
  }
}

//Prints the rank matrix
void AdjacencyList::printList(){
    //Display rank matrix
    for (auto& kv : rankM) 
    {
      cout << fixed << showpoint;
      cout << setprecision(2);
      cout << kv.first << " " << kv.second << endl;
    }
}

//Sets the power iteration variable in the pagerank graph and then does the ranking
//by multiplying the maps
void AdjacencyList::pageRank(int pIter){
  this->pIter = pIter;
  //Creates a temporary new map to hold the new ranks
  map<string, double> newRank;

  //Iterates for however many power iterations given
  for(int i = 1; i < pIter; i++){
    //Iterates through the whole adjacency list
    for(auto it = graph.begin(); it != graph.end(); it++){
        double sum = 0.0;
        //Iterates through vector and sums the weights from the adjacency list multiplied by the ranks
        for(int j = 0; j < it->second.size(); j++){
            string from = it->second[j].first;
            //Extracts the weight from the adjacency list
            double weight = it->second[j].second;
            //Sums the weights into the new rank vector
            sum += rankM[from] * weight;
        }
        //Inserts the new rank value into the corresponding key 
        newRank[it->first] = sum;
    }
    //Sets the current rank map equal to the new rank
    rankM = newRank;
  }
}

int main() {
  int no_of_lines, power_iterations;
  string fromLink, toLink;

  cin >> no_of_lines;
  cin >> power_iterations;

  //Creates adjacency list object
  AdjacencyList pageR;
  
  for(int i = 0; i < no_of_lines; i++)
  {
    cin >> fromLink;
    cin >> toLink;

    //Adds the urls to the adjacency list
    pageR.addPage(toLink, fromLink);
  }

  //Sets the outdegree weights
  pageR.createRankMap();
  //Sets the power iterations in the adjacency list object
  pageR.pageRank(power_iterations);
  //Prints the rank
  pageR.printList();
}