#pragma once


namespace Demo
{
inline namespace Callbacks
{
	template< typename TDeclaration >
	class Callback;


	template< typename TResult, typename... TArguments >
	class Callback<TResult ( TArguments... )> final
	{
	public:
		friend inline const bool empty( const Callback& callback )	{ return callback.IsEmpty(); };
		friend inline void swap( Callback& left, Callback& right )	{ left.Swap( right ); };

	public:
		template< auto FUNCTION >
		static inline Callback From();

		template< auto FUNCTION, typename THost >
		static inline Callback From( THost* host );

		template< typename THost >
		static inline Callback From( THost&& host );

	public:
		inline Callback() noexcept						= default;
		inline Callback( const Callback& ) noexcept		= default;
		inline Callback( Callback&& other ) noexcept	: m_routine{ std::exchange( other.m_routine, nullptr ) }, m_context{ std::move( other.m_context ) } {};
		inline ~Callback() noexcept						= default;


		inline Callback& operator = ( const Callback& ) noexcept	= default;
		inline Callback& operator = ( Callback&& other ) noexcept	{ Callback{ std::move( other ) }.Swap( *this ); return *this; };

	public:
		inline TResult Call( TArguments... arguments ) const;
		inline void Reset();
		inline void Swap( Callback& other );

		inline const bool IsEmpty() const								{ return m_routine == nullptr; };

	public:
		inline TResult operator () ( TArguments... arguments ) const	{ return Call( std::forward<TArguments>( arguments )... ); };


		inline operator const bool () const								{ return !IsEmpty(); };
		inline const bool operator ! () const							{ return IsEmpty(); };


		inline const bool operator == ( const Callback& other ) const	{ return ( m_routine == other.m_routine ) && ( m_context == other.m_context ); };
		inline const bool operator != ( const Callback& other ) const	{ return ( m_routine != other.m_routine ) || ( m_context != other.m_context ); };

	private:
		using Routine = TResult (*)( void* const, TArguments... );

	private:
		Routine					m_routine = nullptr;
		std::shared_ptr<void>	m_context;
	};
}
}
