#include <bits/stdc++.h>
#define FI first
#define SE second
using namespace std;
const int MaxN = 1e5+10;
const int MaxM = 1e7+10;

string graphName, str;
int N, M, K, maxSize;
int len, head[MaxN], deg[MaxN];
vector<int> C, ans;
double startTime, endTime, timeLimit;
bitset<MaxN> Graph[MaxN];
int adj[MaxN], inC[MaxN], delP[MaxN];
pair<int, int> E[MaxM];
vector<int> colourClass[MaxN];

struct ed {
    int to, next;
} edge[MaxM<<1];

struct node {
    int index, degree;
    void fun(int x, int y) {
        index = x;
        degree = y;
    }
} V[MaxN];

void read() {
    scanf("%d%d", &N, &M);
    int u, v;
    for(int i = 0; i < M; i++) {
        scanf("%d%d", &E[i].FI, &E[i].SE);
        --E[i].FI, --E[i].SE;
    }
}

void addedge(int from, int to) {
    edge[len].to = to;
    edge[len].next = head[from];
    head[from] = len++;
}

void init() {
    len = 0;
    memset(head, -1, N*sizeof(int));
    memset(deg, 0, N*sizeof(int));
    memset(inC, 0, N*sizeof(int));
    memset(adj, 0, N*sizeof(int));
    maxSize = 0;

    for(int i = 0; i < M; ++i) {
        int u = E[i].FI, v = E[i].SE;
        addedge(u, v), addedge(v, u);
        ++deg[u], ++deg[v];
        Graph[u].set(v);
        Graph[v].set(u);
    }
}

bool cmp(node a, node b) {
    if(a.degree == b.degree)
        return a.index < b.index;
    return a.degree > b.degree;
}

int conflict(int v, int cnt) {
    for(auto u : colourClass[cnt]) {
        if(Graph[u].test(v))
            return 1;
    }
    return 0;
}

void orderVertices(vector<int> &P) {
    for(int i = 0; i < N; i++) 
        V[i].fun(i, deg[i]);
    sort(V, V+N, cmp);
    for(int i = 0; i < N; i++)
        P.push_back(V[i].index);
}

void numberSort(vector<int> &P, vector<int> &upper) {
    int colours = 0, P_cnt = P.size();
    for(int i = 0; i < P_cnt; ++i) 
        colourClass[i].clear();
    for(int i = 0; i < P_cnt; ++i) {
        int v = P[i];
        int cnt = 0;
        while(conflict(v, cnt)) cnt++;
        colourClass[cnt].push_back(v);
        colours = max(colours, cnt+1);
    }
    int sum = 0, val;
    P.clear();
    upper.clear();
    for(int i = 0; i < colours; ++i) {
        int maxNum = 0;
        for(int j = 0; j < colourClass[i].size(); ++j) {
            int v = colourClass[i][j];
            int notadj = K-(C.size()-adj[v]);
            if(j < K)
                val = sum + min(j+1, notadj);
            else
                val = sum + min(K, notadj);
            maxNum = max(maxNum, val);
            upper.push_back(maxNum);
            P.push_back(v);
        }
        sum = maxNum;
    }
}

void addtoC(int v) {
    inC[v] = 1;
    C.push_back(v);
    for(int i = head[v]; i != -1; i = edge[i].next) {
        int x = edge[i].to;
        ++adj[x];
    }
}

void delfrC(int v) {
    inC[v] = 0;
    C.pop_back();
    for(int i = head[v]; i != -1; i = edge[i].next) {
        int x = edge[i].to;
        --adj[x];
    }
}

void updateP(vector<int> &P) {
    int P_cnt = P.size();
    memset(delP, 0, P_cnt*sizeof(int));
    for(auto x : C) {
        int tmp = C.size()-K;
        if(adj[x] == tmp) {
            for(int i = 0; i < P_cnt; ++i) {
                if(!Graph[x].test(P[i]))
                    delP[i] = 1;
            }
        }
    }
    vector<int> newP;
    for(int i = 0; i < P_cnt; ++i) {
        int tmp = C.size()+1-K;
        if(adj[P[i]] >= tmp && !delP[i])
            newP.push_back(P[i]);
    }
    P.clear();
    for(int i = 0; i < newP.size(); ++i) 
        P.push_back(newP[i]);
    vector<int>().swap(newP);
}

int canAdd(int x) {
    int cnt = 0;
    int tmp = C.size()-K;
    if(adj[x] <= tmp)
        return 0;
    for(auto y : C) {
        if(adj[y] == tmp && !Graph[y].test(x))
            return 0;
    }
    return 1;
}

void saveSolution() {
    ans.clear();
    for(auto x : C)
        ans.push_back(x);
    maxSize = C.size();
}

void expand(vector<int> P, vector<int> upper, int level) {
    int tmp = P.size();
    updateP(P);
    numberSort(P, upper);
    tmp = P.size();

    if(!P.size() && C.size() > maxSize)
        saveSolution();

    for(int i = tmp-1; i >= 0; --i) {
        int v = P[i];
        if(C.size() + upper[i] <= maxSize)
            return ;
        if(deg[v] < maxSize-K) {
            P.pop_back();
            if(!P.size() && C.size() > maxSize)
                saveSolution();
            continue;
        }
        addtoC(v);
        P.pop_back();

        expand(P, upper, level+1);
        delfrC(v);
    }
}

void output() {
    string KK = to_string(K);
    std::ofstream ansOfs("result/kplex_Color/" + graphName + "_ans_" + KK + ".txt");
    ansOfs << "{";
    ansOfs << "algorithm: kplex_Color, name: " << graphName << ", K: " << K << ", ";
    ansOfs << "size: " << maxSize << ", time: " << fixed<< setprecision(10) << (endTime-startTime)/CLOCKS_PER_SEC;
    ansOfs << "}" << endl;

    /*std::ofstream ansOfs("../kplex_CP//" + graphName + "_ans.txt");
    for(int i = 0; i < maxSize; i++)
        ansOfs << ans[i]+1 << " ";
    ansOfs << endl;
    ansOfs << "size: " << maxSize << ", time: " << fixed<< setprecision(10) << (endTime-startTime)/CLOCKS_PER_SEC;*/
}

void search() {
    vector<int> P, upper;
    orderVertices(P);
    expand(P, upper, 0);
    vector<int>().swap(P);
    vector<int>().swap(upper);
}

int main(int argc, char **argv) {
    freopen(argv[1], "r", stdin); 
    //freopen("result111.txt", "w", stdout);
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
    init();
    search();
    endTime = clock();
    output();
}