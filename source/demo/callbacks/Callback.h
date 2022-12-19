#pragma once


namespace Demo
{
inline namespace Callbacks
{
	/**
		@brief	Callback entity.

		Common form of callback template. Can't be instanced due to not implemented.
		Valid callback can be instanced only with function signature as argument.
	*/
	template< typename >
	class Callback;

	/**
		@brief	Callback entity.

		Callbacks can store the instruction to call certain function. It can be instructed to call the global functions, or member functions for certain object,
		or even lambda functions.
		Callback considered empty once it store no instruction to call. This implementation does not track the lifetime of stored objects,
		besides the stored lambda.

		The trick of this callback is to adapt the functions to common type of pointer to global function, named `Routine` internally.
		Callback stores the typeless shared context and the pointer to global functions. The first argument of such global function accept the context on call,
		and other arguments are forwarded from callback call.
		Shared context can store the pointer to non-owned host object, which will should be used on member function call. In this case the `Routine` adapts the
		member function and first argument of adapter will treated as `this` for member function.
		In opposite, owned lambda function may be stored in shared context. In this case the `Routine` adapts the `operator()` of lambda and accepts the pointer
		to lambda as first argument.

		@tparam	TResult		Result of callback call.
		@tparam	TArguments	List of arguments to call the callback.
	*/
	template< typename TResult, typename... TArguments >
	class Callback<TResult ( TArguments... )> final
	{
	// Friendship declarations.
	public:
		// Whether the callback is empty.
		friend inline const bool empty( const Callback& callback )	{ return callback.IsEmpty(); };

		// Swap the state of callbacks.
		friend inline void swap( Callback& left, Callback& right )	{ left.Swap( right ); };

	// Public static interface.
	public:
		// Get the callback from given global function pointer.
		template< auto FUNCTION >
		static inline Callback From();

		// Get the callback from given member function pointer and host object reference.
		template< auto FUNCTION, typename THost >
		static inline Callback From( THost& host );

		// Get the callback from given host object.
		template< typename THost >
		static inline Callback From( THost&& host );

	// Lifetime management.
	public:
		inline Callback() noexcept						= default;
		inline Callback( const Callback& ) noexcept		= default;
		inline Callback( Callback&& other ) noexcept	: m_routine{ std::exchange( other.m_routine, nullptr ) }, m_context{ std::move( other.m_context ) } {};
		inline ~Callback() noexcept						= default;


		inline Callback& operator = ( const Callback& ) noexcept	= default;
		inline Callback& operator = ( Callback&& other ) noexcept	{ Callback{ std::move( other ) }.Swap( *this ); return *this; };

	// Public interface.
	public:
		// Call the callback. The result will be value-initialized on empty callback call.
		inline TResult Call( TArguments... arguments ) const;

		// Reset the callback. Callback become empty after reset.
		inline void Reset();

		// Swap the state of callback with other one.
		inline void Swap( Callback& other );


		// Whether the callback is empty.
		inline const bool IsEmpty() const								{ return m_routine == nullptr; };

	// Public operators.
	public:
		inline TResult operator () ( TArguments... arguments ) const	{ return Call( std::forward<TArguments>( arguments )... ); };


		inline operator const bool () const								{ return !IsEmpty(); };
		inline const bool operator ! () const							{ return IsEmpty(); };


		inline const bool operator == ( const Callback& other ) const	{ return ( m_routine == other.m_routine ) && ( m_context == other.m_context ); };
		inline const bool operator != ( const Callback& other ) const	{ return ( m_routine != other.m_routine ) || ( m_context != other.m_context ); };

	// Private inner types.
	private:
		// Type of callback routine to be stored.
		using Routine = TResult (*)( void* const, TArguments... );

	// Private state.
	private:
		Routine					m_routine = nullptr;	// The routine to call.
		std::shared_ptr<void>	m_context;				// Context of routine on call.
	};
}
}
