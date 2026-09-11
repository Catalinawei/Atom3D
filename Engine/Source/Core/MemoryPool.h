#pragma once
#include <vector>
#include <exception>
#include <memory>

namespace AtomToolKit {

	template <typename T>
	class MemoryPool {
	public:

		MemoryPool() {
			pool.resize(MAX_POOL_SIZE);
			alive.resize(MAX_POOL_SIZE);
			for (int i = 0; i < pool.size(); ++i) {
				freePool.push_back(i);
			}
		}

		T* Create() {
			if (freePool.empty()) {
				return nullptr;
			}
			int id = freePool.back();
			freePool.pop_back();

			new(&pool[id]) T();

			alive[id] = true;

			return &pool[id];
		}

		void DestroyObject(int id) {
			if (id > pool.size() || id < 0)
				throw std::exception("ID out of bounds!");

			pool[id].~T();

			freePool.push_back(id);
		}

		T* GetObject(size_t id) {
			if (id > pool.size() || id < 0) {
				//throw std::exception("ID out of bounds!");
				return nullptr;
			}
			return &pool[id];
		}

		constexpr int GetMaxSize() const noexcept {
			return MAX_POOL_SIZE;
		}

		constexpr int GetBegin() const noexcept {
			return freePool.begin();
		}

		size_t GetPoolSize() const {
			return pool.size();
		}
		 
		bool isValid(size_t id) {
			if (id < 0 || id > pool.size())
				return false;
			if (alive[id])
				return true;
			return false;

		}

	private:

		static constexpr int MAX_POOL_SIZE = 2048;
		std::vector<T> pool;
		std::vector<int> freePool;
		std::vector<bool> alive;
	};
}