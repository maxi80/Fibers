#include "Fiber.hpp"

namespace fiber
{
  	class FiberMgr
    {
    public:
      FiberMgr() = default;
      ~FiberMgr() = default;

      FiberMgr(const FiberMgr&) = delete;
      FiberMgr(FiberMgr&&) = delete;
      FiberMgr &operator=(const FiberMgr&) = delete;
      FiberMgr &operator=(FiberMgr&&) = delete;

      void AddScriptFiber(std::shared_ptr<Fiber> fiber);
      bool RemoveFiber(std::string_view hash);
      void RemoveAllFibers();
      void ScriptTick();
    private:
      std::recursive_mutex m_FibersMutex;
      std::vector<std::shared_ptr<Fiber>> m_ScriptVMFibers;
      std::vector<std::shared_ptr<Fiber>> m_D3DFibers;
    };

	extern FiberMgr g_FiberMgr;
}
