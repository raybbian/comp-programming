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

vector<string> to_subscript = {
    "₀", "₁", "₂", "₃", "₄", "₅", "₆", "₇", "₈", "₉",
};
vector<string> d_to_color = {BLUE, CYAN, GREEN, YELLOW};
string depth_color(int d) { return d_to_color[d % d_to_color.size()]; }
void print_subscript(int n, int depth, int cnt = 0) {
  if (n <= 0 && cnt > 0)
    return;
  print_subscript(n / 10, depth, ++cnt);
  cerr << depth_color(depth) << to_subscript[n % 10] << RESET;
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

struct Debug {
  int max_variable_len = 0;
  int min_tree_prefix = 3;
  queue<string> variable_names;

  template <printable T>
  void debug_print(const T &t, int depth, bool newline, bool last)
    requires(!is_array<T>::value)
  {
    (void)depth;
    (void)newline;
    (void)last;
    cerr << BOLDWHITE << t << RESET;
  }
  void debug_print(const char *t, int depth, bool newline, bool last) {
    (void)depth;
    (void)newline;
    (void)last;
    cerr << '"' << BOLDWHITE << t << RESET << '"';
  }
  void debug_print(const string &t, int depth, bool newline, bool last) {
    (void)depth;
    (void)newline;
    (void)last;
    cerr << '"' << BOLDWHITE << t << RESET << '"';
  }
  template <typename T, typename U>
  void debug_print(const pair<T, U> &t, int depth, bool newline, bool last) {
    (void)newline;
    cerr << depth_color(depth) << "(" << RESET;
    debug_print(t.fi, depth + 1, false, last);
    cerr << ": ";
    debug_print(t.se, depth + 1, false, last);
    cerr << depth_color(depth) << ")" << RESET;
  }
  // https://stackoverflow.com/questions/1198260/how-can-you-iterate-over-the-elements-of-an-stdtuple
  template <typename... T>
  void debug_print(const tuple<T...> &t, int depth, bool newline, bool last) {
    if (newline) {
      cerr << ln << (last ? " " : "│");
      for (int i = 0; i < max_variable_len + min_tree_prefix + depth; i++)
        cerr << ' ';
    }
    int n = 0;
    auto handle_sep = [&](auto &&arg) {
      if (n)
        cerr << " ";
      debug_print(arg, depth + 1, n, last);
      print_subscript(n++, depth);
    };
    apply(
        [&](auto &&...args) {
          cerr << depth_color(depth) << "(" << RESET;
          ((handle_sep(args)), ...);
          cerr << depth_color(depth) << ")" << RESET;
        },
        t);
  }
  template <typename T>
  void debug_print(queue<T> t, int depth, bool newline, bool last) {
    // do not pass by reference!!
    if (newline) {
      cerr << ln << (last ? " " : "│");
      for (int i = 0; i < max_variable_len + min_tree_prefix + depth; i++)
        cerr << ' ';
    }
    cerr << depth_color(depth) << "[" << RESET;
    int n = 0;
    while (!t.empty()) {
      if (n)
        cerr << " ";
      debug_print(t.front(), depth + 1, n, last);
      t.pop();
      print_subscript(n++, depth);
    }
    cerr << depth_color(depth) << "]" << RESET;
  }
  template <topnpop T>
  void debug_print(T t, int depth, bool newline, bool last) {
    // do not pass by reference!!
    if (newline) {
      cerr << ln << (last ? " " : "│");
      for (int i = 0; i < max_variable_len + min_tree_prefix + depth; i++)
        cerr << ' ';
    }
    cerr << depth_color(depth) << "[" << RESET;
    int n = 0;
    while (!t.empty()) {
      if (n)
        cerr << " ";
      debug_print(t.top(), depth + 1, n, last);
      t.pop();
      print_subscript(n++, depth);
    }
    cerr << depth_color(depth) << "]" << RESET;
  }
  template <ranges::range T>
  void debug_print(const T &t, int depth, bool newline, bool last) {
    if (newline) {
      cerr << ln << (last ? " " : "│");
      for (int i = 0; i < max_variable_len + min_tree_prefix + depth; i++)
        cerr << ' ';
    }
    cerr << depth_color(depth) << "[" << RESET;
    int n = 0;
    for (const auto &i : t) {
      if (n)
        cerr << " ";
      debug_print(i, depth + 1, n, last);
      print_subscript(n++, depth);
    }
    cerr << depth_color(depth) << "]" << RESET;
  }

  template <typename T> void debug(const T &t) {
    cerr << "└─";
    for (int i = 0; i < max_variable_len - (int)variable_names.front().length();
         i++) {
      cerr << "─";
    }
    cerr << ' ' << RED << variable_names.front() << ": " << RESET;
    variable_names.pop();
    debug_print(t, 0, false, true);
    cerr << ln;
  }
  template <typename T, typename... U> void debug(const T &t, U &&...u) {
    cerr << "├";
    for (int i = 0; i < max_variable_len - (int)variable_names.front().length();
         i++) {
      cerr << "─";
    }
    cerr << ' ' << RED << variable_names.front() << ": " << RESET;
    variable_names.pop();
    debug_print(t, 0, false, false);
    cerr << ln;
    debug(u...);
  }
  template <typename... T>
  Debug(const char *s, const int sz, const int line, const string &func,
        T &&...t) {
    // parse the arguments
    string name;
    int paren_cnt = 0;
    bool is_quote = false;
    for (int i = 0; i < sz; i++) {
      if (s[i] == ',' && !paren_cnt && !is_quote) {
        variable_names.push(name);
        max_variable_len = max(max_variable_len, (int)name.length());
        name = "";
      } else {
        switch (s[i]) {
        case '(':
          paren_cnt++;
          break;
        case ')':
          paren_cnt--;
          break;
        case '"':
          is_quote = !is_quote;
          break;
        case ' ':
          if (!is_quote)
            continue;
          break;
        }
        name += s[i];
      }
    }
    variable_names.push(name);
    max_variable_len = max(max_variable_len, (int)name.length());

    // iterate over the data
    cerr << RED << "Debug at line " << BOLDRED << line << RESET << RED
         << " in function " << BOLDRED << func << RESET << RED << ": " << RESET
         << ln;
    debug(t...);
  }
};
