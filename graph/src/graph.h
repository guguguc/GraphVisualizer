#pragma once
#include <unordered_map>
#include <memory>
#include "common.h"
#include "utils.h"

template <class T>
class Graph
{
public:
	using PGraph = std::shared_ptr<Graph<T>>;
	using NODE_ID_T = size_t;
	using EDGE_ID_T = size_t;

	class Node
	{
	public:
		Node() = default;
		explicit Node(const T& payload)
		{
			m_id = Utils::gen_id(payload);
			m_payload = payload;
			LOG("ctor")
		}
		Node(const Node& node)
		{
			m_id = Utils::gen_id(payload());
			m_payload = node.payload();
		}
		bool operator==(const Node other) const
		{
			return other.m_payload == m_payload;
		}

		size_t id() const
		{
			return m_id;
		}

		const T& payload() const
		{
			return m_payload;
		}

		bool connectTo(const std::shared_ptr<Node>& other)
		{
			auto it = std::find_if(m_ToNodes.begin(), m_ToNodes.end(), [&](const auto& pItem) {
				return *pItem == *other;
				});
			if (it != m_ToNodes.end())
			{
				return false;
			}
			m_ToNodes.emplace_back(other);
			return true;
		}
		bool connectFrom(const std::shared_ptr<Node>& other)
		{
			auto it = std::find_if(m_FromNodes.begin(), m_FromNodes.end(), [&](const auto& pItem) {
				return *pItem == *other;
				});
			if (it != m_FromNodes.end())
			{
				return false;
			}
			m_FromNodes.emplace_back(other);
			return true;
		}
		const std::vector<std::shared_ptr<Node>>& getToNodes()
		{
			return m_ToNodes;
		}

		const std::vector<std::shared_ptr<Node>>& getFromNodes()
		{
			return m_FromNodes;
		}

	private:
		size_t m_id = 0;
		T m_payload;
		std::vector<std::shared_ptr<Node>> m_ToNodes;
		std::vector<std::shared_ptr<Node>> m_FromNodes;
	};

	class Edge
	{
	public:
		Edge() = default;
		Edge(const Node& src, const Node& dst)
		{
			m_src = std::make_shared<Node>(src);
			m_dst = std::make_shared<Node>(dst);
			init();
		}

		Edge(const T& srcName, const T& dstName)
		{
			m_src = std::make_shared<Node>(srcName);
			m_dst = std::make_shared<Node>(dstName);
			init();
		}
		Edge(const Edge& other)
		{
			m_dst = other.m_dst;
			m_src = other.m_src;
			init();
		}
		Edge& operator=(const Edge& edge)
		{
			this->m_dst = edge.m_dst;
			this->m_src = edge.m_src;
			m_src->connectTo(m_dst);
			m_dst->connectFrom(m_src);
			return *this;
		}
		bool operator==(const Edge& other) const
		{
			return *m_src == *other.srcNode()
				&& *m_dst == *other.dstNode();
		}

		const std::shared_ptr<Node>& srcNode() const
		{
			return m_src;
		}

		const std::shared_ptr<Node>& dstNode() const
		{
			return m_dst;
		}

		size_t id() const
		{
			return m_id;
		}

		std::string dump() const
		{
			return "";
		}
		~Edge() = default;

	private:
		void init()
		{
			m_src->connectTo(m_dst);
			m_dst->connectFrom(m_src);
			m_id = (Utils::gen_id(m_src->payload()) << (sizeof(uintmax_t) * 4)) ^ Utils::gen_id(m_dst->payload());
		}
		size_t m_id = 0;
		std::shared_ptr<Node> m_src;
		std::shared_ptr<Node> m_dst;
	};


	Graph() = default;
	~Graph() = default;

	const std::vector<std::shared_ptr<Node>>& nodes() const
	{
		return m_nodes;
	}

	const std::vector<std::shared_ptr<Edge>>& edges() const
	{
		return m_edges;
	}

	std::vector<std::shared_ptr<Node>> neighbors(const Node& target)
	{
		if (!hasNode(target))
		{
			return {};
		}
		return node(target.id())->getToNodes();
	}

	bool hasEdge(const Edge& edge) const
	{
		return m_edge_id2idx.find(edge.id()) != m_edge_id2idx.end();
	}

	bool hasEdge(const Node& srcNode, const Node& dstNode) const
	{
		return hasEdge(Edge(srcNode, dstNode));
	}

	bool hasNode(const Node& node) const
	{
		return m_node_id2idx.find(node.id()) != m_node_id2idx.end();
	}

	bool addNode(const Node& node)
	{
		auto pNode = std::make_shared<Node>(node);
		return addNode(pNode);
	}

	bool addEdge(const Edge& edge)
	{
		if (hasEdge(edge))
		{
			return false;
		}
		auto pEdge = std::make_shared<Edge>(edge);
		return addEdge(pEdge);
	}

	size_t nodeCount() const
	{
		return m_nodes.size();
	}

	size_t edgeCount() const
	{
		return m_edges.size();
	}

	std::string dump()
	{
		std::string repr;
		for (const auto& node : nodes())
		{
			repr += node->payload() + ":" + "\n";
			for (const auto& item : neighbors(*node))
			{
				repr += node->payload() + "->"  +  item->payload() + "\n";
				item->
			}
		}
		return repr;
	}

private:
	bool addEdge(std::shared_ptr<Edge> edge)
	{
		if (!edge || hasEdge(edge))
		{
			return false;
		}
		m_edges.emplace_back(edge);
		m_edge_id2idx[edge->id()] = m_edges.size() - 1;
		if (!hasNode(*edge->srcNode()))
		{
			addNode(edge->srcNode());
		}
		if (!hasNode(*edge->dstNode()))
		{
			addNode(edge->dstNode());
		}
		node(edge->srcNode()->id())->connectTo(edge->dstNode());
		node(edge->dstNode()->id())->connectFrom(edge->srcNode());
		return true;
	}

	bool addNode(std::shared_ptr<Node> node)
	{
		if (!node || hasNode(*node))
		{
			return false;
		}
		m_nodes.emplace_back(node);
		m_node_id2idx[node->id()] = m_nodes.size() - 1;
		return true;
	}

	bool hasEdge(std::shared_ptr<Edge> edge) const
	{
		if (!edge)
		{
			return false;
		}
		return m_edge_id2idx.find(edge->id()) != m_edge_id2idx.end();
	}

	std::shared_ptr<Node> node(NODE_ID_T id)
	{
		if (m_node_id2idx.find(id) == m_node_id2idx.end())
		{
			return nullptr;
		}
		return m_nodes[m_node_id2idx[id]];
	}

private:
	std::vector<std::shared_ptr<Node>> m_nodes;
	std::vector<std::shared_ptr<Edge>> m_edges;
	std::unordered_map<NODE_ID_T, size_t> m_node_id2idx;
	std::unordered_map<NODE_ID_T, size_t> m_edge_id2idx;
};
