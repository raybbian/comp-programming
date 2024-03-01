#include <bits/stdc++.h>

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

    DebugSingle(E data, int start_row, bool last) : start_row(start_row), last(last) {
        traverse(data, 0, false);
        cerr << "\n";
        cerr.flush();
    }

    void initialize_line(int depth) {
        cerr << "\n" << (last ? " " : "│");
        for (int i = 0; i < start_row + depth; i++)
            cerr << ' ';
    }

    template <printable T>
    void traverse(const T &t, int depth, bool nl)
        requires(!is_arithmetic<T>::value)
    {
        (void)nl;
        (void)depth;
        cerr << BOLDWHITE << t << RESET;
    }

    void traverse(bool t, int depth, bool nl) {
        (void)nl;
        (void)depth;
        if (t) {
            cerr << BOLDGREEN << 'T' << RESET;
        } else {
            cerr << BOLDRED << 'F' << RESET;
        }
    }

    template <printable T>
    void traverse(T t, int depth, bool nl)
        requires(is_arithmetic<T>::value)
    {
        (void)nl;
        (void)depth;
        if (t == numeric_limits<T>::max())
            cerr << BOLDWHITE "inf" << RESET;
        else if (t == numeric_limits<T>::min())
            cerr << BOLDWHITE << "-inf" << RESET;
        else
            cerr << BOLDWHITE << t << RESET;
    }

    void traverse(const char *t, int depth, bool nl) {
        (void)nl;
        (void)depth;
        string s = '"' + string(t) + '"';
        cerr << BOLDMAGENTA << s << RESET;
    }

    void traverse(const string &t, int depth, bool nl) {
        (void)nl;
        (void)depth;
        string s = '"' + t + '"';
        cerr << BOLDMAGENTA << s << RESET;
    }

    template <typename T, typename U> void traverse(const pair<T, U> &t, int depth, bool nl) {
        (void)nl;
        cerr << depth_color(depth) << "(" << RESET;
        traverse(t.fi, depth + 1, false);
        cerr << ": ";
        traverse(t.se, depth + 1, true);
        cerr << depth_color(depth) << ")" << RESET;
    }

    template <typename... T> void traverse(const tuple<T...> &t, int depth, bool nl) {
        (void)nl;
        int n = 0;
        auto handle_sep = [&](auto &&arg) {
            if (n)
                cerr << " ";
            traverse(arg, depth + 1, n);
            print_subscript(n, depth, n);
            n++;
        };
        apply(
            [&](auto &&...args) {
                cerr << depth_color(depth) << "(" << RESET;
                ((handle_sep(args)), ...);
                cerr << depth_color(depth) << ")" << RESET;
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
        if (nl)
            initialize_line(depth);
        cerr << depth_color(depth) << "[" << RESET;
        int n = 0;
        for (const auto &i : t) {
            if (n)
                cerr << " ";
            traverse(i, depth + 1, n);
            print_subscript(n, depth);
            n++;
        }
        cerr << depth_color(depth) << "]" << RESET;
    }
};

struct Debug {
    int max_name_len = 0;
    int min_tree_prefix = 3;
    queue<string> variable_names;

    template <typename T> void init_variable(T data, bool last) {
        cerr << (last ? "└" : "├");
        for (int i = 0; i < max_name_len - (int)variable_names.front().length(); i++) {
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

    template <typename... T> Debug(const char *s, const int line, const string &func, T &&...t) {
        // parse the arguments
        string name;
        int paren_cnt = 0;
        bool is_quote = false;
        for (int i = 0; s[i] != '\0'; i++) {
            if (s[i] == ',' && !paren_cnt && !is_quote) {
                variable_names.push(name);
                max_name_len = max(max_name_len, (int)name.length());
                name = "";
                while (s[i + 1] == ' ')
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
        cerr << RED << "Debug at line " << BOLDRED << line << RESET << RED << " in function "
             << BOLDRED << func << RESET << RED << ": " << RESET << '\n';
        debug(t...);
    }
};
