import networkx as nx
import numpy as np
from collections import defaultdict
import random
import json

class PaymentChannelNetwork:
    def __init__(self, num_nodes=100, avg_degree=4, initial_capacity=1000):
        """Initialize network with random connected graph structure."""
        # Create random connected graph
        self.G = nx.connected_watts_strogatz_graph(n=num_nodes, k=avg_degree, p=0.3)
        self.num_nodes = num_nodes
        
        # Initialize channel capacities and current liquidity states
        self.capacities = {}
        self.liquidity = {}
        for (u, v) in self.G.edges():
            self.capacities[(u,v)] = initial_capacity
            self.capacities[(v,u)] = initial_capacity
            self.liquidity[(u,v)] = initial_capacity // 2
            self.liquidity[(v,u)] = initial_capacity // 2
        
        # Track node balances
        self.balances = {node: initial_capacity * self.G.degree(node) // 2 
                        for node in self.G.nodes()}
        
        # Define cost functions for each edge
        self.cost_functions = {
            edge: lambda amount: 1 + 0.001 * amount
            for edge in self.G.edges()
        }
        
    def get_edge_cost(self, u, v, amount):
        """Calculate cost of routing payment through edge."""
        return self.cost_functions[(u,v)](amount)
    
    def find_path(self, source, target, amount):
        """Find minimum cost path with sufficient liquidity."""
        # Create flow network
        flow_network = nx.DiGraph(self.G)
        
        # Add capacities to edges
        for (u, v) in flow_network.edges():
            flow_network[u][v]['capacity'] = self.liquidity[(u,v)]
        
        # Find max flow paths
        flow_value, flow_dict = nx.maximum_flow(flow_network, source, target, flow_func=nx.algorithms.flow.edmonds_karp)
        
        if flow_value < amount:
            return None
        
        # Extract payment path from flow dict
        path = [source]
        current = source
        while current != target:
            for next_node, flow in flow_dict[current].items():
                if flow > 0:
                    path.append(next_node)
                    current = next_node
                    break
        
        return path
    
    def execute_payment(self, source, target, amount):
        """Execute payment if feasible, return success status."""
        path = self.find_path(source, target, amount)
        if not path:
            return False
        
        # Update liquidity states along path
        for i in range(len(path)-1):
            u, v = path[i], path[i+1]
            self.liquidity[(u,v)] -= amount
            self.liquidity[(v,u)] += amount
        
        # Update balances
        self.balances[source] -= amount
        self.balances[target] += amount
        return True
    
    def get_state(self):
        """Return current network state with serializable format."""
        return {
            'balances': self.balances,
            'liquidity': {
                f"{u},{v}": amount
                for (u,v), amount in self.liquidity.items()
            }
        }

def run_simulation(network, num_payments=1000000, snapshot_intervals=None, output_file='sim_results.json'):
    """Run simulation and save results."""
    results = []
    successful_payments = 0
    
    if snapshot_intervals is None:
        snapshot_intervals = [num_payments // 10] * 10  # Take 10 snapshots
    
    payment_counter = 0
    for interval in snapshot_intervals:
        # Execute payments for this interval
        for _ in range(interval):
            source = random.randrange(network.num_nodes)
            target = random.randrange(network.num_nodes)
            if source == target:
                continue
                
            amount = random.randint(1, 100)
            
            if network.execute_payment(source, target, amount):
                successful_payments += 1
            
            payment_counter += 1
        
        # Save network state at this interval
        results.append({
            'payment_num': payment_counter,
            'state': network.get_state(),
            'success_rate': successful_payments / payment_counter
        })
    
    # Save results to file
    with open(output_file, 'w') as f:
        json.dump(results, f, indent=2)  # Added indent for readability

if __name__ == "__main__":
    network = PaymentChannelNetwork(num_nodes=100, avg_degree=4, initial_capacity=1000)
    run_simulation(network, num_payments=10000)  # Reduced number of payments for testing
    print("Simulation complete. Results saved to sim_results.json")