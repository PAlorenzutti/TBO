import random

with open("test1.txt", "w") as f:
    m = 0
    for i in range(4000):
        j = random.randint(0, 100000000000)
        f.write(f"abc{j},def,jkl{m},mno," + "zazazazazaza"*3000 + "\n")
        if(i % 10000 == 0):
            f.flush()
        m += 1

with open("test2.txt", "w") as f:
    m = 0
    for i in range(4000):
        j = random.randint(0, 100000000000)
        f.write(f"abc{j},def,jkl{m},mno," + "zazazazazaza"*3000 + "\n")
        if(i % 10000 == 0):
            f.flush()
        m += 1