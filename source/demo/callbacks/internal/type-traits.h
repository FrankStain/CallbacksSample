#pragma once


namespace Demo
{
inline namespace Callbacks
{
namespace Internal
{
	/**
		@brief	Test for global functions.

		"False" branch. The given argument represents not a global function.
	*/
	template< typename >
	struct IsGlobalFunction final : std::false_type {};

	/**
		@brief	Test for global functions.

		"True" branch. The given argument represents valid pointer to global function.
	*/
	template< typename TResult, typename... TArguments >
	struct IsGlobalFunction<TResult (*)( TArguments... )> final : std::true_type {};

	/**
		@brief	Test for member functions.

		"False" branch. The given argument represents not a member function.
	*/
	template< typename, typename >
	struct IsMemberFunction final : std::false_type {};

	/**
		@brief	Test for member functions.

		"True" branch. The given argument represents valid regular member function.
	*/
	template< typename THost, typename TResult, typename... TArguments >
	struct IsMemberFunction<TResult (THost::*)( TArguments... ), THost> final : std::true_type {};

	/**
		@brief	Test for member functions.

		"True" branch. The given argument represents valid member function with constant context.
	*/
	template< typename THost, typename TResult, typename... TArguments >
	struct IsMemberFunction<TResult (THost::*)( TArguments... ) const, THost> final : std::true_type {};

	/**
		@brief	Test for member functions.

		"True" branch. The given argument represents valid member function with volatile context.
	*/
	template< typename THost, typename TResult, typename... TArguments >
	struct IsMemberFunction<TResult (THost::*)( TArguments... ) volatile, THost> final : std::true_type {};

	/**
		@brief	Test for member functions.

		"True" branch. The given argument represents valid member function with constant volatile context.
	*/
	template< typename THost, typename TResult, typename... TArguments >
	struct IsMemberFunction<TResult (THost::*)( TArguments... ) const volatile, THost> final : std::true_type {};

	/**
		@brief	Test for functional objects.

		"False" branch. The given argument represents not a type of functional object.
	*/
	template< typename, typename = void >
	struct IsFunctor final : std::false_type {};

	/**
		@brief	Test for functional objects.

		"True" branch. The given argument represents valid type of functional object.
	*/
	template< typename TCandidate >
	struct IsFunctor<TCandidate, std::void_t<decltype( (void)&TCandidate::operator() )>> final : std::is_class<TCandidate> {};
}
}
}
