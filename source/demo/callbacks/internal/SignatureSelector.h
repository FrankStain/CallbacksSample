#pragma once


namespace Demo
{
inline namespace Callbacks
{
namespace Internal
{
	/**
		@brief	Signature selector for various function types.

		False-deduction branch. Shall never be deducted and used.
	*/
	template< typename >
	struct SignatureSelector final
	{
		// Dummy signature.
		using Signature = void ();
	};

	/**
		@brief	Signature selector for pointers to global function.
	*/
	template< typename TResult, typename... TArguments >
	struct SignatureSelector<TResult (*)( TArguments... )> final
	{
		// Signature for given function type.
		using Signature = TResult ( TArguments... );
	};

	/**
		@brief	Signature selector for pointers to regular member function.
	*/
	template< typename THost, typename TResult, typename... TArguments >
	struct SignatureSelector<TResult (THost::*)( TArguments... )> final
	{
		// Signature for given member-function type.
		using Signature = TResult ( TArguments... );
	};

	/**
		@brief	Signature selector for pointers to member function with constant context.
	*/
	template< typename THost, typename TResult, typename... TArguments >
	struct SignatureSelector<TResult (THost::*)( TArguments... ) const> final
	{
		// Signature for given member-function type.
		using Signature = TResult ( TArguments... );
	};

	/**
		@brief	Signature selector for pointers to member function with volatile context.
	*/
	template< typename THost, typename TResult, typename... TArguments >
	struct SignatureSelector<TResult (THost::*)( TArguments... ) volatile> final
	{
		// Signature for given member-function type.
		using Signature = TResult ( TArguments... );
	};

	/**
		@brief	Signature selector for pointers to member function with constant volatile context.
	*/
	template< typename THost, typename TResult, typename... TArguments >
	struct SignatureSelector<TResult (THost::*)( TArguments... ) const volatile> final
	{
		// Signature for given member-function type.
		using Signature = TResult ( TArguments... );
	};
}
}
}
