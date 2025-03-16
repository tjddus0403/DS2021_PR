void RandomWalker(string vertex, int length, double q, double arr[])
{   
    double neighbor_w[MAX_VTXS];
    auto start = std::chrono::steady_clock::now();
    while(length>0)
    {   
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
                        cout<<"can't move\n";
                        break;
                    }
                }
            }
            for(int j=0;j<v_size;j++)
            {
                neighbor_w[j]=neighbor_w[j]/sum;
                if(neighbor_w[j]>0){
                    p_sum+=neighbor_w[j];
                    if(p_sum>=(rand2/100))
                    {
                        length--;
                        arr[j]++;
                        cout<<vertex<<"("<<rand<<")move to ";
                        vertex=vertices[j]->name;
                        cout<<vertex<<"\n";
                        break;
                    }
                }
            }
        }
        else
        {   
            double rand2=dist(gen);
            for(int i=0;i<v_size;i++)
            {   
                p_sum+=1.0/v_size;
                if(p_sum>=(rand2/100))
                {
                    length--;
                    arr[i]++;
                    cout<<vertex<<"("<<rand<<")jump to ";
                    vertex=vertices[i]->name;
                    cout<<vertex<<"\n";
                    break;
                }
            }  
        }
    }  
    auto end = std::chrono::steady_clock::now(); 
    auto etime = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
    cout<<"Random Walker 소요 시간 : "<<etime<<"\n";
}