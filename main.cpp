#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

std::vector<std::vector<bool>> file2graph(const std::string& filename);

void dfs_mod(const std::vector<std::vector<bool>>& graph, const int& vertex, std::vector<bool>& visited, std::vector<int>& order);

std::vector<int> topsort(const std::vector<std::vector<bool>>& graph);

bool is_cyclic(const std::vector<std::vector<bool>>& graph, const int& vertex, std::vector<bool>& visited, const int& prev);

bool has_cycle(const std::vector<std::vector<bool>>& graph);

int main() {
  std::vector<std::string> filelist = {
    "input.txt",
    //"cyclic.txt",
    "input2.txt",
    //"input404.txt",
  };
  for(auto& filename : filelist){
    std::vector<std::vector<bool>> graph;
    try{
      graph = file2graph(filename);
    }catch(std::string err_message){
      std::cout<<err_message<<std::endl<<std::endl;
      continue;
    }
    std::cout<<"Граф, соответствующий файлу \""<<filename<<"\":\n";
    if(has_cycle(graph)){
      std::cout<<"\tГраф содержит цикл, решения нет\n\n";
      continue;
    }
    std::cout << "\tТопологический порядок вершин: ";
    std::vector<int> order = topsort(graph);
    /*for (auto it = order.rbegin(); it != order.rend(); ++it) {
      std::cout << *it+1 << " ";
    }*/
    for (auto& el : order) {
      std::cout << el+1 << " ";
    }
    std::cout<<std::endl<<std::endl;
  }
}

std::vector<std::vector<bool>> file2graph(const std::string& filename){
  std::ifstream infile(filename);
  if(!infile.is_open()){
    //std::cout << "Error opening file!\n";
    throw std::string{"Ошибка чтения файла \""+filename+"\""};
  }
  int size{};
  infile >> size;
  std::vector<std::vector<bool>> graph(size);
  int i{};
  std::string str;
  while(std::getline(infile,str)){
    if(str.empty()) continue;
    std::istringstream ss(str);
    bool v{};
    while(ss>>v){
       graph[i].push_back(v);
    }
    i++;
  }
  return graph;
}

void dfs_mod(const std::vector<std::vector<bool>>& graph, const int& vertex, std::vector<bool>& visited, std::vector<int>& order){
   visited[vertex] = true;
   for(int v{}; v<graph.size(); v++){
     if(graph[vertex][v] && !visited[v]){
        dfs_mod(graph,v,visited,order);
     }
   }
  //order.push_back(vertex);
  order.insert(order.begin(),vertex); //INEFFICIENT FOR VECTOR, LIST COULD BE USED INSTEAD
}

std::vector<int> topsort(const std::vector<std::vector<bool>>& graph){
  std::vector<bool> visited(graph.size(),false);
  std::vector<int> order;
  for(int v{}; v<graph.size();v++){
    if(!visited[v]){
      dfs_mod(graph,v,visited,order);
    }
  }
  return order;
}

bool is_cyclic(const std::vector<std::vector<bool>>& graph, const int& vertex, std::vector<bool>& visited, const int& prev){
   visited[vertex] = true;
   for(int v{}; v<graph.size(); v++){
     if(graph[vertex][v]){
       if(!visited[v]){
         return is_cyclic(graph,v,visited,vertex);
       }else if(v != prev){
         return true;
       }
    }
  }
  return false;
}

bool has_cycle(const std::vector<std::vector<bool>>& graph){
  std::vector<bool> visited(graph.size(),false);
  for(int v{}; v<graph.size();v++){
    if(!visited[v]){
      visited[v] = true;
      return is_cyclic(graph,v,visited,-1);
    }
  }
  return false;
}
