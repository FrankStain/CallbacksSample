## The sample of "Callback" idiom implementation.


#### Prologue.

The sample is based on initial assumption of trying to convert any callable to call of global function.
So, how this can be done? The solution has become both trivial and not, in same time.

As the final decision, one want to get the pointer to global function, that call the target callable on the calling.
It will become the great simplification of callback implementation, once such global function is able to be obtained.

In general, it sounds good enough to try to implement it as sample at last.


#### Analysis.

The callable currently may be represented in one of forms:

- GLobal function and static member-function.
- Member-function with context of any kind.
- Lambda-functions and functional objects.

The question is: how to convert the callable of any of this types to regular global function?
The answer is obvious: one should implement such a template of function, that is able to remember the given callable.
But how should looks like such template of function?


###### Global functions.

Here the general template that describes the global function of any kind is:
						
```.cpp
template< typename TResult, typename... TArguments >
using GlobalFunctionPointer = TResult (*)( TArguments... );
```

Static member functions can be freely used to assign the pointers of `GlobalFunctionPointer`. So the static member functions also covered 
by decision for global functions.

And all looks like the global functions does not required to be converted. But it should be converted. It allows to change the interface of function 
in a purposes of callback implementation.

Using the C++17, one can declare the template of such converting function like this:
	
```.cpp
template< auto FUNCTION, typename TResult, typename... TArguments >
TResult GlobalFunctionWrap( TArguments... );
```

But this implementation looks unusable since it's usage is very error-prone. The signature of `FUNCTION` does not correlate with 
other arguments of `GlobalFunctionWrap`. So, it would be better to implement such template inside of class template.
		   
```.cpp
template< typename TResult, typename... TArguments >
struct GlobalFunctionWrapper final
{
	template<TResult (*FUNCTION)( TArguments... )>
	static TResult Wrap( TArguments... );
};
```

This implementation is much better in use. It allows to remember the given global function. `Wrap` now can be freely modified to fit the requirements.
And it will be needed to be modified after the next callable forms are analyzed.


###### Member-functions.

Any member-function can be described with one of next templates:

```.cpp
template< typename THost, typename TResult, typename... TArguments >
using MemberFunctionPointer = TResult (THost::*)( TArguments... );
```

This template describes ony a set of member-functions with regular object context. Any other context can be expressed by adding the different combinations
of `&`, `&&`, `const` and `volatile`. It is sufficient for current analysis to consider only regular context.

The wrapper for member-functions can be designed like the on for global functions.

```.cpp
template< typename THost, typename TResult, typename... TArguments >
struct MemberFunctionWrapper final
{
	template<TResult (THost::*FUNCTION)( TArguments... )>
	static TResult Wrap( void* const, TArguments... );
};
```

It is crucial to pass the object instance as argument of `Wrap`, since the instance is dynamic and the member-function can't be called without object.
The decision to pass the object instance as `void* const` instead of `THost&` is guided by requirement for all `Wrap` functions to provide the same signature.
Now the `Wrap` for global functions can be implemented so, like it use the first argument `void* const` for purposes of keeping the same signature with `Wrap` 
for member-functions. It is also considered that the first argument can be `nullptr` in certain cases, on the global functions `Wrap` call for ex.


###### Functional objects.

The functional objects are the objects at first. The "Functionness" of functional objects is reached by overloading the `operator()`, which allows to call 
the object like function.
It also needs to note that `operator()` can't be overloaded in out of class form, it always the member-function.

Therefore, it will fair enough to say that all functional objects can be reduced to case of member-functions. It requires no special decisions to implement.
The `Wrap` template will remember the `operator()` and the instance of functional object will be passed as first argument of `Wrap`.

Such assumption covers the lambda-functions as well. Only one optimization may be reached for lambda-functions. It is the memory reduction for empty lambda-closure.
Different implementation of `Wrap` template can be used to ignore the instance of empty lambda function.


#### The implementation.

The implementation of such callback should be able to transform the pointers of given callables to pointers of global function with same signature.
Such requirement means the adaption of given callable. So the decision is to use the *Adapter* idiom for implementation.

The point of adapter is to reduce the set of particular cases to a generic single case. While the analysis there was only three unique cases identified that
are required to be adapted to single interface the callback can store:

- pointer to global function and pointer to a static class function;
- pointer to member-function;
- lambda function without closure (empty lambda).

Exclusive adapter is required to be created for each described case. Such adapters should allow the given callable to be stored in form 
of pointer to global function with general interface.

Here described such adapters: `GlobalFunctionContext`, `MemberFunctionContext` and `EmptyLambdaContext`. All of it are used to adapt the corresponded
callable through the `AdaptedCall` template. As result, any callable with certain signature can be adapted to pointer of global function with same signature.
The first parameter of `AdaptedCall` is used to pass the object instance for member-function and doesn't taken into account in other cases.

The calling contest is stored also by callback. Such context is either the non-empty closure of lambda-function, the object instance 
for member-function, or the `nullptr`.
In common cases, the lambda-closure is owned by callback, but the object instances does not so. It means the callback should be able to store 
owned values as well as not owned ones. Also the question of copying or cloning the callback should be taken into consideration.

It means the callback implementation must use the regular idiom of shared owning. `std::shared_ptr` is the best choice for such implementation.
It covers all the needs of callback to store the calling context. It allows the valid owning and deletion of instances in common form as well as allows
the non-owned storage in easy way.

As result, the callback consists of pair: the pointer (`routine`) to global adapter-function and `std::shared_ptr<void>` (`context`).
And the trivial form of callback invocation looks like:

```.cpp
return routine( context.get(), std::forward<TArguments>( arguments )... );
```

This implementation helps to simplify the design of callback, reducing the functionality to the thin layer between the caller and callee.
Also the callback becomes the lightweight value-object.

The `GetCallback` factory functions implemented here to simplify the production of callbacks for given arguments.
Each template of `GetCallback` function has its own contract to be used: for global functions, for member-functions and for functional objects.
The `SignatureSelector` template helps to simplify the signature type deduction for callback from type of given function. It allows to choose the valid
signature of callback for the type of given function. `FunctionSignature` and `FunctorSignature` simplifies the callback signature selection, it decorates the
`SignatureSelector` inside.
The suitable template of `GetCallback` is determined through the SFINAE and by means the next type traits:

- `IsGlobalFunction` to distinguish the pointer to global function (`IS_GLOBAL_FUNCTION` constant);
- `IsMemberFunction` to distinguish the pointer to member-function (`IS_MEMBER_FUNCTION` constant);
- `IsFunctor` to distinguish the type of a functional object (`IS_FUNCTOR` constant).

This traits allows to distinguish the valid form of given callable and choose the proper adapter to produce the valid callback.



#### Build and testing.

This code is designed in MS Visual Studio 2019. Also this code was tested using Clang 15 and GCC 12.2. No issues was found for this translators.
This code uses C++17 standard and can be considered as cross-platform.

Solution may be found at `./project` folder. The source code itself may be found at `./source` folder.


#### References

The implementation of this prototype is a compilation of my personal experience and it is almost impossible to give a complete list of references I have used.
However, some important sources of information, after all, I will give.

- [Wiki: callback](https://en.wikipedia.org/wiki/Callback_(computer_programming))
- [Wiki: delegate](https://en.wikipedia.org/wiki/Delegate_(CLI))
- [Wiki: event](https://en.wikipedia.org/wiki/Event_(computing))
- [<type_traits> C++ library](https://en.cppreference.com/w/cpp/header/type_traits)
- [SFINAE docs](https://en.cppreference.com/w/cpp/language/sfinae)


#### License

This code is licensed under the MIT license. Details described in the license file.

Main repository for this code: https://github.com/FrankStain/CallbacksSample
