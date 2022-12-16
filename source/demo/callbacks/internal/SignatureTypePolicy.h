#pragma once


namespace Demo
{
inline namespace Callbacks
{
namespace Internal
{
	template< typename >
	struct SignatureSelector final
	{
		using Signature = void ();
	};

	template< typename TResult, typename... TArguments >
	struct SignatureSelector<TResult (*)( TArguments... )> final
	{
		using Signature = TResult ( TArguments... );
	};

	template< typename THost, typename TResult, typename... TArguments >
	struct SignatureSelector<TResult (THost::*)( TArguments... )> final
	{
		using Signature = TResult ( TArguments... );
	};

	template< typename THost, typename TResult, typename... TArguments >
	struct SignatureSelector<TResult (THost::*)( TArguments... ) const> final
	{
		using Signature = TResult ( TArguments... );
	};

	template< typename THost, typename TResult, typename... TArguments >
	struct SignatureSelector<TResult (THost::*)( TArguments... ) volatile> final
	{
		using Signature = TResult ( TArguments... );
	};

	template< typename THost, typename TResult, typename... TArguments >
	struct SignatureSelector<TResult (THost::*)( TArguments... ) const volatile> final
	{
		using Signature = TResult ( TArguments... );
	};
}
}
}
