#include <bits/stdc++.h>
#include <type_traits>

using namespace std;

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

template <typename E> struct DebugSingle {
  int start_row;
  bool last;
  int phase = 0;

  int max_depth = 0;
  int grid_display = 0;
  int column_ptr = 0;
  unordered_map<int, int> max_len_per_column;

  DebugSingle(E data, int start_row, bool last)
      : start_row(start_row), last(last) {
    traverse(data, 0, false); // get max_depth and grid_display dimensions
    phase++;
    traverse(data, 0, false); // get num columns and max_variable_len per column
    phase++;
    traverse(data, 0, false); // print
    cerr << "\n";
  }

  void initialize_line(int depth) {
    cerr << "\n" << (last ? " " : "│");
    for (int i = 0; i < start_row + depth; i++)
      cerr << ' ';
  }

  template <printable T>
  int get_size(T t)
    requires(!is_arithmetic<T>::value)
  {
    stringstream os;
    os << t;
    return os.str().size();
  }

  template <printable T>
  int get_size(T t)
    requires(is_arithmetic<T>::value)
  {
    stringstream os;
    if (t == numeric_limits<T>::max())
      os << "inf";
    else if (t == numeric_limits<T>::min())
      os << "-inf";
    else
      os << t;
    return os.str().size();
  }

  template <printable T>
  void traverse(const T &t, int depth, bool nl)
    requires(!is_arithmetic<T>::value)
  {
    (void)nl;
    if (phase == 0) {
      max_depth = max(max_depth, depth);
    } else if (phase == 1) {
      max_len_per_column[column_ptr] =
          max(max_len_per_column[column_ptr], get_size(t));
      column_ptr++;
    } else if (phase == 2) {
      cerr << BOLDWHITE
           << setw(grid_display ? max_len_per_column[column_ptr] : 0) << t
           << RESET;
      column_ptr++;
    }
  }

  template <printable T>
  void traverse(T t, int depth, bool nl)
    requires(is_arithmetic<T>::value)
  {
    (void)nl;
    if (phase == 0) {
      max_depth = max(max_depth, depth);
    } else if (phase == 1) {
      max_len_per_column[column_ptr] =
          max(max_len_per_column[column_ptr], get_size(t));
      column_ptr++;
    } else if (phase == 2) {
      if (t == numeric_limits<T>::max())
        cerr << BOLDWHITE
             << setw(grid_display ? max_len_per_column[column_ptr] : 0) << "inf"
             << RESET;
      else if (t == numeric_limits<T>::min())
        cerr << BOLDWHITE
             << setw(grid_display ? max_len_per_column[column_ptr] : 0)
             << "-inf" << RESET;
      else
        cerr << BOLDWHITE
             << setw(grid_display ? max_len_per_column[column_ptr] : 0) << t
             << RESET;
      column_ptr++;
    }
  }

  void traverse(const char *t, int depth, bool nl) {
    (void)nl;
    if (phase == 0) {
      max_depth = max(max_depth, depth);
    } else if (phase == 1) {
      max_len_per_column[column_ptr] =
          max(max_len_per_column[column_ptr], (int)strlen(t));
      column_ptr++;
    } else if (phase == 2) {
      string s = '"' + string(t) + '"';
      cerr << BOLDMAGENTA
           << setw(grid_display ? max_len_per_column[column_ptr] : 0) << s
           << RESET;
      column_ptr++;
    }
  }

  void traverse(const string &t, int depth, bool nl) {
    (void)nl;
    if (phase == 0) {
      max_depth = max(max_depth, depth);
    } else if (phase == 1) {
      max_len_per_column[column_ptr] =
          max(max_len_per_column[column_ptr], (int)t.length());
      column_ptr++;
    } else if (phase == 2) {
      string s = '"' + t + '"';
      cerr << BOLDMAGENTA
           << setw(grid_display ? max_len_per_column[column_ptr] : 0) << s
           << RESET;
      column_ptr++;
    }
  }

  template <typename T, typename U>
  void traverse(const pair<T, U> &t, int depth, bool nl) {
    (void)nl;
    if (phase == 2) {
      cerr << depth_color(depth) << "(" << RESET;
    }
    traverse(t.fi, depth + 1, false);
    if (phase == 2) {
      cerr << ": ";
    }
    traverse(t.se, depth + 1, true);
    if (phase == 2) {
      cerr << depth_color(depth) << ")" << RESET;
    }
  }

  template <typename... T>
  void traverse(const tuple<T...> &t, int depth, bool nl) {
    (void)nl;
    int n = 0;
    auto handle_sep = [&](auto &&arg) {
      if (phase == 2 && n) {
        cerr << " ";
      }
      traverse(arg, depth + 1, n);
      if (phase == 2) {
        print_subscript(n, depth, n);
      }
      n++;
    };
    apply(
        [&](auto &&...args) {
          if (phase == 2) {
            cerr << depth_color(depth) << "(" << RESET;
          }
          ((handle_sep(args)), ...);
          if (phase == 2) {
            cerr << depth_color(depth) << ")" << RESET;
          }
        },
        t);
  }

  template <typename T> void traverse(queue<T> t, int depth, bool nl) {
    // do not pass by reference!!
    vector<T> converted;
    while (!t.empty()) {
      converted.push_back(t.front());
      t.pop();
    }
    traverse(converted, depth, nl);
  }

  template <typename T> void traverse(priority_queue<T> t, int depth, bool nl) {
    // do not pass by reference!!
    vector<T> converted;
    while (!t.empty()) {
      converted.push_back(t.top());
      t.pop();
    }
    traverse(converted, depth, nl);
  }

  template <typename T> void traverse(stack<T> t, int depth, bool nl) {
    deque<T> converted;
    while (!t.empty()) {
      converted.push_front(t.top());
      t.pop();
    }
    traverse(converted, depth, nl);
  }

  template <ranges::range T> void traverse(const T &t, int depth, bool nl) {
    if (phase == 0) {
      grid_display = max(grid_display, depth);
    }
    if (phase >= 1) {
      column_ptr = 0;
    }
    if (phase == 2) {
      if (nl)
        initialize_line(depth);
      cerr << depth_color(depth) << "[" << RESET;
    }
    int n = 0;
    for (const auto &i : t) {
      if (phase == 2 && n) {
        cerr << " ";
      }
      traverse(i, depth + 1, n);
      if (phase == 2) {
        print_subscript(n, depth);
      }
      n++;
    }
    if (phase == 2) {
      cerr << depth_color(depth) << "]" << RESET;
    }
  }
};

struct Debug {
  int max_name_len = 0;
  int min_tree_prefix = 3;
  queue<string> variable_names;

  template <typename T> void init_variable(T data, bool last) {
    cerr << (last ? "└" : "├");
    for (int i = 0; i < max_name_len - (int)variable_names.front().length();
         i++) {
      cerr << "─";
    }
    cerr << ' ' << RED << variable_names.front() << ": " << RESET;
    variable_names.pop();
    DebugSingle(data, max_name_len + min_tree_prefix, last);
  }

  template <typename T> void debug(const T &t) { init_variable(t, true); }

  template <typename T, typename... U> void debug(const T &t, U &&...u) {
    init_variable(t, false);
    debug(u...);
  }

  template <typename... T>
  Debug(const char *s, const int sz, const int line, const string &func,
        T &&...t) {
    // parse the arguments
    string name;
    int paren_cnt = 0;
    bool is_quote = false;
    for (int i = 0; i < sz - 1; i++) {
      if (s[i] == ',' && !paren_cnt && !is_quote) {
        variable_names.push(name);
        max_name_len = max(max_name_len, (int)name.length());
        name = "";
        if (i < sz - 2 && s[i + 1] == ' ')
          i++;
      } else {
        switch (s[i]) {
        case '(':
        case '{':
        case '<':
        case '[':
          paren_cnt++;
          break;
        case ')':
        case '}':
        case '>':
        case ']':
          paren_cnt--;
          break;
        case '"':
          is_quote = !is_quote;
          break;
        }
        name += s[i];
      }
    }
    variable_names.push(name);
    max_name_len = max(max_name_len, (int)name.length());

    // iterate over the data
    cerr << RED << "Debug at line " << BOLDRED << line << RESET << RED
         << " in function " << BOLDRED << func << RESET << RED << ": " << RESET
         << '\n';
    debug(t...);
  }
};
