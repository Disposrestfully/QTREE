#include<bits/stdc++.h>
#define read() Read<int>()
namespace pb_ds{   
    namespace io{
        const int MaxBuff=1<<15;
        const int Output=1<<23;
        char B[MaxBuff],*S=B,*T=B;
		#define getc() ((S==T)&&(T=(S=B)+fread(B,1,MaxBuff,stdin),S==T)?0:*S++)
        char Out[Output],*iter=Out;
        inline void flush(){
            fwrite(Out,1,iter-Out,stdout);
            iter=Out;
        }
    }
    template<class Type> inline Type Read(){
        using namespace io;
        register char ch;
        register Type ans=0; 
        register bool neg=0;
        while(ch=getc(),(ch<'0' || ch>'9') && ch!='-');
        ch=='-'?neg=1:ans=ch-'0';
        while(ch=getc(),'0'<= ch && ch<='9') ans=ans*10+ch-'0';
        return neg?-ans:ans;
    }
    template<class Type> inline void Print(register Type x,register char ch='\n'){
        using namespace io;
        if(!x) *iter++='0';
        else{
            if(x<0) *iter++='-',x=-x;
            static int s[100]; 
            register int t=0;
            while(x) s[++t]=x%10,x/=10;
            while(t) *iter++='0'+s[t--];
        }
        *iter++=ch;
    }
}
using namespace pb_ds;
using namespace io;
using namespace std;
typedef long long ll;
const int N=1e5+5;
struct edge{
	int next,to;
}a[N<<1];
int head[N],cnt;
inline void add(int u,int v){
	a[++cnt].to=v;
	a[cnt].next=head[u];
	head[u]=cnt;
}
int n,rt,tot,cot,m;
int fa[N],dep[N],pos[N],lg[N<<1];
int dfn[N<<1],mn[N<<1][18];
int vis[N],sz[N],f[N],col[N];
struct Set{
	priority_queue<int,vector<int>,greater<int> >q1,q2;
	inline void ins(int x){
		if (x>=0) q1.push(x);
	}
	inline void del(int x){
		if (x>=0) q2.push(x);	
	}
	inline int top(){
		while (!q2.empty() && q1.top()==q2.top()) q1.pop(),q2.pop();
		if (q1.empty()) return -1;
		return q1.top();
	}
}s[N];
void dfs(int x,int las){
	dep[x]=dep[las]+1;
	dfn[pos[x]=++tot]=x;
	for (int i=head[x];i;i=a[i].next){
		int y=a[i].to;
		if (y==las) continue;
		dfs(y,x);
		dfn[++tot]=x;
	}
}
inline void RMQ(){
	for (int i=1;i<=tot;++i){
		mn[i][0]=dfn[i];
		lg[i]=i==1?0:lg[i>>1]+1;
	}
	for (int j=1;j<=17;++j)
		for (int i=1;i+(1<<j)-1<=tot;++i)
			mn[i][j]=dep[mn[i][j-1]]<dep[mn[i+(1<<j-1)][j-1]]?mn[i][j-1]:mn[i+(1<<j-1)][j-1];
}
inline int lca(int u,int v){
	int l=pos[u],r=pos[v];
	if (l>r) swap(l,r);
	int k=lg[r-l+1];
	return dep[mn[l][k]]<dep[mn[r-(1<<k)+1][k]]?mn[l][k]:mn[r-(1<<k)+1][k];
}
inline int dist(int u,int v){
	return dep[u]+dep[v]-(dep[lca(u,v)]<<1);
}
void find_root(int x,int las,int size){
	sz[x]=1,f[x]=0;
	for (int i=head[x];i;i=a[i].next){
		int y=a[i].to;
		if (y==las || vis[y]) continue;
		find_root(y,x,size);
		sz[x]+=sz[y],f[x]=max(f[x],sz[y]);
	}
	f[x]=max(f[x],size-sz[x]);
	if (!rt || f[x]<f[rt]) rt=x;
}
void build(int x,int las){
	fa[x]=las,vis[x]=1;
	for (int i=head[x];i;i=a[i].next){
		int y=a[i].to;
		if (vis[y] || y==las) continue;
		rt=0;
		find_root(y,0,sz[y]);
		build(rt,x);
	}
}
int main(){
	freopen("5.in","r",stdin);
	freopen("5.out","w",stdout);
	n=read();
	for (int i=1;i<n;++i){
		int u=read(),v=read();
		add(u,v),add(v,u);
	}
	m=read();
	dfs(1,0),RMQ();
	rt=0;
	find_root(1,0,n);
	build(rt,0);
	while (m--){
		int opt=read(),x=read();
		if (!opt){
			if (col[x]) --cot;
			else ++cot;
			col[x]^=1;
			for (int i=x;i;i=fa[i]){
				if (col[x]) s[i].ins(dist(x,i));
				else s[i].del(dist(x,i));
			}
		}else{
			int ans=n+1;
			if (!cot){
				Print(-1);
				continue;
			}
			for (int i=x;i;i=fa[i])
				if (s[i].top()!=-1) ans=min(ans,dist(x,i)+s[i].top());
			Print(ans);
		}
	}
	flush();
	return 0;
}
