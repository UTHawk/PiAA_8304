#include <iostream>
#include <algorithm>
#include <vector>


class Neighbour {
public:
	char vertex;
	int start_flow;
	int flow;
	int final_flow;

	Neighbour(char vertex, int start_flow) : vertex(vertex), start_flow(start_flow), flow(start_flow) {}
	Neighbour(char vertex) : vertex(vertex) {}
};

class Vertex {
public:
	char vertex;
	char prev_vertex;
	std::vector<Neighbour> neighbours;

	Vertex() {}
	Vertex(char vertex) : vertex(vertex) {}
};

bool isExist(std::vector<Vertex> vector, char vertex) {
	for (int i = 0; i < vector.size(); ++i)
		if (vector[i].vertex == vertex)
			return true;

	return false;
}

int findVertex(std::vector<Vertex> vector, char vertex) {
	for (int i = 0; i < vector.size(); i++)
		if (vertex == vector[i].vertex)
			return i;

	return -1;
}

int findNeighbour(Vertex vertex, char neighbour) {
	if (vertex.neighbours.empty())
		return -1;

	for (int i = 0; i < vertex.neighbours.size(); ++i) 
		if (vertex.neighbours[i].vertex == neighbour)
			return i;

	return -1;
}

int maxFlowCount(std::vector<Vertex> vector_of_vertex, Vertex stock) {		// ������ ������� ������� ������������ ����� � �������� ����
	int flow, min_flow, index_prev, index_neighbour;
	std::vector<int> flows;
	Vertex current = stock;		// ������� ������� - ����

	while (current.prev_vertex != '0')														// ���� �� ����� �� ������,
	{																						// ������� ������� ���������� �������
		index_prev = findVertex(vector_of_vertex, current.prev_vertex);						// � �������� ����, � � ��� �������� ������
		index_neighbour = findNeighbour(vector_of_vertex[index_prev], current.vertex);		// �� ������� �������. ��������� ���������
		flow = vector_of_vertex[index_prev].neighbours[index_neighbour].flow;				// �������� � ������ � ��������. �������
		flows.push_back(flow);																// �������� ���������� ���������� �
		current = vector_of_vertex[index_prev];												// �������� ����.
	}																						

	min_flow = flows[0];

	for (int i = 1; i < flows.size(); ++i)		// ������� �����������
		if (flows[i] < min_flow)				// �� ������� ��������� ����
			min_flow = flows[i];

	return min_flow;		// ���������� ������������ ����� ��������� ����
}

void recountFlow(std::vector<Vertex>& vector_of_vertex, Vertex stock, int max_flow) {		// ������ ������� ���������� ��������
	int index_current, index_prev, index_neighbour_current, index_neighbour_prev;			// ������ �� ��������� ����
	Vertex current = stock;		// ������� ������� - ����

	while (current.prev_vertex != '0')																		// ���� �� ����� �� ������,
	{																										// ������� ������� � ����������
		index_current = findVertex(vector_of_vertex, current.vertex);										// ������� �������, � � ��� �����
		index_prev = findVertex(vector_of_vertex, current.prev_vertex);										// ���� �� �����.
		index_neighbour_current = findNeighbour(vector_of_vertex[index_current], current.prev_vertex);		
		index_neighbour_prev = findNeighbour(vector_of_vertex[index_prev], current.vertex);

		if (index_neighbour_current == -1)											// ���� �� ������� ������� ��� �����
		{																			// �� ���������� � �������� ����
			Neighbour neighbour = Neighbour(current.prev_vertex);					// �������, �� ��� ��������� �
			neighbour.flow = max_flow;												// ��� ���������� �����������
			neighbour.start_flow = 0;												// ���������������� ������������
			vector_of_vertex[index_current].neighbours.push_back(neighbour);		
		}
		else																							// ����� ���������� �����������
			vector_of_vertex[index_current].neighbours[index_neighbour_current].flow += max_flow;		// ������������� �� �������� �������������
																										// ������ ������� �������� ����
		vector_of_vertex[index_prev].neighbours[index_neighbour_prev].flow -= max_flow;		// ���������� ����������� �� ���������� � ��������
		current = vector_of_vertex[index_prev];												// ���� ������� �� ������� ����������� �� ��������
	}
}

bool cmpVertex(const Vertex& a, const Vertex& b) {
	if (a.vertex < b.vertex)
		return true;
	else
		return false;
}

bool cmpNeighbours(const Neighbour& a, const Neighbour& b) {
	if (a.vertex < b.vertex)
		return true;
	else
		return false;
}


int main() {
	int count, start_flow, max_flow, index;
	int Pmax = 0;
	char source, stock, start, end;
	std::vector<Vertex> vector_of_vertex, visited_vertex;

	std::cin >> count >> source >> stock;

	for (int i = 0; i < count; ++i)		// ��������� ��������� �����
	{		
		std::cin >> start >> end >> start_flow;
		Vertex first;
		Vertex second;

		if (!isExist(vector_of_vertex, start)) 
		{															// ���� ��������� ������� ��� ���
			first = Vertex(start);									// � ������� ������, �� ������ �,
			Neighbour neighbour = Neighbour(end, start_flow);		// � ������(�������� �������) �
			first.neighbours.push_back(neighbour);					// ��������� ������ � ������ �������.
			vector_of_vertex.push_back(first);						// ����� ��������� ������� � ������ ������
		}
		else 
		{																	// ����� ���� ��������� ������� ��� ����
			Neighbour neighbour = Neighbour(end, start_flow);				// � ������� ������, �� ������ � ������,
			index = findVertex(vector_of_vertex, start);					// ������� ��������� ������� � ������� ������
			vector_of_vertex[index].neighbours.push_back(neighbour);		// � ��������� ������ � ������ ������� ���������
		}																	// �������.

		if (!isExist(vector_of_vertex, end))		// ���� �������� ������� ��� � ������� ������,
		{											// �� ��� ��������� � ����������� � ������ ������
			second = Vertex(end);					
			vector_of_vertex.push_back(second);
		}
	}

	index = findVertex(vector_of_vertex, source);
	vector_of_vertex[index].prev_vertex = '0';
	Vertex current = vector_of_vertex[index];		// ������� ������� - �����
	visited_vertex.push_back(current);

	while (1) {
		int index_min_priority = -1;
		int priority, min_priority;
		bool near_start;

		for (int i = 0; i < current.neighbours.size(); i++)			// ����� ������ ������� �������
		{																											
			if (current.neighbours[i].flow > 0 && !isExist(visited_vertex, current.neighbours[i].vertex))		// ���� ���������� ����������� ���� �� ������
			{																									// ������ ���� � ����� �� ��������� � �������
				priority = abs(current.neighbours[i].vertex - current.vertex);									// ���������� ������, �� �������������� ���������	

				if (index_min_priority == -1 || priority < min_priority)		// ������ ����� � ����������� ����������� � � ������
				{																// ��������� �����������, ���������� �����
					min_priority = priority;									// ��� �������� � �������� ��������� � ������ ��������.
					index_min_priority = i;										
																				
					if (current.neighbours[i].vertex < current.vertex)		// ��� ��� ��� ���������� ���������� ������ ����� ����������
						near_start = true;									// ���� ��, ���� ����� �� ���������� "����������" �� �������
					else													// �������, �� ���������� near_start �������� �� ��� ���������.
						near_start = false;                                
				}
				else if (priority == min_priority) 
					if (current.neighbours[i].vertex < current.vertex && near_start == false) 
					{
						index_min_priority = i;
						near_start = true;
					}
			}
		}

		if (index_min_priority != -1)																	// ���� ����� ��� ������
		{																								// �� ��������� � �������
			index = findVertex(vector_of_vertex, current.neighbours[index_min_priority].vertex);		// ������ � ���������� ��������
			vector_of_vertex[index].prev_vertex = current.vertex;										// � ���� ����������� �������
			current = vector_of_vertex[index];															
			visited_vertex.push_back(current);

			if (current.vertex == stock)												// ���� �� ������ �������� ����� �� �����
			{																			// �������������� ������������ �����,
				max_flow = maxFlowCount(vector_of_vertex, current);						// ������������ �������� ���������� ������������ ����,
				recountFlow(vector_of_vertex, vector_of_vertex[index], max_flow);		// �������������� �������� ������������ �����
				Pmax += max_flow;														
				visited_vertex.clear();
				index = findVertex(vector_of_vertex, source);							// ������� �������� ���������� �����
				current = vector_of_vertex[index];
				visited_vertex.push_back(current);
			}
		}
		else		// ���� ����� �� ��� ������
		{		
			if (current.prev_vertex == '0')		// ���� ������� ������� - �����, �� ������ �������� ����� �� ���������.
				break;		// ������� � ������������ �������
			else		// �����, ����� � ���������� �������
			{				
				index = findVertex(vector_of_vertex, current.prev_vertex);
				current = vector_of_vertex[index];
			}
		}
	}

	for (int i = 0; i < vector_of_vertex.size(); ++i)		// �������������� �������� ������ ����� ��� ����� � �����
		for (int j = 0; j < vector_of_vertex[i].neighbours.size(); ++j)
			vector_of_vertex[i].neighbours[j].final_flow = vector_of_vertex[i].neighbours[j].start_flow - vector_of_vertex[i].neighbours[j].flow;

	sort(vector_of_vertex.begin(), vector_of_vertex.end(), cmpVertex);
	// ���������� �����
	for (int i = 0; i < vector_of_vertex.size(); ++i) 
		sort(vector_of_vertex[i].neighbours.begin(), vector_of_vertex[i].neighbours.end(), cmpNeighbours);

	// ����� ����������
	std::cout << Pmax << std::endl;

	for (int i = 0; i < vector_of_vertex.size(); ++i) 
		for (int j = 0; j < vector_of_vertex[i].neighbours.size(); ++j) 
			if (vector_of_vertex[i].neighbours[j].start_flow != 0)		// ���� ����� ���� ������ ����������(� �� ������� �� ����� ���������)
			{		
				std::cout << vector_of_vertex[i].vertex << " " << vector_of_vertex[i].neighbours[j].vertex << " ";

				if (vector_of_vertex[i].neighbours[j].final_flow >= 0)
					std::cout << vector_of_vertex[i].neighbours[j].final_flow;
				else
					std::cout << 0;

				std::cout << std::endl;
			}
	return 0;
}