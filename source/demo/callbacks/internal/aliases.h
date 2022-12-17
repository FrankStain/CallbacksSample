#pragma once


namespace Demo
{
inline namespace Callbacks
{
namespace Internal
{
	/**
		@brief	Signature of function by pointer.

		The alias takes non-type template argument, and deduces the type of it signature.
		For non-function arguments the `void ()` signature will be deducted as shortcut.

		@tparam	CANDIDATE	Arbitrary entity, which signature need to be deducted.
	*/
	template< auto CANDIDATE >
	using FunctionSignature = typename SignatureSelector<decltype( CANDIDATE )>::Signature;

	/**
		@brief	Signature of functor by type.

		The alias takes type of functor, and deduces the type of it signature.

		@tparam	TFunctor	Type of valid functor, which signature need to be deducted.
	*/
	template< typename TFunctor >
	using FunctorSignature = typename SignatureSelector<decltype( &TFunctor::operator() )>::Signature;
}
}
}
