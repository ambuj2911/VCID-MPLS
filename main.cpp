#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string.h>						// for strcat() and strcpy()
#include <limits.h>						// for INT_MAX
#include <algorithm>					// for min()
#include <stdio.h>
#include <fstream>
#include <bits/stdc++.h>
#define ld long double
#define max1 50
using namespace std;

vector<vector<int>> graph;
vector<vector<int>> graph_capacity;
 int max_nodes, num_edges;


 void floyd_warshall(int cost_path[][max1], int parent_path[][max1], int num_nodes)
{

    int i, j, k;
    for (k = 0; k < num_nodes; k++)
    {
        // Pick all vertices as source one by one
        for (i = 0; i < num_nodes; i++)
        {
            // Pick all vertices as destination for the above picked source
            for (j = 0; j < num_nodes; j++)
            {
                // If vertex k is on the shortest path from i to j, then update the value of dist[i][j]
                if (cost_path[i][k]!=INT_MAX && cost_path[k][j]!=INT_MAX && cost_path[i][k] + cost_path[k][j] < cost_path[i][j])
                {
                    cost_path[i][j] = cost_path[i][k] + cost_path[k][j];
                    parent_path[i][j] = parent_path[k][j];
                }
            }
        }
    }

}




void original( vector<vector<int>> topology, int cost_path[][max1], int parent_path[][max1], int num_nodes, string hop_or_dist )
{
	
	int i, j;
	for(i = 0; i < num_nodes; i++)
	{
		for(j = 0; j < num_nodes; j++)
		{
			if(i == j)			// loop
			{
				// no loop exists
				cost_path[i][j] = 0;
				parent_path[i][j] = -1;
			}

			else if(topology[i][j] != 0)		// delay exists means link exists
			{
				
				if(hop_or_dist == "hop")
					cost_path[i][j] = 1;
				else
					cost_path[i][j] = topology[i][j];

				parent_path[i][j] = i;

			}

			else				// no direct edge exist so make the weight as high as possible
                                // Set Parent as -1
			{
				cost_path[i][j] = INT_MAX;
				parent_path[i][j] = -1;

			}
		}
	}

}




void remove_path(vector<vector<vector<int>>> shortest, int cost[][max1], int parent[][max1], int src, int dest)
{
    int inter_dest = 1;
    int p = src;
    int q;

    while(inter_dest < shortest[src][dest].size())
    {
        q = shortest[src][dest][inter_dest];
        cost[p][q] = INT_MAX;
        cost[q][p] = INT_MAX;
        parent[p][q] = -1;
        parent[q][p] = -1;
        p = q;
        inter_dest++;

    }



}



void read_topo(string top_file, string hop_or_dist)
{

    ifstream file(top_file);
    string myText;
    FILE *reader;
    const char * c = top_file.c_str();
    reader = fopen(c,"r");
    int num, N ,E;
    fscanf(reader,"%d %d", &N, &E);
    int source, dest, cost, capacity;
    vector<int> vect1(N);
    int value = 0;
    fill(vect1.begin(), vect1.end(), value);

    for(int i = 0; i < N; i++)
        graph.push_back(vect1);

    for(int i = 0; i < N; i++)
        graph_capacity.push_back(vect1);

    max_nodes = N;
    num_edges = E;

    if(hop_or_dist == "hop")
    {
         while(!feof(reader))
        {
          fscanf(reader, "%d", &source);
          fscanf(reader, "%d", &dest);
          fscanf(reader, "%d", &cost);
          fscanf(reader, "%d", &capacity);
         // cout << source << " " << dest << " " << cost  << " " << capacity << endl;
           graph[source][dest] = 1;
           graph[dest][source] = 1;
           graph_capacity[source][dest] = capacity;
           graph_capacity[dest][source] = capacity;
        }


    }

    else
    {
         while(!feof(reader))
        {
          fscanf(reader, "%d", &source);
          fscanf(reader, "%d", &dest);
          fscanf(reader, "%d", &cost);
          fscanf(reader, "%d", &capacity);
         // cout << source << " " << dest << " " << cost  << " " << capacity << endl;
           graph[source][dest] = cost;
           graph[dest][source] = cost;
           graph_capacity[source][dest] = capacity;
           graph_capacity[dest][source] = capacity;
        }


    }
//
//    for(int i = 0; i < graph.size(); i++)
//    {
//        for(int j= 0; j < graph[i].size(); j++)
//            cout << graph[i][j] << " ";
//        cout << endl;
//
//    }

 //   cout << graph.size();

}



void path1(int parent[][max1], vector<vector<vector<int>>> &path, int num_nodes)
{
    for(int i = 0; i < num_nodes; i++)
    {
        vector<vector<int>> temp;
        for(int j = 0; j < num_nodes; j++)
        {
           vector<int> temp1;
           int source = i;
           int dest = j;
           while(source != dest)
           {
                temp1.push_back(dest);
                dest = parent[i][dest];
           }
             temp1.push_back(source);
             reverse(temp1.begin(), temp1.end());
            temp.push_back(temp1);
        }
        path.push_back(temp);
    }
}


void make_routing_table_file(string filename, vector<vector<vector<int>>> path1, vector<vector<vector<int>>> path2, int num_nodes, int path_cost1[][max1], int path_cost2[][max1])
{
    
     const char * c = filename.c_str();
    ofstream file(c);

    for(int i = 0; i < num_nodes; i++)
    {
        file << "For Source Node " << i << " the routing table is:" << endl;

        for(int j = 0; j < num_nodes; j++)
        {
            if(i != j)
            {
                file << "Destination: " << j << " " << endl;
                file << "First Shortest Path: " ;
                for(int k = 0; k < path1[i][j].size(); k++)
                    file << path1[i][j][k] << " ";
                file << "\t Cost: " << path_cost1[i][j] << endl;
                file << "Second Shortest Path: ";
                int k = 0;
                while(true)
                {
                    if(path2[i][j][k] == -1)
                        break;
                    else
                        file << path2[i][j][k] << " ";
                    k++;
                }
                file << "\t Cost: " << path_cost2[i][j] << endl;
            }



        }
        file << "----------------------------------------------------" << endl;

    }

}


void reduce_capacity(vector<vector<vector<int>>> path, ld biequiv, int src, int dest)
{
    int temp_dest = 1;
    int node_src = src;
    while(true)
    {
        int node_dest = path[src][dest][temp_dest];
        if(node_dest == dest)
        {
            graph_capacity[node_src][node_dest] -= biequiv;
            graph_capacity[node_dest][node_src] -= biequiv;
            break;

        }
        else
        {
            graph_capacity[node_src][node_dest] -= biequiv;
            graph_capacity[node_dest][node_src] -= biequiv;
        }
        node_src = node_dest;
        temp_dest++;
    }

}


bool check_capacity(vector<vector<vector<int>>> path, ld biequiv, int src, int dest)
{
    int temp_dest = 1;
    int node_src = src;
    vector<int> interface;
    while(true)
    {
        int node_dest = path[src][dest][temp_dest];
        if(node_dest == dest)
        {
            if(graph_capacity[node_src][node_dest] < biequiv)
            {
                return false;
            }
            break;

        }
        else
        {
            if(graph_capacity[node_src][node_dest] < biequiv)
            {
                return false;
            }

        }
        node_src = node_dest;
        temp_dest++;
    }
    reduce_capacity(path, biequiv, src, dest);
    return true;
}


void make_connections(string filename, int p, vector<vector<vector<int>>>path1, vector<vector<vector<int>>>path2, int num_nodes, string forwarding, string paths_file, int path1_cost[][max1], int path2_cost[][max1] )
{
    int no = 0;
     ofstream file7("output.txt");
    ifstream file(filename);
    int num_conn;
    long long vci = 400;
    file >> num_conn;
    vector<vector<vector<int>>> forwarding_table;
    vector<vector<int>> temp;
    vector<int> l;
    vector<vector<int>> interface;
     vector<int> path_data;
    for(int i = 0; i < 4; i++)
        l.push_back(0);

    for(int i = 0; i < num_conn; i++)
    {
        temp.push_back(l);
    }

    l.clear();

    for(int i = 0; i < num_nodes; i++)
        l.push_back(i);



    for(int i = 0; i < num_nodes; i++)
    {
        forwarding_table.push_back(temp);
        interface.push_back(l);
    }

    l.clear();
     for(int i = 0; i < num_nodes; i++)
        l.push_back(-1);

    for(int i = 0; i < num_conn; i++)
    {
        int src;
        int dest;
        int bimin, biavg, bimax;
        file >> src;
        file >> dest;
         file7 << "Connection ID " << i << endl ;
           file7 <<"Source: " <<   src <<  '\t' ;
             file7 <<"Destination:  " <<   dest <<  endl;
        ld biequiv;
        
        file >> bimin;
        file >> biavg;
        file >> bimax;

        if(p == 1) // pessimistic approach
        {
            biequiv = bimax;
        }
        else // optimistic approach
        {
            ld temp;
            temp = biavg + 0.25 * (bimax - bimin);
            if(temp > bimax)
                biequiv = bimax;
            else
                biequiv = temp;
        }

       if(check_capacity(path1, biequiv, src, dest)) // Connection  Made
       {
            int i = 0;
               file7 << "The path used here is: " << endl;
               while(i < path1[src][dest].size())
               {
                        file7 << path1[src][dest][i] << " ";
                        i++;
               }
                file7 <<endl<< " The path cost is "  <<  path1_cost[src][dest ] << endl;
                file7 << "The label list is: "<< endl;

            
            path_data.push_back(1);
            int temp_dest = 1;
            int node_src = src;
            while(true)
            {
              
                int node_dest = path1[src][dest][temp_dest];
                l[node_src]++;
                if(node_dest == dest)
                {

                    if(temp_dest == 1)
                    {
                        forwarding_table[node_src][l[node_src]][0] = -1;
                        
                    }
                        
                    else
                    {
                        int y = path1[src][dest][temp_dest - 1];
                        forwarding_table[node_src][l[node_src]][0] = interface[y][node_src];
                        
                    }


                    forwarding_table[node_src][l[node_src]][1] = vci;
                    file7 << vci << endl;
                    forwarding_table[node_src][l[node_src]][2] = interface[node_src][node_dest];
                    forwarding_table[node_src][l[node_src]][3] = vci++;
                    break;
                }

                else
                {
                     if(temp_dest == 1)
                            forwarding_table[node_src][l[node_src]][0] = -1;
                     else
                     {
                        int y = path1[src][dest][temp_dest - 1];
                        forwarding_table[node_src][l[node_src]][0] = interface[y][node_src];
                     }

                    forwarding_table[node_src][l[node_src]][1] = vci;
                    file7 << vci << '\t';
                    forwarding_table[node_src][l[node_src]][2] = interface[node_src][node_dest];
                    forwarding_table[node_src][l[node_src]][3] = vci++;

                }
                node_src = node_dest;
                temp_dest++;
            }
       }
       else // Connection not established via shortest path 1
       {
            if(check_capacity(path2, biequiv, src, dest)) // Connection  Made
            {
              
               int i = 0;
                while(path2[src][dest][i] != -1)
                {
                        file7<< path2[src][dest][i] << " ";
                        i++;
                 }
                
                file7 <<endl<< " The path cost is: "  <<  path2_cost[src][dest ] << endl;
                file7 << "The label list is: "<< endl;

               path_data.push_back(2);
                int temp_dest = 1;
                int node_src = src;
                while(true)
                {
                    int node_dest = path2[src][dest][temp_dest];
                    l[node_src]++;
                    if(node_dest == dest)
                    {

                        if(temp_dest == 1)
                            forwarding_table[node_src][l[node_src]][0] = -1;
                        else
                        {
                            int y = path2[src][dest][temp_dest - 1];
                            forwarding_table[node_src][l[node_src]][0] = interface[y][node_src];
                        }


                        forwarding_table[node_src][l[node_src]][1] = vci;
                         file7 << vci << '\t';
                        forwarding_table[node_src][l[node_src]][2] = interface[node_src][node_dest];
                        forwarding_table[node_src][l[node_src]][3] = vci++;
                        file7 << endl;
                        break;
                    }
                    else
                    {
                        if(temp_dest == 1)
                            forwarding_table[node_src][l[node_src]][0] = -1;
                        else
                        {
                            int y = path2[src][dest][temp_dest - 1];
                            forwarding_table[node_src][l[node_src]][0] = interface[y][node_src];
                        }

                        forwarding_table[node_src][l[node_src]][1] = vci;
                         file7 << vci << '\t';
                        forwarding_table[node_src][l[node_src]][2] = interface[node_src][node_dest];
                        forwarding_table[node_src][l[node_src]][3] = vci++;
                        
                    }
                    node_src = node_dest;
                    temp_dest++;

                }

            }
            else // Connection failed
            {

                    // donothing
                    path_data.push_back(-1);
                    no++;

            }

        }



    }

 //cout << no << endl;
//
    /*for(int i = 0; i < num_nodes; i++)
    {
        for(int j = 0; j < num_conn; j++)
       {
           for(int k = 0; k < 4; k++)
            cout<< forwarding_table[i][j][k] << " ";
            cout << endl;

        }
        cout << "---------------------------------------"<<endl; 
       }
*/
   
    
    ofstream file5(forwarding);
     
    
    for(int i = 0; i < num_nodes; i++)
    {
        for(int j = 0; j < num_conn; j++)
        {
            int sum = 0;
            for(int k = 0; k < 4; k++)
             {
                sum += forwarding_table[i][j][k];
                file5 << forwarding_table[i][j][k];
                file5 << "  ";
             }
             if(sum == 0)
                break;
              file5 << endl;

        }        
        file5 << endl<< "---------------------------------------"<<endl;
  }
    
    ofstream file6(paths_file);
    
    file6 << "Total Connections requested " << num_conn << " and total connections allowed " << num_conn - no;
    
   
  
    
    }
    







void select_path(int parent[][max1], vector<vector<vector<int>>> &path, int src, int dest)
{
    int k = dest;
    vector<int> temp;
    temp.push_back(dest);

    while(true)
    {
        if(parent[src][k] == src)
        {
            temp.push_back(src);
            break;
        }
        else
        {
            temp.push_back(parent[src][k]);
            k = parent[src][k];
        }


    }
    reverse(temp.begin(), temp.end());

    for(int i = 0; i < temp.size(); i++)
        path[src][dest][i] = temp[i];

}



int main(int argc, char** argv)
{

       int path_cost1[max1][max1], path_cost2[max1][max1], costpath2[max1][max1];
	int path_parent1[max1][max1],path_parent2[max1][max1];



        memset(path_cost1, -1, sizeof(path_cost1));
	memset(path_cost2, -1, sizeof(path_cost2));
	memset(path_parent1, -1, sizeof(path_parent1));
	memset(path_parent2, -1, sizeof(path_parent2));
	memset(costpath2, -1, sizeof(costpath2));

     /*char top_file[256] = "TOP14.txt";
   char hop_or_dist[256] = "hop";
  read_topo(top_file, hop_or_dist);
   char routing_filename[256] = "Routing.txt";
  int p = 1 ;// Optimistic or pessimistic
 char connection_filename[256] = "NSFNET.txt";
 char paths_file[256];
   char forwarding_filename[256];*/


   /* char top_file[256];
    char hop_or_dist[256];
    char routing_filename[256];
    int p = 1 ;// Optimistic or pessimistic
    char connection_filename[256];
    char paths_file[256];
    char forwarding_filename[256];*/
    
    string  top_file (argv[2]);
    string hop_or_dist(argv[12]);
    string routing_filename(argv[6]);
    string connection_filename(argv[4]);
    string  paths_file(argv[10]);
    string  forwarding_filename(argv[8]);
    string flag(argv[14]);
    const char * c = flag.c_str();
    int p = atoi(c);
   
    
        
   read_topo(top_file, hop_or_dist);

    original(graph, path_cost1, path_parent1, max_nodes, hop_or_dist);
   original(graph, path_cost2, path_parent2, max_nodes, hop_or_dist);


//	for(int i = 0; i < graph.size(); i++)
//    {
//        for(int j= 0; j < graph[i].size(); j++)
//            cout << path_cost1[i][j] << " ";
//        cout << endl;
//

//    }

    floyd_warshall(path_cost1, path_parent1, max_nodes);
//
//    for(int i = 0; i < graph.size(); i++)
//    {
//        for(int j= 0; j < graph[i].size(); j++)
//            cout << path_parent1[i][j] << " ";
//        cout << endl;
//
//    }
        vector<vector<vector<int>>> path;

        path1(path_parent1, path, max_nodes);

//        for(int i = 0; i < path.size(); i++)
//        {
//
//            for(int j = 0; j < path[i].size(); j++)
//            {
//
//                for(int k = 0; k < path[i][j].size(); k++)
//                    cout << path[i][j][k] << " ";
//                cout << endl;
//            }
//            cout << "----------------------------------" << endl;
//        }
       vector<vector<vector<int>>> path2;
       for(int i = 0; i < max_nodes; i++)
       {
           vector<vector<int>> temp1;
           for(int j = 0; j < max_nodes; j++)
           {
               vector<int> temp;
               for(int k = 0; k < max_nodes; k++)
               {
                   temp.push_back(-1);
               }
               temp1.push_back(temp);
           }
           path2.push_back(temp1);

       }


        for(int i = 0; i < max_nodes; i++)
        {

            for(int j = 0; j < max_nodes; j++)
            {
               if(i != j)
               {
                   remove_path(path, path_cost2, path_parent2, i, j);
                   floyd_warshall(path_cost2, path_parent2, max_nodes);
                   select_path(path_parent2, path2, i, j);
                   costpath2[i][j] = path_cost2[i][j];
                   original(graph, path_cost2, path_parent2, max_nodes, hop_or_dist);
               }

            }

        }



        make_routing_table_file(routing_filename, path, path2, max_nodes, path_cost1, costpath2);

        make_connections(connection_filename, p, path, path2, max_nodes, forwarding_filename,  paths_file, path_cost1, costpath2);
 
    return 0;
    }
