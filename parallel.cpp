#include <bits/stdc++.h>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

class Graph{
    int edges;
    unordered_map<char, vector<char>> adlist;

    public:

    Graph(int edges){
        this->edges = edges;
        this->adlist.clear();
    }

    void construct(){
        for(int i=0; i<edges; i++){
            cout<<"Enter only unique edges: ";
            char u,v;
            cin>>u>>v;
            adlist[u].push_back(v);
            adlist[v].push_back(u);
        }
    }

    void display(){
        for(auto e: adlist){
            cout<<e.first<<" -> ";
            for(char n: e.second){
                cout<<n<<" ";
            }
            cout<<endl;
        }
    }

    void seq_bfs(char start){
        unordered_map<char, bool> visited;
        for(auto e: adlist){
            visited[e.first] = false;
        }

        queue<char> q;
        q.push(start);
        visited[start] = true;

        while(!q.empty()){
            char u = q.front();
            q.pop();
            vector<char> neighbors = adlist[u];
            cout<<u<<" ";

            for(char n: neighbors){
               if(!visited[n]){
                q.push(n);
                visited[n] = true;  
               } 
            }
        }
        cout<<endl;
    }

    void par_bfs(char start){
        unordered_map<char, bool> visited;
        for(auto e: adlist){
            visited[e.first] = false;
        }

        queue<char> q;
        q.push(start);
        visited[start] = true;

        // sequential bfs processes one node at a time
        // parallel bfs processes mutliple nodes at the same time on the same level
        // consider multiple nodes in the queue
        while(!q.empty()){

            // first collect all current level nodes
            vector<char> current_level_nodes;
            for(int i=0; i<q.size(); i++){
                char u = q.front();
                q.pop();
                cout<<u<<" ";
                current_level_nodes.push_back(u);
            }

            // process them parallely
            #pragma omp parallel for
            for(int i=0; i<current_level_nodes.size(); i++){
                char current_node = current_level_nodes[i];
                for(char neighbor: adlist[current_node]){
                    if(!visited[neighbor]){
                        #pragma omp critical
                        if(!visited[neighbor]){
                            q.push(neighbor);
                            visited[neighbor] = true;
                        }
                    }
                }
            }

        }
        cout<<endl;
    }
    
    // same as seq bfs just replace queue with stack
    void seq_dfs(char start){
        unordered_map<char, bool> visited;
        for(auto e: adlist){
            visited[e.first] = false;
        }

        stack<char> s;
        s.push(start);
        visited[start] = true;

        while(!s.empty()){
            char u = s.top();
            s.pop();
            vector<char> neighbors = adlist[u];
            cout<<u<<" ";

            for(char n: neighbors){
               if(!visited[n]){
                s.push(n);
                visited[n] = true;  
               } 
            }
        }
        cout<<endl;
    }

    // same as seq dfs with few additions
    void par_dfs(char start){
        unordered_map<char, bool> visited;
        for(auto e: adlist){
            visited[e.first] = false;
        }

        stack<char> s;
        s.push(start);
        visited[start] = true;

        while(!s.empty()){
            char u = s.top();
            s.pop();
            vector<char> neighbors = adlist[u];
            cout<<u<<" ";

            #pragma omp parallel for
            for(char n: neighbors){
               if(!visited[n]){
                #pragma omp critical
                if(!visited[n]){
                    s.push(n);
                    visited[n] = true;
                }  
               } 
            }
        }
        cout<<endl;
    }

    void run_bfs(char start){

        auto seq_start = high_resolution_clock::now();
        seq_bfs(start);
        auto seq_end = high_resolution_clock::now();
        duration<double> seq_time = seq_end - seq_start;
        cout<<"Time for seq bfs: "<<seq_time.count()<<"s."<<endl;


        auto par_start = high_resolution_clock::now();
        par_bfs(start);
        auto par_end = high_resolution_clock::now();
        duration<double> par_time = par_end - par_start;
        cout<<"Time for par bfs: "<<par_time.count()<<"s."<<endl;
    }

    void run_dfs(char start){

        auto seq_start = high_resolution_clock::now();
        seq_dfs(start);
        auto seq_end = high_resolution_clock::now();
        duration<double> seq_time = seq_end - seq_start;
        cout<<"Time for seq dfs: "<<seq_time.count()<<"s."<<endl;


        auto par_start = high_resolution_clock::now();
        par_dfs(start);
        auto par_end = high_resolution_clock::now();
        duration<double> par_time = par_end - par_start;
        cout<<"Time for par dfs: "<<par_time.count()<<"s."<<endl;
    }
};

int main(){
    cout<<"Enter number of edges: ";
    int edges;
    cin>>edges;
    Graph g(edges);
    g.construct();
    g.display();
    g.run_bfs('A');
    g.run_dfs('A');
}
/*
Enter number of edges: 5
Enter only unique edges: A B
Enter only unique edges: A C
Enter only unique edges: B C
Enter only unique edges: D C
Enter only unique edges: E C
E -> C 
D -> C 
C -> A B D E 
B -> A C 
A -> B C 
A B C D E 
Time for seq bfs: 2.1e-05s.
A B C D E 
Time for par bfs: 1.2063e-05s.
A C E D B 
Time for seq dfs: 1.026e-05s.
A C E D B 
Time for par dfs: 1.066e-05s.
*/