import numpy as np
import csv
import random
import heapq

def generate_nodes_sphere(n_points, radius=1.0, filename="nodes.csv"):
    """
    Genera nodi distribuiti uniformemente all'interno di una sfera.
    Salva il risultato in un file CSV.
    
    :param n_points: Numero di nodi da generare
    :param radius: Raggio della sfera
    :param filename: Nome del file CSV di output
    """
    nodes = []
    
    for node_id in range(n_points):
        r = radius * np.cbrt(np.random.rand())  # Radice cubica per distribuzione uniforme in volume
        theta = np.arccos(1 - 2 * np.random.rand())  # Angolo polare
        phi = 2 * np.pi * np.random.rand()  # Angolo azimutale
        
        x = r * np.sin(theta) * np.cos(phi)
        y = r * np.sin(theta) * np.sin(phi)
        z = r * np.cos(theta)
        
        nodes.append((node_id, x, y, z))
    
    # Scrive i nodi in un file CSV
    with open(filename, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(["id", "x", "y", "z"])  # Intestazione
        writer.writerows(nodes)

    print(f"Generati {len(nodes)} nodi e salvati in {filename}")
    return nodes


def generate_random_edges(nodes, num_edges, filename="edges.csv"):
    """Genera archi casuali tra i nodi e li salva in un CSV."""
    edges = set()
    while len(edges) < num_edges:
        a, b = random.sample(nodes, 2)
        if a != b and (a, b) not in edges and (b, a) not in edges:
            weight = random.random()  # Genera un peso casuale tra 0 e 1
            edges.add((a[0], b[0], weight))  # Aggiungi l'arco con peso (id nodo1, id nodo2, peso)
    
    print(f"Generati {len(edges)} archi con peso e salvati in {filename}")
    return edges


def dijkstra_shortest_path(nodes, edges, start_node, end_node, output_file="shortest_path.csv"):
    """
    Trova il percorso ottimo tra due nodi usando l'algoritmo di Dijkstra e salva il risultato su CSV.
    
    :param nodes: Lista dei nodi (id, x, y, z)
    :param edges: Lista degli archi (node1, node2, weight)
    :param start_node: Nodo di partenza (id)
    :param end_node: Nodo di arrivo (id)
    :param output_file: Nome del file CSV di output
    :return: Lista dei nodi nel percorso ottimo
    """
    graph = {node[0]: {} for node in nodes}  # Dizionario {nodo: {adiacente: peso}}
    for node1, node2, weight in edges:
        graph[node1][node2] = weight
        graph[node2][node1] = weight  # Grafo non orientato
    
    # Inizializzazione Dijkstra
    pq = [(0, start_node)]  # (costo, nodo)
    distances = {node[0]: float('inf') for node in nodes}
    distances[start_node] = 0
    previous_nodes = {node[0]: None for node in nodes}
    
    while pq:
        current_distance, current_node = heapq.heappop(pq)
        
        if current_node == end_node:
            break
        
        for neighbor, weight in graph[current_node].items():
            distance = current_distance + weight
            if distance < distances[neighbor]:
                distances[neighbor] = distance
                previous_nodes[neighbor] = current_node
                heapq.heappush(pq, (distance, neighbor))
    
    # Ricostruzione del percorso
    path = []
    node = end_node
    while node is not None:
        path.append(node)
        node = previous_nodes[node]
    path.reverse()
    
    # Salva il percorso su CSV
    with open(output_file, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(["path_nodes"])
        for node in path:
            writer.writerow([node])
    
    print(f"Percorso ottimo salvato in {output_file}")
    return path


# Esempio di utilizzo
nodes = generate_nodes_sphere(n_points=100, radius=10.0, filename="nodes.csv")
edges = generate_random_edges(nodes, num_edges=100, filename="edges.csv")
path = dijkstra_shortest_path(nodes, edges, 30, 50)

for index in range(1, len(path) + 1):
    edges = [item for item in edges if item[1] != path[index-1] and item[2] != path[index]]

with open("edges.csv", mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(["node1", "node2", "weight"])  # Intestazione
    writer.writerows(edges)