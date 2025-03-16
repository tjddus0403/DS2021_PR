#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<ctime>
#include <random>
#include <chrono>
#define INF 9999
#define MAX_VTXS 10000
using namespace std;

static mt19937 gen = mt19937((unsigned int)time(NULL));
static uniform_real_distribution<> dist(0, 100); 
struct Node
{
    int id; 
    string name;
}; //vertex 내용이 id, name 두가지로 되어있으므로

class WAdjMatGraph
{
    protected:
        int v_size ; //vertex 수
        Node *vertices[MAX_VTXS]; //vertex들을 저장할 포인터 배열
        int **adj; //인접 행렬 사용

    public:
        WAdjMatGraph(){
            adj=new int*[MAX_VTXS]; 
            for(int i=0;i<MAX_VTXS;i++)
            {
                adj[i]=new int[MAX_VTXS]; //인접행렬이 2차원이므로 2차원 배열 동적할당
            }
            reset();}
        ~WAdjMatGraph(){
            for(int i=0;i<MAX_VTXS;i++)
            {
                delete [] adj[i]; 
            }
            delete[] adj; //동적할당 해제
        }
        string getVertex(int id) //vertex의 id를 이용해 vertex 찾기
        {
            for(int i=0;i<v_size;i++)
            {
                if(vertices[i]->id==id) 
                    return vertices[i]->name;
            }
        }
        int getEdge(int source, int target) //source vertex의 id와 target vertex의 id를 이용해 두 vertex 사이의 가중치 찾기
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
        void setEdge(int source, int target, int weight) //두 vertex 사이의 엣지 정보 (source id, target id, weigth)받아 인접행렬에서 두 vertex사이의 가중치 값 설정
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
        void reset() //인접행렬 초기화
        {
            v_size=0;
            for(int i=0;i<MAX_VTXS;i++)
            {
                for(int j=0;j<MAX_VTXS;j++)
                {
                    adj[i][j]=INF; //연결이 되어있지 않은 상태를 INF로 표시
                }
            }
        }
        void insertVertex(int id, string name) //vertex 삽입
        {
            if(!isFull()){vertices[v_size]=new Node; //동적할당 통한 vertex 삽입
            vertices[v_size]->id=id; vertices[v_size++]->name=name;}
            else{cout<<"Exceeding maximum number of vertices\n";}
        }
        void insertEdge(int source, int target, int weight, bool dir) //edge 삽입
        {   
            if(weight>=INF){weight=INF;}
            setEdge(source, target, weight);
            if(dir==false) //방향성이 없는 경우 edge 양방향 설정
                setEdge(target, source, weight); 
        }
        bool hasEdge(int source, int target){return (getEdge(source, target)<INF);} 
        void vertex_load(string filename) //vertex 파일 내용 불러오기
        {
            ifstream ifs(filename);
            string line;
            getline(ifs, line); //첫 줄은 id    name 이라고 적힌 부분이기에 저장할 정보가 아니어서 그냥 읽고 넘어가기
            while(!ifs.eof())
            {
                getline(ifs, line);
                if(line=="") continue; //line에 내용이 없으면 그냥 넘기기 (데이터들 마지막 줄이 다 공백으로 되어있어서 설정함)
                int id;
                string name;
                stringstream ls(line);
                ls>>id;
                getline(ls, name);
                name.replace(name.find("\t"),1,""); //name 앞에 붇는 tab 공백 없애기 위함
                insertVertex(id, name); //읽어온 정보로 vertex 삽입
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
                else //가중치가 없는 그래프이면 연결이 되어있다는 상태를 1로 표현함
                    weight=1;
                insertEdge(source,target,weight,dir); //읽어온 정보로 edge 삽입
            }
            ifs.close();
        }
        void RandomWalker(string vertex, int length, double q, double arr[])
        {   
            double neighbor_w[MAX_VTXS]; //인접노드와의 가중치 저장하는 배열 (확률계산 위해 생성)
            //auto start = std::chrono::steady_clock::now();
            while(length>0) //인자로 받아온 length만큼만 random walker 이동(length==weight length)
            {   
                bool jump=false;
                double p_sum=0;
                int sum=0;
                double rand=dist(gen); //랜덤 수 뽑음
                if((rand/100)>q) //랜덤 수를 100으로 나누었을 때 q보다 크면 move
                {
                    double rand2=dist(gen); //random walker가 인접행렬로 이동할 때 사용할 랜덤 수 뽑음
                    for(int i=0;i<v_size;i++)
                    {
                        if(vertices[i]->name==vertex)
                        {
                            for(int j=0;j<v_size;j++)
                            {
                                if(adj[i][j]<INF)
                                {
                                    sum+=adj[i][j]; //확률 계산 시 분모가 될 sum
                                    neighbor_w[j]=adj[i][j]; //확률 계산에 사용할 가중치 값 저장
                                }
                                else
                                {
                                    neighbor_w[j]=0; //어차피 random walker는 확률에 의존하여 이동하는데, 연결이 안되어 있어서 못가는 것과 
                                }       //가중치가 0이어서 확률이 0이 되어 못가는 것과 결과적으로 같기 때문에 연결이 안되어 있는 경우도 0으로 넣어줌
                            }
                            if(sum==0) 
                            {
                                jump=true; 
                                //cout<<"can't move\n"; //->Random Walker 실행 과정 보여줌
                                break;
                            }
                        }
                    }
                    for(int j=0;j<v_size;j++)
                    {
                        if(jump==true) break; //move를 할 수 없는 상황(인접노드가 없는 상황)이면 해당 반복문을 나가 jump하게 함
                        if(neighbor_w[j]==-1) continue; //해당 노드와의 가중치 값이 -1이라는 것은 연결되어있지 않다는 것이므로 차례를 넘어감
                        neighbor_w[j]=neighbor_w[j]/sum; //가중치에 따른 확률
                        if(neighbor_w[j]>0){ 
                            p_sum+=neighbor_w[j]; //지금까지의 확률들을 더해서 
                            if(p_sum>=(rand2/100)) //랜덤으로 뽑은 확률이 더한 확률들보다 작거나 같아지게 되는 순간에 해당하는 인접노드로 이동
                            {
                                length--;
                                arr[j]++;
                                //cout<<vertex<<"("<<rand<<")move to ";  //->Random Walker 실행 과정 보여줌
                                vertex=vertices[j]->name;
                                //cout<<vertex<<"\n";
                                break;
                            }
                        }
                    }
                }

                if((jump==true)||((rand/100)<=q))//랜덤 수를 100으로 나누었을 때 q보다 작으면 jump 또는 move를 하지 못해 jump를 해야할 상황이면 jump
                {   
                    /*if(jump==true)
                    {
                        cout<<"so jump!\n";
                    }*/                         //->Random Walker 실행 과정 보여줌
                    double rand2=dist(gen); //random walker가 jump할 때 사용할 랜덤 수 뽑음
                    for(int i=0;i<v_size;i++)
                    {   
                        p_sum+=1.0/v_size; //move와 같은 원리 하지만, jump는 비율이 항상 같음(1/전체 노드의 수)
                        if(p_sum>=(rand2/100))
                        {
                            length--;
                            arr[i]++;
                            //cout<<vertex<<"("<<rand<<")jump to ";  //->Random Walker 실행 과정 보여줌
                            vertex=vertices[i]->name;
                            //cout<<vertex<<"\n";
                            break;
                        }
                    }
                }
            }  
            /*auto end = std::chrono::steady_clock::now(); 
            auto etime = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
            cout<<"Random Walker 소요 시간 : "<<etime<<"\n"; */                                    //->Random Walker 실험 시간 체크
        }
        void PageRank(string vertex, int length, double q)
        {
            double arr[v_size]={0};
            RandomWalker(vertex, length, q, arr); 
            double sum=0;
            int max=0;
            cout.setf(ios::fixed); 
            cout.setf(ios::showpoint);
            cout.precision(5);  //소수점 5자리 수까지 표현
            /*for(int i=0;i<v_size;i++)
            {
                cout<<vertices[i]->name<<" : "<<arr[i]<<" ";
                sum+=arr[i];
            }
            cout<<"총 : "<<sum<<"\n";
            sum=0;*/     //각 노드를 몇번 지나쳤는지 횟수로 보여줌
            cout<<"<pagerank>\n";
            for(int i=0;i<v_size;i++)
            {
                arr[i]=arr[i]/(double)length; //어떤 vertex의 pagerank=random walker가 해당 vertex를 지난 횟수/weight length
                if(arr[max]<arr[i]) max=i; 
                cout<<vertices[i]->name<<" : "<<arr[i]<<" ";
                sum+=arr[i];
            }
            cout<<"\n모든 노드의 pagerank 합 : "<<sum<<"\n";
            cout<<"가장 중요 vertex : "<<vertices[max]->name<<" ("<<arr[max]<<")\n"; 
        }
};

int main()
{
    WAdjMatGraph g1;
    //WAdjMatGraph g2;
    g1.vertex_load("DS2021_PR1_20201786/dataset/starwars/starwars-episode-1-interactions-allCharacters-nodes.tsv");
    g1.edge_load("DS2021_PR1_20201786/dataset/starwars/starwars-episode-1-interactions-allCharacters-links.tsv", false, true);
    g1.PageRank("R2-D2",10000, 0.1);
    /*g2.vertex_load("DS2021_PR1_20201786/dataset/bicycle/station_names.tsv");
    g2.edge_load("DS2021_PR1_20201786/dataset/bicycle/bicycle_trips_over_100.tsv", true, true);
    g2.PageRank("서울역 서부", 100000, 0.1);*/
    return 0;
}