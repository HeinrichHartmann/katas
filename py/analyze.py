import json
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
from collections import defaultdict
from ortools.linear_solver import pywraplp
import time

def read_snapshots(file_path):
    with open(file_path, 'r') as f:
        snapshots = json.load(f)
    return snapshots

def create_graph(snapshot):
    G = nx.Graph()
    for edge_str, liquidity in snapshot['state']['liquidity'].items():
        u, v = map(int, edge_str.split(','))
        G.add_edge(u, v)
    return G

def estimate_liquidity(G, balances, time_limit=10):
    solver = pywraplp.Solver('Liquidity Estimation', pywraplp.Solver.CBC_MIXED_INTEGER_PROGRAMMING)
    
    # Variables
    liquidity = {}
    for u, v in G.edges():
        liquidity[(u, v)] = solver.IntVar(0, solver.infinity(), f'l_{u}_{v}')
        liquidity[(v, u)] = solver.IntVar(0, solver.infinity(), f'l_{v}_{u}')
    
    # Constraints
    for node in G.nodes():
        solver.Add(sum(liquidity[(node, v)] for v in G.neighbors(node)) == balances[str(node)])
    
    for u, v in G.edges():
        solver.Add(liquidity[(u, v)] + liquidity[(v, u)] >= 1)
    
    # Objective
    solver.Maximize(sum(liquidity[(u, v)] + liquidity[(v, u)] for u, v in G.edges()))
    
    # Solve with time limit
    solver.set_time_limit(time_limit * 1000)
    status = solver.Solve()
    
    estimated_liquidity = {}
    if status == pywraplp.Solver.OPTIMAL or status == pywraplp.Solver.FEASIBLE:
        for u, v in G.edges():
            estimated_liquidity[(u, v)] = liquidity[(u, v)].solution_value()
            estimated_liquidity[(v, u)] = liquidity[(v, u)].solution_value()
    
    return estimated_liquidity

def compare_liquidity(actual, estimated):
    mse = 0
    for edge, value in actual.items():
        u, v = map(int, edge.split(','))
        mse += (value - estimated.get((u, v), 0)) ** 2
    mse /= len(actual)
    return mse

def visualize_results(results):
    fig, ax = plt.subplots()
    ax.plot(range(len(results)), results, marker='o')
    ax.set_xlabel('Simulation Step')
    ax.set_ylabel('Mean Squared Error')
    ax.set_title('Difference between Actual and Estimated Liquidity')
    plt.show()

if __name__ == "__main__":
    snapshots = read_snapshots('sim_results.json')
    results = []
    
    for snapshot in snapshots:
        G = create_graph(snapshot)
        balances = snapshot['state']['balances']
        
        start_time = time.time()
        estimated_liquidity = estimate_liquidity(G, balances)
        end_time = time.time()
        
        if end_time - start_time > 10:
            break
        
        actual_liquidity = snapshot['state']['liquidity']
        mse = compare_liquidity(actual_liquidity, estimated_liquidity)
        results.append(mse)
    
    visualize_results(results) 