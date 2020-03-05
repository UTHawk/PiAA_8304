from sys import stdin


class PriorityQueue:
    def __init__(self):
        self.repository = []

    def push(self, item, value):
        self.repository.append((item, value))
        self.repository.sort(key=lambda pair: pair[1])

    def pop(self):
        return self.repository.pop(0)

    def empty(self):
        return len(self.repository) == 0


def del_same_value(reverse_dict, end_pos):
    for key in list(reverse_dict):
        if key not in reverse_dict.values() and key != end_pos:
            del reverse_dict[key]


def same_value_in(reverse_dict):
    for value in list(reverse_dict.values()):
        if list(reverse_dict.values()).count(value) > 1:
            return True

    return False


start_position = input()
graph = {}
result = {}

for line in stdin:
    source, destination, value = line.split()
    if source not in graph:
        graph[source] = {destination: float(value)}
    else:
        graph[source].update({destination: float(value)})

vertexes = [vertex for vertex in graph if vertex != start_position]
for key in graph:
    for vertex in graph[key]:
        if vertex not in vertexes:
            vertexes.append(vertex)


for end_position in vertexes:
    queue = PriorityQueue()
    queue.push(start_position, 0)

    path_to = {start_position: 0}
    came_from = {start_position: None}

    while not queue.empty():
        current_element = queue.pop()

        if current_element[0] == end_position:
            break

        if current_element[0] in graph:
            for neighbour in graph[current_element[0]]:
                path_length = path_to[current_element[0]] + graph[current_element[0]][neighbour]
                if neighbour not in path_to or path_length <= path_to[neighbour]:
                    path_to[neighbour] = path_length
                    queue.push(neighbour, path_length)
                    came_from[neighbour] = current_element[0]
                    print("path: ", path_to)
                    print("came_from: ", came_from)
                    print()

    while same_value_in(came_from):
        del_same_value(came_from, end_position)

    reverse_list = {}

    for key in came_from:
        reverse_list[came_from[key]] = key
    path = start_position

    while path[-1] != end_position:
        try:
            path += reverse_list[path[-1]]
        except KeyError:
            print(f'you can\'t achieve this vertex: {end_position}')
            break

    result[end_position] = path

print(result)
