#pragma once


namespace Demo
{
inline namespace Callbacks
{
namespace Internal
{
	// Whether the given candidate is valid global function.
	template< auto CANDIDATE >
	inline constexpr bool IS_GLOBAL_FUNCTION = IsGlobalFunction<decltype( CANDIDATE )>::value;

	// Whether the given candidate is valid member function for given host type.
	template< auto CANDIDATE, typename THost >
	inline constexpr bool IS_MEMBER_FUNCTION = IsMemberFunction<decltype( CANDIDATE ), THost>::value;

	// Whether the given candidate is valid type of functor.
	template< typename TCandidate >
	inline constexpr bool IS_FUNCTOR = IsFunctor<TCandidate>::value;
}
}
}
