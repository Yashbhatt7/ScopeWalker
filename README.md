# ScopeWalker

A toy C++ program that demonstrates lexical scoping implementation. This interactive REPL lets you create variables and navigate between scope levels to understand how lexical environments work.

## Build & Run
```
clang++ env.cpp -o scope
./scope
```

## Commands
- `let <var> = <value>` - Declare a variable in the current scope
- `<var>` - Look up and display a variable's value
- `deep` - Enter a new nested scope
- `shallow` - Exit current scope

## Example
```
>> let a = 4
4
>> a
4
>> deep
1: Scope Deep!
>> let b = a
4
>> b
4
>> deep
2: Scope Deep!
>> let c = b
4
>> b
4
>> c
4
>> a
4
>> shallow
Out Of Scope 2
>> c
0
>> b
4
>> a
4
>> let a = a
4
>> a
4
>> b
4
>> shallow
Out Of Scope 1
>> b
0
>> c
0
>> a
4
>>
```

