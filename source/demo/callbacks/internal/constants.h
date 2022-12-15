#pragma once


namespace Demo
{
inline namespace Callbacks
{
namespace Internal
{
	template< auto CANDIDATE >
	inline constexpr bool IS_GLOBAL_FUNCTION = IsGlobalFunction<decltype( CANDIDATE )>::value;

	template< auto CANDIDATE, typename THost >
	inline constexpr bool IS_MEMBER_FUNCTION = IsMemberFunction<decltype( CANDIDATE ), THost>::value;

	template< typename TCandidate >
	inline constexpr bool IS_FUNCTOR = IsCallable<TCandidate>::value;
}
}
}
