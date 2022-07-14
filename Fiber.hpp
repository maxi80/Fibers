namespace fiber 
{
  class Fiber
	{
	public:
		using Function = std::function<void()>;

		explicit Fiber(std::string name, Function function);
		~Fiber();

		Fiber(const Fiber&) = delete;
		Fiber(Fiber&&) = delete;
		Fiber &operator=(const Fiber&) = delete;
		Fiber &operator=(Fiber&&) = delete;

		[[nodiscard]] bool IsReady() const;
		[[nodiscard]] bool IsFinished() const;
		[[nodiscard]] std::string_view GetName() const;
		
		void Tick();
		void Yield(std::optional<std::chrono::high_resolution_clock::duration> time = std::nullopt);

		static Fiber *Get();
		static void EnsureThreadIsAFiber();
	private:
		std::string m_Name;
		Function m_Function;

		void *m_Fiber = nullptr;
		void *m_ParentFiber = nullptr;
		void FiberProc();

		bool m_Finished = false;
		std::optional<std::chrono::high_resolution_clock::time_point> m_YieldUntil;
	};

	void Yield(std::optional<std::chrono::high_resolution_clock::duration> time = std::nullopt);
}
