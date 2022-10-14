#pragma once

#include <functional>
#include <memory>
#include <type_traits>
#include <unordered_map>

template <typename Callable>
struct lazy_invoke {
	using Result = std::invoke_result_t<Callable>;
	Callable callable;

	constexpr lazy_invoke(Callable &&callable) : callable(std::move(callable)) {}
	constexpr operator Result() const { return std::invoke(callable); }
};


auto make_int_cached(int key) {
	static std::unordered_map<int, int> cache;
	auto [it, res] = cache.try_emplace(key, lazy_invoke([] { return 1 + 1; }));
	return it->second;
}

template <typename M, typename K, typename F>
auto try_emplace_lazy(M &map, K &&key, F &&value_factory) {
	return map.try_emplace(std::forward<K>(key), lazy_invoke(std::forward<F>(value_factory)));
}
