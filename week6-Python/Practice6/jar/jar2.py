
def main():
    capacity = int(input("Enter jar capacity: "))
    jar = Jar(capacity)

    print(str(jar.capacity))
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


class Jar:

    def __init__(self, capacity=12):
        if capacity < 0:
            raise ValueError
        self._capacity = capacity
        self._size = 0

    def __str__(self):
        return "\U0001F36A" * self.size

    def deposit(self, n):
        if n > self.capacity - self.size:
            raise ValueError
        self._size += n

    def withdraw(self, n):
        if n > self.capacity - self.size:
            raise ValueError
        self._size -= n

    @property
    def capacity(self):
        return self._capacity

    @property
    def size(self):
        return self._size




main()
