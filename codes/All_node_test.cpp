#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<ctime>
#include <random>
#define INF 9999
#define MAX_VTXS 10000
using namespace std;

static mt19937 gen = mt19937((unsigned int)time(NULL));
static uniform_real_distribution<> dist(0, 100); 
struct Node
{
    int id;
    string name;
};

class WAdjMatGraph
{
    protected:
        int v_size ;
        Node *vertices[MAX_VTXS];
        int **adj;

    public:
        WAdjMatGraph(){
            adj=new int*[MAX_VTXS];
            for(int i=0;i<MAX_VTXS;i++)
            {
                adj[i]=new int[MAX_VTXS];
            }
            reset();}
        ~WAdjMatGraph(){
            for(int i=0;i<MAX_VTXS;i++)
            {
                delete [] adj[i];
            }
            delete[] adj;
        }
        string getVertex(int id)
        {
            for(int i=0;i<v_size;i++)
            {
                if(vertices[i]->id==id)
                    return vertices[i]->name;
            }
        }
        int getEdge(int source, int target)
        {
            int Source=-1;
            int Target=-1;
            for(int i=0;i<v_size;i++)
            {
                if(vertices[i]->id==source)
                    Source=i;
                if(vertices[i]->id==target)
                    Target=i;
            }
            return adj[Source][Target];   
        }
        void setEdge(int source, int target, int weight)
        {   
            int Source=-1;
            int Target=-1;
            for(int i=0;i<v_size;i++)
            {
                if(vertices[i]->id==source)
                    Source=i;
                if(vertices[i]->id==target)
                    Target=i;
            }
            adj[Source][Target]=weight;
        }
        bool isEmpty(){return v_size==0;}
        bool isFull(){return v_size>=MAX_VTXS;}
        void reset()
        {
            v_size=0;
            for(int i=0;i<MAX_VTXS;i++)
            {
                for(int j=0;j<MAX_VTXS;j++)
                {
                    adj[i][j]=0;
                }
            }
        }
        void insertVertex(int id, string name)
        {
            if(!isFull()){vertices[v_size]=new Node;
            vertices[v_size]->id=id; vertices[v_size++]->name=name;}
            else{cout<<"Exceeding maximum number of vertices\n";}
        }
        void insertEdge(int source, int target, int weight, bool dir)
        {   
            if(weight>=INF){weight=INF;}
            setEdge(source, target, weight);
            if(dir==false)
                setEdge(target, source, weight); 
        }
        bool hasEdge(int source, int target){return (getEdge(source, target)<INF);}
        void vertex_load(string filename)
        {
            ifstream ifs(filename);
            string line;
            getline(ifs, line);
            while(!ifs.eof())
            {
                getline(ifs, line);
                if(line=="") continue;
                int id;
                string name;
                stringstream ls(line);
                ls>>id;
                getline(ls, name);
                name.replace(name.find("\t"),1,"");
                insertVertex(id, name);
            };
            ifs.close();
        }
        void edge_load(string filename, bool dir, bool wei)
        {
            ifstream ifs(filename);
            string line;
            stringstream ls(line);
            getline(ifs,line);
            while(!ifs.eof())
            {
                getline(ifs, line);
                if(line=="") continue;
                int source, target, weight;
                stringstream ls(line);
                ls>>source;
                ls>>target;
                if(wei)
                    ls>>weight;
                else
                    weight=1;
                insertEdge(source,target,weight,dir);
            }
            ifs.close();
        }
        void RandomWalker(string vertex, int length, double q, double arr[])
        {   
            double neighbor_w[MAX_VTXS];
            while(length>0)
            {   
                bool jump=false;
                double p_sum=0;
                int sum=0;
                double rand=dist(gen);
                if((rand/100)>q)
                {
                    double rand2=dist(gen);
                    for(int i=0;i<v_size;i++)
                    {
                        if(vertices[i]->name==vertex)
                        {
                            for(int j=0;j<v_size;j++)
                            {
                                if(adj[i][j]<INF)
                                {
                                    sum+=adj[i][j];
                                    neighbor_w[j]=adj[i][j];
                                }
                                else
                                {
                                    neighbor_w[j]=0;
                                }
                            }
                            if(sum==0) 
                            {
                                jump=true; 
                                break;
                            }
                        }
                    }
                    for(int j=0;j<v_size;j++)
                    {
                        if(jump==true) break;
                        neighbor_w[j]=neighbor_w[j]/sum;
                        if(neighbor_w[j]>0){
                            p_sum+=neighbor_w[j];
                            if(p_sum>=(rand2/100))
                            {
                                length--;
                                arr[j]++;
                                vertex=vertices[j]->name;
                                break;
                            }
                        }
                    }
                }

                if((jump==true)||((rand/100)<=q))
                {   
                    double rand2=dist(gen);
                    for(int i=0;i<v_size;i++)
                    {   
                        p_sum+=1.0/v_size;
                        if(p_sum>=(rand2/100))
                        {
                            length--;
                            arr[i]++;
                            vertex=vertices[i]->name;
                            break;
                        }
                    }
                }
            }  
        }
        string PageRank(string vertex, int length, double q, int idx_arr[])
        {
            double arr[v_size]={0};
            RandomWalker(vertex, length, q, arr);
            double sum=0;
            int max=0;
            cout.setf(ios::fixed);
            cout.setf(ios::showpoint);
            cout.precision(5);
            for(int i=0;i<v_size;i++)
            {
                arr[i]=arr[i]/(double)length;
                if(arr[max]<arr[i]) 
                {
                    if(idx_arr[i]==0)
                    {
                        max=i;
                    }
                }
                else if(idx_arr[max]==1&&arr[max]>=arr[i])
                {
                    max=i;
                }
                sum+=arr[i];
            }
            cout<<"가장 중요 vertex : "<<vertices[max]->name<<" ("<<arr[max]<<")\n";
            idx_arr[max]=1; 
            return vertices[max]->name;
        }
        void All_node_rank(string vertex, int length, double q)
        {
            int idx_arr[v_size]={0};
            for(int i=0;i<v_size;i++)
            {
                if(vertices[i]->name==vertex)
                {
                    cout<<"시작 vertex : "<<vertex<<"\n";
                    idx_arr[i]=1;
                }
            }
            int num=v_size;
            int push=0;
            while(num-1>0)
            {   
                vertex=PageRank(vertex, length, q, idx_arr);
                push++;
                num--;
            }
        }
};

int main()
{
    WAdjMatGraph g1;
    //WAdjMatGraph g2;
    g1.vertex_load("DS2021_PR1_20201786/dataset/starwars/starwars-episode-1-interactions-allCharacters-nodes.tsv");
    g1.edge_load("DS2021_PR1_20201786/dataset/starwars/starwars-episode-1-interactions-allCharacters-links.tsv", true, true);
    g1.All_node_rank("R2-D2",300,0.1);
    /*g2.vertex_load("DS2021_PR1_20201786/dataset/bicycle/station_names.tsv");
    g2.edge_load("DS2021_PR1_20201786/dataset/bicycle/bicycle_trips_over_100.tsv", true, false);
    g2.All_node_rank("서울역 서부", 10000, 0.1);*/
    return 0;
}