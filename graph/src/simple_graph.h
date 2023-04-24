#pragma once
#ifndef _SIMPLEGRAPH
#define _SIMPLEGRAPH
#include <cstring>
#include <vector>
#include <string>
#include <map>
#define N 100

// N(E) >> N(V) use adjcentMatrix
struct Edge
{
	Edge(const std::string& src, const std::string& dst) :from(src), to(dst) {}
	std::string from;
	std::string to;
};

class SimpleGraph
{
public:
	SimpleGraph()
	{
		memset(m_adjMatrix, 0, N * N * sizeof (int));
		m_nodeCount = 0;
		m_edgeCount = 0;
	}
	SimpleGraph(const SimpleGraph& graph);
	// 添加从src节点到dst节点的边
	bool addEdge(std::string src, std::string dst);
	// 删除从src节点到dst节点的边
	bool deleteEdge(std::string src, std::string dst);
	bool hasEdge(std::string src, std::string dst);
	bool hasNode(std::string node) const;
	// target节点的邻接点
	std::vector<std::string> neighbors(std::string target);
	// target节点的邻接边
	std::vector<Edge> linkedEdge(std::string &targetNode);
	// target节点入度
	int nodeInDegree(std::string target);
	// target节点出度
	int nodeOutDegree(std::string node);
	size_t nodeCount() const;
	size_t edgeCount() const;
	// 深度优先遍历
	void dfs(std::vector<std::vector<std::string>>& seq);
	// 宽度优先遍历
	void bfs(std::vector<std::vector<std::string>>& seq);
	// 图的拓扑排序序列
	void topoSequence(std::vector<std::vector<std::string>>& seq);
	// T: O(N ** 2) S: O(E)
	// 获取图中所有节点列表
	std::vector<std::string> nodes() const;
	// 获取从src到dst所有的路径, 存在路径返回true, 不存在返回false
	bool path(std::string src, std::string dst, std::vector<std::vector<std::string>>& path);
	std::vector<Edge> edges();
	// 环检测
	// 连通分量
	// 合并子图
	// 最短路径
	std::string to_str();
	void debug();

private:
	void _dfs(std::string startNode, std::vector<std::string>& seq);
	void _bfs(std::string startNode, std::vector<std::string>& seq);
	void _topoSequence(std::string startNode, std::vector<std::string>& seq);
	int nodeName2Id(std::string nodeName);
	std::string nodeId2Name(int nodeId);

private:
	int m_adjMatrix[N][N];
	std::map<std::string, int> m_nodeName2Id;
	std::map<int, std::string> m_nodeId2Name;
	size_t m_nodeCount;
	size_t m_edgeCount;
};

// N(V) >> N(E) use adjcent link list
#endif
