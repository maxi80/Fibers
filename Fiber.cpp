#include "Fiber.hpp"

namespace fiber 
{
    Fiber::Fiber(std::string name, Function function) :
		m_Name(std::move(name)),
		m_Function(std::move(function))
	{
		m_Fiber = CreateFiber(0, [](void *param)
		{
			reinterpret_cast<Fiber*>(param)->FiberProc();
		}, this);
	}

	Fiber::~Fiber()
	{
		if (m_Fiber)
			DeleteFiber(m_Fiber);
	}

	bool Fiber::IsReady() const
	{
		return !m_YieldUntil || std::chrono::high_resolution_clock::now() >= *m_YieldUntil;
	}

	bool Fiber::IsFinished() const
	{
		return m_Finished;
	}

	std::string_view Fiber::GetName() const
	{
		return m_Name;
	}

	joaat_t Fiber::GetNameHash() const
	{
		return joaat(m_Name);
	}

	void Fiber::Tick()
	{
		if (!m_Finished)
		{
			m_ParentFiber = GetCurrentFiber();

			if (IsReady())
				SwitchToFiber(m_Fiber);
		}
	}

	void Fiber::Yield(std::optional<std::chrono::high_resolution_clock::duration> time)
	{
		if (time)
			m_YieldUntil = std::chrono::high_resolution_clock::now() + *time;
		else
			m_YieldUntil = std::nullopt;

		SwitchToFiber(m_ParentFiber);
	}

	Fiber *Fiber::Get()
	{
		return reinterpret_cast<Fiber*>(GetFiberData());
	}

	void Fiber::EnsureThreadIsAFiber()
	{
		if (!IsThreadAFiber())
			ConvertThreadToFiber(nullptr);
	}

	void Fiber::FiberProc()
	{
		if (m_Function)
			m_Function();

		m_Finished = true;
		SwitchToFiber(m_ParentFiber);
	}

	void Yield(std::optional<std::chrono::high_resolution_clock::duration> time)
	{
		Fiber::Get()->Yield(time);
	}
}
