#pragma once


namespace Demo
{
inline namespace Callbacks
{
namespace Internal
{
	template< typename TResult, typename... TArguments >
	struct GlobalFunctionContext final
	{
		template< TResult (*FUNCTION)( TArguments... ) >
		static inline TResult ProxyCall( void* const, TArguments... arguments )
		{
			return FUNCTION( std::forward<TArguments>( arguments )... );
		};
	};
}
}
}
