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
int n,dep[N];
char opt[10];
int d[N],anc[N][15];
inline int lca(int u,int v){
	if (dep[u]<dep[v]) swap(u,v);
	for (int i=14;~i;--i)
		if (dep[anc[u][i]]>dep[v]) u=anc[u][i];
	if (dep[u]>dep[v]) u=anc[u][0];
	if (u==v) return u;
	for (int i=14;~i;--i)
		if (anc[u][i]!=anc[v][i])
			u=anc[u][i],v=anc[v][i];
	return anc[u][0];
}
inline int dist(int u,int v){
	return d[u]+d[v]-(d[lca(u,v)]<<1);
}
void dfs(int x,int fa){
	anc[x][0]=fa;
	for (int i=1;i<=14;++i)
		anc[x][i]=anc[anc[x][i-1]][i-1];
	for (int i=head[x];i;i=a[i].next){
		int y=a[i].to;
		if (y==fa) continue;
		d[y]=d[x]+a[i].w,dep[y]=dep[x]+1;
		dfs(y,x);
	}
}
inline int calc(int u,int v,int k){
	--k;
	for (int i=14;~i;--i)
		if (k>=(1<<i)) k-=(1<<i),u=anc[u][i];
	return u;
}
int main(){
	freopen("2.in","r",stdin);
	freopen("2.out","w",stdout);
	int T=read();
	while (T--){
		n=read();
		memset(head,0,sizeof(head));
		memset(anc,0,sizeof(anc));
		cnt=0;
		for (int i=1;i<n;++i){
			int u=read(),v=read(),w=read();
			add(u,v,w),add(v,u,w);
		}
		dfs(1,0);
		while (1){
			scanf("%s",opt+1);
			if (opt[2]=='O') break;
			if (opt[1]=='D'){
				int u=read(),v=read();
				printf("%d\n",dist(u,v));
			}else{
				int u=read(),v=read(),l=lca(u,v),k=read();
				if (dep[u]-dep[l]+1>=k) printf("%d\n",calc(u,l,k));
				else{
					k-=(dep[u]-dep[l]);
					printf("%d\n",calc(v,l,dep[v]-dep[l]-k+2));
				}
			}
		}
	}
	return 0;
}
