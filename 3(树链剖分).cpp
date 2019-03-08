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
using namespace std;
typedef long long ll;
const int N=1e5+5;
int n,m,tot;
int dfn[N],top[N],son[N];
int dep[N],sz[N],fa[N],rk[N];
namespace seg{
	#define ls (x<<1)
	#define rs (ls|1)
	#define mid (l+r>>1)
	int f[N<<2],g[N<<2];
	inline void pushup(int x){
		f[x]=min(f[ls],f[rs]);
		g[x]=min(g[ls],g[rs]);
	}
	void build(int x,int l,int r){
		f[x]=n+1,g[x]=l;
		if (l==r) return;
		build(ls,l,mid),build(rs,mid+1,r);
	}
	void update(int x,int l,int r,int pos){
		if (l==r){
			swap(f[x],g[x]);
			return;
		}
		if (pos<=mid) update(ls,l,mid,pos);
		else update(rs,mid+1,r,pos);
		pushup(x);
	}
	int query(int x,int l,int r,int L,int R){
		if (L<=l && r<=R) return f[x];
		int res=n+1;
		if (L<=mid) res=min(res,query(ls,l,mid,L,R));
		if (res<=n) return res;
		if (R>mid) res=min(res,query(rs,mid+1,r,L,R));
		return res;
	}
}
using namespace seg;
struct edge{
	int next,to;
}a[N<<1];
int head[N],cnt;
inline void add(int u,int v){
	a[++cnt].to=v;
	a[cnt].next=head[u];
	head[u]=cnt;
}
void dfs1(int x){
	dep[x]=dep[fa[x]]+1,sz[x]=1;
	for (int i=head[x];i;i=a[i].next){
		int y=a[i].to;
		if (y==fa[x]) continue;
		fa[y]=x,dfs1(y),sz[x]+=sz[y];
		if (sz[y]>sz[son[x]]) son[x]=y;
	}
}
void dfs2(int x){
	rk[dfn[x]=++tot]=x;
	if (x^son[fa[x]]) top[x]=x;
	else top[x]=top[fa[x]];
	if (son[x]) dfs2(son[x]);
	for (int i=head[x];i;i=a[i].next){
		int y=a[i].to;
		if (y==fa[x] || y==son[x]) continue;
		dfs2(y);
	}
}
inline int find(int x){
	int res=n+1;
	while (x){
		res=min(res,query(1,1,n,dfn[top[x]],dfn[x]));
		x=fa[top[x]];
	}
	return rk[res];
}
int main(){
	freopen("3.in","r",stdin);
	freopen("3.out","w",stdout);
	n=read(),m=read();
	for (int i=1;i<n;++i){
		int u=read(),v=read();
		add(u,v),add(v,u);
	}	
	dfs1(1),dfs2(1);
	build(1,1,n),rk[n+1]=-1;
	while (m--){
		int opt=read(),u=read();
		if (!opt) update(1,1,n,dfn[u]);
		else printf("%d\n",find(u));
	}
	return 0;
}
