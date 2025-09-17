class Solution(object):
    def minMaxWeight(self, n, edges, threshold):
        from collections import deque

        if not edges:
            return -1

        def can(mid):

            graph_rev = [[] for _ in range(n)]
            for u, v, w in edges:
                if w <= mid:
                    graph_rev[v].append(u)

            seen = [False] * n
            q = deque([0])
            seen[0] = True
            while q:
                x = q.popleft()
                for y in graph_rev[x]:
                    if not seen[y]:
                        seen[y] = True
                        q.append(y)
            return all(seen)

        low, high = 1, max(w for _, _, w in edges)
        ans = -1
        while low <= high:
            mid = (low + high) // 2
            if can(mid):
                ans = mid
                high = mid - 1
            else:
                low = mid + 1
        return ans
