#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1e4+5;
inline int read(){
	int x=0,w=1;
	char ch=0;
	while (ch<'0' || ch>'9'){
		ch=getchar();
		if (ch=='-') w=-1;	
	}
	while (ch<='9' && ch>='0'){
		x=(x<<1)+(x<<3)+ch-'0';
		ch=getchar();
	}
	return x*w;
}
namespace seg{
	#define ls (x<<1)
	#define rs (ls|1)
	#define mid (l+r>>1)
	int mx[N<<2];
	inline void pushup(int x){
		mx[x]=max(mx[ls],mx[rs]);
	}
	void update(int x,int l,int r,int pos,int k){
		if (l==r){
			mx[x]=k;
			return;
		}
		if (pos<=mid) update(ls,l,mid,pos,k);
		else update(rs,mid+1,r,pos,k);
		pushup(x);
	}
	int query(int x,int l,int r,int L,int R){
		if (L>R) return -1e9;
		if (L<=l && r<=R) return mx[x];
		int res=-1e9;
		if (L<=mid) res=max(res,query(ls,l,mid,L,R));
		if (R>mid) res=max(res,query(rs,mid+1,r,L,R));
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
int n,tot;
char opt[10];
int st[N],ed[N],w[N];
int fa[N],dep[N],dfn[N];
int top[N],sz[N],son[N];
void dfs1(int x){
	dep[x]=dep[fa[x]]+1;
	sz[x]=1;
	for (int i=head[x];i;i=a[i].next){
		int y=a[i].to;
		if (y==fa[x]) continue;
		fa[y]=x,dfs1(y);
		sz[x]+=sz[y];
		if (sz[y]>sz[son[x]]) son[x]=y;
	}
}
void dfs2(int x){
	dfn[x]=++tot;
	if (x==son[fa[x]]) top[x]=top[fa[x]];
	else top[x]=x;
	if (son[x]) dfs2(son[x]);
	for (int i=head[x];i;i=a[i].next){
		int y=a[i].to;
		if (y==fa[x] || y==son[x]) continue;
		dfs2(y);
	}
	
}
int main(){
	freopen("1.in","r",stdin);
	freopen("1.out","w",stdout);
	int T=read();
	while (T--){
		memset(head,0,sizeof(head));
		memset(son,0,sizeof(son));
		cnt=tot=0;
		n=read();
		for (int i=1;i<n;++i){
			st[i]=read(),ed[i]=read(),w[i]=read();
			add(st[i],ed[i]),add(ed[i],st[i]);	
		}
		dfs1(1),dfs2(1);
		for (int i=1;i<n;++i){
			if (dep[st[i]]>dep[ed[i]]) swap(st[i],ed[i]);
			update(1,1,n,dfn[ed[i]],w[i]);
		}
		while (1){
			scanf("%s",opt+1);
			if (opt[1]=='D') break;
			if (opt[1]=='C'){
				int x=read(),y=read();
				w[x]=y;
				update(1,1,n,dfn[ed[x]],w[x]);
			}else{
				int u=read(),v=read(),ans=-1e9;
				while (top[u]^top[v]){
					if (dep[top[u]]<dep[top[v]]) swap(u,v);
					ans=max(ans,query(1,1,n,dfn[top[u]],dfn[u]));
					u=fa[top[u]];
				}
				if (dep[u]>dep[v]) swap(u,v);
				ans=max(ans,query(1,1,n,dfn[u]+1,dfn[v]));
				printf("%d\n",ans);
			}
		}
	}
	return 0;
}
