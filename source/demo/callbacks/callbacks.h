#pragma once


// External dependencies.
#include <type_traits>


// Namespace definition.
namespace Deom
{
// Callbacks subsystem.
namespace Callbacks
{
// Internal stuff for callbacks.
namespace Internal
{

}
}
}


// Most fundamental internal stuff.
#include "internal/SignatureTypePolicy.h"
#include "internal/type-traits.h"


// Usable internal stuff.
#include "internal/FunctionProxyBuilder.h"
#include "internal/MethodProxyBuilder.h"
#include "internal/EmptyLambdaProxyBuilder.h"

// Public interface.


// Deferred inline definitions.
