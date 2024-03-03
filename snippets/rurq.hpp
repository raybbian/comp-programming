struct RURQ {
    struct F { // lazy update
        ll inc = 0;
        F() {}
        F(int x) { inc = x; }
        F &operator*=(const F &a) {
            inc += a.inc;
            return *this;
        }
    };
    vector<F> lazy;
    struct T { // data you need to store for each interval
        ll sz = 1, mn = 1e9, sum = 0;
        T() {}
        T(int x) { mn = sum = x; }
        friend T operator+(const T &a, const T &b) {
            T res;
            res.sz = a.sz + b.sz;
            res.mn = min(a.mn, b.mn), res.sum = a.sum + b.sum;
            return res;
        }
        T &operator*=(const F &a) {
            mn += a.inc;
            sum += (ll)sz * a.inc;
            return *this;
        }
    };
    vector<T> seg;
    int SZ = 1;
    void init(const vector<T> &_seg) { //_seg len must be power of 2!
        while (SZ < (int)_seg.size())
            SZ *= 2;
        seg.resize(2 * SZ);
        lazy.resize(2 * SZ);
        for (int i = 0; i < SZ; i++)
            seg[SZ + i] = _seg[i];
        for (int i = SZ; i >= 1; i--)
            pull(i);
    }
    void push(int ind) { /// modify values for current node
        seg[ind] *= lazy[ind];
        if (ind < SZ)
            for (int i = 0; i < 2; i++)
                lazy[2 * ind + i] *= lazy[ind];
        lazy[ind] = F();
    } // recalc values for current node
    void pull(int ind) { seg[ind] = seg[2 * ind] + seg[2 * ind + 1]; }
    void upd(int lo, int hi, F inc, int ind, int L, int R) {
        push(ind);
        if (hi < L || R < lo)
            return;
        if (lo <= L && R <= hi) {
            lazy[ind] = inc;
            push(ind);
            return;
        }
        int M = (L + R) / 2;
        upd(lo, hi, inc, 2 * ind, L, M);
        upd(lo, hi, inc, 2 * ind + 1, M + 1, R);
        pull(ind);
    }
    void upd(int lo, int hi, int inc) { upd(lo, hi, {inc}, 1, 0, SZ - 1); }
    T query(int lo, int hi, int ind, int L, int R) {
        push(ind);
        if (lo > R || L > hi)
            return T();
        if (lo <= L && R <= hi)
            return seg[ind];
        int M = (L + R) / 2;
        return query(lo, hi, 2 * ind, L, M) + query(lo, hi, 2 * ind + 1, M + 1, R);
    }
    T query(int lo, int hi) { return query(lo, hi, 1, 0, SZ - 1); }
};
