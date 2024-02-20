#include <bits/stdc++.h>

using namespace std;

// policy based ds
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;
typedef tree<int, null_type, less<int>, rb_tree_tag,
             tree_order_statistics_node_update>
    ordered_set;
typedef tree<pair<int, int>, null_type, less<pair<int, int>>, rb_tree_tag,
             tree_order_statistics_node_update>
    ordered_multiset; // use index as pair second

// typing is hard
using ll = long long;
using db = long double;
using str = string;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using pdd = pair<db, db>;
#define fi first
#define se second
#define ln "\n"
#define pb push_back
#define pf push_front
#define eb emplace_back
#define ef emplace_front
#define ins insert

// n-dimensional vector
template <typename T> vector<T> ndvec(size_t size, T initial_value) {
  return vector<T>(size, initial_value);
}

template <typename T, typename... U> auto ndvec(size_t head, U &&...u) {
  auto inner = ndvec<T>(u...);
  return vector<decltype(inner)>(head, inner);
}

// better math
ll c_div(ll a, ll b) { return a / b + ((a ^ b) > 0 && a % b); }

ll f_div(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }

// closest element in a set
template <typename T>
auto closest_element(T &set, const typename T::value_type &value)
    -> decltype(set.begin()) {
  const auto it = set.lower_bound(value);
  if (it == set.begin())
    return it;
  const auto prev_it = prev(it);
  return (it == set.end() || value - *prev_it <= *it - value) ? prev_it : it;
}

// custom hash
struct chash {
  static uint64_t splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
  }

  size_t operator()(uint64_t x) const {
    static const uint64_t FIXED_RANDOM =
        chrono::steady_clock::now().time_since_epoch().count();
    return splitmix64(x + FIXED_RANDOM);
  }

  template <typename T1, typename T2>
  size_t operator()(const pair<T1, T2> &x) const {
    uint64_t a = (*this)(x.fi);
    uint64_t b = (*this)(x.se);
    return a + 3 * b;
  }
};

// grid problems
vector<pii> steps_orthogonal = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
vector<pii> steps_diagonal = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}};
vector<pii> steps_8dirs = {{1, 0}, {0, 1},  {-1, 0}, {0, -1},
                           {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

#ifdef LOCAL
#include "/home/rayb/Documents/comp-programming/debug/debug.hpp"
#define dbg(...)                                                               \
  Debug(#__VA_ARGS__, sizeof(#__VA_ARGS__), __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define dbg(...) 42
#endif

// insert snippets below:

void solve() {}

int main() {
  cin.tie(0)->sync_with_stdio(false);
  array<int, 15> arr;
  vector<int> vec;
  deque<int> deq;
  forward_list<int> fwl;
  list<int> lst;
  stack<int> st;
  queue<int> q;
  priority_queue<int> pq;
  set<int> s;
  multiset<int> ms;
  map<int, int> mp;
  multimap<int, int> mmp;
  unordered_set<int> us;
  unordered_multiset<int> ums;
  unordered_map<int, int> ump;
  unordered_multimap<int, int> ummp;

  vector<unordered_map<pair<int, int>, string, chash>> what;

  for (int i = 8; i < 12; i++) {
    srand(time(NULL));
    int n = rand() % 10 + 1;
    arr[i] = n;
    vec.pb(n);
    deq.pb(n);
    fwl.pf(n);
    lst.pb(n);
    st.push(n);
    q.push(n);
    pq.push(n);
    s.ins(n);
    ms.ins(n);
    mp[i] = n;
    mmp.ins({n, i});
    us.ins(i);
    ums.ins(n);
    ump[i] = n;
    ummp.ins({n, i});
    unordered_map<pair<int, int>, string, chash> temp;
    for (int j = 0; j < 5; j++) {
      srand(time(NULL));
      int y = rand() % 10 + 1;
      temp.ins({{y, j}, "oogaboogalmaoxd"});
    }
    what.pb(temp);
  }
  vector<int> infns(5, INT_MAX);
  infns[3] = INT_MIN;
  dbg(arr, vec, deq, fwl, lst, st, q, pq, s, ms, mp, mmp, us, ums, ump, ummp);
  dbg(what, "HI , ", infns);

  int a = 5;
  int b = 8;
  tuple t = {1, 'a', 2.2f};
  pii pr = {2, 3};
  string str = "THIS IS A STRING";
  dbg(t, pr, c_div(a, b), a, b, "THIS IS SO HARWEHD",
      pair<string, int>{"LMLFAODAS", 0}, str);

  auto dvec =
      ndvec<tuple<int, int, int>>(7, 7, 7, tuple<int, int, int>{0, 0, 0});
  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 7; j++) {
      for (int k = 0; k < 7; k++) {
        dvec[i][j][k] = tuple<int, int, int>{i, i + j, i + j + k};
      }
    }
  }
  dbg(dvec);
  auto big_tup = make_tuple(t, pr, dvec);
  dbg(big_tup);
}
