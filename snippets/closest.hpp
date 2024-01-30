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
