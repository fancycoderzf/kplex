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
pair<int, int> E[MaxN];

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

void orderVertices(vector<int> &P) {
    for(int i = 0; i < N; i++)
        V[i].fun(i, deg[i]);
    sort(V, V+N, cmp);
    for(int i = 0; i < N; i++)
        P.push_back(V[i].index);
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

void updateP(vector<int> P, vector<int> &newP) {
    memset(delP, 0, P.size()*sizeof(int));
    for(auto x : C) {
        int tmp = C.size()-K;
        if(adj[x] == tmp) {
            for(int i = 0; i < P.size(); ++i) {
                if(!Graph[x].test(P[i]))
                    delP[i] = 1;
            }
        }
    }
    for(int i = 0; i < P.size()-1; ++i) {
        int tmp = C.size()+1-K;
        if(adj[P[i]] >= tmp && !delP[i])
            newP.push_back(P[i]);
    }
}

void saveSolution() {
    ans.clear();
    for(auto x : C)
        ans.push_back(x);
    maxSize = C.size();
}

void expand(vector<int> P) {
    int tmp = P.size();
    for(int i = tmp-1; i >= 0; --i) {
        if(C.size() + P.size() <= maxSize)
            return ;

        int v = P[i];
        if(deg[v] <= maxSize-K) {
            P.pop_back();
            continue;
        }
        addtoC(v);
        vector<int> newP;
        updateP(P, newP);

        /*cout << "C: ";
        for(auto zz : C)
            cout << zz << " ";
        cout << endl << "P: ";
        for(auto xx : P)
            cout << xx << " ";
        cout << endl <<  "newP: ";
        for(auto yy : newP)
            cout << yy << " ";
        cout << endl << endl;*/

        if(newP.empty() && C.size() > maxSize)
            saveSolution();
        if(newP.size())
            expand(newP);
        delfrC(v);
        P.pop_back();
    }
}

void search() {
    vector<int> P;
    orderVertices(P);
    expand(P);
}

void output() {
    string KK = to_string(K);
    std::ofstream ansOfs("result/kplex_CP/" + graphName + "_ans_" + KK + ".txt");

    ansOfs << "{";
    ansOfs << "algorithm: kplex_CP, name: " << graphName << ", K: " << K << ", ";
    ansOfs << "size: " << maxSize << ", time: " << fixed<< setprecision(10) << (endTime-startTime)/CLOCKS_PER_SEC;
    ansOfs << "}" << endl;

    /*std::ofstream ansOfs("../kplex_CP//" + graphName + "_ans.txt");
    for(int i = 0; i < maxSize; i++)
        ansOfs << ans[i]+1 << " ";
    ansOfs << endl;
    ansOfs << "size: " << maxSize << ", time: " << fixed<< setprecision(10) << (endTime-startTime)/CLOCKS_PER_SEC;*/
}

int main(int argc, char **argv) {
    freopen(argv[1], "r", stdin); 

    str = argv[1];
    K = atoi(argv[2]);
    int tmp = 0;
    for(int i = 0; i < str.length(); i++) {
        if(str[i] == '/')
            graphName = "";
        else if(str[i] == '.' && str[i+1] == 't') {
            break;
        }
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
