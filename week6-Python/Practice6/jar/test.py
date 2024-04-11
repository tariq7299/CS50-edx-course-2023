def main():
    JAR = jar_test()

    # JAR.create()
    # print(JAR.cookies_jar)
    JAR.fill()
    print(JAR.cookies_jar)
    # JAR.size()
    print(JAR.size)






class jar_test:

    def __init__(self, size=0):
        self.cookies_jar = self.create()
        self._size = size



    def create(self):
        self.cookies_jar = [None] * 10
        return self.cookies_jar

    def fill(self):
        for empty_place in self.cookies_jar:
            self.cookies_jar[0] = "\U0001F36A"
            self.cookies_jar[1] = "\U0001F36A"
            self.cookies_jar[2] = "\U0001F36A"
            self.cookies_jar[3] = "\U0001F36A"
            self._size += 4
            return self.cookies_jar
    @property
    def size(self):
        return self._size









main()