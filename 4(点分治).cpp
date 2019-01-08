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
const int inf=2e9+10;
int n,m,tot,rt,now,root;
int dep[N],d[N],vis[N];
int dfn[N<<1],pos[N],col[N];
int mn[N<<1][18],lg[N<<1],cot;
int sz[N],fa[N],f[N];
struct edge{
	int next,to,w;
}a[N<<1];
int head[N],cnt;
inline void add(int u,int v,int w){
	a[++cnt].to=v;
	a[cnt].next=head[u];
	head[u]=cnt;
	a[cnt].w=w;
}
struct set{
	priority_queue<int>q1,q2;
	inline void ins(int x){
		if (x>-inf) q1.push(x);
	}
	inline void del(int x){
		if (x>-inf) q2.push(x);
	}
	inline int top(){
		while (!q2.empty() && q2.top()==q1.top())
			q1.pop(),q2.pop();
		if (q1.empty()) return -inf;
		return q1.top();
	}
	inline int sec(){
		int x=top();
		if (x==-inf) return -inf;
		del(x);
		int res=top();ins(x);
		return res;
	}
	inline int sum(){
		int l1=top(),l2=sec();
		if (l1==-inf || l2==-inf) return -inf;
		return l1+l2;
	}
}s[N],mx[N],ans;
void dfs(int x,int fa){
	dfn[pos[x]=++tot]=x;
	dep[x]=dep[fa]+1;
	for (int i=head[x];i;i=a[i].next){
		int y=a[i].to;
		if (y==fa) continue;
		d[y]=d[x]+a[i].w,dfs(y,x);
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
			mn[i][j]=dep[mn[i][j-1]]<dep[mn[i+(1<<(j-1))][j-1]]?mn[i][j-1]:mn[i+(1<<(j-1))][j-1];
}
inline int lca(int u,int v){
	int l=pos[u],r=pos[v];
	if (r<l) swap(l,r);
	int k=lg[r-l+1];
	return dep[mn[l][k]]<dep[mn[r-(1<<k)+1][k]]?mn[l][k]:mn[r-(1<<k)+1][k];
}
inline int dist(int u,int v){
	return d[u]+d[v]-(d[lca(u,v)]*2);
}
void find_root(int x,int las,int size){
	sz[x]=1;
	f[x]=0;
	for (int i=head[x];i;i=a[i].next){
		int y=a[i].to;
		if (vis[y] || y==las) continue;	
		find_root(y,x,size),sz[x]+=sz[y];
		f[x]=max(f[x],sz[y]);
	}
	f[x]=max(f[x],size-sz[x]);
	if (!rt || f[x]<f[rt]) rt=x;
}
void solve(int x,int las,int ww){
	fa[x]=las;vis[x]=1;
	for (int i=head[x];i;i=a[i].next){
		int y=a[i].to;
		if (vis[y] || y==las) continue;
		rt=0;	
		find_root(y,0,sz[x]);
		solve(rt,x,a[i].w);
	}
}
int main(){
	freopen("4.in","r",stdin);
	freopen("4.out","w",stdout);
	cot=n=read();
	for (int i=1;i<n;++i){
		int u=read(),v=read(),w=read();
		add(u,v,w),add(v,u,w);
	}
	dfs(1,0),RMQ();
	rt=0,find_root(1,0,n);
	root=rt;
	solve(rt,0,0);
	for (int i=1;i<=n;++i)
		for (int j=i;fa[j];j=fa[j])
				s[j].ins(dist(i,fa[j]));
	for (int i=1;i<root;++i)
		mx[fa[i]].ins(s[i].top());
	for (int i=root+1;i<=n;++i)
		mx[fa[i]].ins(s[i].top());
	for (int i=1;i<=n;++i)
		ans.ins(mx[i].sum()),ans.ins(mx[i].top());
	m=read();
	while (m--){
		char ch=0;
		while (ch!='A' && ch!='C') ch=getc();
		if (ch=='A'){
			int Ans=ans.top();
			if (!cot) printf("They have disappeared.\n");
			else if (cot==1) printf("0\n");
			else printf("%d\n",max(Ans,0));
		}else{
			int x=read();
			if (col[x]){
				++cot;
				ans.ins(mx[x].top());
				for (int i=x;fa[i];i=fa[i]){
					ans.del(mx[fa[i]].sum());
					if (!col[fa[i]]) ans.del(mx[fa[i]].top());
					mx[fa[i]].del(s[i].top());	
					s[i].ins(dist(x,fa[i]));
					mx[fa[i]].ins(s[i].top());	
					ans.ins(mx[fa[i]].sum());
					if (!col[fa[i]]) ans.ins(mx[fa[i]].top());
				}
			}else{
				--cot;
				ans.del(mx[x].top());
				for (int i=x;fa[i];i=fa[i]){
					ans.del(mx[fa[i]].sum());
					if (!col[fa[i]]) ans.del(mx[fa[i]].top());
					mx[fa[i]].del(s[i].top());	
					s[i].del(dist(x,fa[i]));
					mx[fa[i]].ins(s[i].top());	
					ans.ins(mx[fa[i]].sum());
					if (!col[fa[i]]) ans.ins(mx[fa[i]].top());
				}
			}
			col[x]^=1;
		}
	}
	return 0;
}
