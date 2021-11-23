from strongtyping.strong_typing import type_checking

@type_checking
def f() -> list[int]:
    return 5

print(f())
