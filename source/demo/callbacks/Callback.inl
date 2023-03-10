#pragma once


namespace Demo
{
inline namespace Callbacks
{
	template< typename TResult, typename... TArguments >
	template< auto FUNCTION >
	inline Callback<TResult ( TArguments... )> Callback<TResult ( TArguments... )>::From()
	{
		Callback<TResult ( TArguments... )> result{};

		result.m_routine = &Internal::GlobalFunctionContext<TResult, TArguments...>::template AdaptedCall<FUNCTION>;

		return result;
	}

	template< typename TResult, typename... TArguments >
	template< auto FUNCTION, typename THost >
	inline Callback<TResult ( TArguments... )> Callback<TResult ( TArguments... )>::From( THost& host )
	{
		Callback<TResult ( TArguments... )> result{};

		result.m_routine = &Internal::MemberFunctionContext<THost, TResult, TArguments...>::template AdaptedCall<FUNCTION>;
		result.m_context = std::static_pointer_cast<void>( std::shared_ptr<THost>{ &host, []( THost* ) {} } );

		return result;
	}

	template< typename TResult, typename... TArguments >
	template< typename THost >
	inline Callback<TResult ( TArguments... )> Callback<TResult ( TArguments... )>::From( [[maybe_unused]] THost&& host )
	{
		static_assert( Internal::IS_FUNCTOR<THost>, "Only functor types allowed." );
		Callback<TResult ( TArguments... )> result{};

		// Stateless lambdas should be stored differently.
		if constexpr( std::is_empty_v<THost> )
		{
			result.m_routine = &Internal::EmptyLambdaContext<TResult, TArguments...>::template AdaptedCall<THost>;
		}
		else
		{
			result.m_routine = &Internal::MemberFunctionContext<THost, TResult, TArguments...>::template AdaptedCall<&THost::operator()>;
			result.m_context = std::static_pointer_cast<void>( std::make_shared<THost>( std::forward<THost>( host ) ) );
		}

		return result;
	}

	template< typename TResult, typename... TArguments >
	inline TResult Callback<TResult ( TArguments... )>::Call( TArguments... arguments ) const
	{
		if( IsEmpty() )
		{
			// Use value initialization of result.
			return TResult();
		}

		return m_routine( m_context.get(), std::forward<TArguments>( arguments )... );
	}

	template< typename TResult, typename... TArguments >
	inline void Callback<TResult ( TArguments... )>::Reset()
	{
		Callback temp{};
		swap( *this, temp );
	}

	template< typename TResult, typename... TArguments >
	inline void Callback<TResult ( TArguments... )>::Swap( Callback& other )
	{
		using std::swap;
		swap( m_routine, other.m_routine );
		swap( m_context, other.m_context );
	}
}
}
