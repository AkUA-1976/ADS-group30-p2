#include "graph_reader.h"

#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

// 读取 DIMACS 格式的图文件，转成稀疏表（邻接表）
// DIMACS 格式的每一行：
//   c ...      注释，跳过
//   p sp n m   图的信息：n 个顶点、m 条边
//   a u v w    一条有向边：从 u 到 v，长度 w
vector<node>* read_graph(const string& filename, int& n, int& m)
{
    // 打开文件：先按给定路径找，找不到再依次尝试上一级、上两级目录，
    // 这样从仓库根目录、build/ 或 build/Debug/ 下运行都能找到 data/ 里的图。
    ifstream fin(filename);
    if(!fin.is_open())
    {
        fin.clear();
        fin.open("../" + filename);
    }
    if(!fin.is_open())
    {
        fin.clear();
        fin.open("../../" + filename);
    }
    if(!fin.is_open())
    {
        cout << "Error opening file: " << filename << endl;
        return nullptr;
    }

    string line;
    string p, sp;                    // 临时接住每行开头的字母（p / a）
    vector<node>* graph = NULL;      // 稀疏表，还没建好

    // 逐行读文件
    while(getline(fin, line))
    {
        // p 行：图的基本信息，读出顶点数 n 和边数 m
        if(line[0] == 'p')
        {
            istringstream iss(line);
            iss >> p >> sp >> n >> m;
            // 顶点从 1 开始编号，所以开 n+1 个位置，下标 0 空着不用
            graph = new vector<node>[n + 1];
        }
        // a 行：一条有向边，挂到起点 u 的邻接表里
        else if(line[0] == 'a')
        {
            istringstream iss(line);
            int u, v;
            std::int64_t w;
            iss >> p >> u >> v >> w;

            node temp;
            temp.length = w;            // 边权（长度）
            temp.vertex = v;            // 终点
            graph[u].push_back(temp);
        }
        // c 行是注释，什么都不用做，直接跳过
    }

    return graph;
}
