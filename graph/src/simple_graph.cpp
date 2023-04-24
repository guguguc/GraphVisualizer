#include "SimpleGraph.h"
#include "Common.h"

bool SimpleGraph::addEdge(std::string src, std::string dst)
{
	if (hasEdge(src, dst))
	{
		return false;
	}
	if (!hasNode(src))
	{
		m_nodeName2Id[src] = m_nodeCount++;
		m_nodeId2Name[m_nodeCount] = src;
	}
	if (!hasNode(dst))
	{
		m_nodeName2Id[dst] = m_nodeCount++;
		m_nodeId2Name[m_nodeCount] = dst;
	}
	m_edgeCount += 1;
	m_adjMatrix[m_nodeName2Id[src]][m_nodeName2Id[dst]] = 1;
	return true;
}

bool SimpleGraph::hasEdge(std::string src, std::string dst)
{
	if (!hasNode(src) || !hasNode(dst))
	{
		return false;
	}
	return m_adjMatrix[nodeName2Id(src)][nodeName2Id(dst)] == true;
}

bool SimpleGraph::hasNode(std::string node) const
{
	return m_nodeName2Id.find(node) != m_nodeName2Id.end();
}

int  SimpleGraph::nodeInDegree(std::string target)
{
	if (!hasNode(target))
	{
		return -1;
	}
	size_t cnt = 0;
	for (const auto& node : nodes())
	{
		cnt += hasEdge(node, target) ? 1 : 0;
	}
	return  cnt;
}		

int  SimpleGraph::nodeOutDegree(std::string node)
{
	if (!hasNode(node))
	{
		return -1;
	}
	size_t cnt = 0;
	for (int j = 0; j < N; ++j)
	{
		cnt += (m_adjMatrix[m_nodeName2Id[node]][j] ? 1 : 0);
	}
	return  cnt;
}

size_t SimpleGraph::nodeCount() const
{
	return m_nodeCount;
}

size_t SimpleGraph::edgeCount() const
{
	return m_edgeCount;
}

std::vector<std::string> SimpleGraph::neighbors(std::string target)
{
	if (!hasNode(target))
	{
		return std::vector<std::string>();
	}
	std::vector<std::string> ret;
	for (const auto& node : nodes())
	{
		if (hasEdge(target, node))
		{
			ret.push_back(node);
		}
	}
	return ret;
}

std::vector<Edge>
SimpleGraph::linkedEdge(std::string& targetNode)
{
	std::vector<Edge> edges;
	auto nbs = neighbors(targetNode);
	for (const auto& node : nbs)
	{
		edges.push_back(Edge(targetNode, node));
	}
	return edges;
}

bool SimpleGraph::path(std::string src, std::string dst, std::vector<std::vector<std::string>>& path)
{
	if (!hasNode(src) || !hasNode(dst))
	{
		return false;
	}
	std::vector<std::string> seqs;
	_dfs(src, seqs);
}

SimpleGraph::SimpleGraph(const SimpleGraph &graph)
{
	m_edgeCount = graph.m_edgeCount;
	m_nodeCount = graph.m_nodeCount;
	m_nodeName2Id = graph.m_nodeName2Id;
	m_nodeId2Name = graph.m_nodeId2Name;
	memcpy(m_adjMatrix, graph.m_adjMatrix, N * N * sizeof(int));
}

void SimpleGraph::topoSequence(std::vector<std::vector<std::string>>& seq)
{
	for (const auto& node : nodes())
	{
		if (nodeInDegree(node))
		{
			continue;
		}
#ifdef _DEBUG
		std::cout << node << "'s indegree is 0\n";
#endif
		std::vector<std::string> path;
		_topoSequence(node, path);
		path.insert(path.begin(), node);
		seq.push_back(path);
	}
}

void SimpleGraph::dfs(std::vector<std::vector<std::string>>& seq)
{
	for (const auto& node : nodes())
	{
		if (nodeInDegree(node))
		{
			continue;
		}
#ifdef _DEBUG
		std::cout << node << "'s indegree is 0\n";
#endif
		std::vector<std::string> path;
		_dfs(node, path);
		path.insert(path.begin(), node);
		seq.push_back(path);
	}
}

void SimpleGraph::bfs(std::vector<std::vector<std::string>>& seq)
{
	for (const auto& node : nodes())
	{
		if (nodeInDegree(node))
		{
			continue;
		}
#ifdef _DEBUG
		std::cout << node << "'s indegree is 0\n";
#endif
		std::vector<std::string> path;
		_bfs(node, path);
		//path.insert(path.begin(), node);
		seq.push_back(path);
	}
}

void SimpleGraph::_dfs(std::string startNode, std::vector<std::string>& seq)
{
	if (!hasNode(startNode))
	{
		return;
	}
	static std::vector<bool> visted(m_nodeCount + 1, false);
	auto vecNeighbors = neighbors(startNode);
	for (const auto& node : vecNeighbors)
	{
		auto nodeId = nodeName2Id(node);
		if (visted[nodeId])
		{
			continue;
		}
		visted[nodeId] = true;
		seq.push_back(node);
		_dfs(node, seq);
	}
}

void SimpleGraph::_bfs(std::string startNode, std::vector<std::string>& seq)
{
	if (!hasNode(startNode))
	{
		return;
	}
	std::map<std::string, bool> isVisted;
	std::queue<std::string> q;
	q.push(startNode);
	isVisted[startNode] = true;
	while (!q.empty())
	{
		auto frontNode = q.front();
		seq.push_back(frontNode);
		q.pop();
		for (const auto& node : this->neighbors(frontNode))
		{
			if (isVisted[node])
			{
				#ifdef _DEBUG
					LOG("skip " + node)
				#endif
				continue;
			}
			q.push(node);
			isVisted[node] = true;
		}
	}
}

void
SimpleGraph::_topoSequence(std::string startNode, std::vector<std::string>& seq)
{
	static SimpleGraph g(*this);
	for (const auto& node : g.neighbors(startNode))
	{
		bool ret = g.deleteEdge(startNode, node);
		assert(ret);
		#ifdef _DEBUG
			LOG("delete edge " + startNode + "->" + node)
		#endif
		if (g.nodeInDegree(node))
		{
		#ifdef _DEBUG
			LOG("skip " + node)
		#endif
			continue;
		}
		#ifdef _DEBUG
			LOG("indegreee is 0, delete node " + node)
		#endif
		// 入度为0
		seq.push_back(node);
		_topoSequence(node, seq);
	}
}

int SimpleGraph::nodeName2Id(std::string nodeName)
{
	if (m_nodeName2Id.find(nodeName) == m_nodeName2Id.end())
	{
		return -1;
	}
	return m_nodeName2Id[nodeName];
}

std::string SimpleGraph::nodeId2Name(int nodeId)
{
	if (m_nodeId2Name.find(nodeId) == m_nodeId2Name.end())
	{
		return "";
	}
	return m_nodeId2Name[nodeId];
}

bool SimpleGraph::deleteEdge(std::string src, std::string dst)
{
	if (!hasEdge(src, dst))
	{
		return false;
	}
	m_adjMatrix[m_nodeName2Id[src]][m_nodeName2Id[dst]] = 0;
	m_edgeCount -= 1;
	return true;
}

std::vector<std::string> SimpleGraph::nodes() const
{
	std::vector<std::string> ret;
	for (const auto& item : m_nodeName2Id)
	{
		ret.push_back(item.first);
	}
	return ret;
}

std::vector<Edge> SimpleGraph::edges()
{
	std::vector<Edge> ret;
	auto vecNodes = nodes();
	for (auto& n1 : vecNodes) {
		for (auto& n2 : vecNodes)
		{
			if (n1 == n2 || !hasEdge(n1, n2))
			{
				continue;
			}
			auto edge = Edge(n1, n2);
			ret.emplace_back(edge);
		}
	}
	return ret;
}

std::string SimpleGraph::to_str()
{
	std::string repr;
	std::vector<std::string> lst;

	lst.push_back("\"node count\":" + std::to_string(m_nodeCount));
	lst.push_back("\"edge count\":" + std::to_string(m_edgeCount));

	lst.push_back("\n\"edges\":");
	lst.push_back("\n[");
	for (const auto& e : edges())
	{
		lst.push_back(e.from);
		lst.push_back("->");
		lst.push_back(e.to);
		lst.push_back("\n");
	}
	lst.push_back("]\n");

	lst.push_back("\"node name to id\":");
	lst.push_back("\n[");
	for (const auto& it : m_nodeName2Id)
	{
		lst.push_back(it.first);
		lst.push_back(":");
		lst.push_back(std::to_string(it.second));
		lst.push_back(",");
	}
	lst.push_back("]\n");
	repr = Common::string_join(lst, " ");
	return "{" + repr + "}";
}

void SimpleGraph::debug()
{
	// dump graph matrix
	std::string repr;
	for (int i = 0; i < m_nodeCount; ++i) {
		for (int j = 0; j < m_nodeCount; ++j)
		{
			repr += m_adjMatrix[i][j] ? "1" : "0";
			repr += " ";
		}
		repr += "\n";
	}
	repr += std::to_string(sizeof(*this) / 1024) + "kB";
	repr += "ratio: ";
	repr += std::to_string(m_edgeCount) + "/" + std::to_string(N * N);
	std::cout << repr;
}
