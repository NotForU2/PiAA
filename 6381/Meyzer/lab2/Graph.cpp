#include "Graph.hpp"
#include <limits>

char DM::Vertex::getName() const
{
	return name;
}

void DM::Vertex::addEdge(char a, double weight)
{
	assocVertices.push_back(DM::Dyad(a, weight));
}

std::list<DM::Dyad>::iterator DM::Vertex::getAssocBegin()
{
	return assocVertices.begin();
}
std::list<DM::Dyad>::iterator DM::Vertex::getAssocEnd()
{
	return assocVertices.end();
}

void DM::Vertex::sortEdgesByName()
{
	assocVertices.sort([](const Dyad &a, const Dyad &b) { return (a.name < b.name); });
}

void DM::Vertex::sortEdgesByWeight()
{
	assocVertices.sort([](const Dyad &a, const Dyad &b) { return (a.weight < b.weight); });
}

bool DM::Vertex::hasEdges()
{
	return !(assocVertices.empty());
}

void DM::Vertex::printEdges()
{
	// std::cout << "Vertex printEdges called" << std::endl;
	for(auto i = assocVertices.begin(), end = assocVertices.end(); i != end; ++i)
	{
		std::cout << "   " << (*i).name << " " << (*i).weight << std::endl;
	}
}

void DM::Graph::read()
{
	char a = 0, b = 0;
	double w = 0;
	while (std::cin >> a)
	{
		std::cin >> b >> w;
		// std::cout << a << " " << b << " " << w << std::endl;
		addEdge(a, b, w);
		addEdge(b, a, w);
	}
}

void DM::Graph::readOr()
{
	char a = 0, b = 0;
	double w = 0;
	while (std::cin >> a)
	{
		std::cin >> b >> w;
		std::cout << a << " " << b << " " << w << std::endl;
		addEdge(a, b, w);
	}
}

void DM::Graph::addEdge(char a, char b, double weight)
{
	vertices.at(a-'a').addEdge(b, weight);
}

DM::Vertex& DM::Graph::getVertex(char a)
{
	return vertices.at(a-'a');
}

void DM::Graph::sortEdgesByName()
{
	for (auto i = vertices.begin(); i < vertices.end(); ++i)
	{
		i->sortEdgesByName();
	}
}

void DM::Graph::sortEdgesByWeight()
{
	for (auto i = vertices.begin(); i < vertices.end(); ++i)
	{
		i->sortEdgesByWeight();
	}
}

void DM::Graph::print()
{
	// std::cout << "Called print" << std::endl;
	for (int i = 0; i < vertN; ++i)
	{
		if (vertices.at(i).hasEdges())
		{
			// std::cout << "Printing edges of " << 'a'+i << " vertex" << std::endl;
			std::cout << char('a'+i) << ": " << std::endl;
			vertices.at(i).printEdges();
		}
	}
}

void DM::Graph::pathSearchGreedy(char s, char e, std::list<char>& Result)
{
	Result.clear();
	std::queue<char> q;
	std::array<char, 26> path;
	std::array<double, 26> dist;
	std::array<bool, 26> vis;
	dist.fill(std::numeric_limits<double>::max());
	vis.fill(false);
	sortEdgesByName();
	sortEdgesByWeight();
	dist.at(s-'a') = 0;
    	char curr = s;
		// std::cout << curr << e << std::endl;
	while (curr != e)
	{
		// std::cout << curr << std::endl;
		vis.at(curr-'a') = true;
		Dyad next{0,0};
		for (auto i = getVertex(curr).getAssocBegin(),
			end = getVertex(curr).getAssocEnd(); i != end; ++i)
		{
			next = *i;
			if (!vis.at((*i).name-'a')) break;
		}
		// Если все вершины, смежные с данной, уже были посещены, или их нет:
		if (next.name==0 || vis.at(next.name-'a'))
		{
			curr = path.at(curr - 'a');
			continue;
		}
        	path.at(next.name-'a') = curr;
        	dist.at(next.name-'a') = dist.at(curr-'a') + next.weight;
        	curr = next.name;
		// std::cout << curr << dist.at(curr-'a') << std::endl; //

		// std::cout << (*i).name << " vertex done " << dist.at((*i).name-'a') << std::endl; //
	}
	// std::cout << curr << std::endl; //
	curr = e;
	while (curr != s)
	{
		Result.push_front(curr);
		curr = path.at(curr-'a');
	}
	Result.push_front(curr);
}

void DM::Graph::pathSearchAStar(char s, char e, std::list<char>& Result)
{
	Result.clear();
	struct n_pr{ // Name-priority struct
	    char name;
	    double priority;
	    bool operator<(const n_pr& b) const
	    {
		    return priority > b.priority;
	    }
	};
	std::priority_queue<n_pr> q;
	std::array<char, 26> path;
	std::array<double, 26> dist;
	dist.fill(std::numeric_limits<double>::max());
	// sortEdgesByName();
	// sortEdgesByWeight();
	q.push({s, 0});
	dist.at(s-'a') = 0;

	while (!q.empty())
	{
	 char curr = q.top().name;
	 // std::cout << curr << dist.at(curr-'a') << std::endl; //
	 q.pop();
	 for (auto i = getVertex(curr).getAssocBegin(),
		 end = getVertex(curr).getAssocEnd(); i != end; ++i)
	 {
		 if (dist.at((*i).name-'a') > (dist.at(curr-'a') + (*i).weight))
		 {
			 path.at((*i).name-'a') = curr;
			 dist.at((*i).name-'a') = dist.at(curr-'a') + (*i).weight;
			 q.push({(*i).name, dist.at((*i).name-'a') + abs((*i).name-e)}); // prior = path + euristic
			 // std::cout << "Added ";
		 }
		 // std::cout << (*i).name << " vertex done " << dist.at((*i).name-'a') << std::endl; //
	 }
	}
	// std::cout << std::endl; //
	char curr = e;
	while (curr != s)
	{
	 Result.push_front(curr);
	 curr = path.at(curr-'a');
	}
	Result.push_front(curr);
}
