import sys
import os
import networkx as nx

def is_complete_subgraph(graph, subgraph):
    num_vertices = len(subgraph)
    expected_num_edges = num_vertices * (num_vertices - 1) // 2  # Formula for complete graph
    num_edges_in_subgraph = graph.subgraph(subgraph).number_of_edges()
    return num_edges_in_subgraph == expected_num_edges



def find_largest_complete_subgraph(graph):
    largest_complete_subgraph = set()
    largest_size = -1
    graph = nx.Graph(graph)
    all_largest_subgraphs=[]
    for subgraph in nx.find_cliques(graph):       
        is_complete = True
        for u in subgraph:
            for v in subgraph:
                if u != v and not graph.has_edge(u, v):
                    is_complete = False
                    break
        
        if is_complete:
            if len(subgraph) >= largest_size:
                largest_size = len(subgraph)
                largest_complete_subgraph = set(subgraph)
                all_largest_subgraphs.append(largest_complete_subgraph)

    if len(all_largest_subgraphs)>1:
    	if len(all_largest_subgraphs[0])<len(all_largest_subgraphs[1]):
    		return all_largest_subgraphs[1:]
    	else: 
    		return all_largest_subgraphs
    
    else: 
    	return all_largest_subgraphs


def is_largest_complete_subgraph(graph, subgraph):
    largest_complete_subgraph = find_largest_complete_subgraph(graph)
    return set(sorted(subgraph)) in largest_complete_subgraph


def checker_problem2(graph_file, outfile):	
	
	fp = open(outfile,"r")
	lines = fp.readlines()
	num_lines = len(lines)
	fp.close()

	if num_lines == 1:
		if lines[0].startswith('0'):
			print('Format is OK')
		else:
			print('ERROR: Wrong Format')
		sys.exit(1)

	if num_lines!=2:
		print('ERROR: Output should have 1 subgraph')
		sys.exit(1)
	

	G={}
	#tempG={}
	with open(graph_file,'r') as f:
		rows = f.readlines()
		i = 0
		for row in rows:
			x = row.strip().split(' ')
			x = [int(v) for v in x]
			if i == 0:
				N = x[0]
				E = x[1]
			else:
				if x[0] in G:
					G[x[0]].append(x[1])
				else:
					G[x[0]] = [x[1]]
			i+=1
		
	for i in range(1,N+1):
		if i not in G:
			G[i] = []
	i=0  
	subgraph = {}  
	for line in lines:
		if line.startswith('#'):
			continue
		else:
			i += 1
			v = line.split(' ')
			subgraph[i] = [int(x) for x in v]
		
			if is_largest_complete_subgraph(G, subgraph[i]):
					print("The given subgraph is the largest complete subgraph.")
			else:
					print("The given subgraph is not the largest complete subgraph.")

	
def is_subgraph(G,tempG,s):
	n = len(s)
	for i in range(n):
		for j in range(i+1,n):
			if not ((s[j] in G[s[i]]) or (s[i] in G[s[j]])):
				return False
			try:
				tempG[s[i]].remove(s[j])
			except:
				try:
					tempG[s[j]].remove(s[i])
				except:
					pass
	return True
	
def is_subset(s1,s2):
	#is s2 a subset of s1
	if(len(s2)>len(s1)):
		return False
	else:
		for v in s2:
			if not(v in s1):
				return False
	return True


def have_no_common_nodes(graph_dict, key1, key2):
	# Get the lists of neighbors for key1 and key2
	neighbors_key1 = set(graph_dict.get(key1, []))
	neighbors_key2 = set(graph_dict.get(key2, []))

	# Check if the intersection of the neighbor lists is empty (no common nodes)
	return len(neighbors_key1.intersection(neighbors_key2)) == 0

	
def checker_problem1(graph_file, outfile):

	fp = open(outfile,"r")
	lines = fp.readlines()
	num_lines = len(lines)
	fp.close()

	if num_lines == 1:
		if lines[0].startswith('0'):
			print('Format is OK')
		else:
			print('ERROR: Wrong Format')
		sys.exit(1)

	G={}
	tempG={}
	with open(graph_file,'r') as f:
		rows = f.readlines()
		i = 0
		for row in rows:
			x = row.strip().split(' ')
			x = [int(v) for v in x]
			if i == 0:
				N = x[0]
				E = x[1]
				try:
					K1 = x[2]
					K2 = x[3]
				except:
					sys.exit(1)
			else:
				if x[0] in G:
					G[x[0]].append(x[1])
					tempG[x[0]].append(x[1])
				else:
					G[x[0]] = [x[1]]
					tempG[x[0]] = [x[1]]
			i+=1
			
	for i in range(1,N+1):
		if i not in G:
			G[i] = []
			tempG[i] = []
	i=0  
	subgraph = {}  
	for line in lines:
		if line.startswith('#'):
			#n = int(line.split(' ')[1].strip())
			#print(n)
			continue
		else:
			i += 1
			v = line.split(' ')
			subgraph[i] = [int(x) for x in v]
			if not (is_subgraph(G,tempG,subgraph[i])):
				print('ERROR: Subgraph %d is not complete'%i)
				sys.exit(1)

	if have_no_common_nodes(subgraph, 1, 2):
		pass
	else:
		print(f"Error: Subgraphs have common nodes.")
		sys.exit(1)

	if i!=2:
		print('ERROR: Output should have 2 subgraphs')
		sys.exit(1)
		
	print("Output is OK")


if __name__=='__main__':

	if not(os.path.isfile(sys.argv[1])):
		print("ERROR: Input Graph File does not exist")
		sys.exit(1)
	if not(os.path.isfile(sys.argv[2])):
		print("ERROR: Output Subgraph File does not exist")
		sys.exit(1)
	if sys.argv[3]=='1':
		checker_problem1(sys.argv[1], sys.argv[2])
	if sys.argv[3]=='2':
		checker_problem2(sys.argv[1],  sys.argv[2])
	if sys.argv[3]!='1' and sys.argv[3]!='2':
		print("Check the arguments carefully")