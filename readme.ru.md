## ?????? ?????????? ?????? "???????? ?????" (??? Callback).


#### ??????.

? ?????? ????? ???? ????? ??????????? ????????????? ? ???, ????? ??????????? ?????? ????? ????? ???????, ??? ????????, ? ?????? ?????????? ???????.
??? ??? ???????? ??????? ???, ????????????, ? ??????, ? ?? ??????.

?? ?????? ?? ????? ???????? ????????? ?? ?????????? ???????, ????????? ? ??????? ??? ??? ????? ??????? ?? ? ?????? ??????? ???????, ?????? ??? ????????.
????????? ?????? ????????? ?? ??????? ????????? ??????????? ????????? ?????? ?????? `?????????` ? `????????? ??????`.

??????, ? ?????, ????????? ??? ????, ????? ??????????? ??????????? ???? ?? ????????.


#### ??????.

???????????? ??????? ????? ???? ????? ???? ?? ????????? ????:

- ?????????? ??????? ? ??????????? ??????? ???????.
- ?????? ???????? ???? ??????????????.
- ??????-????????? ? ?????????????? ???????.

????? ??????? ????? ???????? ?????????? ???????, ??????? ?? ?????? ???? ???????? ?????????? ? ??? ????????????? ????????, ??? ????? ??????? ?????? ??????????
?? ??????? ???????. ??? ???????? ???????? ???? ??, ??? ????? ??????? ? ?????????? ??????????????? ?????? ????????


###### ?????????? ???????.

??? ????????? ?? ?????????? ??????? ????? ??????? ????????? ????????.

```.cpp
template< typename TResult, typename... TArguments >
using GlobalFunctionPointer = TResult (*)( TArguments... );
```

? ????????? ?????????? ??????? ???????? ???????? ? ??????????? ??????? ???????, ?? ??? ?? ?????????.

?, ????? ??, ????????? ?? ??????????? ??????? ?????? ??? ?????????? ??????? ?? ????????? ? ????????????, ?.?. ???? ?? ???? ???????????? ?????????? ???????,
?? ????????? ?? ??? ????? ????? ??? ????, ????? ????????????? ???????????? ???? ????????? ?? ????????? ??? ??? ?????????? ???????.

? ????????? C++17 ?????? ????? ???????-??????? ????? ??????????? ???????? ???:

```.cpp
template< auto FUNCTION, typename TResult, typename... TArguments >
TResult GlobalFunctionWrap( TArguments... );
```

?? ????? ?????????? ?? ???????? ??????? ? ????????? ????????? ?????? ?????????????? ????? ????? ?????????? `FUNCTION` ? ??????????? ??????? `GlobalFunctionWrap`.
??????? ?????? ????? ??????? ????? ???????? ?????? ??????? ???????.

```.cpp
template< typename TResult, typename... TArguments >
struct GlobalFunctionWrapper final
{
	template<TResult (*FUNCTION)( TArguments... )>
	static TResult Wrap( TArguments... );
};
```

????? ?????? ????????? ?????????? ???????? ?????? ??????? ? ????. ??? ??????? ???, ??? ?????? `Wrap` ????? ???????? ??? ????? ??????????? ?????????????.
? ???????? ?? ??? ????????, ????? ???????? ????????? ???? `Wrap` ? ?????? ???? ???????.


###### ??????.

????? ????? ????? ???????? ????? ???? ?? ??????????? ???? ????????.

```.cpp
template< typename THost, typename TResult, typename... TArguments >
using MemberFunctionPointer = TResult (THost::*)( TArguments... );
```

????????? ?????? ?????? ????????? ?????? ????? ??????? ?? ??????? ????????? ???????. ?????? ???????? ????? ??????? ??????????? ? ????? ??????? ?????? ??????????
?????????????? ? ?????????????: `&`, `&&`, `const` ? `volatile`. ??? ??????? ?????????? ??????????? ?????? ??????? ????????.

?? ???????? ? ?????????? ????????, ??????? ?????? ???? ????? ??????????? ????? ??? ???????.

```.cpp
template< typename THost, typename TResult, typename... TArguments >
struct MemberFunctionWrapper final
{
	template<TResult (THost::*FUNCTION)( TArguments... )>
	static TResult Wrap( void* const, TArguments... );
};
```

??? ?????? ?????? ????? ????????? ???????, ? ????????? ???????? ????? ? ????? ??????. ????????? ????? ????????? ??????? ?? ??? ????????? ?? ?????.
?? ?? ????? ???????? ?????? ??? ?????? ?????? ? ???????? ?????? ?? ?????????? ??? ?????? ???????-???????, ????? ?????? ??????????.
???? ?? ????? ?????? ???????? ????? ???????? ??? `THost&`, ????? ????????? ??? ????????? ? ??????????. ?? ????? ?????????? ?? ???????? ?????, ????? ????? ??????
?????????? ???????????? ???????? `Wrap` ??? ??????? ? ?????????? ???????. ??????? ?????? ???????? ????????? ??? `void* const`, ???????????? ? ??, ??? ????
????? ???? ??????? `nullptr` ? ?????? ? ???????? ?????????? ???????.


###### ?????????????? ???????.

?????????????? ??????? ?????????? ?? ??????? ???, ??? ???????? ? ?????? ??????? ?????? ?????????. "????????????????" ?????? ??????? ?????????? ? ??????? ? ????
?????????? ????????? `operator()`, ??????? ????? ???????????????, ??????????? ? ??????? ?? ?????? ??? ? ???????.
????????????? ? ??, ??? ?????????? `operator()` ?? ????? ???? ????????? ??????? ??????. ??? ??????, ??? `operator()` ?????? ???????? ??????? ???????.

?????????????, ???????????? ????? ??, ??? ??? ?????????????? ??????? ????? ?????? ? ?????? ??????? ? ??????? ?????????? ???????????? ????? ?? ?????????.
?????? `Wrap` ????? ?????????? ????????? ?? `operator()` ??????????????? ???????, ? ??? ?????? ????? ???????????? ??? ????? ?????? ?????????? ??? ??????.

????? ???????? ???????? ???? ??? ??????-???????, ??? `operator()`, ? ????? ??????, ????? ??????????? ???????? ??? ??????? ???????? ??? ?????? `mutable` ???????.
??? ??????-??????? ????? ??????? ?????? ??????????? ?? ??????, ??????????? ??????? ????????? ??????? ? ?????? ??????????.


#### ??????????.

?????????? ?????? ????????? ?????? ?????? ????? ????????? ????????? ?? ?????????? ??????? ? ??????? ???? ????????? ?? ?????????? ???????.
????? ?????????? ???????? ?????????. ?????????????? ??? ?????????? ??????? ??????????????? ??????? *???????*.

???? ???????? ? ???, ????? ????? ??????? ??????? ?????? ? ?????????? ????? ?????. ? ???? ??????? ???? ???????? ????? ??? ?????????? ??????, ??????? ?????????
???????????? ? ??????? ?????????? ????????? ??????:

- ????????? ?? ?????????? ??????? ? ????????? ?? ??????????? ??????? ??????;
- ????????? ?? ????? ??????;
- ??????-??????? ??? ????????? (??????).

??? ??????? ?? ??????? ????????? ??????? ???? ???????, ?????????? ??????? ??????????? ?????? ? ?????? ?????????? ????????? ?? ?????????? ???????.

???????? `GlobalFunctionContext`, `MemberFunctionContext` ? `EmptyLambdaContext` ????????? ????? ??????????.
?????? ??????? `AdaptedCall` ????? ??????????????? ?? ???? ???? ??????? ???????? ? ??????? ???? ????????? ?? ???????.
?????? ???????? ???????????? ??? ???????? ????????? ?????? ??????, ???? ???????? ??????, ??? ???????? ????? ??????????. ? ???? ?????? ??????? ??????
???????? ?? ??????????? ?? ?????????????.

? ???????? ????????? ? ???????? ?????? ????????? ??? ????????? ??????-???????, ??? ?????? ??? ?????? ??????. ???????? ???? ???? ?????????? ?????????? ????? ?????.
??????? ??????? ??? ??????-??????? ???????? ???????? ?? ?? ???????? ? ????? ?? ??????. ??? ?????? ??? ?????? ??? ????????? ?????? ?????? ???? ???????? ??????????
??????-????????. ?????? ??, ????????, ?????? ?????????? ?? ??????/????????? ??? ????? ???????? ?? ??????????? ???????.

??? ???????? ??? ?????????? ????????? ?????? ?????? ????? ??? ??????? ?????????? ??????????, ??? ? ????????? ???????? ????????? ??? ????????.
??????? ??? ????????? ?????? ??????????? ??? ???????????? ????????? ? ??? ??????, ????? ???????? ????? ?????????? ??? ???????????.
? ?????? ??????? ??????????? ?????????? ????????? ?? ???????????????, ?????? ????? ??????????? ?????????? ???????? ??????????.
? ???????? ?????????? ?????? ????????? ?????? ???????????? `std::shared_ptr`, ??????? ????? ????? ????????? ??????????? ????????? ?????? ? ????? ???????? ?
???????? ?????????. ?????????? `std::shared_ptr` ????????? ??? ?????????? ???????? ? ???????? ??????? ????? ?????????? ????????? ? ????? ?????,
??? ? ?????? ?????? ??????????? ??????????? ?????????? ????????.

? ??????????, ???????? ????? ??????????? ??? ???? ?? ????????? (`routine`) ?? ?????????? ??????? ???????? ? `std::shared_ptr<void>` (`context`).
? ?????????? ????????? ?????? ??????? ??????????? ???:

```.cpp
return routine( context.get(), std::forward<TArguments>( arguments )... );
```

????? ?????????? ??????????? ???????? ??? ???????? ?????, ????? ??? ? ????????? ??????????? ?????? ????????? ?? ?????????? ???????, ? ??? ???????? ?????, ?? 
????? ????, ?????????? ??????????? Value Object-??.

??? ???????? ???????? ????????? ????????? ?????? ? ?????? ???? ??????? ??? ??????? ????????? ??????? (`GetCallback`), ?? ?????????? ???? ?????????? ????????????.
?????? ?????? ????? ???? ???????? ??????????: ??? ?????????? ??????? ? ??????????? ??????? ??????, ??? ??????? ? ??? ?????????????? ????????.
??? ?????????? ?????????? ???? ?????????? ??????? ? ??? ????????? ?????? ??? ?????? ?????? `SignatureSelector`, ??????????? ????????? ??????? ?????????? 
????????? ????????? ?????? ??? ???? ?????????? ???????, ? ????? ??????? `FunctionSignature` ? `FunctorSignature`, ???????????? ?????? ? `SignatureSelector`.
????? ?????????? ?????????? ??????? `GetCallback` ???????????? ????? SFINAE, ??? ??????????? ?????? ??????? ???? ??????????? ??????? ?????:

- `IsGlobalFunction` ????? ???????? ????????? ?? ?????????? ??????? ?? ?????? (????????? `IS_GLOBAL_FUNCTION`);
- `IsMemberFunction` ????? ???????? ????????? ?? ????? ?? ?????? ? ????????? ?????????????? ?????? ? ?????????? ?????? ????????? (????????? `IS_MEMBER_FUNCTION`);
- `IsFunctor` ????? ???????? ??? ??????????????? ??????? ?? ?????? (????????? `IS_FUNCTOR`).

??? ????? ????????? ????????? ???????? ???????? ?????????? ? `GetCallback` ? ???????? ?????????? ?????????? ??? ?????????????? ?????????? ?????????? 
? ???????? ?????.


#### ?????? ? ????????.
			  
?????? ????? ???? ??????????? ? MS Visual Studio 2019. ??????? ???? ??? ?????????? ?? Clang 15 ? GCC 12.2.
??????? ? ?????? ??? ?????? ???????? ?? ????. ???? ??? ??????? ? ????????? C++17 ? ?? ????? ????? ????????? ?????-?????????????.
?????? ????? ???? ?????????? ????? ? VS2019, ? ??????? ? ? Clang 15 ? GCC 12.2.

?????? ??? VS19 ????????? ? ????? `./project`. ???????? ??? ?????????? ? ????? `./source`.


#### ???????????? ? ?????? ??????????.

?????????? ????? ????????? ???????? ??????????? ????? ??????? ????? ? ?????? ?????? ???????????? ?????????? ???????? ??????????? ??????????.
??????, ????????? ?????? ????????? ??????????, ???-????, ? ???????.

- [Wiki: ???????? ?????](https://ru.wikipedia.org/wiki/Callback_(%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5))
- [Wiki: ???????](https://ru.wikipedia.org/wiki/%D0%94%D0%B5%D0%BB%D0%B5%D0%B3%D0%B0%D1%82_(%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5))
- [Wiki: ???????](https://ru.wikipedia.org/wiki/%D0%A1%D0%BE%D0%B1%D1%8B%D1%82%D0%B8%D0%B5_(%D0%BE%D0%B1%D1%8A%D0%B5%D0%BA%D1%82%D0%BD%D0%BE-%D0%BE%D1%80%D0%B8%D0%B5%D0%BD%D1%82%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%BD%D0%BE%D0%B5_%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5))
- [?????????? <type_traits> ?++](https://en.cppreference.com/w/cpp/header/type_traits)
- [???????????? SFINAE](https://en.cppreference.com/w/cpp/language/sfinae)


#### ????????.

???? ??? ????????????? ?? ???????? MIT ????????. ??????????? ???? ? ????? ????????.

???????? ??????????? ????? ????: https://github.com/FrankStain/CallbacksSample
