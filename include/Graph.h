#ifndef CSPALLOC_GRAPH_H
#define CSPALLOC_GRAPH_H
#include<bits/stdc++.h>
#include"edge.h"
#include"pathalg.h"
#include<set>
using namespace std;
enum SPWAY {NORMAL,ROUTE,ROTATE,ROTATE_DELETE,PUSH};
struct levelGraph {
    vector<int>startnodes;
    vector<int>startedges;
    int nodenum=-1;
    int edgenum=-1;
    vector<edge>edges;
    vector<vector<edge>>neartable;
    int levelsize;
};
struct fedge{
	int from,to,price,cap;
	fedge(int f,int t,int p,int c):from(f),to(t),price(p),cap(c){};
};
class Graph
{
    public:
        double remain;
        vector<edge>edges,extenedges;
        vector<vector<edge>>extable;
        unordered_map<int,levelGraph>levels ;
        int n,width,maxnode,maxedge;
        vector<int>etn2n,exe2e;
        vector<vector<int>>relate,neartable;
        algbase&router1;
        algbase&router2;
        pair<int,int>prepush(int s,int t,int n,ofstream& out)
        {
        	//pair<int,int>a=router1.prepush(s,t,0);
        	//pair<int,int>b=router2.prepush(s,t,0);
        	return make_pair(0,0);
        }
        pair<int,int>routalg(int s,int t,int bw)
		{
        	vector<vector<pair<int,int>>>stpair(2,vector<pair<int,int>>());
        	stpair[0].push_back(make_pair(0,1));
        	stpair[0].push_back(make_pair(3,2));
        	stpair[1].push_back(make_pair(1,2));
        	stpair[1].push_back(make_pair(4,2));
        	router1.updatS(stpair);
			vector<vector<int>>b=router1.routalg(0,0,0);
        	router2.updatS(stpair);
			vector<vector<int>>a=router2.routalg(0,0,0);
			int flag=0;
			return make_pair(0,0);
		}
        virtual ~Graph(){};
    protected:
        void addedge(int _s,int _t,int _w,double _bw=500){
            neartable[_s].push_back(edges.size());
            edges.push_back(edge(_s,_t,1));
            neartable[_t].push_back(edges.size());
            edges.push_back(edge(_t,_s,1));
        };
        virtual void GenGraph()=0;
        Graph(int _n,int _degree,algbase&alg1,algbase&alg2):n(_n),width(WD),remain(500),etn2n(n*(width+1),-1),maxnode(0),router1(alg1),router2(alg2),neartable(_n,vector<int>()){
        };
        pair<vector<edge>,vector<vector<int>>> extend()
        {
        	vector<vector<int>>rs(edges.size(),vector<int>());
        	relate=rs;
        	vector<int>es(edges.size()*(width+1),-1);
        	exe2e=es;
        	for(int i=0;i<neartable.size();i++)
				for(int k=0;k<width;k++)
					for(int j=0;j<neartable[i].size();j++)
						{
							if(edges[neartable[i][j]].w+k<=width)
							{
								edge e=edges[neartable[i][j]];
								int s=i+n*k;
								etn2n[s]=i;
								int t=e.t+n*(k+1);
								etn2n[t]=e.t;
								exe2e[extenedges.size()]=neartable[i][j];
								relate[neartable[i][j]].push_back(extenedges.size());
								extenedges.push_back(edge(s,t,e.w));
								if(s>maxnode||t>maxnode)
									maxnode=max(s,t);
							}
						}
        	vector<vector<int>>erelate(extenedges.size(),vector<int>());
        	for(int i=0;i<extenedges.size();i++)
        		erelate[i]=relate[exe2e[i]];
            maxedge=extenedges.size()-1;
            vector<edge> pedges;
            for(int i=0;i<edges.size();i++)
            {
                for(int k=0;k<width;k++)
                	{
                		int s=edges[i].s+n*k;
                		int t=edges[i].t+n*(k+1);
                		pedges.push_back(edge(s,t,1));
                	}
            }
            vector<vector<int>>near(maxnode,vector<int>());
            for(int i=0;i<edges.size();i++)
            {
            	int s=edges[i].s;
            	int t=edges[i].t;
            	near[s].push_back(edges[i].t);
            }
            vector<edge>redges;
            for(int i=0;i<near.size();i++)
            	for(int j=0;j<near[i].size();j++)
            		redges.push_back(edge(i,near[i][j],1));
            cout<<"asasd"<<endl;
            
            vector<vector<int>>esigns(LY,vector<int>());
            for(int i=0;i<LY;i++)
            	for(int j=0;j<2*edges.size();j++)
            	{
            		
            		//int ran=rand()%100;
            		//if(ran<20)
            			//esigns[i].push_back(-1);
            		//else
            			esigns[i].push_back(rand()%10+1);
            	}
            //assdsasd.
            int W=WD+1;
            vector<vector<int>>nesigns(LY,vector<int>());
            vector<edge>nedges;
            n=n*W;
            for(int i=0;i<redges.size();i++)
            {
            	int s=redges[i].s;
            	int t=redges[i].t;
            	for(int j=0;j<W;j++)
            	{
            		int s=s*W+i;
            		int t=t*W+i+1;
            		nedges.push_back(edge(s,t,1));
            	}
            }
            for(int k=0;k<LY;k++)
				for(int i=0;i<redges.size();i++)
				{
					int s=redges[i].s;
					int t=redges[i].t;
					for(int j=0;j<W;j++)
					{
						int s=s*W+i;
						int t=t*W+i+1;
						nedges.push_back(edge(s,t,1));
						nesigns[k].push_back(esigns[k][i]);
					}
				}
            //asdasdasdasd.
            vector<pair<int,int>> stpair;
            int count=0;
            for(int i=0;i<YE;i++)
            {
            	int t=i;
            	while(t==i)t=rand()%(n/W);
            	stpair.push_back(make_pair(i,t));
            }
            /*vector<pair<int,int>> exstpair;
            for(int i=0;i<YE;i++)
            	{
            		int s=stpair[i].first;
            		int t=stpair[i].second;
            		exstpair.push_back(make_pair(s*W,t*W));
            	}
            pair<vector<edge>,vector<vector<int>>>exte,exce;
            if(IFHOP>0)
            	{
            	router1.init(make_pair(nedges,nesigns),exstpair,erelate,ginfo(maxedge+1,edges.size(),n,maxnode+1,etn2n));
            	router2.init(make_pair(nedges,nesigns),exstpair,erelate,ginfo(maxedge+1,edges.size(),n,maxnode+1,etn2n));
            	}*/
            //else
            {
            	router1.init(make_pair(redges,esigns),stpair,erelate,ginfo(maxedge+1,edges.size(),n/W,maxnode+1,etn2n));
            	router2.init(make_pair(redges,esigns),stpair,erelate,ginfo(maxedge+1,edges.size(),n/W,maxnode+1,etn2n));
            }
            return make_pair(redges,esigns);
        };
};
class ERGraph:public Graph{
public:
    ERGraph(int _n,int _degree,algbase&alg1,algbase&alg2):Graph(_n,_degree,alg1,alg2){
    	cout<<"before gen graph"<<endl;
    	GenGraph();
    	cout<<"gen graph success"<<endl;
    	extend();
    };
private:
    virtual void GenGraph(){
        int count = 0;
        set<pair<int, int>>flag;
        double threhod = 6*n/((n-1));
        for (int i = 0; i <n; i++)
            for (int j =i+1; j<n;j++)
                if (i != j)
                {
                    double ran=(double)(rand()%n);
                    if (ran<threhod)
                    {
                        if (flag.find(make_pair(i, j))==flag.end())
                        {
                            addedge(i,j,rand()%3+1);
                            flag.insert(make_pair(i,j));
                            flag.insert(make_pair(j,i));
                        }
                    }
                }
    };
private:
    void checkflow(int src,int dst,enum SPWAY cache = NORMAL)
    {}
};
class BAGraph:public Graph{
public:
    BAGraph(int _n,int _degree,algbase&alg1,algbase&alg2):Graph(_n,_degree,alg1,alg2){GenGraph();extend();};
private:
    virtual void GenGraph(){
        int todu = 0;
        int count = 0;
        int k =3;
        vector<int>du(n,0);
        for (int i = 0; i < 5; i++)
        {
            addedge(i,i+1,rand()%3+1);
            du[i]++;
            du[i + 1]++;
            todu += 2;
        }
        for (int i = 5; i < n; i++)
        {
            int addin = 0;
            while (addin < k)
            {
                for (int j = 0; j < n; j++)
                {
                    if (i!=j&&rand() % todu < du[j])
                    {
                        addedge(i,j,rand()%3+1);
                        du[i]++;
                        du[j]++;
                        todu += 2;
                        addin++;
                    }
                    if (addin >= k)
                        break;
                }
            }
        }
    };
};
#endif
