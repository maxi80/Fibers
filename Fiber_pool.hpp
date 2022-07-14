#include "Fiber.hpp"

namespace fiber 
{
  class FiberPool
	{
	  public:
		FiberPool() = default;
		~FiberPool() = default;
		FiberPool(const FiberPool &) = delete;
		FiberPool(FiberPool &&) = delete;
		FiberPool &operator=(const FiberPool &) = delete;
		FiberPool &operator=(FiberPool &&) = delete;

	  public:
		std::vector<std::shared_ptr<Fiber>> m_Fibers;
		std::deque<std::function<void()>> m_Tasks;

	  public:
		void QueueTask(std::function<void()> task);
		void Init();
		void Uninit();
		void Tick();

	  private:
		void Run(size_t index);
		std::recursive_mutex m_TasksMutex;
	};
	inline FiberPool g_FiberPool;

	template <typename F> void QueueFiberTask(F &&task) { g_FiberPool.QueueTask(std::forward<F>(task)); }

	class QueueFiber
	{
	  public:
		explicit QueueFiber(const std::function<void()> &functor) : m_Functor(functor) {}
		explicit QueueFiber(std::function<void()> &&functor) : m_Functor(std::move(functor)) {}

	  public:
		template <typename... Args> explicit QueueFiber(Args &&...args) : m_Functor(std::forward<Args>(args)...) {}
		void operator()() { QueueFiberTask(m_Functor); }

	  private:
		std::function<void()> m_Functor;
	};
}
