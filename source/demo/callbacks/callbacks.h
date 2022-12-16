#pragma once


// External dependencies.
#include <type_traits>
#include <utility>
#include <memory>


// Namespace definition.
namespace Demo
{
// Callbacks subsystem.
inline namespace Callbacks
{
// Internal stuff for callbacks.
namespace Internal
{

}
}
}


// Most fundamental internal stuff.
#include "internal/SignatureSelector.h"
#include "internal/type-traits.h"


// Usable internal stuff.
#include "internal/aliases.h"
#include "internal/constants.h"

#include "internal/FunctionProxyBuilder.h"
#include "internal/MethodProxyBuilder.h"
#include "internal/EmptyLambdaProxyBuilder.h"

// Public interface.
#include "Callback.h"


// Deferred inline definitions.
#include "factory-functions.inl"
#include "Callback.inl"
