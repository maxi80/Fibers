#include "Fiber_pool.hpp"
#include "Fiber_mgr.hpp"

namespace fibers
{
	constexpr size_t FIBER_POOL_SIZE = 8;

	void FiberPool::QueueTask(std::function<void()> task)
	{
		std::lock_guard lock(m_TasksMutex);
		m_Tasks.push_back(std::move(task));
	}

	void FiberPool::Init()
	{
		m_Fibers.reserve(FIBER_POOL_SIZE);

		for (size_t i = 0; i < FIBER_POOL_SIZE; ++i)
		{
			auto fiber = std::make_shared<Fiber>(fmt::format(XORSTR("Job_{}"), i), [this, i] { Run(i); });

			m_Fibers.push_back(std::move(fiber));
		}
	}

	void FiberPool::Uninit() { m_Fibers = {}; }

	void FiberPool::Tick()
	{
		for (auto &fiber : m_Fibers)
		{
			fiber->Tick();
		}
	}

	void FiberPool::Run(size_t index)
	{
		while (true)
		{
			decltype(m_Tasks)::value_type task;
			{
				std::lock_guard lock(m_TasksMutex);
				if (!m_Tasks.empty())
				{
					task = std::move(m_Tasks.front());
					m_Tasks.pop_front();
				}
			}

			if (task)
			{
				std::invoke(task);
			}

			Yield();
		}
	}
} 
