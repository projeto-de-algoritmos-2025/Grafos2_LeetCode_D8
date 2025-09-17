class Solution(object):
    def maxScore(self, n, edges):
        from collections import defaultdict

        if n == 1:
            return 0 

        g = defaultdict(list)
        deg = [0]*n
        for u,v in edges:
            g[u].append(v)
            g[v].append(u)
            deg[u] += 1
            deg[v] += 1

        is_cycle = all(d == 2 for d in deg)

        order = []
        seen = [False]*n
        def dfs(u):
            order.append(u)
            seen[u] = True
            for w in g[u]:
                if not seen[w]:
                    dfs(w)

        if is_cycle:
            start = 0
        else:
            start = next(i for i,d in enumerate(deg) if d == 1)
        dfs(start)

        if is_cycle:
            evens = list(range(4, n+1, 2))
            start_odd = n if n % 2 == 1 else n-1
            odds_desc = list(range(start_odd, 2, -2))
            seq_vals = [1, 2] + evens + odds_desc
        else:
            odds = list(range(1, n+1, 2))
            start_even = n if n % 2 == 0 else n-1
            evens_desc = list(range(start_even, 0, -2))
            seq_vals = odds + evens_desc

        assign = [0]*n
        for i, node in enumerate(order):
            assign[node] = seq_vals[i]
        total = 0
        for u, v in edges:
            total += assign[u] * assign[v]
        return total