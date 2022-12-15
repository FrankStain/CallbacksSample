#pragma once


namespace Demo
{
inline namespace Callbacks
{
namespace Internal
{
	template< typename, typename = void >
	struct IsCallable final : std::false_type {};

	template< typename TCandidate >
	struct IsCallable<TCandidate, std::void_t<decltype( (void)&TCandidate::operator() )>> final : std::is_class<TCandidate> {};

	template< typename >
	struct IsGlobalFunction final : std::false_type {};

	template< typename TResult, typename... TArguments >
	struct IsGlobalFunction<TResult (*)( TArguments... )> final : std::true_type {};

	template< typename, typename >
	struct IsMemberFunction final : std::false_type {};

	template< typename THost, typename TResult, typename... TArguments >
	struct IsMemberFunction<TResult (THost::*)( TArguments... ), THost> final : std::true_type {};

	template< typename THost, typename TResult, typename... TArguments >
	struct IsMemberFunction<TResult (THost::*)( TArguments... ) const, THost> final : std::true_type {};

	template< typename THost, typename TResult, typename... TArguments >
	struct IsMemberFunction<TResult (THost::*)( TArguments... ) volatile, THost> final : std::true_type {};

	template< typename THost, typename TResult, typename... TArguments >
	struct IsMemberFunction<TResult (THost::*)( TArguments... ) const volatile, THost> final : std::true_type {};
}
}
}
