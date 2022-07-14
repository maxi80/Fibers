#include "Fiber_Mgr.hpp"

namespace fiber
{
  FiberMgr g_FiberMgr;

    void FiberMgr::AddScriptFiber(std::shared_ptr<Fiber> fiber)
    {
      std::lock_guard lock(m_FibersMutex);
      m_ScriptVMFibers.push_back(std::move(fiber));
    }

    bool FiberMgr::RemoveFiber(std::string_view name)
    {
      return RemoveFiber(joaat(name));
    }

    bool FiberMgr::RemoveFiber(joaat_t hash)
    {
      std::lock_guard lock(m_FibersMutex);
      for (auto it = m_ScriptVMFibers.begin(); it != m_ScriptVMFibers.end(); ++it)
      {
        const auto &fiber = *it;

        if (fiber->GetNameHash() == hash)
        {
          m_ScriptVMFibers.erase(it);
          return true;
        }
      }

      return false;
    }

    void FiberMgr::RemoveAllFibers()
    {
      std::lock_guard lock(m_FibersMutex);
      m_ScriptVMFibers.clear();
    }

    void FiberMgr::ScriptTick()
    {
      std::lock_guard lock(m_FibersMutex);
      for (auto it = m_ScriptVMFibers.begin(); it != m_ScriptVMFibers.end();)
      {
        const auto &fiber = *it;

        fiber->Tick();

        if (fiber->IsFinished())
        {
          it = m_ScriptVMFibers.erase(it);
        }
        else
        {
          ++it;
        }
      }
    }
}
