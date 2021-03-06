#ifndef CSPALLOC_PATHALG_H
#include"limits.h"
#define CSPALLOC_PATHALG_H
#define INFCOST INT_MAX/2
#include<bits/stdc++.h>
#include <unistd.h>
#include"edge.h"
#include<sys/time.h>
#include<queue>
#include"Heap.h"
#include <algorithm>
#define ML 50
#define BS 5
#define WD 8
#ifndef LY 
	#define LY 100
#endif
#define PC 2
#define LY1 50
#define LY2 50
#define YE 2
#define IFHOP 1
#define inf INT_MAX/2
using namespace std;
class algbase {
    protected:
        vector<int> getrout(int &s, int &t, vector<edge> &edges, vector<int> &pre) {
            vector<int> rout;
            int pp = pre[t];
            while (pp >= 0) {
                rout.push_back(pp);
                pp = pre[edges[pp].s];
            }
            reverse(rout.begin(), rout.end());
            return rout;
        }
    public:
        algbase(){};
        virtual bool cutcake(int)=0;
        virtual void updatE(vector<int>esigns){};
	 	virtual void updatS(vector<vector<pair<int,int>>>&stpair){};	 	 
        virtual void init(pair<vector<edge>,vector<vector<int>>>extenedges,vector<pair<int,int>>stpair,vector<vector<int>>&relate,ginfo)=0;
	 	virtual vector<vector<int>> routalg(int s,int t,int bw)=0;
	 	virtual pair<int,int> prepush(int s,int t,int bw)=0;
};
class PBellmanor:public algbase{
    public:
		vector<vector<int>>neibour;
		vector<int>ancestor;
		int edgesize,nodenum,pesize,pnodesize,maxbw;
		vector<vector<vector<int>>>mask;
		vector<edge>edges;
		vector<int>dist;
		vector<int>pre;
		vector<int>leveln;
		vector<int>exn2n;
		vector<vector<int>>rela;
		vector<int>order;
		vector<vector<vector<int>>>neie;
		vector<vector<vector<int>>>nein;
		vector<vector<vector<int>>>neiw;
		vector<int>esign;
		vector<pair<int,int>>stes;
		int W;
		vector<vector<pair<int,int>>>stpairs;
		vector<int>L;
		PBellmanor():L(3,0){};
        virtual void updatS(vector<vector<pair<int,int>>>&stpair){
        	stpairs=stpair;
        	L[0]=0;
        	L[1]=LY1;
        	L[2]=LY;
        }
        void topsort()
        {
        }
        virtual bool cutcake(int index){
        }
        virtual void init(pair<vector<edge>,vector<vector<int>>>extenedges,vector<pair<int,int>>stpair,vector<vector<int>>&relate,ginfo ginf){
        	maxbw=500;
        	rela=relate;
        	stes=stpair;
        	edges=extenedges.first;
        	vector<vector<int>>esigns=extenedges.second;
        	edgesize=edges.size(),nodenum=ginf.enodesize,pesize=ginf.pesize,pnodesize=ginf.pnodesize;
        	nodenum=pnodesize;
        	exn2n=ginf.exn2n;
        	vector<vector<int>>nd(nodenum*LY,vector<int>());
        	neibour=nd;
        	vector<int>ad(nodenum*LY,0);
        	ancestor=ad;	
    		vector<int>bl(BS,0);
    		leveln=bl;
    		vector<vector<vector<int>>>nm(BS,vector<vector<int>>());
    		mask=nm;
    		vector<int>dd(nodenum*LY,inf);
    		dist=dd;
    		vector<int>pp(nodenum*LY,-1);
    		pre=pp;
			pesize=edges.size();
			W=WD+1;
			for(int k=0;k<LY;k++)
			{
				vector<vector<int>>tmpn(pnodesize,vector<int>());
				vector<vector<int>>tmpe(pnodesize,vector<int>());
				for(int i=0;i<edges.size();i++)
					{
						int s=edges[i].s;
						int t=edges[i].t;
						tmpn[s].push_back(t);
						tmpe[s].push_back(esigns[k][i]);
					}
				
				neie.push_back(tmpe);
				nein.push_back(tmpn);
			}
			cout<<"good so far "<<endl;
        }
        virtual vector<vector<int>> routalg(int s,int t,int bw){
        		cout<<"in rout alg"<<endl;
				cout<<"nodenum: "<<nodenum<<endl;
				cout<<"edge size :"<<edges.size()<<endl;
        		time_t start,end;
        		start=clock();
        		vector<vector<int>>result(LY,vector<int>());
        		for(int y=1;y<PC;y++)
					for(int k=L[y-1];k<L[y];k++)
					{
						int tnode=-1;
						vector<int>d(nodenum,INT_MAX);
						vector<int>peg(nodenum,-1);
						vector<int>flag(nodenum,0);
						for(int l=0;l<stpairs[y-1].size();l++)
						{	
							int s=stpairs[y-1][l].first;
							int t=stpairs[y-1][l].second;
							for (int i = 0;i<nodenum;i++)
								if (i == s)
									d[i]=0;
								else
									d[i]=INT_MAX/2;
							for (int i=0; i<nodenum; i++)
							{
								flag[i]=0;
								peg[i]=-1;
							}
							int cur = s;
							Heap heap(nodenum);
							for (int i = 0;i<nodenum;i++)
								heap.push(i, d[i]);
							do{
								int cur = heap.pop();
								flag[cur] = 1;
								if (cur == t)
									{	
										tnode=t;
										break;
									}
								int size = nein[k][cur].size();
								for (int i = 0;i<size; i++){
										int to=nein[k][cur][i];
										if (flag[to] ==0&&d[to]>(d[cur]+neie[k][cur][i])&&neie[k][cur][i]>0){
											d[to] = d[cur]+neie[k][cur][i];
											heap.update(to, d[to]);
											peg[to]=cur;
									}
								}
							} while (!heap.empty());
							if(tnode>=0)
							{
								int prn=tnode;
								while(prn!=s)
								{
									//cout<<prn<<" ";
									prn=peg[prn];
								}
								//cout<<prn<<" ";
							}
							//cout<<endl;
						}
					}
        		end=clock();
        		cout<<"cpu time is: "<<end-start<<endl;
        		cout<<"good sofor"<<endl;
        		return vector<vector<int>>();
	 	}
        static bool compare(pair<int,int>&a,pair<int,int>&b)
        {
        	if(a.second<b.second)
        		return true;
        	return false;
        }
        virtual pair<int,int> prepush(int s,int t,int bw)
        {
        };
};
class Bellmanor:public algbase
{
	private:
		edge *dev_edges,*aedges;
		int*dev_m1,*m1,*dev_pre,*pre,*pred,*dev_pred,*dev_d,*d,*dev_mask,*mask,*dev_leveln,*leveln;
		int*dev_m2,*m2;
		int*dev_rela,*rela;
		int presize,dsize,masksize,levelnsize;
		int edgesize,nodenum,pesize,pnodesize;
		int neisize,duansize;
		int *choosel,*dev_choosel;
		int *rout,*dev_rout;
		int *routn,*dev_routn,*order,*dev_order;
		vector<int>hleveln,ancestor;
		int maxbw;
		int *dev_qian,*qian,*dev_qbeg,*qbeg,*dev_qsize,*qsize;
		epair *dev_nei,*nei;
		int *dev_duan,*duan;
		int *dev_beg,*beg;
		int *dev_value,*value;
		int *dev_height,*height;
		int *dev_has,*has;
		int*w,*dev_w;
		vector<vector<int>>neibour;
		vector<edge>edges;
		vector<int>ordernode;
		void allocate(int maxn,int maxedges);
		void copydata(int s,vector<edge>&edges,int nodenum);
		void dellocate();
		int W;
		int *st,*te,*dev_st,*dev_te;
		int *chan,*dev_chan;
		int*p,*dev_p;
		int*esignes;
		vector<vector<int>>neibn;
		int *mark,*dev_mark;
		vector<pair<int,int>>stp;
		//
		vector<int>L;
		vector<int>S;
		vector<int>NF;
		int ncount;
		vector<int>size;
		vector<int>nodeoff;
		vector<int>leveloff;
		
	public:
		 Bellmanor();
	 	 void topsort();
	 	 virtual pair<int,int> prepush(int s,int t,int bw){};
	 	 virtual bool cutcake(int index){};
	 	 virtual vector<vector<int>> routalg(int s,int t,int bw);
	 	 virtual ~Bellmanor(){}
	 	 virtual void updatE(vector<int>esigns);
	 	 virtual void updatS(vector<vector<pair<int,int>>>&stpair);	 	 
	 	 virtual void init(pair<vector<edge>,vector<vector<int>>>extenedges,vector<pair<int,int>>stpair,vector<vector<int>>&relate,ginfo ginf);
};
class PBFSor:public algbase{
    public:
		vector<vector<int>>neibour;
		vector<int>ancestor;
		int edgesize,nodenum,pesize,pnodesize,maxbw;
		vector<vector<vector<int>>>mask;
		vector<edge>edges;
		vector<int>dist;
		vector<int>pre;
		vector<int>leveln;
		vector<int>exn2n;
		vector<vector<int>>rela;
		vector<int>order;
		vector<vector<vector<int>>>neie;
		vector<vector<vector<int>>>nein;
		vector<int>esign;
		vector<pair<int,int>>stes;
		vector<vector<pair<int,int>>>stpairs;
		vector<int>L;
		int W;
		PBFSor(){};
        void topsort()
        {
        }
        virtual bool cutcake(int index){
        }
        virtual void init(pair<vector<edge>,vector<vector<int>>>extenedges,vector<pair<int,int>>stpair,vector<vector<int>>&relate,ginfo ginf){
        	maxbw=500;
        	rela=relate;
        	stes=stpair;
        	cout<<"in pathalg init "<<stes.size()<<endl;
        	edges=extenedges.first;
        	vector<vector<int>>esigns=extenedges.second;
        	edgesize=edges.size(),nodenum=ginf.enodesize,pesize=ginf.pesize,pnodesize=ginf.pnodesize;
        	exn2n=ginf.exn2n;
        	vector<vector<int>>nd(nodenum*LY,vector<int>());
        	neibour=nd;
        	vector<int>ad(nodenum*LY,0);
        	ancestor=ad;	
    		vector<int>bl(BS,0);
    		leveln=bl;
    		vector<vector<vector<int>>>nm(BS,vector<vector<int>>());
    		mask=nm;
    		vector<int>dd(nodenum*LY,inf);
    		dist=dd;
    		vector<int>pp(nodenum*LY,-1);
    		pre=pp;
			pesize=edges.size();
			W=WD+1;
			for(int k=0;k<LY;k++)
			{
				vector<vector<int>>tmpn(pnodesize,vector<int>());
				vector<vector<int>>tmpe(pnodesize,vector<int>());
				for(int i=0;i<edges.size();i++)
					{
						int s=edges[i].s;
						int t=edges[i].t;
						tmpn[s].push_back(t);
						tmpn[t].push_back(s);
						tmpe[s].push_back(esigns[k][i]);
						tmpe[t].push_back(esigns[k][i]);
					}
				
				neie.push_back(tmpe);
				nein.push_back(tmpn);
			}
        }
        virtual vector<vector<int>> routalg(int s,int t,int bw){
        	cout<<"in rout alg"<<endl;
			time_t start,end;
			start=clock();
			vector<vector<int>>result(LY,vector<int>());
			for(int k=0;k<LY;k++)
			{
				for(int l=0;l<stes.size();l++)
				{
					int tnode=-1;
					int tv=WD+1;
					vector<int>dist(pnodesize,INT_MAX);
					vector<int>pre(pnodesize,-1);
					int vflag=1;
					priority_queue<pair<int, int>,vector<pair<int,int>>,std::less<std::pair<int, int>>>que;
					int s=stes[l].first;
					int t=stes[l].second;
					que.push(make_pair(s,0));
					dist[s]=0;
					while(!que.empty()&&vflag)
					{
						int node=que.top().first;
						int v=que.top().second;
						que.pop();
						for(int i=0;i<nein[k][node].size();i++)
						{
							if(neie[k][node][i]>0)
							{	
								int to=nein[k][node][i];
								if(dist[to]>dist[node]+1)
								{
									pre[to]=node;
									dist[to]=dist[node]+1;
									que.push(make_pair(to,dist[to]));
								}
								else
									continue;
								if(to==t){tnode=to;tv=v+1;vflag=0;break;}
							}
						}
					}
					int prn=tnode;
					int len=0;
					//cout<<"tnode is: "<<tnode<<endl;
					if(tnode>=0)
					{
						int prn=tnode;
						while(prn!=s)
						{
							//cout<<prn<<" ";
							prn=pre[prn];
						}
						//cout<<prn<<" ";
					}
					//cout<<endl;
					result[k].push_back(tv);
				}
			}
			end=clock();
			cout<<"cpu time is: "<<end-start<<endl;
			cout<<"good sofor"<<endl;
			return vector<vector<int>>();
	 	}
        static bool compare(pair<int,int>&a,pair<int,int>&b)
        {
        	if(a.second<b.second)
        		return true;
        	return false;
        }
        virtual pair<int,int> prepush(int s,int t,int bw)
        {
        };
};
class BFSor:public algbase
{
	private:
		edge *dev_edges,*aedges;
		int*dev_m,*m,*dev_pre,*pre,*pred,*dev_pred,*dev_d,*d,*dev_mask,*mask,*dev_leveln,*leveln;
		int*dev_rela,*rela;
		int presize,dsize,masksize,levelnsize;
		int edgesize,nodenum,pesize,pnodesize;
		int neisize,duansize;
		int *choosel,*dev_choosel;
		int *rout,*dev_rout;
		int *routn,*dev_routn,*order,*dev_order;
		vector<int>hleveln,ancestor;
		int maxbw;
		int *dev_qian,*qian,*dev_qbeg,*qbeg,*dev_qsize,*qsize;
		epair *dev_nei,*nei;
		int *dev_duan,*duan;
		int *dev_beg,*beg;
		int *dev_value,*value;
		int *dev_height,*height;
		vector<vector<int>>neibour;
		vector<edge>edges;
		vector<int>ordernode;
		void allocate(int maxn,int maxedges);
		void copydata(int s,vector<edge>&edges,int nodenum);
		void dellocate();
		int W;
		int *st,*te,*dev_st,*dev_te;
		int *chan,*dev_chan;
		int*esignes;
		vector<vector<int>>neibn;
		int *mark,*dev_mark;
		vector<pair<int,int>>stp;
	public:
		 BFSor();
	 	 void topsort();
	 	 virtual pair<int,int> prepush(int s,int t,int bw){};
	 	 virtual bool cutcake(int index){};
	 	 virtual vector<vector<int>> routalg(int s,int t,int bw);
	 	 virtual ~BFSor(){dellocate();}
	 	 virtual void init(pair<vector<edge>,vector<vector<int>>>extenedges,vector<pair<int,int>>stpair,vector<vector<int>>&relate,ginfo ginf);
};
#endif //CSPALLOC_PATHALG_H
