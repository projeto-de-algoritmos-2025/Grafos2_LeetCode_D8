import java.util.*;

class Solution {

    int[] roots;
    Map<String, Integer> nonCriticalEdges = new HashMap<>();
    Map<String, Integer> criticalEdges = new HashMap<>();
    int[][] edgesData;
    Set<Integer>[] graph;

    @SuppressWarnings("unchecked")
    public List<List<Integer>> findCriticalAndPseudoCriticalEdges(int n, int[][] edges) {
        PriorityQueue<int[]> pq = new PriorityQueue<>((a, b) -> a[2] - b[2]);
        edgesData = new int[edges.length][];
        for (int i = 0; i < edges.length; i++) {
            int[] e = edges[i];
            pq.add(edgesData[i] = new int[]{Math.min(e[0], e[1]), Math.max(e[0], e[1]), e[2], i});
        }

        roots = new int[n];
        graph = new HashSet[n];
        for (int i = 0; i < n; i++) {
            roots[i] = i;
            graph[i] = new HashSet<>();
        }

        while (pq.size() > 0) {
            int[] e = pq.poll();
            int u = get(e[0]);
            int v = get(e[1]);

            graph[e[0]].add(e[1]);
            graph[e[1]].add(e[0]);

            if (u != v) {
                connect(u, v);
                criticalEdges.put(getKey(e), e[3]);
            } else {
                dfs(-1, e[0], e, new Stack<>(), new boolean[n]);
                graph[e[0]].remove(e[1]);
                graph[e[1]].remove(e[0]);

            }
        }

        return Arrays.asList(new ArrayList<>(criticalEdges.values()), new ArrayList<>(nonCriticalEdges.values()));
    }

    private String getKey(int[] e) {
        return e[0] + "_" + e[1];
    }

    private boolean dfs(int prev, int u, int[] lastEdge, Stack<int[]> stack, boolean[] visited) {
        if (visited[u]) {
            // stack has the last edge and the start edge in the cycle, ignore them
            int ignore = 2;
            while (ignore > 0) {
                int[] e = stack.pop();
                if (e[0] == u || e[1] == u) {
                    ignore--;
                }
                String key = getKey(e);
                Integer val = criticalEdges.get(key);
                if (val != null && edgesData[val][2] == lastEdge[2]) {
                    nonCriticalEdges.put(key, criticalEdges.get(key));
                    criticalEdges.remove(key);
                }

                Integer ncVal = nonCriticalEdges.get(key);
                if (ncVal != null && edgesData[ncVal][2] == lastEdge[2]) {
                    nonCriticalEdges.put(getKey(lastEdge), lastEdge[3]);
                }
            }
            return false;
        }

        visited[u] = true;
        for (int v : graph[u]) {
            if (v != prev && v != u) {
                stack.add(new int[]{Math.min(u, v), Math.max(u, v)});
                if (!dfs(u, v, lastEdge, stack, visited)) {
                    return false;
                }
                stack.pop();
            }
        }
        return true;
    }

    private void connect(int u, int v) {
        int rU = get(u);
        int rV = get(v);
        roots[rU] = rV;
    }

    private int get(int u) {
        int orig = u;
        while (u != roots[u]) {
            u = roots[u];
        }
        while (orig != roots[orig]) {
            int tmp = roots[orig];
            roots[orig] = u;
            orig = tmp;
        }

        return u;
    }
}