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

		result.m_routine = &Internal::FunctionProxyBuilder<TResult, TArguments...>::template Proxy<FUNCTION>;

		return result;
	}

	template< typename TResult, typename... TArguments >
	template< auto FUNCTION, typename THost >
	inline Callback<TResult ( TArguments... )> Callback<TResult ( TArguments... )>::From( THost* host )
	{
		Callback<TResult ( TArguments... )> result{};

		result.m_routine = &Internal::MethodProxyBuilder<THost, TResult, TArguments...>::template Proxy<FUNCTION>;
		result.m_context = std::static_pointer_cast<void>( std::shared_ptr<THost>{ host, []( THost* ) {} } );

		return result;
	}

	template< typename TResult, typename... TArguments >
	template< typename THost >
	inline Callback<TResult ( TArguments... )> Callback<TResult ( TArguments... )>::From( [[maybe_unused]] THost&& host )
	{
		static_assert( Internal::IsCallable<THost>::value, "Only callable types allowed." );
		Callback<TResult ( TArguments... )> result{};

		if constexpr( std::is_empty_v<THost> )
		{
			result.m_routine = &Internal::EmptyLambdaProxyBuilder<TResult, TArguments...>::template Proxy<THost>;
		}
		else
		{
			result.m_routine = &Internal::MethodProxyBuilder<THost, TResult, TArguments...>::template Proxy<&THost::operator()>;
			result.m_context = std::static_pointer_cast<void>( std::make_shared<THost>( std::forward<THost>( host ) ) );
		}

		return result;
	}

	template< typename TResult, typename... TArguments >
	inline TResult Callback<TResult ( TArguments... )>::Call( TArguments... arguments ) const
	{
		if( IsEmpty() )
		{
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
