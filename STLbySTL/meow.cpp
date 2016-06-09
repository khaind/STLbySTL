#include <algorithm>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace meow {
	namespace detail {
		struct vectorlike_tag { };
		struct listlike_tag { };
		struct associative_tag { };

		template <typename C> struct container_traits;


		template <typename T, typename A>
		struct container_traits<std::vector<T, A>> {
			typedef vectorlike_tag category;
		};

		template <typename T, typename A>
		struct container_traits<std::deque<T, A>> {
			typedef vectorlike_tag category;
		};


		template <typename T, typename A>
		struct container_traits<std::list<T, A>> {
			typedef listlike_tag category;
		};

		template <typename T, typename A>
		struct container_traits<std::forward_list<T, A>> {
			typedef listlike_tag category;
		};


		template <typename T, typename C, typename A>
		struct container_traits<std::set<T, C, A>> {
			typedef associative_tag category;
		};

		template <typename T, typename C, typename A>
		struct container_traits<std::multiset<T, C, A>> {
			typedef associative_tag category;
		};

		template <typename T, typename C, typename A>
		struct container_traits<std::unordered_set<T, C, A>> {
			typedef associative_tag category;
		};

		template <typename T, typename C, typename A>
		struct container_traits<std::unordered_multiset<T, C, A>> {
			typedef associative_tag category;
		};

		template <typename K, typename V, typename C, typename A>
		struct container_traits<std::map<K, V, C, A>> {
			typedef associative_tag category;
		};

		template <typename K, typename V, typename C, typename A>
		struct container_traits<std::multimap<K, V, C, A>> {
			typedef associative_tag category;
		};

		template <typename K, typename V, typename C, typename A>
		struct container_traits<std::unordered_map<K, V, C, A>> {
			typedef associative_tag category;
		};

		template <typename K, typename V, typename C, typename A>
		struct container_traits<std::unordered_multimap<K, V, C, A>> {
			typedef associative_tag category;
		};


		template <typename Container, typename X>
		void erase_helper(Container& c, const X& x, vectorlike_tag) {

			c.erase(std::remove(c.begin(), c.end(), x), c.end());
		}

		template <typename Container, typename Predicate>
		void erase_if_helper(Container& c, Predicate p, vectorlike_tag) {

			c.erase(std::remove_if(c.begin(), c.end(), p), c.end());
		}


		template <typename Container, typename X>
		void erase_helper(Container& c, const X& x, listlike_tag) {

			c.remove(x);
		}

		template <typename Container, typename Predicate>
		void erase_if_helper(Container& c, Predicate p, listlike_tag) {

			c.remove_if(p);
		}


		template <typename Container, typename X>
		void erase_helper(Container& c, const X& x, associative_tag) {

			c.erase(x);
		}

		template <typename Container, typename Predicate>
		void erase_if_helper(Container& c, Predicate p, associative_tag) {

			for (auto i = c.begin(); i != c.end(); ) {
				if (p(*i)) {
					c.erase(i++);
				}
				else {
					++i;
				}
			}
		}
	}

	template <typename Container, typename X> void erase(Container& c, const X& x) {
		detail::erase_helper(c, x, typename detail::container_traits<Container>::category());
	}

	template <typename Container, typename Predicate> void erase_if(Container& c, Predicate p) {
		detail::erase_if_helper(c, p, typename detail::container_traits<Container>::category());
	}
}

#include <iostream>
#include <numeric>
#include <ostream>
using namespace std;
using namespace meow;

template <typename Container> void print(const Container& c) {
	for (auto i = c.begin(); i != c.end(); ++i) {
		cout << *i << " ";
	}

	cout << endl;
}

int main() {
	vector<int> v(7);

	iota(v.begin(), v.end(), 1);
	print(v);
	for_each(v.begin(), v.end(), [](int& n) { n *= 11; });
	print(v);

	cout << "--" << endl;

	list<int> l(v.begin(), v.end());
	set<int> s(v.begin(), v.end());

	erase(v, 44);
	print(v);
	erase_if(v, [](int n) { return n % 2 == 0; });
	print(v);

	cout << "--" << endl;

	erase(l, 33);
	print(l);
	erase_if(l, [](int n) { return n % 2 != 0; });
	print(l);

	cout << "--" << endl;

	erase(s, 22);
	print(s);
	erase_if(s, [](int n) { return n > 40 && n < 70; });
	print(s);
}