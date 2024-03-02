struct DSU {
    vector<int> e;
    DSU(int n) { e = vector<int>(n, -1); }

    int get(int x) { return e[x] < 0 ? x : e[x] = get(e[x]); }

    bool is_same(int a, int b) { return get(a) == get(b); }

    int size(int x) { return -e[get(x)]; }

    bool unite(int x, int y) {
        x = get(x), y = get(y);
        if (x == y)
            return false;
        if (e[x] > e[y])
            swap(x, y);
        e[x] += e[y];
        e[y] = x;
        return true;
    }
};
