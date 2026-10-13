#pragma once
#include "algo/common.h"

namespace algo::ds {

// Maps elements in [First, First + Size) to child slots. Any sequence of Key
// works as a word: std::string, std::vector<int>, ...
template <typename T, T First, int Size>
struct range_alphabet {
    using Key = T;
    static constexpr int size = Size;
    static int index(Key c) {
        return (int)(c - First);
    }
    static Key key(int i) {
        return (Key)(First + i);
    }
};
using lower_alphabet = range_alphabet<char, 'a', 26>;
using upper_alphabet = range_alphabet<char, 'A', 26>;
using digit_alphabet = range_alphabet<char, '0', 10>;
using binary_alphabet = range_alphabet<int, 0, 2>;

// Trie with per-node counts.
//   trie<lower_alphabet> t;
//   t.insert("abc");                 // node of the word
//   t.count("abc");                  // copies inserted
//   t.count_prefix("ab");            // words starting with "ab"
//   t.find("ab");                    // node, or -1
//   t.longest_prefix_match("abcde"); // {len, node} of longest word that
//                                    // prefixes it, {-1, -1} if none
//   t.word(node);                    // string (or vector<Key>) ending there
template <typename A>
struct trie {
    using Key = typename A::Key;
    using Word = std::conditional_t<std::is_same_v<Key, char>, std::string,
                                    std::vector<Key>>;

    trie() : nodes(1) {
    }
    int size() const {
        return (int)nodes.size();
    }
    template <typename S>
    int insert(const S &s) {
        int u = 0;
        nodes[u].prefix++;
        for (Key c : s) {
            u = child(u, c, true);
            nodes[u].prefix++;
        }
        nodes[u].end++;
        return u;
    }
    int insert(const char *s)
        requires std::is_same_v<Key, char>
    {
        return insert(std::string_view(s));
    }
    // Removes one copy of s; the word must be present.
    template <typename S>
    void erase(const S &s) {
        int u = 0;
        nodes[u].prefix--;
        for (Key c : s) {
            u = child(u, c, false);
            assert(u != -1);
            nodes[u].prefix--;
        }
        assert(nodes[u].end > 0);
        nodes[u].end--;
    }
    void erase(const char *s)
        requires std::is_same_v<Key, char>
    {
        erase(std::string_view(s));
    }
    template <typename S>
    int find(const S &s) const {
        int u = 0;
        for (Key c : s) {
            u = nodes[u].next[A::index(c)];
            if (u == -1) break;
        }
        return u;
    }
    int find(const char *s) const
        requires std::is_same_v<Key, char>
    {
        return find(std::string_view(s));
    }
    template <typename S>
    int count(const S &s) const {
        int u = find(s);
        return u == -1 ? 0 : nodes[u].end;
    }
    int count(const char *s) const
        requires std::is_same_v<Key, char>
    {
        return count(std::string_view(s));
    }
    template <typename S>
    int count_prefix(const S &s) const {
        int u = find(s);
        return u == -1 ? 0 : nodes[u].prefix;
    }
    int count_prefix(const char *s) const
        requires std::is_same_v<Key, char>
    {
        return count_prefix(std::string_view(s));
    }
    template <typename S>
    std::pair<int, int> longest_prefix_match(const S &s) const {
        std::pair<int, int> best{-1, -1};
        int u = 0, len = 0;
        if (nodes[u].end) best = {len, u};
        for (Key c : s) {
            u = nodes[u].next[A::index(c)];
            if (u == -1) break;
            len++;
            if (nodes[u].end) best = {len, u};
        }
        return best;
    }
    std::pair<int, int> longest_prefix_match(const char *s) const
        requires std::is_same_v<Key, char>
    {
        return longest_prefix_match(std::string_view(s));
    }
    Word word(int u) const {
        Word w;
        for (; u != 0; u = nodes[u].parent)
            w.push_back(A::key(nodes[u].slot));
        std::reverse(w.begin(), w.end());
        return w;
    }
    friend std::ostream &operator<<(std::ostream &os, const trie &t) {
        bool first = true;
        auto dfs = [&](auto &self, int u) -> void {
            if (t.nodes[u].end) {
                if (!first) os << ", ";
                first = false;
                Word w = t.word(u);
                if constexpr (std::is_same_v<Key, char>) {
                    os << '"' << w << '"';
                } else {
                    os << "[";
                    for (int i = 0; i < (int)w.size(); i++)
                        os << (i ? ", " : "") << w[i];
                    os << "]";
                }
                os << ": " << t.nodes[u].end;
            }
            for (int v : t.nodes[u].next)
                if (v != -1) self(self, v);
        };
        os << "{";
        dfs(dfs, 0);
        return os << "}";
    }

private:
    struct node {
        std::array<int, A::size> next;
        int parent, slot; // edge into this node; unused on the root
        int end = 0, prefix = 0;
        node(int _parent = -1, int _slot = -1) : parent(_parent), slot(_slot) {
            next.fill(-1);
        }
    };
    std::vector<node> nodes;

    int child(int u, Key c, bool create) {
        int i = A::index(c);
        if (nodes[u].next[i] == -1 && create) {
            nodes.emplace_back(u, i); // may reallocate, so index afterwards
            nodes[u].next[i] = size() - 1;
        }
        return nodes[u].next[i];
    }
};

} // namespace algo::ds
