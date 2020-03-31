#include <bits/stdc++.h>
#define FI first
#define SE second
using namespace std;
const int MaxN = 15000; //1e5+10;
const int MaxM = 1200000; //1e7+10;
const int Maxn = 1100; //3e3+10;
string str, graphName;
int N, M, K, maxSize;
pair<int, int> E[MaxM];
int head[MaxN], len;
double startTime, endTime, timeLimit;
struct node {
    int to, next;
} edge[MaxM<<1];
int deg[MaxN], vis[MaxN], dis[MaxN];
int n, nV[MaxN], LB;
bitset<MaxN> Graph[MaxN];
int del[MaxN], ins[MaxN], notAdj[MaxN];
queue<int> que;
vector<int> svex;

void addedge(int from, int to) {
    edge[len].to = to;
    edge[len].next = head[from];
    head[from] = len++;
}

void read() {
    scanf("%d%d", &N, &M);
    for(int i = 0; i < M; i++) {
        scanf("%d%d", &E[i].FI, &E[i].SE);
        --E[i].FI, --E[i].SE;
    }
}

int PreProcess(int S) {
    len = 0;
    memset(head, -1, N*sizeof(int));
    memset(deg, 0, N*sizeof(int));
    for(int i = 0; i < M; ++i) {
        ++deg[E[i].FI], ++deg[E[i].SE];
        addedge(E[i].FI, E[i].SE);
        addedge(E[i].SE, E[i].FI);
    }

    while(!que.empty())
        que.pop();
    memset(vis, 0, N*sizeof(int));

    for(int i = 0; i < N; i++) {
        if(deg[i] < S - K) {
            que.push(i);
            vis[i] = 1;
        }
    }
    while(!que.empty()) {
        int u = que.front();
        que.pop();
        for(int i = head[u]; i != -1; i = edge[i].next) {
            int v = edge[i].to;
            if(vis[v]) continue;
            if(--deg[v] < S - K) {
                que.push(v);
                vis[v] = 1;
            }
        }
    }
    n = 0;
    for(int i = 0; i < N; ++i) {
        if(!vis[i])
            nV[i] = n++;
    }
    if(n < S)
        return 0;

    len = 0;
    memset(deg, 0, n*sizeof(int));
    memset(head, -1, n*sizeof(int));
    for(int i = 0; i < n; ++i)
        Graph[i].reset();

    for(int i = 0; i < M; i++) {
        int u = E[i].FI, v = E[i].SE;
        if(vis[u] || vis[v])
            continue;
        u = nV[u]; v = nV[v];
        ++deg[u], ++deg[v];
        addedge(u, v);
        addedge(v, u);
        Graph[u].set(v);
        Graph[v].set(u);
    }

    memset(del, 0, n*sizeof(int));
    memset(ins, 0, n*sizeof(int));
    memset(notAdj, 0, n*sizeof(int));
    svex.clear();
    LB = S - 1;
    return 1;
}

inline void delfrD(int u) {
    del[u] = 1;
    for(int i = head[u]; i != -1; i = edge[i].next)
        --deg[edge[i].to];
}

inline void addtoD(int u) {
    del[u] = 0;
    for(int i = head[u]; i != -1; i = edge[i].next)
        ++deg[edge[i].to];
}

inline int canadd(int u) {
    int tot = 0;
    for(auto x : svex) {
        if(!Graph[u].test(x)) {
            if((++tot >= K))
                return 0;
            if(notAdj[x] >= K-1)
                return 0;
        }
    }
    return 1;
}

inline void addtoS(int u) {
    ins[u] = 1;
    svex.push_back(u);
    for(int x = 0; x < n; ++x) {
        if(x != u && !Graph[u].test(x))
            ++notAdj[x];
    }
}

inline void delfrS(int u) {
    ins[u] = 0;
    svex.pop_back();
    for(int x = 0; x  < n; ++x) {
        if(x != u && !Graph[u].test(x))
            --notAdj[x];
    }
}

void bfs(int s) {
    memset(dis, -1, n*sizeof(int));
    while(!que.empty())
        que.pop();
    dis[s] = 0;
    que.push(s);
    while(!que.empty()) {
        int u = que.front();
        que.pop();
        for(int i = head[u]; i != -1; i = edge[i].next) {
            int v = edge[i].to;
            if(del[v] || dis[v] != -1)
                continue;
            dis[v] = dis[u] + 1;
            que.push(v);
        }
    }
}

int dfs(int curS) {
    if(curS <= LB)
        return 0;

    int minID = -1;
    vector<int> minDeg;
    for(int i = 0; i < n; ++i) {
        if(del[i])
            continue;
        if(minID == -1 || deg[minID] > deg[i])
            minID = i;
        if(deg[i] < LB + 1 - K) {
            if(ins[i])
                return 0;
            minDeg.push_back(i);
        }
    }

    if(deg[minID] >= curS - K)
        return 1;

    if(minDeg.size()) {
        for(auto x : minDeg) 
            delfrD(x);
        int ret = dfs(curS - minDeg.size());
        for(auto x : minDeg)
            addtoD(x);
        return ret;
    }

    int maxID = -1;
    vector<int> maxNotAdj, delD;
    set<int> todel;
    for(int i = 0; i < n; i++) {
        if(del[i])
            continue;
        if(maxID == -1 || notAdj[maxID] < notAdj[i])
            maxID = i;
        if(notAdj[i] == K-1 && ins[i])
            delD.push_back(i);
        if(notAdj[i] >= K) {
            if(ins[i])
                return 0;
            maxNotAdj.push_back(i);
        }
    }
    if(maxNotAdj.size()) {
        for(auto x : maxNotAdj) 
            delfrD(x);
        int ret = dfs(curS-maxNotAdj.size());
        for(auto x : maxNotAdj)
            addtoD(x);
        return ret;
    }
    if(delD.size()) {
        for(auto u : delD) {
            for(int i = 0; i < n; ++i) {
                if(!del[i] && !ins[i] && !Graph[u].test(i)) 
                    todel.insert(i);
            }
        }
        if(todel.size()) {
            for(auto x : todel)
                delfrD(x);
            int ret = dfs(curS - todel.size());
            for(auto x : todel)
                addtoD(x);
            return ret;
        }
    }

    set<int> sofar;
    for(auto u : svex) {
        bfs(u);
        for(int i = 0; i < n; i++) {
            if(!del[i]) {
                if(dis[i] == -1 || dis[i] > max(2, 2*K-LB)) {
                    if(ins[i])
                        return false;
                    sofar.insert(i);
                }
            }
        }
    }
    if(sofar.size()) {
        for(auto x : sofar)
            delfrD(x);
        int ret = dfs(curS-sofar.size());
        for(auto x : sofar)
            addtoD(x);
        return ret;
    }

    vector<int> branch;
    for(int x = 0; x < n; x++) {
        if(!del[x] && x != minID && !ins[x]) {
            if(!Graph[minID].test(x))
                branch.push_back(x);
        }
    }

    /*cout << "D: ";
    for(int i = 0; i < n; i++) {
        if(del[i] == 0) 
            cout << i << " ";
    }
    cout << endl;
    cout << "S: ";
    for(int i = 0; i < n; i++) {
        if(ins[i] == 1)
            cout << i << " ";
    }
    cout << endl;*/
    
    int flag = 0;
    if(!ins[minID]) {
        delfrD(minID);
        int ret = dfs(curS - 1);
        addtoD(minID);
        if(ret) 
            return 1;
        if(!canadd(minID))
            return 0;
        addtoS(minID);
        flag = 1;
    }

    if(ins[minID]) {
        int canselete = K - 1- notAdj[minID], pos = -1;
        int ret = 0;
        for(int i = 0; !ret && i < canselete; ++i) {
            delfrD(branch[i]);
            if(i) {
                if(!canadd(branch[i-1])) {
                    addtoD(branch[i]);
                    break;
                }
                else {
                    addtoS(branch[i-1]);
                    pos = i-1;
                }
            }
            ret = dfs(curS - 1);
            addtoD(branch[i]);
        }
        if(ret) {
            for(int i = 0; i <= pos; ++i) 
                delfrS(branch[i]);
            if(flag)
                delfrS(minID);
            return 1;
        }
        for(int i = canselete; i < branch.size(); ++i) 
            delfrD(branch[i]);
        if(!canselete || canadd(branch[canselete - 1])) {
            if(canselete) 
                addtoS(branch[canselete - 1]);
            ret = dfs(curS - branch.size() + canselete);
            if(canselete)
                delfrS(branch[canselete-1]);
        }
        for(int i = 0; i <= pos; ++i)
            delfrS(branch[i]);
        if(flag)
            delfrS(minID);
        for(int i = canselete; i < branch.size(); ++i)
            addtoD(branch[i]);
        return ret;
    }
}

void search() {
    int l = K + 1, r = N;
    int ansL = min(N, K), ansR = N;
    while(l <= r) {
        int m = (l + r) >> 1;
        if(!PreProcess(m)) 
            ansR = r = m - 1;
        else {
            if(dfs(n)) {
                ansL = m;
                l = m + 1;
            }
            else
                ansR = r = m - 1;
        }
    }
    maxSize = ansL;
    //printf("%d %d\n", ansL, ansR);
}

void output() {
    string KK = to_string(K);
    std::ofstream ansOfs("result/copyBS/" + graphName + "_ans_" + KK + ".txt");
    ansOfs << "{";
    ansOfs << "algorithm: copyBS, name: " << graphName << ", K: " << K << ", ";
    ansOfs << "size: " << maxSize << ", time: " << fixed<< setprecision(10) << (endTime-startTime)/CLOCKS_PER_SEC;
    ansOfs << "}" << endl;

    /*cout << "{";
    cout << "algorithm: copyBS, name: " << graphName << ", ";
    cout << "size: " << maxSize << ", time: " << fixed<< setprecision(10) << (endTime-startTime)/CLOCKS_PER_SEC;
    cout << "}" << endl;*/
}

int main(int argc, char **argv) {
    freopen(argv[1], "r", stdin); 
    //freopen("ans.txt", "w", stdout);
    str = argv[1];
    K = atoi(argv[2]);
    int tmp = 0;
    for(int i = 0; i < str.length(); i++) {
        if(str[i] == '/')
            graphName = "";
        else if(str[i] == '.' && str[i+1] == 't') 
            break;
        else
            graphName += str[i];
    } 

    read();
    startTime = clock();
    search();
    endTime = clock();
    output();
}