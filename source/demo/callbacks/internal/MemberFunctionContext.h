#pragma once


namespace Demo
{
inline namespace Callbacks
{
namespace Internal
{
	template< typename THost, typename TResult, typename... TArguments >
	struct MemberFunctionContext final
	{
		template< TResult (THost::*METHOD)( TArguments... ) >
		static inline TResult ProxyCall( void* const context, TArguments... arguments )
		{
			THost* host = reinterpret_cast<THost*>( context );

			return (host->*METHOD)( std::forward<TArguments>( arguments )... );
		};

		template< TResult (THost::*METHOD)( TArguments... ) const >
		static inline TResult ProxyCall( void* const context, TArguments... arguments )
		{
			THost* host = reinterpret_cast<THost*>( context );

			return (host->*METHOD)( std::forward<TArguments>( arguments )... );
		};

		template< TResult (THost::*METHOD)( TArguments... ) volatile >
		static inline TResult ProxyCall( void* const context, TArguments... arguments )
		{
			THost* host = reinterpret_cast<THost*>( context );

			return (host->*METHOD)( std::forward<TArguments>( arguments )... );
		};

		template< TResult (THost::*METHOD)( TArguments... ) const volatile >
		static inline TResult ProxyCall( void* const context, TArguments... arguments )
		{
			THost* host = reinterpret_cast<THost*>( context );

			return (host->*METHOD)( std::forward<TArguments>( arguments )... );
		};
	};
}
}
}
