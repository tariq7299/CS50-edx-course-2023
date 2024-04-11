
def main():
    capacity = int(input("Enter jar capacity: "))
    jar = Jar(capacity)
    print(jar.capacity)
    jar.deposit(16)
    print(jar.size)
    jar.withdraw(5)
    print(jar.size)
    jar.deposit(2)
    print(jar.size)
    jar.deposit(2)
    print(jar.size)
    jar.deposit(2)
    print(jar.size)
    jar.withdraw(6)
    print(jar.size)
    jar.deposit(10)
    print(jar.size)
    print(jar)
    print(jar.cookies_jar)


class Jar:

    def __init__(self, capacity=12):
        self.capacity = capacity
        self.cookies_jar = [None] * self.capacity
        self._size = 0

    def __str__(self):
        return "\U0001F36A" * self.size

    def deposit(self, n):
        if n + self._size <= self.capacity:
            k = 0
            i = 0
            for empty_place in self.cookies_jar:
                if empty_place ==  None and k < n:
                    self.cookies_jar[i] = "\U0001F36A"
                    self._size += 1
                    i += 1
                    k += 1
                else:
                    i+= 1
        else:
            raise ValueError(f"No space left in the jar !, There is room for {self.capacity - self.size}")

    def withdraw(self, n):
        if n <= self.capacity and n <= self._size:
            k = 0
            i = 0
            for empty_place in self.cookies_jar:
                if empty_place == "\U0001F36A" and k < n:
                    self.cookies_jar[i] = None
                    self._size -= 1
                    i += 1
                    k += 1
                else:
                    i+= 1
        else:
            raise ValueError(f"No cookies left in the jar !, Only {self.capacity - self.size} have been withdrawed")

    @property
    def capacity(self):
        return self._capacity

    @capacity.setter
    def capacity(self, capacity_value):
        if capacity_value > 0:
            self._capacity = capacity_value
        else:
            raise ValueError('capacity should be a non-negative Integer number !')

    @property
    def size(self):
        return self._size


main()
