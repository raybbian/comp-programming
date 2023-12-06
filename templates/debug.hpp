// https://stackoverflow.com/questions/9158150/colored-output-in-c
#define RESET "\033[0m"
#define BLACK "\033[30m"              /* Black */
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define CYAN "\033[36m"               /* Cyan */
#define WHITE "\033[37m"              /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"   /* Bold White */

inline namespace Helpers {
unordered_map<int, string> to_subscript{{1, "₁"}, {2, "₂"}, {3, "₃"}, {4, "₄"},
                                        {5, "₅"}, {6, "₆"}, {7, "₇"}, {8, "₈"},
                                        {9, "₉"}, {0, "₀"}};
void print_subscript(int n, int cnt = 0) {
  if (n <= 0 && cnt > 0)
    return;
  print_subscript(n / 10, ++cnt);
  cerr << GREEN << to_subscript[n % 10] << RESET;
}
// https://www.reddit.com/r/cpp/comments/fblqwd/is_printable_check_if_a_value_is_printable_at/
template <typename T>
concept printable = requires(T &t) {
  { cout << t } -> same_as<ostream &>;
};
// for stack and priority_queue
template <typename T>
concept topnpop = requires(T t) {
  t.pop();
  t.top();
};
} // namespace Helpers

inline namespace Printers {
// predeclare
template <typename... T> void debug_print(const tuple<T...> &t);
template <typename T> void debug_print(queue<T> t);
template <topnpop T> void debug_print(T t);
template <ranges::range T> void debug_print(const T &t);

// functions
template <printable T>
void debug_print(const T &t)
  requires(!is_array<T>::value)
{
  cerr << CYAN << t << RESET;
}
void debug_print(const char *t) { cerr << CYAN << t << RESET; }
void debug_print(const string &t) { cerr << CYAN << t << RESET; }
template <typename T, typename U> void debug_print(const pair<T, U> &t) {
  cerr << "[";
  debug_print(t.fi);
  cerr << ": ";
  debug_print(t.se);
  cerr << "]";
}
// https://stackoverflow.com/questions/1198260/how-can-you-iterate-over-the-elements-of-an-stdtuple
template <typename... T> void debug_print(const tuple<T...> &t) {
  int n = 0;
  auto handle_sep = [&n](auto &&arg) {
    if (n)
      cerr << ", ";
    debug_print(arg);
    print_subscript(n++);
  };
  apply(
      [&handle_sep](auto &&...args) {
        cerr << "[";
        ((handle_sep(args)), ...);
        cerr << "]";
      },
      t);
}
template <typename T>
void debug_print(queue<T> t) { // do not pass by reference!!
  cerr << "[";
  int n = 0;
  while (!t.empty()) {
    if (n)
      cerr << ", ";
    debug_print(t.front());
    t.pop();
    print_subscript(n++);
  }
  cerr << "]";
}
template <topnpop T> void debug_print(T t) { // do not pass by reference!!
  cerr << "[";
  int n = 0;
  while (!t.empty()) {
    if (n)
      cerr << ", ";
    debug_print(t.top());
    t.pop();
    print_subscript(n++);
  }
  cerr << "]";
}
template <ranges::range T> void debug_print(const T &t) {
  cerr << "[";
  int n = 0;
  for (const auto &i : t) {
    if (n)
      cerr << ", ";
    debug_print(i);
    print_subscript(n++);
  }
  cerr << "]";
}
} // namespace Printers

inline namespace Parsers {
template <typename T> void debug(queue<string> names, const T &t) {
  cerr << "└── " << RED << names.front() << ": " << RESET;
  names.pop();
  debug_print(t);
  cerr << ln;
}
template <typename T, typename... U>
void debug(queue<string> names, const T &t, U &&...u) {
  cerr << "├── " << RED << names.front() << ": " << RESET;
  names.pop();
  debug_print(t);
  cerr << ln;
  debug(names, u...);
}
template <typename... T>
void debug_first(const char *s, const int sz, const int line,
                 const string &func, T &&...t) {
  queue<string> names;
  string name;
  int ignore_cnt = 0;
  for (int i = 0; i < sz; i++) {
    if (s[i] == ',' && !ignore_cnt) {
      names.push(name);
      name = "";
    } else {
      if (s[i] == ' ' && !ignore_cnt)
        continue;
      name += s[i];
      if (s[i] == '(')
        ignore_cnt++;
      else if (s[i] == ')')
        ignore_cnt--;
    }
  }
  names.push(name);
  cerr << RED << "Debug at line " << line << " in function " << func << ": "
       << RESET << ln;
  debug(names, t...);
}
} // namespace Parsers
